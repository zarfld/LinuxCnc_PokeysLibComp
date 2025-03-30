typedef enum {
    HOME_SEQUENCE_IDLE = 0,        // valid start state
    HOME_SEQUENCE_START,           // valid start state
    HOME_SEQUENCE_DO_ONE_JOINT,    // valid start state
    HOME_SEQUENCE_DO_ONE_SEQUENCE, // valid start state
    HOME_SEQUENCE_START_JOINTS,    // internal usage
    HOME_SEQUENCE_WAIT_JOINTS,     // internal usage
} home_sequence_state_t;

typedef enum {
    HOME_IDLE = 0,
    HOME_START,                 // 1
    HOME_UNLOCK,                // 2
    HOME_UNLOCK_WAIT,           // 3
    HOME_INITIAL_BACKOFF_START, // 4
    HOME_INITIAL_BACKOFF_WAIT,  // 5
    HOME_INITIAL_SEARCH_START,  // 6
    HOME_INITIAL_SEARCH_WAIT,   // 7
    HOME_SET_COARSE_POSITION,   // 8
    HOME_FINAL_BACKOFF_START,   // 9
    HOME_FINAL_BACKOFF_WAIT,    // 10
    HOME_RISE_SEARCH_START,     // 11
    HOME_RISE_SEARCH_WAIT,      // 12
    HOME_FALL_SEARCH_START,     // 13
    HOME_FALL_SEARCH_WAIT,      // 14
    HOME_SET_SWITCH_POSITION,   // 15
    HOME_INDEX_ONLY_START,      // 16
    HOME_INDEX_SEARCH_START,    // 17
    HOME_INDEX_SEARCH_WAIT,     // 18
    HOME_SET_INDEX_POSITION,    // 19
    HOME_FINAL_MOVE_START,      // 20
    HOME_FINAL_MOVE_WAIT,       // 21
    HOME_LOCK,                  // 22
    HOME_LOCK_WAIT,             // 23
    HOME_FINISHED,              // 24
    HOME_ABORT                  // 25
} home_state_t;

static int base_1joint_state_machine(int joint_num) {
    emcmot_joint_t *joint;
    double offset, tmp;
    int home_sw_active, homing_flag;
    bool immediate_state = 0;

    homing_flag = 0;
    joint = &joints[joint_num];
    home_sw_active = H[joint_num].home_sw;
    if (H[joint_num].home_state != HOME_IDLE) {
        homing_flag = 1; /** @brief  at least one joint is homing */
    }

    /** @brief  when a joint is homing, 'check_for_faults()' ignores its limit
       switches, so that this code can do the right thing with them. Once
       the homing process is finished, the 'check_for_faults()' resumes
       checking */

    /** @brief  homing state machine */

    /** @brief  Some portions of the homing sequence can run thru two or more
       states during a single servo period.  This is done using
       'immediate_state'.  If a state transition sets it true (non-zero),
       this 'do-while' will loop executing switch(home_state) immediately
       to run the new state code.  Otherwise, the loop will fall thru, and
       switch(home_state) runs only once per servo period. Do _not_ set
       'immediate_state' true unless you also change 'home_state', unless
       you want an infinite loop! */
    do {
        immediate_state = 0;
        switch (H[joint_num].home_state) {
            case HOME_IDLE:
                /** @brief  nothing to do */
                break;

            case HOME_START:
                /** @brief  This state is responsible for getting the homing process
                   started.  It doesn't actually do anything, it simply
                   determines what state is next */
                if (H[joint_num].home_flags & HOME_IS_SHARED && home_sw_active) {
                    rtapi_print_msg(RTAPI_MSG_ERR, _("Cannot home while shared home switch is closed j=%d"), joint_num);
                    H[joint_num].home_state = HOME_IDLE;
                    break;
                }
                /** @brief  set flags that communicate with the rest of EMC */
                if ((H[joint_num].home_flags & HOME_NO_REHOME) && H[joint_num].homed) {
                    H[joint_num].home_state = HOME_IDLE;
                    break; // no rehome allowed if absolute_enoder
                } else {
                    H[joint_num].homing = 1;
                    H[joint_num].homed = 0;
                }
                joint->free_tp.enable = 0;    /** @brief  stop any existing motion */
                sync_reset();                 /** @brief  stop any interrupted/canceled sync */
                H[joint_num].pause_timer = 0; /** @brief  reset delay counter */
                /** @brief  figure out exactly what homing sequence is needed */
                if (H[joint_num].home_flags & HOME_ABSOLUTE_ENCODER) {
                    H[joint_num].home_flags &= ~HOME_IS_SHARED; // shared not applicable
                    H[joint_num].home_state = HOME_SET_SWITCH_POSITION;
                    immediate_state = 1;
                    // Note: H[joint_num].homed
                    // is not set in case there is a final move requested
                    break;
                }
                if (H[joint_num].home_flags & HOME_UNLOCK_FIRST) {
                    H[joint_num].home_state = HOME_UNLOCK;
                } else {
                    H[joint_num].home_state = HOME_UNLOCK_WAIT;
                    immediate_state = 1;
                }
                break;

            case HOME_UNLOCK:
                // @startuml
                // note right of HOME_START
                // Transition to HOME_UNLOCK if HOME_UNLOCK_FIRST flag is set
                // end note
                // @enduml
                // unlock now
                SetRotaryUnlock(joint_num, 1);
                H[joint_num].home_state = HOME_UNLOCK_WAIT;
                break;

            case HOME_UNLOCK_WAIT:
                // @startuml
                // note right of HOME_UNLOCK
                // Transition to HOME_UNLOCK_WAIT if not yet unlocked
                // end note
                // @enduml
                // if not yet unlocked, continue waiting
                if ((H[joint_num].home_flags & HOME_UNLOCK_FIRST) && !GetRotaryIsUnlocked(joint_num))
                    break;

                // either we got here without an unlock needed, or the
                // unlock is now complete.
                if (H[joint_num].home_search_vel == 0.0) {
                    if (H[joint_num].home_latch_vel == 0.0) {
                        /** @brief  both vels == 0 means home at current position */
                        H[joint_num].home_state = HOME_SET_SWITCH_POSITION;
                        immediate_state = 1;
                    } else if (H[joint_num].home_flags & HOME_USE_INDEX) {
                        /** @brief  home using index pulse only */
                        H[joint_num].home_state = HOME_INDEX_ONLY_START;
                        immediate_state = 1;
                    } else {
                        rtapi_print_msg(RTAPI_MSG_ERR, _("invalid homing config: non-zero LATCH_VEL needs either SEARCH_VEL or USE_INDEX"));
                        H[joint_num].home_state = HOME_IDLE;
                    }
                } else {
                    if (H[joint_num].home_latch_vel != 0.0) {
                        /** @brief  need to find home switch */
                        H[joint_num].home_state = HOME_INITIAL_SEARCH_START;
                        immediate_state = 1;
                    } else {
                        rtapi_print_msg(RTAPI_MSG_ERR, _("invalid homing config: non-zero SEARCH_VEL needs LATCH_VEL"));
                        H[joint_num].home_state = HOME_IDLE;
                    }
                }
                break;

            case HOME_INITIAL_BACKOFF_START:
                // @startuml
                // note right of HOME_UNLOCK_WAIT
                // Transition to HOME_INITIAL_BACKOFF_START if home_search_vel is non-zero
                // end note
                // @enduml
                /** @brief  This state is called if the homing sequence starts at a
                   location where the home switch is already tripped. It
                   starts a move away from the switch. */
                /** @brief  is the joint still moving? */
                if (joint->free_tp.active) {
                    /** @brief  yes, reset delay, wait until joint stops */
                    H[joint_num].pause_timer = 0;
                    break;
                }
                /** @brief  has delay timed out? */
                if (H[joint_num].pause_timer < (HOME_DELAY * servo_freq)) {
                    /** @brief  no, update timer and wait some more */
                    H[joint_num].pause_timer++;
                    break;
                }
                H[joint_num].pause_timer = 0;
                /** @brief  set up a move at '-search_vel' to back off of switch */
                home_start_move(joint, -H[joint_num].home_search_vel);
                /** @brief  next state */
                H[joint_num].home_state = HOME_INITIAL_BACKOFF_WAIT;
                break;

            case HOME_INITIAL_BACKOFF_WAIT:
                // @startuml
                // note right of HOME_INITIAL_BACKOFF_START
                // Transition to HOME_INITIAL_BACKOFF_WAIT if joint is still moving
                // end note
                // @enduml
                /** @brief  This state is called while the machine is moving off of
                   the home switch.  It terminates when the switch is cleared
                   successfully.  If the move ends or hits a limit before it
                   clears the switch, the home is aborted. */
                /** @brief  are we off home switch yet? */
                if (!home_sw_active) {
                    /** @brief  yes, stop motion */
                    joint->free_tp.enable = 0;
                    /** @brief  begin initial search */
                    H[joint_num].home_state = HOME_INITIAL_SEARCH_START;
                    immediate_state = 1;
                    break;
                }
                ABORT_CHECK(joint_num);
                break;

            case HOME_INITIAL_SEARCH_START:
                /** @brief  This state is responsible for starting a move toward the
                   home switch.  This move is at 'search_vel', which can be
                   fairly fast, because once the switch is found another
                   slower move will be used to set the exact home position. */
                /** @brief  is the joint already moving? */
                if (joint->free_tp.active) {
                    /** @brief  yes, reset delay, wait until joint stops */
                    H[joint_num].pause_timer = 0;
                    break;
                }
                /** @brief  has delay timed out? */
                if (H[joint_num].pause_timer < (HOME_DELAY * servo_freq)) {
                    /** @brief  no, update timer and wait some more */
                    H[joint_num].pause_timer++;
                    break;
                }
                H[joint_num].pause_timer = 0;
                /** @brief  make sure we aren't already on home switch */
                if (home_sw_active) {
                    /** @brief  already on switch, need to back off it first */
                    H[joint_num].home_state = HOME_INITIAL_BACKOFF_START;
                    immediate_state = 1;
                    break;
                }
                /** @brief  set up a move at 'search_vel' to find switch */
                home_start_move(joint, H[joint_num].home_search_vel);
                /** @brief  next state */
                H[joint_num].home_state = HOME_INITIAL_SEARCH_WAIT;
                break;

            case HOME_INITIAL_SEARCH_WAIT:
                /** @brief  This state is called while the machine is looking for the
                   home switch.  It terminates when the switch is found.  If
                   the move ends or hits a limit before it finds the switch,
                   the home is aborted. */
                /** @brief  have we hit home switch yet? */
                if (home_sw_active) {
                    /** @brief  yes, stop motion */
                    joint->free_tp.enable = 0;
                    /** @brief  go to next step */
                    H[joint_num].home_state = HOME_SET_COARSE_POSITION;
                    immediate_state = 1;
                    break;
                }
                ABORT_CHECK(joint_num);
                break;

            case HOME_SET_COARSE_POSITION:
                /** @brief  This state is called after the first time the switch is
                   found.  At this point, we are approximately home. Although
                   we will do another slower pass to get the exact home
                   location, we reset the joint coordinates now so that screw
                   error comp will be appropriate for this portion of the
                   screw (previously we didn't know where we were at all). */
                /** @brief  set the current position to 'home_offset' */
                offset = H[joint_num].home_offset - joint->pos_fb;
                /** @brief  this moves the internal position but does not affect the
                   motor position */
                joint->pos_cmd += offset;
                joint->pos_fb += offset;
                joint->free_tp.curr_pos += offset;
                joint->motor_offset -= offset;
                /** @brief  The next state depends on the signs of 'search_vel' and
                   'latch_vel'.  If they are the same, that means we must
                   back up, then do the final homing moving the same
                   direction as the initial search, on a rising edge of the
                   switch.  If they are opposite, it means that the final
                   homing will take place on a falling edge as the machine
                   moves off of the switch. */
                tmp = H[joint_num].home_search_vel * H[joint_num].home_latch_vel;
                if (tmp > 0.0) {
                    /** @brief  search and latch vel are same direction */
                    H[joint_num].home_state = HOME_FINAL_BACKOFF_START;
                } else {
                    /** @brief  search and latch vel are opposite directions */
                    H[joint_num].home_state = HOME_FALL_SEARCH_START;
                }
                immediate_state = 1;
                break;

            case HOME_FINAL_BACKOFF_START:
                /** @brief  This state is called once the approximate location of the
                   switch has been found.  It is responsible for starting a
                   move that will back off of the switch in preparation for a
                   final slow move that captures the exact switch location. */
                /** @brief  is the joint already moving? */
                if (joint->free_tp.active) {
                    /** @brief  yes, reset delay, wait until joint stops */
                    H[joint_num].pause_timer = 0;
                    break;
                }
                /** @brief  has delay timed out? */
                if (H[joint_num].pause_timer < (HOME_DELAY * servo_freq)) {
                    /** @brief  no, update timer and wait some more */
                    H[joint_num].pause_timer++;
                    break;
                }
                H[joint_num].pause_timer = 0;
                /** @brief  we should still be on the switch */
                if (!home_sw_active) {
                    rtapi_print_msg(RTAPI_MSG_ERR, _("Home switch inactive before start of backoff move j=%d"), joint_num);
                    H[joint_num].home_state = HOME_IDLE;
                    break;
                }
                /** @brief  set up a move at '-search_vel' to back off of switch */
                home_start_move(joint, -H[joint_num].home_search_vel);
                /** @brief  next state */
                H[joint_num].home_state = HOME_FINAL_BACKOFF_WAIT;
                break;

            case HOME_FINAL_BACKOFF_WAIT:
                /** @brief  This state is called while the machine is moving off of
                   the home switch after finding its approximate location.
                   It terminates when the switch is cleared successfully.  If
                   the move ends or hits a limit before it clears the switch,
                   the home is aborted. */
                /** @brief  are we off home switch yet? */
                if (!home_sw_active) {
                    /** @brief  yes, stop motion */
                    joint->free_tp.enable = 0;
                    /** @brief  begin final search */
                    H[joint_num].home_state = HOME_RISE_SEARCH_START;
                    immediate_state = 1;
                    break;
                }
                ABORT_CHECK(joint_num);
                break;

            case HOME_RISE_SEARCH_START:
                /** @brief  This state is called to start the final search for the
                   point where the home switch trips.  It moves at
                   'latch_vel' and looks for a rising edge on the switch */
                /** @brief  is the joint already moving? */
                if (joint->free_tp.active) {
                    /** @brief  yes, reset delay, wait until joint stops */
                    H[joint_num].pause_timer = 0;
                    break;
                }
                /** @brief  has delay timed out? */
                if (H[joint_num].pause_timer < (HOME_DELAY * servo_freq)) {
                    /** @brief  no, update timer and wait some more */
                    H[joint_num].pause_timer++;
                    break;
                }
                H[joint_num].pause_timer = 0;
                /** @brief  we should still be off of the switch */
                if (home_sw_active) {
                    rtapi_print_msg(RTAPI_MSG_ERR, _("Home switch active before start of latch move j=%d"), joint_num);
                    H[joint_num].home_state = HOME_IDLE;
                    break;
                }
                /** @brief  set up a move at 'latch_vel' to locate the switch */
                home_start_move(joint, H[joint_num].home_latch_vel);
                /** @brief  next state */
                H[joint_num].home_state = HOME_RISE_SEARCH_WAIT;
                break;

            case HOME_RISE_SEARCH_WAIT:
                /** @brief  This state is called while the machine is moving towards
                   the home switch on its final, low speed pass.  It
                   terminates when the switch is detected. If the move ends
                   or hits a limit before it hits the switch, the home is
                   aborted. */
                /** @brief  have we hit the home switch yet? */
                if (home_sw_active) {
                    /** @brief  yes, where do we go next? */
                    if (H[joint_num].home_flags & HOME_USE_INDEX) {
                        /** @brief  look for index pulse */
                        H[joint_num].home_state = HOME_INDEX_SEARCH_START;
                        immediate_state = 1;
                        break;
                    } else {
                        /** @brief  no index pulse, stop motion */
                        joint->free_tp.enable = 0;
                        /** @brief  go to next step */
                        H[joint_num].home_state = HOME_SET_SWITCH_POSITION;
                        immediate_state = 1;
                        break;
                    }
                }
                ABORT_CHECK(joint_num);
                break;

            case HOME_FALL_SEARCH_START:
                /** @brief  This state is called to start the final search for the
                   point where the home switch releases.  It moves at
                   'latch_vel' and looks for a falling edge on the switch */
                /** @brief  is the joint already moving? */
                if (joint->free_tp.active) {
                    /** @brief  yes, reset delay, wait until joint stops */
                    H[joint_num].pause_timer = 0;
                    break;
                }
                /** @brief  has delay timed out? */
                if (H[joint_num].pause_timer < (HOME_DELAY * servo_freq)) {
                    /** @brief  no, update timer and wait some more */
                    H[joint_num].pause_timer++;
                    break;
                }
                H[joint_num].pause_timer = 0;
                /** @brief  we should still be on the switch */
                if (!home_sw_active) {
                    rtapi_print_msg(RTAPI_MSG_ERR, _("Home switch inactive before start of latch move j=%d"), joint_num);
                    H[joint_num].home_state = HOME_IDLE;
                    break;
                }
                /** @brief  set up a move at 'latch_vel' to locate the switch */
                home_start_move(joint, H[joint_num].home_latch_vel);
                /** @brief  next state */
                H[joint_num].home_state = HOME_FALL_SEARCH_WAIT;
                break;

            case HOME_FALL_SEARCH_WAIT:
                /** @brief  This state is called while the machine is moving away from
                   the home switch on its final, low speed pass.  It
                   terminates when the switch is cleared. If the move ends or
                   hits a limit before it clears the switch, the home is
                   aborted. */
                /** @brief  have we cleared the home switch yet? */
                if (!home_sw_active) {
                    /** @brief  yes, where do we go next? */
                    if (H[joint_num].home_flags & HOME_USE_INDEX) {
                        /** @brief  look for index pulse */
                        H[joint_num].home_state = HOME_INDEX_SEARCH_START;
                        immediate_state = 1;
                        break;
                    } else {
                        /** @brief  no index pulse, stop motion */
                        joint->free_tp.enable = 0;
                        /** @brief  go to next step */
                        H[joint_num].home_state = HOME_SET_SWITCH_POSITION;
                        immediate_state = 1;
                        break;
                    }
                }
                ABORT_CHECK(joint_num);
                break;

            case HOME_SET_SWITCH_POSITION:
                /** @brief  This state is called when the machine has determined the
                   switch position as accurately as possible.  It sets the
                   current joint position to 'home_offset', which is the
                   location of the home switch in joint coordinates. */
                /** @brief  set the current position to 'home_offset' */
                if (H[joint_num].home_flags & HOME_ABSOLUTE_ENCODER) {
                    offset = H[joint_num].home_offset;
                } else {
                    offset = H[joint_num].home_offset - joint->pos_fb;
                }
                /** @brief  this moves the internal position but does not affect the
                   motor position */
                joint->pos_cmd += offset;
                joint->pos_fb += offset;
                joint->free_tp.curr_pos += offset;
                joint->motor_offset -= offset;
                if (H[joint_num].home_flags & HOME_ABSOLUTE_ENCODER) {
                    if (H[joint_num].home_flags & HOME_NO_FINAL_MOVE) {
                        H[joint_num].home_state = HOME_FINISHED;
                        immediate_state = 1;
                        H[joint_num].homed = 1; // finished absolute encoder
                        break;
                    }
                }
                /** @brief  next state */
                H[joint_num].home_state = HOME_FINAL_MOVE_START;
                immediate_state = 1;
                break;

            case HOME_INDEX_ONLY_START:
                /** @brief  This state is used if the machine has been pre-positioned
                   near the home position, and simply needs to find the
                   next index pulse.  It starts a move at latch_vel, and
                   sets index-enable, which tells the encoder driver to
                   reset its counter to zero and clear the enable when the
                   next index pulse arrives. */
                /** @brief  is the joint already moving? */
                if (joint->free_tp.active) {
                    /** @brief  yes, reset delay, wait until joint stops */
                    H[joint_num].pause_timer = 0;
                    break;
                }
                /** @brief  has delay timed out? */
                if (H[joint_num].pause_timer < (HOME_DELAY * servo_freq)) {
                    /** @brief  no, update timer and wait some more */
                    H[joint_num].pause_timer++;
                    break;
                }
                H[joint_num].pause_timer = 0;
                /** @brief  Although we don't know the exact home position yet, we
                   we reset the joint coordinates now so that screw error
                   comp will be appropriate for this portion of the screw
                   (previously we didn't know where we were at all). */
                /** @brief  set the current position to 'home_offset' */
                offset = H[joint_num].home_offset - joint->pos_fb;
                /** @brief  this moves the internal position but does not affect the
                   motor position */
                joint->pos_cmd += offset;
                joint->pos_fb += offset;
                joint->free_tp.curr_pos += offset;
                joint->motor_offset -= offset;
                /** @brief  set the index enable */
                H[joint_num].index_enable = 1;
                /** @brief  set up a move at 'latch_vel' to find the index pulse */
                home_start_move(joint, H[joint_num].home_latch_vel);
                /** @brief  next state */
                H[joint_num].home_state = HOME_INDEX_SEARCH_WAIT;
                break;

            case HOME_INDEX_SEARCH_START:
                /** @brief  This state is called after the machine has made a low
                   speed pass to determine the limit switch location. It
                   sets index-enable, which tells the encoder driver to
                   reset its counter to zero and clear the enable when the
                   next index pulse arrives. */
                /** @brief  set the index enable */
                H[joint_num].index_enable = 1;
                /** @brief  and move right into the waiting state */
                H[joint_num].home_state = HOME_INDEX_SEARCH_WAIT;
                immediate_state = 1;
                ABORT_CHECK(joint_num);
                break;

            case HOME_INDEX_SEARCH_WAIT:
                /** @brief  This state is called after the machine has found the
                   home switch and "armed" the encoder counter to reset on
                   the next index pulse. It continues at low speed until
                   an index pulse is detected, at which point it sets the
                   final home position.  If the move ends or hits a limit
                   before an index pulse occurs, the home is aborted. */
                /** @brief  has an index pulse arrived yet? encoder driver clears
                   enable when it does */
                if (H[joint_num].index_enable == 0) {
                    /** @brief  yes, stop motion */
                    joint->free_tp.enable = 0;
                    /** @brief  go to next step */
                    H[joint_num].home_state = HOME_SET_INDEX_POSITION;
                    immediate_state = 1;
                    break;
                }
                ABORT_CHECK(joint_num);
                break;

            case HOME_SET_INDEX_POSITION:
                /** @brief  This state is called when the encoder has been reset at
                   the index pulse position.  It sets the current joint
                   position to 'home_offset', which is the location of the
                   index pulse in joint coordinates. */
                /** @brief  set the current position to 'home_offset' */
                joint->motor_offset = -H[joint_num].home_offset;
                joint->pos_fb = joint->motor_pos_fb - (joint->backlash_filt + joint->motor_offset);
                joint->pos_cmd = joint->pos_fb;
                joint->free_tp.curr_pos = joint->pos_fb;

                if (H[joint_num].home_flags & HOME_INDEX_NO_ENCODER_RESET) {
                    /** @brief  Special case: encoder does not reset on index pulse.
                       This moves the internal position but does not affect
                       the motor position */
                    offset = H[joint_num].home_offset - joint->pos_fb;
                    joint->pos_cmd += offset;
                    joint->pos_fb += offset;
                    joint->free_tp.curr_pos += offset;
                    joint->motor_offset -= offset;
                }

                /** @brief  next state */
                H[joint_num].home_state = HOME_FINAL_MOVE_START;
                immediate_state = 1;
                break;

            case HOME_FINAL_MOVE_START:
                /** @brief  This state is called once the joint coordinate system is
                   set properly.  It moves to the actual 'home' position,
                   which is not necessarily the position of the home switch
                   or index pulse. */
                /** @brief  is the joint already moving? */
                if (joint->free_tp.active) {
                    /** @brief  yes, reset delay, wait until joint stops */
                    H[joint_num].pause_timer = 0;
                    break;
                }
                /** @brief  has delay timed out? */
                if (H[joint_num].pause_timer < (HOME_DELAY * servo_freq)) {
                    /** @brief  no, update timer and wait some more */
                    H[joint_num].pause_timer++;
                }
                H[joint_num].pause_timer = 0;

                // neg home sequence: sync final move
                if ((H[joint_num].home_sequence < 0) && !sync_ready(joint_num)) {
                    break; // not all joints at *this* state, wait for them
                }

                /** @brief  plan a final move to home position */
                joint->free_tp.pos_cmd = H[joint_num].home;
                /** @brief  if home_vel is set (>0) then we use that, otherwise we rapid there */
                if (H[joint_num].home_final_vel > 0) {
                    joint->free_tp.max_vel = fabs(H[joint_num].home_final_vel);
                    /** @brief  clamp on max vel for this joint */
                    if (joint->free_tp.max_vel > joint->vel_limit)
                        joint->free_tp.max_vel = joint->vel_limit;
                } else {
                    joint->free_tp.max_vel = joint->vel_limit;
                }
                /** @brief  start the move */
                joint->free_tp.enable = 1;
                H[joint_num].home_state = HOME_FINAL_MOVE_WAIT;
                break;

            case HOME_FINAL_MOVE_WAIT:
                /** @brief  This state is called while the machine makes its final
                   move to the home position.  It terminates when the machine
                   arrives at the final location. If the move hits a limit
                   before it arrives, the home is aborted. */

                // neg home sequence: sync final move
                if ((H[joint_num].home_sequence < 0) && !sync_ready(joint_num)) {
                    break; // not all joints at *this* state, wait for them
                }

                /** @brief  have we arrived (and stopped) at home? */
                if (!joint->free_tp.active) {
                    /** @brief  yes, stop motion */
                    joint->free_tp.enable = 0;
                    /** @brief  we're finally done */
                    H[joint_num].home_state = HOME_LOCK;
                    immediate_state = 1;
                    break;
                }
                if (joint->on_pos_limit || joint->on_neg_limit) {
                    /** @brief  on limit, check to see if we should trip */
                    if (!(H[joint_num].home_flags & HOME_IGNORE_LIMITS)) {
                        /** @brief  not ignoring limits, time to quit */
                        rtapi_print_msg(RTAPI_MSG_ERR, _("hit limit in home state j=%d"), joint_num);
                        H[joint_num].home_state = HOME_ABORT;
                        immediate_state = 1;
                        break;
                    }
                }
                break;

            case HOME_LOCK:
                if (H[joint_num].home_flags & HOME_UNLOCK_FIRST) {
                    SetRotaryUnlock(joint_num, 0);
                } else {
                    immediate_state = 1;
                }
                H[joint_num].home_state = HOME_LOCK_WAIT;
                break;

            case HOME_LOCK_WAIT:
                // if not yet locked, continue waiting
                if ((H[joint_num].home_flags & HOME_UNLOCK_FIRST) && GetRotaryIsUnlocked(joint_num))
                    break;

                // either we got here without a lock needed, or the
                // lock is now complete.
                H[joint_num].home_state = HOME_FINISHED;
                immediate_state = 1;
                break;

            case HOME_FINISHED:
                H[joint_num].homing = 0;
                H[joint_num].homed = 1; // finished
                H[joint_num].home_state = HOME_IDLE;
                if (!(H[joint_num].home_flags & HOME_ABSOLUTE_ENCODER)) {
                    joints[joint_num].free_tp.curr_pos = H[joint_num].home;
                }
                immediate_state = 1;
                H[joint_num].joint_in_sequence = 0;
                break;

            case HOME_ABORT:
                H[joint_num].homing = 0;
                H[joint_num].homed = 0;
                H[joint_num].joint_in_sequence = 0;
                joint->free_tp.enable = 0;
                H[joint_num].home_state = HOME_IDLE;
                H[joint_num].index_enable = 0;
                immediate_state = 1;
                break;

            default:
                /** @brief  should never get here */
                rtapi_print_msg(RTAPI_MSG_ERR, _("unknown state '%d' during homing j=%d"), H[joint_num].home_state, joint_num);
                H[joint_num].home_state = HOME_ABORT;
                immediate_state = 1;
                break;
        } /** @brief  end of switch(H[joint_num].home_state) */
    } while (immediate_state);

    return homing_flag;
} // base_1joint_state_machine()

static bool base_do_homing(void) {
    int joint_num;
    int homing_flag = 0;
    bool beginning_allhomed = get_allhomed();

    do_homing_sequence();
    /** @brief  loop thru joints, treat each one individually */
    for (joint_num = 0; joint_num < all_joints; joint_num++) {
        if (!H[joint_num].joint_in_sequence) {
            continue;
        }
        if (!GET_JOINT_ACTIVE_FLAG(&joints[joint_num])) {
            continue;
        }
        // DEFAULT joint homing state machine:
        homing_flag += base_1joint_state_machine(joint_num);
    }
    if (homing_flag > 0) { /** @brief  one or more joint is homing */
        homing_active = 1;
    } else { /** @brief  is a homing sequence in progress? */
        if (sequence_state == HOME_SEQUENCE_IDLE) {
            /** @brief  no, single joint only, we're done */
            homing_active = 0;
        }
    }
    // return 1 if homing completed this period
    if (!beginning_allhomed && get_allhomed()) {
        homing_active = 0;
        return 1;
    }
    return 0;
} // base_do_homing()

static void do_homing_sequence(void) {
    int i, ii;
    int seen;
    emcmot_joint_t *joint;
    int sequence_is_set = 0;

    switch (sequence_state) {
        case HOME_SEQUENCE_IDLE:
            current_sequence = 0;
            /** @brief  nothing to do */
            break;

        case HOME_SEQUENCE_DO_ONE_JOINT:
            // Expect one joint with home_state==HOME_START
            for (i = 0; i < all_joints; i++) {
                if (H[i].home_state == HOME_START) {
                    H[i].joint_in_sequence = 1;
                    current_sequence = ABS(H[i].home_sequence);
                } else {
                    H[i].joint_in_sequence = 0;
                }
            }
            sequence_is_set = 1;
            /** @brief  Fallthrough */

        case HOME_SEQUENCE_DO_ONE_SEQUENCE:
            // Expect multiple joints with home_state==HOME_START
            // specified by a negative sequence
            // Determine current_sequence and set H[i].joint_in_sequence
            // based on home_state[i] == HOME_START
            if (!sequence_is_set) {
                for (i = 0; i < all_joints; i++) {
                    if (H[i].home_state == HOME_START) {
                        if (sequence_is_set && (ABS(H[i].home_sequence) != current_sequence)) {
                            rtapi_print_msg(RTAPI_MSG_ERR, _("homing.c Unexpected joint=%d jseq=%d current_seq=%d\n"), i, H[i].home_sequence, current_sequence);
                        }
                        current_sequence = ABS(H[i].home_sequence);
                        sequence_is_set = 1;
                    }
                    H[i].joint_in_sequence = 1; // disprove
                    if ((H[i].home_state != HOME_START) || (current_sequence != ABS(H[i].home_sequence))) {
                        H[i].joint_in_sequence = 0;
                    }
                }
            }
            sequence_state = HOME_SEQUENCE_START;
            /** @brief  Fallthrough */

        case HOME_SEQUENCE_START:
            // Request to home all joints or a single sequence
            // A negative H[i].home_sequence means sync final move
            if (!sequence_is_set) {
                // sequence_is_set not otherwise established: home-all
                for (i = 0; i < EMCMOT_MAX_JOINTS; i++) {
                    H[i].joint_in_sequence = 1;
                    // unspecified joints have an unrealizable H[i].home_sequence:
                    if (H[i].home_sequence > 100) {
                        // docs: 'If HOME_SEQUENCE is not specified then this joint
                        //        will not be homed by the HOME ALL sequence'
                        H[i].joint_in_sequence = 0; // per docs
                    }
                }
                sequence_is_set = 1;
                current_sequence = 0;
            }
            /** @brief  Initializations */
            for (i = 0; i < all_joints; i++) {
                if (!H[i].joint_in_sequence)
                    continue;
                if ((H[i].home_flags & HOME_NO_REHOME) && H[i].homed) {
                    continue;
                } else {
                    H[i].homed = 0;
                }
                if (H[i].home_sequence < 0) {
                    // If a H[i].home_sequence is neg, find all joints that
                    // have the same ABS sequence value and make them the same.
                    // Note: overrides inconsistent mixed positive/negative settings
                    //       for H[ii].home_sequence
                    for (ii = 0; ii < all_joints; ii++) {
                        if (H[ii].home_sequence == ABS(H[i].home_sequence)) {
                            H[ii].home_sequence = H[i].home_sequence;
                        }
                    }
                }
            }
            for (i = 0; i < all_joints; i++) {
                if (!H[i].joint_in_sequence)
                    continue;
                if (H[i].home_state != HOME_IDLE && H[i].home_state != HOME_START) {
                    /** @brief  a home is already in progress, abort the home-all */
                    sequence_state = HOME_SEQUENCE_IDLE;
                    return;
                }
            }
            /** @brief  tell the world we're on the job */
            homing_active = 1;
            /** @brief  Fallthrough */

        case HOME_SEQUENCE_START_JOINTS:
            seen = 0;
            /** @brief  start all joints whose sequence number matches H[i].home_sequence */
            for (i = 0; i < all_joints; i++) {
                joint = &joints[i];
                if (ABS(H[i].home_sequence) == current_sequence) {
                    if (!H[i].joint_in_sequence)
                        continue;
                    /** @brief  start this joint */
                    joint->free_tp.enable = 0;
                    H[i].home_state = HOME_START;
                    seen++;
                }
            }
            if (seen || current_sequence == 0) {
                sequence_state = HOME_SEQUENCE_WAIT_JOINTS;
            } else {
                /** @brief  no joints have this sequence number, we're done */
                sequence_state = HOME_SEQUENCE_IDLE;
                /** @brief  tell the world */
                homing_active = 0;
            }
            break;

        case HOME_SEQUENCE_WAIT_JOINTS:
            seen = 0;
            for (i = 0; i < all_joints; i++) {
                if (!H[i].joint_in_sequence)
                    continue;
                // negative H[i].home_sequence means sync final move
                if (ABS(H[i].home_sequence) != current_sequence) {
                    /** @brief  this joint is not at the current sequence number, ignore it */
                    continue;
                }
                if (H[i].home_state != HOME_IDLE) {
                    /** @brief  still busy homing, keep waiting */
                    seen = 1;
                    continue;
                }
            }
            if (!seen) {
                /** @brief  all joints at this step have finished, move on to next step */
                current_sequence++;
                sequence_state = HOME_SEQUENCE_START_JOINTS;
            }
            break;

        default:
            /** @brief  should never get here */
            rtapi_print_msg(RTAPI_MSG_ERR, _("unknown state '%d' during homing sequence"), sequence_state);
            sequence_state = HOME_SEQUENCE_IDLE;
            homing_active = 0;
            break;
    }
} // do_homing_sequence()

static bool base_do_homing(void) {
    int joint_num;
    int homing_flag = 0;
    bool beginning_allhomed = get_allhomed();

    do_homing_sequence();
    /** @brief  loop thru joints, treat each one individually */
    for (joint_num = 0; joint_num < all_joints; joint_num++) {
        if (!H[joint_num].joint_in_sequence) {
            continue;
        }
        if (!GET_JOINT_ACTIVE_FLAG(&joints[joint_num])) {
            continue;
        }
        // DEFAULT joint homing state machine:
        homing_flag += base_1joint_state_machine(joint_num);
    }
    if (homing_flag > 0) { /** @brief  one or more joint is homing */
        homing_active = 1;
    } else { /** @brief  is a homing sequence in progress? */
        if (sequence_state == HOME_SEQUENCE_IDLE) {
            /** @brief  no, single joint only, we're done */
            homing_active = 0;
        }
    }
    // return 1 if homing completed this period
    if (!beginning_allhomed && get_allhomed()) {
        homing_active = 0;
        return 1;
    }
    return 0;
} // base_do_homing()
