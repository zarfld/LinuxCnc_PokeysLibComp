/**
 * @file PoKeysComp.h
 * @brief Main header file for the PoKeys HAL component in LinuxCNC.
 *
 * This file defines the central data structure @ref sPoKeysDevice, which represents
 * a PoKeys device instance. It also declares all related functions used for IO access,
 * PulseEngine v2 configuration, encoder reading, PoExtBus/PoNet communication,
 * and component initialization.
 *
 * @ingroup hal_component_pokeys
 */

#ifndef _POKEYS_COMP_H_
#define _POKEYS_COMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @defgroup hal_component_pokeys PoKeys HAL Component
 * @brief HAL interface for PoKeys devices and PulseEngine integration.
 *
 * This group contains all functions, structures, and definitions related to
 * the PoKeys HAL component for LinuxCNC.
 * @{
 */

/**
 * @class PoKeysHALComponent
 * @brief Represents a single PoKeys device and its runtime state.
 *
 * This class-like structure holds configuration, IO mappings, and
 * PulseEngine data for a single PoKeys device. All functions operating
 * on this structure are considered class methods.
 */
typedef struct {
    int device_number;
    // other configuration and runtime fields...
} PoKeysHALComponent;

// ========================== General Setup & I/O ===============================

/**
 * @typedef one_adcout_data_t
 * @brief Structure holding data for one analog output channel in the PoKeys HAL component.
 *
 * This structure is used by the HAL component to manage one analog output
 * signal. It contains pointers to HAL pins and local configuration data.
 *
 * The values are updated in real-time to reflect the current analog output
 * state and allow HAL users to control output voltage or current levels
 * within defined bounds.
 *
 * @var one_adcout_data_t::deb_out
 * Pointer to the HAL pin for the debounced output value (u32).

 * @var one_adcout_data_t::deb_setval
 * Pointer to the HAL pin used to set the debounced output value (u32).

 * @var one_adcout_data_t::value
 * Pointer to the HAL pin holding the final analog output value (float).

 * @var one_adcout_data_t::enable
 * Pointer to the HAL pin enabling or disabling the output channel (bit).

 * @var one_adcout_data_t::offset
 * Software offset added to the analog output value (float).

 * @var one_adcout_data_t::scale
 * Scaling factor applied to the analog output value (float).

 * @var one_adcout_data_t::high_limit
 * Upper limit for the analog output value (float).

 * @var one_adcout_data_t::low_limit
 * Lower limit for the analog output value (float).

 * @var one_adcout_data_t::max_v
 * Maximum physical output value supported by the hardware (float).

 * @var one_adcout_data_t::PinId
 * Identifier of the physical PoKeys pin associated with this analog output (u32).
 */
typedef struct {
    hal_u32_t *deb_out;
    hal_u32_t *deb_setval;
    hal_float_t *value;
    hal_bit_t *enable;
    hal_float_t offset;
    hal_float_t scale;
    hal_float_t high_limit;
    hal_float_t low_limit;
    hal_float_t max_v;
    hal_u32_t PinId;
} one_adcout_data_t;

/**
 * @typedef one_adcin_data_t
 * @brief Structure holding data for one analog input channel in the PoKeys HAL component.
 *
 * This structure contains the HAL pin references and conversion parameters
 * for processing an analog input signal. The raw input is scaled and offset
 * to produce the final processed value exposed to HAL.
 *
 * @var one_adcin_data_t::value_raw
 * Pointer to the HAL pin for the raw analog input value (float), as read from the PoKeys device.

 * @var one_adcin_data_t::value
 * Pointer to the HAL pin for the processed analog input value (float), after applying scale and offset.

 * @var one_adcin_data_t::scale
 * Scaling factor applied to the raw input value (float). Useful for converting ADC values to physical units.

 * @var one_adcin_data_t::offset
 * Offset applied after scaling the raw input value (float).
 */
typedef struct {
    hal_float_t *value_raw;
    hal_float_t *value;
    hal_float_t scale;
    hal_float_t offset;
} one_adcin_data_t;

/**
  * @typedef one_digiIO_data_t
  * @brief Structure representing a single digital I/O pin in the PoKeys HAL component.
  *
  * This structure holds the configuration and HAL pin references for a digital input or output pin.
  * It supports inverted logic and can also act as a simple counter input if needed.
  *
  * @var one_digiIO_data_t::digin_in
  * Pointer to the HAL pin representing the current digital input signal (non-inverted).

  * @var one_digiIO_data_t::digin_in_not
  * Pointer to the HAL pin representing the inverted digital input signal.

  * @var one_digiIO_data_t::digout_out
  * Pointer to the HAL pin representing the output signal sent to the digital output pin.

  * @var one_digiIO_data_t::digin_invert
  * Inversion flag for the input signal. If true, the input logic is interpreted as inverted.

  * @var one_digiIO_data_t::digout_invert
  * Inversion flag for the output signal. If true, the output logic is inverted before being sent to hardware.

  * @var one_digiIO_data_t::counter_value
  * Pointer to the HAL pin representing a simple counter value, incremented on rising edges.

  * @var one_digiIO_data_t::PinFunction
  * The current function assigned to the pin, e.g., digital input, output, or other capabilities (matches PoKeys API definitions).

  * @var one_digiIO_data_t::DigitalValueSet_ignore
  * Flag to indicate that output value should not be updated in this cycle (typically set during initial configuration).
  */
typedef struct {
    hal_bit_t *digin_in;
    hal_bit_t *digin_in_not;
    hal_bit_t *digout_out;
    hal_bit_t digin_invert;
    hal_bit_t digout_invert;

    hal_u32_t *counter_value;

    hal_u32_t PinFunction;

    bool DigitalValueSet_ignore;
} one_digiIO_data_t;

/**
  * @typedef all_IO_data_t
  * @brief Aggregated structure for managing all analog and digital I/O HAL connections for a PoKeys device.
  *
  * This structure encapsulates all HAL-related I/O data, including analog outputs (DAC/PWM),
  * analog inputs (ADC), and digital I/O pins. It serves as the main container for mapping HAL
  * pins to the PoKeys hardware configuration in the LinuxCNC HAL component.
  *
  * @var all_IO_data_t::adcout
  * Array of analog output channel data structures (DAC or PWM outputs), indexed from 0 to 5.

  * @var all_IO_data_t::adcout_pwm_period
  * PWM period in microseconds for analog output channels operating in PWM mode.

  * @var all_IO_data_t::adcout_deb_outv
  * Pointer to a HAL u32 pin that exposes the current analog output debug value (optional).

  * @var all_IO_data_t::adcin
  * Array of analog input channel data structures, indexed from 0 to 6.

  * @var all_IO_data_t::Pin
  * Array of digital input/output channel data structures, indexed from 0 to 54.
  * Each entry corresponds to a physical I/O pin on the PoKeys device.

  * @var all_IO_data_t::deb_out
  * Optional debug pointer for tracking output state of digital pins or other debugging information.
  */
typedef struct {
    one_adcout_data_t adcout[6];
    hal_u32_t adcout_pwm_period;
    hal_u32_t *adcout_deb_outv;

    one_adcin_data_t adcin[7];

    one_digiIO_data_t Pin[55];

    hal_u32_t *deb_out;
} all_IO_data_t;

/**
 * @brief Export HAL pins and parameters for PoKeys analog and digital I/O.
 *
 * This function creates and registers HAL pins and parameters for all supported
 * analog outputs, analog inputs, digital I/O, and counters based on the device
 * capabilities and configuration. It binds the relevant fields in the provided
 * `all_IO_data_t` structure to HAL pins using the specified `prefix`.
 *
 * @param prefix      The prefix string for naming HAL pins (typically the HAL component name).
 * @param extra_arg   Unused extra argument (reserved for future use or callback compatibility).
 * @param id          The HAL component ID.
 * @param Io_data     Pointer to the I/O data structure containing HAL pin references.
 *                    If NULL, a new structure is allocated via `hal_malloc`.
 * @param dev         Pointer to the PoKeys device structure with hardware capabilities.
 *
 * @return Zero on success, or a negative error code if HAL pin/parameter creation fails.
 *
 * @note If `Io_data` is NULL, this function allocates and initializes a new I/O data block
 *       and assigns it to the static global pointer `IO_data`.
 *
 * @see all_IO_data_t
 * @see one_adcout_data_t
 * @see one_adcin_data_t
 * @see one_digiIO_data_t
 * @memberof PoKeysHALComponent
 */
int PKIO_export_pins(char *prefix, long extra_arg, int id, all_IO_data_t *Io_data, sPoKeysDevice *dev);

/**
 * @brief Updates PoKeys I/O states from and to hardware.
 *
 * This function performs periodic synchronization between the PoKeys hardware device
 * and the corresponding LinuxCNC HAL pins. It:
 * - Retrieves current pin configuration, digital I/O states, analog input values,
 *   digital counters, and PWM settings from the device.
 * - Updates internal HAL structures (`IO_data`) with the retrieved values.
 * - Writes analog and digital output values from HAL back to the device if enabled.
 * - Handles logic such as scaling, clamping, and PWM duty calculation.
 * - Optionally reconfigures PWM period or enables/disables channels when values change.
 *
 * @param dev Pointer to the sPoKeysDevice structure representing the connected PoKeys device.
 *
 * @note The static pointer `IO_data` must be initialized with valid memory prior to calling this function.
 * @note The function uses `sleepdur` (if compiled with `ULAPI`) to avoid overrunning communication.
 *
 * @details
 * This function combines the following device interactions:
 * - `PK_PinConfigurationGet()`
 * - `PK_DigitalIOGet()` / `PK_DigitalIOSet()`
 * - `PK_AnalogIOGet()`
 * - `PK_DigitalCounterGet()`
 * - `PK_PWMConfigurationGet()` / `PK_PWMConfigurationSet()` / `PK_PWMUpdate()`
 *
 * @see PKIO_export_pins()
 * @see sPoKeysDevice
 * @see all_IO_data_t
 * @memberof PoKeysHALComponent
 */
void PKIO_Update(sPoKeysDevice *dev);

/**
 * @brief Initializes the PoKeys digital and analog I/O configuration.
 *
 * This function configures the I/O pins and PWM settings of the PoKeys device
 * based on either previously applied settings or the current device state.
 *
 * It performs the following actions:
 * - Reads the current pin configuration from the device via `PK_PinConfigurationGet()`.
 * - For each pin:
 *   - Sets the pin function and inversion flags (input/output polarity) based on `ApplyIniSettings`.
 *   - Compares and updates the pin configuration if changes are detected.
 * - Applies updated pin configuration to the device via `PK_PinConfigurationSet()` if needed.
 * - For devices with PWM outputs:
 *   - Retrieves PWM configuration using `PK_PWMConfigurationGet()`.
 *   - Applies period, channel enablement, and value settings (scale, offset, limits).
 *   - Calls `PK_PWMConfigurationSet()` if PWM configuration changes were detected.
 *
 * @param dev Pointer to the PoKeys device structure (`sPoKeysDevice`).
 *
 * @note The global `IO_data` structure must be initialized before calling this function.
 * @note PWM scaling and voltage limits are normalized and defaulted if not set.
 * @note The last PWM period is forcibly set to 2500 if not specified.
 * @note This function is typically called during HAL component initialization.
 *
 * @see ApplyIniSettings
 * @see PK_PinConfigurationGet()
 * @see PK_PinConfigurationSet()
 * @see PK_PWMConfigurationGet()
 * @see PK_PWMConfigurationSet()
 * @see all_IO_data_t
 * @memberof PoKeysHALComponent
 */
void PKIO_Setup(sPoKeysDevice *dev);

/**
 * @brief Reads and applies I/O configuration values for the PoKeys device from the INI file.
 *
 * This function loads pin functions, input/output inversion flags, analog input/output
 * scale and offset values, voltage limits, and PWM period from the [POKEYS] section
 * of the HAL component's INI configuration.
 *
 * It covers:
 * - Digital pin function and inversion flags:
 *   - `Pin_<n>_Function`
 *   - `DigIn_<n>_invert`
 *   - `DigOut_<n>_invert`
 * - Analog output (PWM) settings:
 *   - `AdcOut_<n>_offset`, `AdcOut_<n>_scale`
 *   - `AdcOut_<n>_high_limit`, `AdcOut_<n>_low_limit`, `AdcOut_<n>_max_v`
 *   - `AdcOut_<n>_enable`
 * - Global PWM period setting:
 *   - `AdcOut_PWM_Period`
 * - Analog input scale and offset values:
 *   - `AdcIn_<n>_scale`, `AdcIn_<n>_offset`
 *
 * @param dev Pointer to the PoKeys device structure (`sPoKeysDevice`) used to determine the number of digital and analog pins.
 *
 * @note The global `IO_data` pointer must point to a valid `all_IO_data_t` structure before calling this function.
 * @note Defaults are applied if keys are missing: digital pins get 0, analog input scale defaults to 1, offset to 0.
 * @note The function assumes the corresponding HAL pins and parameters were already created.
 *
 * @see all_IO_data_t
 * @see ini_read_int()
 * @see ini_read_float()
 * @memberof PoKeysHALComponent
 */
void PKIO_ReadIniFile(sPoKeysDevice *dev);

/**
 * @brief Writes the current PoKeys I/O configuration to the INI file.
 *
 * This function saves all relevant I/O parameters from the current HAL state
 * (`IO_data`) into the [POKEYS] section of the INI file for later reuse.
 * This includes:
 * - Digital pin configuration:
 *   - `Pin_<n>_Function`
 *   - `DigIn_<n>_invert`
 *   - `DigOut_<n>_invert`
 * - Analog output (PWM) configuration:
 *   - `AdcOut_<n>_offset`, `AdcOut_<n>_scale`
 *   - `AdcOut_<n>_high_limit`, `AdcOut_<n>_low_limit`, `AdcOut_<n>_max_v`
 *   - `AdcOut_<n>_enable`
 * - Global PWM period:
 *   - `AdcOut_PWM_Period`
 * - Analog input scale and offset:
 *   - `AdcIn_<n>_scale`, `AdcIn_<n>_offset`
 *
 * @param dev Pointer to the PoKeys device structure (`sPoKeysDevice`), used to determine pin and channel counts.
 *
 * @note The global `IO_data` pointer must point to a valid `all_IO_data_t` structure before calling this function.
 * @note This function is the counterpart to `PKIO_ReadIniFile()`, and is used for persisting configuration across restarts.
 *
 * @see all_IO_data_t
 * @see PKIO_ReadIniFile()
 * @see ini_write_int()
 * @see ini_write_float()
 * @memberof PoKeysHALComponent
 */
void PKIO_WriteIniFile(sPoKeysDevice *dev);

// ========================== Encoder Support ===================================
/**
 * @brief Bitfield definitions for encoder options (used with command 0x11, byte 4).
 *
 * These options control sampling modes, key/macro mapping and enabling for PoKeys encoders.
 */
typedef enum {
    PK_ENCODER_OPTION_ENABLE = (1 << 0),      ///< Enable encoder
    PK_ENCODER_OPTION_4X_SAMPLING = (1 << 1), ///< Enable 4x sampling (default is 1x)
    PK_ENCODER_OPTION_2X_SAMPLING = (1 << 2), ///< Enable 2x sampling (default is 1x)
    // bit 3 reserved
    PK_ENCODER_OPTION_KEYMAP_DIR_A = (1 << 4), ///< Direct key mapping for direction A
    PK_ENCODER_OPTION_MACRO_DIR_A = (1 << 5),  ///< Map to macro for direction A
    PK_ENCODER_OPTION_KEYMAP_DIR_B = (1 << 6), ///< Direct key mapping for direction B
    PK_ENCODER_OPTION_MACRO_DIR_B = (1 << 7)   ///< Map to macro for direction B
} ePK_EncoderOptions;

typedef enum { PK_FASTENCODER_CONF_CFG1 = (0x01), PK_FASTENCODER_CONF_CFG2 = (0x10), PK_FASTENCODER_DISABLE_4X_SAMPLING = (1 << 4), PK_FASTENCODER_INVERT_E1 = (1 << 5), PK_FASTENCODER_INVERT_E2 = (1 << 6), PK_FASTENCODER_INVERT_E3 = (1 << 7) } ePK_FastEncoderOptions;

typedef enum { PK_UFENCODER_INVERT_DIRECTION = (1 << 0), PK_UFENCODER_SIGNAL_MODE = (1 << 1), PK_UFENCODER_ENABLE_4X_SAMPLING = (1 << 2) } ePK_UltraFastEncoderOptions;

/**
 * @brief Structure representing a single encoder channel with HAL and PoKeys configuration.
 *
 * This structure encapsulates both standard LinuxCNC HAL encoder interface elements
 * and additional parameters required for configuring and communicating with a PoKeys device.
 *
 * The structure is typically used in arrays, where each element corresponds to one encoder instance.
 *
 * Canonical HAL pins:
 * - `count` (s32 out): Current encoder count.
 * - `position` (float out): Scaled encoder position.
 * - `velocity` (float out): Measured velocity (if available or computed).
 * - `reset` (bit in): Resets encoder count to zero when set.
 * - `index_enable` (bit in): Enables index pulse detection.

 * HAL parameter:
 * - `scale` (float rw): Scale factor for converting encoder counts to position units.
 *   Units: counts per position unit.

 * PoKeys-specific configuration:
 * - `encoderOptions` (u32 io): Bitmask for encoder options (see ePK_EncoderOptions):
 *   - Bit 0: Enable encoder
 *   - Bit 1: 4x sampling
 *   - Bit 2: 2x sampling
 *   - Bit 3: Reserved
 *   - Bit 4: Direct key mapping for direction A
 *   - Bit 5: Mapped to macro for direction A
 *   - Bit 6: Direct key mapping for direction B
 *   - Bit 7: Mapped to macro for direction B
 * - `channelApin` (u32 io): PoKeys pin number used for encoder channel A.
 * - `channelBpin` (u32 io): PoKeys pin number used for encoder channel B.
 *
 * @see PKENC_export_encoders
 * @see PKENC_Update
 */
typedef struct {
    // canonicaldevice interface pins
    hal_s32_t *count;        // pointer for "pin out s32 encoder.#.count[29]"
    hal_float_t *position;   // pointer for "pin out float encoder.#.position[29]"
    hal_float_t *velocity;   // pointer for "pin out float encoder.#.velocity[29]"
    hal_bit_t *reset;        // pointer for "pin in bit encoder.#.reset[29]"
    hal_bit_t *index_enable; // pointer for "pin in bit encoder.#.index-enable[29]"

    hal_float_t scale; // pointer for "param rw float encoder.#.scale[29]  "The scale factor used to convert counts to position units. It is in “counts per position unit”""

    // Custom parameters for communication with PoKeys
    hal_u32_t encoderOptions; // pointer for "pin io unsigned Encoders.#.encoderOptions [26]" see ePK_EncoderOptions
    hal_bit_t enable;         // pointer for "parameter io bit Encoders.#.enable [26]"  // Enable encoder
    hal_bit_t x4_sampling;    // pointer for "parameter io bit Encoders.#.x4_sampling [26]"  // 4x sampling
    hal_bit_t x2_sampling;    // pointer for "parameter io bit Encoders.#.x2_sampling [26]"  // 2x sampling
    hal_bit_t keymap_dirA;    // pointer for "parameter io bit Encoders.#.keymap_dirA [26]"  // Direct key mapping for direction A
    hal_bit_t macro_dirA;     // pointer for "parameter io bit Encoders.#.macro_dirA [26]"  // Map to macro for direction A
    hal_bit_t keymap_dirB;    // pointer for "parameter io bit Encoders.#.keymap_dirB [26]"  // Direct key mapping for direction B
    hal_bit_t macro_dirB;     // pointer for "parameter io bit Encoders.#.macro_dirB [26]"  // Map to macro for direction B

    hal_u32_t channelApin; // pointer for "pin io unsigned Encoders.#.channelApin [26]"         // Channel A encoder pin"
    hal_u32_t channelBpin; // pointer for "pin io unsigned Encoders.#.channelBpin [26]"         // Channel B encoder pin"

} one_encoder_data_t;

/**
 * @brief Structure representing all encoder channels and debug output for the PoKeys HAL component.
 *
 * This structure aggregates all encoder-related data in the system, including an array
 * of individual encoder configurations and a debug output pin.
 *
 * - `encoder[29]`: Array of encoder channel structures (`one_encoder_data_t`), each representing
 *   a separate encoder interface (up to 29 channels supported).
 *
 * - `encoder_deb_out` (s32 out): Debug output pin used for internal state tracking or error codes.
 *
 * This structure is typically allocated once per device and passed to encoder-related
 * setup and update routines.
 *
 * @see one_encoder_data_t
 * @see PKENC_export_encoders
 * @see PKENC_Update
 */
typedef struct {
    one_encoder_data_t encoder[29];

    // FastEncoder Parameters
    hal_u32_t FastEncodersOptions;       //
    hal_u32_t FastEncodersConfiguration; //  for "param io unsigned Encoders.#.FastEncodersConfiguration [26]" see ePK_FastEncoderOptions

    /* There are two different fast encoders configurations. On newer PoKeys56 and PoKeys57 series devices, only
    second configuration can be selected.
    Configuration 1: pins 1-2 as encoder 1, pins 3-4 as encoder 2, pins 15-16 as encoder 3
    Configuration 2: pins 1-2 as encoder 1, pins 5-6 as encoder 2, pins 15-16 as encoder 3

    Set to 1 to enable fast encoders with configuration 1, set to 10 to enable configuration 2, set to 2 to read
setup
     */
    hal_bit_t encoderFastEnable; //  for "param io bit Encoders.#.encoderFastEnable [26]"  // Enable fast encoder
    hal_bit_t alternativeconfig; //  to switch between two configurations (if applicable)
    hal_bit_t FastEncodersInvert[3];
    hal_bit_t disable_4x_sampling;

    /* UltraFastEncoder Parameters

    */
    uint8_t UltraFastEncoderConfiguration; // Ultra fast encoder configuration (see protocol specification for details)
    uint8_t UltraFastEncoderOptions;       // Ultra fast encoder additional options
    hal_s32_t UltraFastEncoderFilter;      // Ultra fast encoder digital filter setting

    hal_bit_t UltraFastEncoderOptions_INVERT_DIRECTION;
    hal_bit_t UltraFastEncoderOptions_SIGNAL_MODE;
    hal_bit_t UltraFastEncoderOptions_ENABLE_4X_SAMPLING;
    hal_bit_t UltraFastEncoderOptions_RESET_ON_INDEX;

    hal_s32_t *encoder_deb_out; // pin out s32 deb.out;

} all_encoder_data_t;

/**
 * @brief Exports HAL encoder pins for the PoKeys device.
 *
 * This function creates and registers all necessary HAL pins for a specified number of encoders
 * based on the given joint count. It assigns pointers to the internal `encoder_data` structure
 * for later use during runtime updates.
 *
 * The following HAL pins are exported per encoder channel:
 * - `encoder.N.count` (s32, output): Current encoder count
 * - `encoder.N.position` (float, output): Calculated position
 * - `encoder.N.velocity` (float, output): Calculated velocity
 * - `encoder.N.reset` (bit, input): Resets encoder count
 * - `encoder.N.index-enable` (bit, input): Enables index pulse processing
 *
 * Additionally, the following debug pin is exported:
 * - `encoder.deb.out` (s32, output): Debug value for diagnostics
 *
 * If `Encoder_data` is `NULL`, memory is allocated via `hal_malloc`.
 *
 * @param[in] prefix       HAL prefix string used for naming the exported pins
 * @param[in] extra_arg    Reserved, not used
 * @param[in] id           HAL component ID
 * @param[in] njoints      Number of encoders (typically equal to number of joints)
 * @param[in] Encoder_data Pointer to encoder data structure (can be NULL to auto-allocate)
 * @return 0 on success, or negative HAL error code on failure
 *
 * @note This function must be called during HAL component initialization.
 * @see PKENC_Update()
 * @see one_encoder_data_t
 * @see all_encoder_data_t
 */

int PKEncoder_export_pins(char *prefix, long extra_arg, int id, int njoints, all_encoder_data_t *Encoder_data);
/**
 * @brief Exports HAL parameters related to encoder configuration.
 *
 * This function registers additional HAL parameters for each encoder channel,
 * such as scale factor and PoKeys-specific encoder settings like pin assignments
 * and options. These parameters allow runtime configuration and status feedback.
 *
 * For each encoder (up to `njoints`), the following HAL parameters are created:
 * - `encoder.N.scale` (float, rw): Scale factor used to convert counts to position units
 * - `Encoders.N.encoderOptions` (u32, rw): Bitmask defining encoder behavior
 * - `Encoders.N.channelApin` (u32, rw): Assigned PoKeys pin for channel A
 * - `Encoders.N.channelBpin` (u32, rw): Assigned PoKeys pin for channel B
 *
 * @param[in] prefix   HAL prefix string used for naming the exported parameters
 * @param[in] extra_arg Reserved, not used
 * @param[in] id       HAL component ID
 * @param[in] njoints  Number of encoders (typically equal to number of joints)
 * @return 0 on success, or a negative HAL error code on failure
 *
 * @note This function should be called during component initialization, after memory allocation
 *       for `encoder_data` has been completed.
 *
 * @see PKEncoder_export_pins()
 * @see one_encoder_data_t
 */
int PKEncoder_export_params(char *prefix, long extra_arg, int id, int njoints);

/**
 * @brief Updates encoder data from the PoKeys device and writes values to HAL pins.
 *
 * This function retrieves encoder values (basic and ultra-fast) from the PoKeys device
 * and updates the corresponding HAL pins for count and position. It also checks if the
 * encoder should be reset (based on HAL reset pin or during initialization).
 * If reset is requested, it sets the encoder value on the device to zero.
 *
 * The following values are updated for each encoder:
 * - `encoder.#.count`         – the raw encoder count value
 * - `encoder.#.position`      – the scaled position based on count and scale parameter
 * - `encoder.#.velocity`      – currently not set (placeholder)
 *
 * If `reset` pin is high or this is the first update (`initEncodersDone == false`),
 * the encoder is reset on the device using `PK_EncoderValuesSet()`.
 *
 * Debug values are written to `encoder_data->encoder_deb_out` to help trace progress or hangs.
 *
 * @param[in,out] dev Pointer to the initialized PoKeys device structure
 *
 * @note The function supports both basic and ultra-fast encoders.
 *       Fast encoders are not handled in the current implementation (commented out).
 *
 * @see PK_EncoderValuesGet()
 * @see PK_EncoderValuesSet()
 * @see one_encoder_data_t
 */
void PKEncoder_Update(sPoKeysDevice *dev);
/**
 * @brief Placeholder function to perform encoder-specific setup actions.
 *
 * This function is intended to configure encoder-related parameters or device settings
 * before starting regular encoder operation. Currently, it is not implemented.
 *
 * You may extend this function to:
 * - Configure encoder sampling modes (1x, 2x, 4x)
 * - Assign pins for channel A/B
 * - Set scaling or filtering options
 * - Apply configuration stored from INI
 *
 * @param[in,out] dev Pointer to the initialized PoKeys device structure
 */
void PKEncoder_Setup(sPoKeysDevice *dev);
/**
 * @brief Reads encoder-related settings from the INI configuration file.
 *
 * This function is intended to load encoder configuration values (e.g., scale, pin assignments,
 * inversion flags, or other options) from an INI file section, typically used for persisting
 * user-defined settings between sessions.
 *
 * Currently, this function is a placeholder and does not perform any operation.
 *
 * You may extend it to:
 * - Load encoder scale factors
 * - Load encoder pin mappings (channel A/B)
 * - Load encoder mode or options
 *
 * @param[in,out] dev Pointer to the initialized PoKeys device structure
 */
void PKEncoder_ReadIniFile(sPoKeysDevice *dev);
/**
 * @brief Writes encoder-related configuration to the INI file.
 *
 * This function is intended to save current encoder configuration values (e.g., scale, pin assignments,
 * inversion flags, or other options) into an INI file section. This enables restoring consistent
 * settings across restarts or deployments.
 *
 * Currently, this function is a placeholder and does not perform any operation.
 *
 * You may extend it to:
 * - Write encoder scale factors
 * - Write encoder pin mappings (channel A/B)
 * - Write encoder options or flags
 *
 * @param[in] dev Pointer to the initialized PoKeys device structure
 */
void PKEncoder_WriteIniFile(sPoKeysDevice *dev);

// ========================== PoExtBus (e.g., HCT595) ===========================

/**
 * @brief Updates the PoExtBus data from and to the PoKeys device.
 *
 * This function synchronizes the state of the PoExtBus (PoKeys Extension Bus), which supports
 * digital input and output via external shift registers or I/O expanders.
 *
 * It performs the following steps:
 * - Reads the current state from the PoKeys device using `PK_PoExtBusGet()`
 * - Updates the `PoExtBus_digin_in` and `PoExtBus_digin_in_not` HAL pins for all bus channels
 * - Reads desired digital output values from `PoExtBus_digout_out` HAL pins
 * - Applies optional inversion logic for outputs
 * - Compares desired vs actual state (`PoExtBus_DataSet` vs `PoExtBus_DataGet`)
 * - If there are changes, updates the PoKeys device using `PK_PoExtBusSet()`
 *
 * Debug values are stored in `PoExtBus_deb_out` for diagnostics.
 *
 * @param[in,out] dev Pointer to the initialized PoKeys device structure
 */
void PKPoExtBus_Update(sPoKeysDevice *dev);

/**
 * @brief Initializes or configures the PoExtBus interface on the PoKeys device.
 *
 * This function is intended to set up the PoExtBus (PoKeys Extension Bus), which allows
 * communication with external modules such as I/O expanders or shift registers.
 *
 * Currently, this function is a placeholder and does not perform any operation.
 *
 * You may extend it to:
 * - Configure the PoExtBus timing or mode
 * - Initialize attached modules (e.g., HCT595 shift registers, PoExtBusOC16)
 * - Load module-specific configuration from the INI file
 *
 * @param[in] dev Pointer to the initialized PoKeys device structure
 */
void PKPoExtBus_Setup(sPoKeysDevice *dev);

// ========================== PoNet Support =====================================

/**
 * @brief Structure holding HAL parameters and pin references for one PoNET device.
 *
 * This structure represents a single PoNET module connected to the PoKeys device,
 * including identification, addressing, and data exchange over the PoNET bus.
 *
 * It supports:
 * - Read-only identification and configuration parameters
 * - 16 status input pins (HAL_OUT)
 * - 16 status output pins (HAL_IN)
 */
typedef struct {
    hal_u32_t PoNET_moduleID;       // RO Parameter
    hal_u32_t PoNET_i2cAddress;     // RO Parameter
    hal_u32_t PoNET_moduleType;     // RO Parameter
    hal_u32_t PoNET_moduleSize;     // RO Parameter
    hal_u32_t PoNET_moduleOptions;  // RO Parameter
    hal_u32_t *PoNET_statusIn[16];  // OUT pin
    hal_u32_t *PoNET_statusOut[16]; // IN pin
} one_PoNET_data_t;

/**
 * @brief Represents a single button and LED pair on the kbd48CNC PoNET module.
 *
 * This structure holds HAL pointers to the button input and LED output for a single key
 * on a PoKeys kbd48CNC keyboard extension module.
 */
typedef struct {
    hal_bit_t *LED;
    hal_bit_t *Button;

} one_kbd48CNCButton_data_t;

/**
 * @brief Contains all PoNET-related data for communication with PoKeys devices.
 *
 * This structure manages the state and HAL representation of all connected PoNET devices,
 * including:
 * - Up to 16 generic PoNET modules
 * - A kbd48CNC keyboard module with 48 keys (each with LED and Button)
 * - Status and control values like PWM and brightness
 */

typedef struct {
    one_PoNET_data_t PoNET[16];   /**< Data for up to 16 PoNET modules */
    hal_u32_t *PoNET_PWMduty;     /**< HAL input pin for controlling the PoNET PWM duty cycle */
    hal_u32_t *PoNET_lightValue;  /**< HAL input pin for the global PoNET light value */
    hal_u32_t *PoNET_PoNETstatus; /**< HAL output pin for status reporting */
    hal_u32_t PoNET_DevCount;     /**< Read-only parameter: Number of active PoNET devices */

    hal_bit_t kbd48CNC_available;       /**< Flag indicating availability of the kbd48CNC module */
    hal_u32_t kbd48CNC_PoNetID;         /**< PoNET ID of the kbd48CNC module */
    hal_u32_t *kbd48CNC_KeyBrightness;  /**< HAL input pin for controlling key brightness */
    hal_u32_t *kbd48CNC_prevBrightness; /**< HAL output pin for previous brightness state */
    hal_u32_t *kbd48CNC_lightValue;     /**< HAL input pin for the kbd48CNC light value */

    one_kbd48CNCButton_data_t *kbd48CNCio[48]; /**< Array of 48 key-button mappings for the kbd48CNC */
    uint8_t kbd48CNC_Counter;                  /**< Internal counter used for kbd48CNC state updates */

    hal_s32_t *deb_out; /**< Debug output pin for internal state information */
} all_PoNET_data_t;

/**
 * @brief Exports HAL pins and parameters for PoNET modules and optionally kbd48CNC devices.
 *
 * This function initializes and exports all relevant HAL pins and parameters for PoNET modules
 * connected to a PoKeys device. It dynamically allocates memory for internal state (if needed)
 * and assigns module metadata (ID, type, size, options) and I/O channels as HAL pins.
 *
 * Additionally, if a `kbd48CNC` module is detected, it exports button and LED pins
 * for up to 48 keys and sets up backlight brightness and control pins.
 *
 * The function supports up to 16 PoNET modules with up to 16 I/O channels each.
 *
 * @param prefix         The string prefix for the HAL pin and parameter names (e.g. "pokeys").
 * @param extra_arg      Reserved extra argument, unused.
 * @param id             Unique HAL component ID used for pin/parameter creation.
 * @param njoints        Number of PoNET modules expected or to be scanned.
 * @param poNET_data     Pointer to an existing `all_PoNET_data_t` structure or NULL to auto-allocate.
 * @param dev            Pointer to the PoKeys device structure. Must not be NULL.
 *
 * @return 0 on success, negative value on failure (e.g. malloc failure, pin export error).
 *
 * @note This function uses `PK_PoNETGetModuleSettings()` to query each module slot (0–15)
 *       and only exports pins for detected and valid modules.
 * @note Memory allocation for `PoNet_data->kbd48CNCio[]` elements is done on demand.
 * @note Requires `dev->info.iPoNET` > 0 (i.e., PoNET support available on the device).
 */
int PKPoNet_export_pins(char *prefix, long extra_arg, int id, int njoints, all_PoNET_data_t *poNET_data, sPoKeysDevice *dev);

/**
 * @brief Updates the state of the connected PoNET modules, specifically kbd48CNC.
 *
 * This function performs a full update cycle for the `kbd48CNC` device connected via PoNET,
 * including:
 * - Fetching module settings and light (backlight) values
 * - Reading key/button states from the device
 * - Reflecting input state into HAL pins
 * - Updating LED states based on HAL input
 * - Writing updated status information back to the module
 *
 * The function contains debug outputs and a `deb_out` pin to trace execution and states.
 * It supports fault handling and retries for communication with the device.
 *
 * @param dev Pointer to the sPoKeysDevice structure, representing the connected PoKeys device.
 *
 * @note This function only performs actions if `kbd48CNC_available` is non-zero,
 *       indicating that a kbd48CNC PoNET module has been detected.
 *
 * @note The `kbd48CNC` buttons are processed in a remapped ID order using a 4-group offset scheme,
 *       to match hardware layout with software index logic.
 *
 * @warning This function assumes that `PoNet_data` has already been initialized and populated
 *          by `PKPoNet_export_pins()`. If not, behavior is undefined.
 */
void PKPoNet_Update(sPoKeysDevice *dev);

/**
 * @brief Initializes or configures PoNET-related settings for the given device.
 *
 * This function is a placeholder for any future setup logic required for PoNET modules.
 * Currently, it does not perform any operations.
 *
 * @param dev Pointer to the sPoKeysDevice structure representing the connected PoKeys device.
 *
 * @note Intended to be called during component initialization or device setup.
 *       Can be expanded to handle default configurations, pre-allocations, or hardware checks.
 */
void PKPoNet_Setup(sPoKeysDevice *dev);

// ========================== PulseEngine v2 ===================================
/**
 * @brief pokeys_homing_algorithm_t
 * @memberof PoKeysHALComponent
 */
typedef enum {
    PK_PEv2Homing_OnHomeStop = (1 << 3),              // Axis  in IDLE
    PK_PEv2Homing_OnHomeArmEncoder = (1 << 2),        // Start Homing procedure
    PK_PEv2Homing_OnHomeReverseDirection = (1 << 1),  // Cancel Homing procedure
    PK_PEv2Homing_OnHomeReducedSpeed = (1 << 0),      // Cancel Homing procedure
    PK_PEv2Homing_OutHomeStop = (1 << 7),             // Axis  in IDLE
    PK_PEv2Homing_OutHomeArmEncoder = (1 << 6),       // Start Homing procedure
    PK_PEv2Homing_OutHomeReverseDirection = (1 << 5), // Cancel Homing procedure
    PK_PEv2Homing_OutHomeReducedSpeed = (1 << 4),     // Cancel Homing procedure
} pokeys_homing_algorithm_t;

/**
 * @enum pokeys_axis_state_ext_t
 * @brief Extended PoKeys Pulse Engine axis states used for homing synchronization.
 *
 * These states are not part of the original PoKeysLib `ePK_PEAxisState` enum,
 * but are introduced to support additional homing steps required by LinuxCNC-style sequences.
 *
 * They are used internally by the HAL component to track the progress of encoder arming
 * and synchronized final moves after PoKeys has reached `PK_PEAxisState_axHOME`.
 *
 * @note These states are assigned IDs that continue beyond the original PoKeysLib enum values.
 */
typedef enum {

    /**
     * Axis has finished (pokeys)homing sequence.
     */
    PEAxisStateEx_axReadyToFinalizeHoming = 3, // (linuxcnc spec additional state) pokeys ready to finalize PKhoming

    /**
     * Axis is ready to arm the encoder after PoKeys internal homing is complete.
     * This state is used to synchronize with other axes in the same homing sequence.
     */
    PEAxisStateEx_axReadyToArmEncoder = 4, // (linuxcnc spec additional state) pokeys ready to finalize PKhoming
    /**
     * Axis is preparing to reset encoder position to zero.
     * Sent after PoKeys internal homing is done, just before `PK_PEAxisCommand_axARMENCODER`.
     */
    PEAxisStateEx_HOMINGARMENCODER = 17,

    /**
     * Axis is waiting for all other joints in the sequence to be ready
     * before executing the final move to the home position.
     */
    PEAxisStateEx_HOMINGWaitFINALMOVE = 18,

    /**
     * Axis is currently performing the final coordinated move to its home position.
     */
    PEAxisStateEx_HOMINGFINALMOVE = 19,
} pokeys_axis_state_ext_t;

/**
 * @enum pokeys_home_status_t
 * @brief Internal homing state machine used by the PoKeys HAL component
 *        to coordinate LinuxCNC-style homing sequences in conjunction with
 *        PoKeys Pulse Engine v2.
 *
 * These states are used for synchronized transitions between multiple joints
 * via `PEv2_HomingStateSyncedTrigger()`. They do **not** reflect the PoKeysLib
 * `AxesState`, but a separate synchronization FSM maintained inside the HAL component.
 */
typedef enum {
    /**
     * Axis is idle and not part of a current homing sequence.
     */
    PK_Homing_axIDLE = 0,

    /**
     * Homing procedure has been initiated (e.g., via `PK_PEAxisCommand_axHOMINGSTART`).
     * PoKeys internal homing starts here.
     */
    PK_Homing_axHOMINGSTART = 1,

    /**
     * Encoder arming phase: Axis resets its position to the configured zero point.
     * This usually happens after the PoKeys homing sequence has completed.
     */
    PK_Homing_axARMENCODER = 2,

    /**
     * Axis waits for all other joints in the same sequence to reach this state
     * before it triggers a synchronized final move to the home position.
     */
    PK_Homing_axHOMINGWaitFinalMove = 3,

    /**
     * Axis executes the final coordinated move to its home position (absolute).
     */
    PK_Homing_axHOMINGFinalMove = 4,

    /**
     * Homing procedure has been aborted.
     */
    PK_Homing_axHOMINGCancel = 5,

    /**
     * Internal PoKeys homing procedure has completed.
     * This is typically the last state *before* encoder arming or final move is triggered.
     *
     * @note Despite the name, this is **not** the final state of the entire LinuxCNC homing sequence.
     */
    PK_Homing_axHOMINGFinalize = 6
} pokeys_home_status_t;

/**
 * @brief Homing command for the axis
 * @memberof PoKeysHALComponent
 */
typedef enum {
    PK_PEAxisCommand_axIDLE = 0,                // Axis  in IDLE
    PK_PEAxisCommand_axHOMINGSTART = 1,         // Start Homing procedure
    PK_PEAxisCommand_axARMENCODER = 2,          // reset position to zeros
    PK_PEAxisCommand_axHOMINGWaitFinalMove = 3, // move to homeposition
    PK_PEAxisCommand_axHOMINGFinalMove = 4,     // move to homeposition
    PK_PEAxisCommand_axHOMINGCancel = 5,        // Cancel Homing procedure
    PK_PEAxisCommand_axHOMINGFinalize = 6,      // Finish Homing procedure
} pokeys_home_command_t;

/**
 * @brief State machine transitions for homing logic
 *
 * This diagram visualizes the internal transition states for
 * pokeys_home_status_t and PEv2 AxesState (intAxesState).
 *
 * HomingStatus (`PK_Homing_ax...`) states manage synchronization
 * with LinuxCNC, while PEv2_AxesState (`PK_PEAxisState_...`) tracks
 * internal PulseEngine status.
 *
 * @dot
 * digraph HomingCombinedStateMachine {
 *   rankdir=LR;
 *
 *   // HomingStatus states
 *   HS_IDLE               [label="HS: IDLE"];
 *   HS_HOMINGSTART        [label="HS: HOMINGSTART"];
 *   HS_HOMINGFinalize     [label="HS: HOMINGFinalize"];
 *   HS_ARMENCODER         [label="HS: ARMENCODER"];
 *   HS_HOMINGWaitFinalMove[label="HS: HOMINGWaitFinalMove"];
 *   HS_HOMINGFinalMove    [label="HS: HOMINGFinalMove"];
 *   HS_HOMINGCancel       [label="HS: HOMINGCancel"];
 *
 *   // PEv2 AxesState nodes
 *   AS_HOMINGSTART     [label="AS: HOMINGSTART"];
 *   AS_HOMINGSEARCH    [label="AS: HOMINGSEARCH"];
 *   AS_HOMINGBACK      [label="AS: HOMINGBACK"];
 *   AS_HOMING_RESETTING[label="AS: HOMING_RESETTING"];
 *   AS_HOME            [label="AS: HOME"];
 *   AS_RUNNING         [label="AS: RUNNING"];
 *   AS_READY           [label="AS: READY"];
 *   AS_STOPPED         [label="AS: STOPPED"];
 *
 *   // HomingStatus transitions
 *   HS_IDLE               -> HS_HOMINGSTART;
 *   HS_HOMINGSTART        -> HS_HOMINGFinalize;
 *   HS_HOMINGFinalize     -> HS_ARMENCODER;
 *   HS_ARMENCODER         -> HS_HOMINGWaitFinalMove;
 *   HS_HOMINGWaitFinalMove-> HS_HOMINGFinalMove;
 *   HS_HOMINGFinalMove    -> HS_HOMINGFinalize;
 *   HS_HOMINGFinalMove    -> HS_IDLE [label="already at position"];
 *   HS_HOMINGSTART        -> HS_HOMINGCancel;
 *   HS_HOMINGCancel       -> HS_IDLE;

 *   // PEv2 AxesState transitions (as observed in intAxesState)
 *   AS_HOMINGSTART     -> AS_HOMINGSEARCH;
 *   AS_HOMINGSEARCH    -> AS_HOMINGBACK;
 *   AS_HOMINGBACK      -> AS_HOMING_RESETTING;
 *   AS_HOMING_RESETTING-> AS_HOME;
 *   AS_HOME            -> AS_RUNNING [label="Final move"];
 *   AS_RUNNING         -> AS_HOME [label="Reached position"];
 *   AS_HOME            -> AS_READY;
 *   AS_READY           -> AS_STOPPED;

 *   // Crosslinks between the two state machines
 *   HS_HOMINGSTART        -> AS_HOMINGSTART     [style=dashed, label="PK_PEv2_HomingStart"];
 *   HS_HOMINGFinalize     -> AS_HOME            [style=dashed, label="PK_PEv2_HomingFinish"];
 *   HS_ARMENCODER         -> AS_HOME            [style=dashed, label="PK_PEv2_PositionSet"];
 *   HS_HOMINGFinalMove    -> AS_RUNNING         [style=dashed, label="PK_PEv2_PulseEngineMove"];
 *   HS_IDLE               -> AS_STOPPED         [style=dashed, label="index_enable"];
 * }
 * @enddot
 */

/**
 * @brief PEv2_data_t
 * @memberof PoKeysHALComponent
 */
typedef struct {
    hal_s32_t *PEv2_deb_out;
    hal_s32_t *PEv2_deb_estop;
    hal_s32_t *PEv2_deb_axxisout[8];
    hal_bit_t *PEv2_deb_ishoming[8];
    hal_bit_t *PEv2_deb_inposition[8];
    hal_bit_t *PEv2_deb_PosMode[8];
    hal_bit_t *PEv2_deb_PosModeAct[8];
    hal_s32_t *PEv2_deb_velmode_count[8];
    hal_s32_t *PEv2_deb_posmode_count[8];
    hal_bit_t *PEv2_deb_doMove[8];
    hal_float_t *PEv2_deb_RefSpeed[8];
    hal_float_t *PEv2_deb_RefPos[8];
    hal_float_t *PEv2_deb_RefPosSpeed[8];
    hal_u32_t *PEv2_nrOfAxes;
    hal_u32_t *PEv2_maxPulseFrequency;
    hal_u32_t *PEv2_bufferDepth;
    hal_u32_t *PEv2_slotTiming;
    hal_bit_t *PEv2_params_ApplyIniSettings;
    /**
     * @var PEv2_AxesState
     * @brief HAL pin array representing the current state of each axis in PulseEngine v2.
     *
     * These pins are referenced in the homing component via `H[joint_num].PEv2_AxesState`.
     *
     * @see H
     * @see pokeys_homecomp.c
     */
    hal_u32_t *PEv2_AxesState[8];
    hal_u32_t *PEv2_AxesCommand[8];
    hal_u32_t PEv2_AxesConfig[8];
    hal_u32_t PEv2_SoftLimitMaximum[8];
    hal_u32_t PEv2_SoftLimitMinimum[8];
    hal_u32_t PEv2_HomingSpeed[8];
    hal_u32_t *PEv2_HomingStatus[8];
    hal_bit_t *PEv2_index_enable[8];
    hal_u32_t PEv2_HomingReturnSpeed[8];
    hal_u32_t PEv2_HomeOffsets[8];
    hal_u32_t *PEv2_ProbePosition[8];
    hal_u32_t *PEv2_ProbeMaxPosition[8];
    hal_s32_t *PEv2_CurrentPosition[8];
    hal_s32_t *PEv2_PositionSetup[8];
    hal_u32_t *PEv2_ReferencePositionSpeed[8];
    hal_u32_t *PEv2_ReferencePosition[8];
    hal_u32_t *PEv2_ReferenceSpeed[8];
    hal_float_t PEv2_MaxSpeed[8];
    hal_float_t PEv2_MaxAcceleration[8];
    hal_float_t PEv2_MaxDecceleration[8];
    hal_float_t *PEv2_params_Feedback_Encoder_Id[8];
    hal_s32_t PEv2_ZeroPosition[8];
    hal_s32_t PEv2_ArmPosition[8];
    hal_s32_t PEv2_HomePosition[8];

    hal_s32_t PEv2_PositionScale[8];
    hal_s32_t PEv2_PositionOffset[8];

    hal_float_t *PEv2_joint_vel_cmd[8];
    hal_float_t *PEv2_joint_pos_cmd[8];
    hal_float_t *PEv2_joint_pos_fb[8];
    hal_bit_t *PEv2_joint_out_home[8];
    hal_bit_t *PEv2_joint_in_position[8];
    hal_bit_t *PEv2_joint_kb_jog_active[8];
    hal_bit_t *PEv2_joint_wheel_jog_active[8];
    hal_s32_t *PEv2_stepgen_TYPE[8];
    hal_float_t PEv2_stepgen_HOME[8];
    hal_float_t PEv2_stepgen_STEPGEN_MAXVEL[8];
    hal_float_t PEv2_stepgen_STEPGEN_MAXACCEL[8];

    hal_float_t *PEv2_stepgen_DEADBAND[8];
    hal_float_t *PEv2_stepgen_MAX_OUTPUT[8];
    hal_float_t *PEv2_stepgen_ENCODER_SCALE[8];
    hal_float_t PEv2_stepgen_STEP_SCALE[8];
    hal_float_t PEv2_stepgen_MIN_LIMIT[8];
    hal_float_t PEv2_stepgen_MAX_LIMIT[8];

    hal_float_t PEv2_stepgen_HOME_OFFSET[8];
    hal_float_t PEv2_stepgen_HOME_SEARCH_VEL[8];
    hal_float_t PEv2_stepgen_HOME_LATCH_VEL[8];
    hal_float_t *PEv2_stepgen_HOME_FINAL_VEL[8];
    hal_s32_t *PEv2_stepgen_HOME_IGNORE_LIMITS[8];
    hal_u32_t PEv2_MPGjogMultiplier[8];
    hal_u32_t PEv2_MPGjogEncoder[8];
    hal_u32_t PEv2_MPGjogDivider[8];
    hal_u32_t PEv2_HomeBackOffDistance[8];
    hal_bit_t *PEv2_digin_Error_in[8];
    hal_bit_t *PEv2_digin_Error_in_not[8];
    hal_u32_t *PEv2_MiscInputStatus;
    hal_u32_t *PEv2_BacklashWidth[8];
    hal_u32_t *PEv2_BacklashRegister[8];
    hal_u32_t *PEv2_BacklashAcceleration[8];
    hal_s32_t PEv2_home_sequence[8];
    hal_s32_t PEv2_AxisEnabled[8];
    hal_s32_t PEv2_AxisInverted[8];
    hal_s32_t PEv2_AxisInternalPlanner[8];
    hal_s32_t PEv2_AxisPositionMode[8];
    hal_s32_t PEv2_AxisInvertedHome[8];
    hal_s32_t PEv2_AxisSoftLimitEnabled[8];
    hal_s32_t PEv2_AxisEnabledMasked[8];
    hal_u32_t PEv2_AxesSwitchConfig[8];
    hal_u32_t PEv2_HomingAlgorithm[8];
    hal_bit_t PEv2_HomeAlg_OnHome_Stop[8];
    hal_bit_t PEv2_HomeAlg_OnHome_ArmEncoder[8];
    hal_bit_t PEv2_HomeAlg_OnHome_RevDirection[8];
    hal_bit_t PEv2_HomeAlg_OnHome_ReducedSpeed[8];
    hal_bit_t PEv2_HomeAlg_OutHome_Stop[8];
    hal_bit_t PEv2_HomeAlg_OutHome_ArmEncoder[8];
    hal_bit_t PEv2_HomeAlg_OutHome_RevDirection[8];
    hal_bit_t PEv2_HomeAlg_OutHome_ReducedSpeed[8];
    hal_u32_t PEv2_digin_Home_Offset[8];
    hal_u32_t PEv2_digin_SoftLimit_PosMin[8];
    hal_u32_t PEv2_digin_SoftLimit_PosMax[8];
    hal_bit_t PEv2_digin_LimitN_Enabled[8];
    hal_bit_t PEv2_digin_LimitP_Enabled[8];
    hal_bit_t PEv2_digin_Home_Enabled[8];
    hal_bit_t PEv2_digin_Home_OnLimitN[8];
    hal_bit_t PEv2_digin_Home_OnLimitP[8];
    hal_bit_t PEv2_digin_LimitN_invert[8];
    hal_bit_t PEv2_digin_LimitP_invert[8];
    hal_bit_t PEv2_digin_Home_invert[8];
    hal_u32_t PEv2_digin_LimitN_Pin[8];
    hal_u32_t PEv2_digin_LimitN_Filter[8];
    hal_u32_t PEv2_digin_LimitP_Pin[8];
    hal_u32_t PEv2_digin_LimitP_Filter[8];
    hal_u32_t PEv2_digin_Home_Pin[8];
    hal_u32_t PEv2_digin_Home_Filter[8];
    hal_u32_t PEv2_digout_AxisEnable_Pin[8];
    hal_bit_t PEv2_digout_AxisEnable_invert[8];
    hal_bit_t *PEv2_digin_Misc_in[8];
    hal_bit_t *PEv2_digin_Misc_in_not[8];
    hal_bit_t *PEv2_digin_Probe_in[8];
    hal_bit_t *PEv2_digin_Probe_in_not[8];
    hal_u32_t PEv2_digin_Probe_Filter[8];
    hal_u32_t *PEv2_digin_SoftLimit_in[8];
    hal_bit_t *PEv2_digout_AxisEnabled_out[8];
    hal_bit_t *PEv2_digin_AxisEnabled_in[8];
    hal_bit_t *PEv2_digout_LimitOverride_out[8];
    hal_bit_t *PEv2_digin_LimitN_in[8];
    hal_bit_t *PEv2_digin_LimitN_in_not[8];
    hal_bit_t *PEv2_digin_LimitN_DedicatedInput[8];
    hal_bit_t *PEv2_digin_LimitP_in[8];
    hal_bit_t *PEv2_digin_LimitP_in_not[8];
    hal_bit_t *PEv2_digin_LimitP_DedicatedInput[8];
    hal_bit_t *PEv2_digin_Home_in[8];
    hal_bit_t *PEv2_digin_Home_in_not[8];
    hal_bit_t *PEv2_digin_Home_DedicatedInput[8];
    hal_bit_t *PEv2_digout_AxisEnable_out[8];
    hal_u32_t PEv2_PulseEngineEnabled;
    hal_u32_t PEv2_PulseGeneratorType;
    hal_bit_t PEv2_PG_swap_stepdir;
    hal_bit_t PEv2_PG_extended_io;
    hal_u32_t PEv2_ChargePumpEnabled;
    hal_u32_t *PEv2_PulseEngineActivated;
    hal_u32_t *PEv2_PulseEngineState;
    hal_u32_t *PEv2_PulseEngineStateSetup;
    hal_u32_t *PEv2_LimitOverride;
    hal_u32_t *PEv2_LimitOverrideSetup;
    hal_bit_t *PEv2_digin_Probed_in;
    hal_bit_t *PEv2_digin_Emergency_in;
    hal_bit_t *PEv2_digin_Emergency_in_not;
    hal_bit_t *PEv2_digout_Emergency_out;
    hal_bit_t *PEv2_digout_ExternalRelay_out[8];
    hal_bit_t *PEv2_digout_ExternalOC_out[8];
    hal_u32_t PEv2_digin_Emergency_Pin;
    hal_u32_t PEv2_digin_Emergency_invert;
    hal_u32_t PEv2_digout_Emergency_Pin;
    hal_u32_t PEv2_digin_Probe_Pin;
    hal_u32_t PEv2_digin_Probe_invert;
    hal_u32_t *PEv2_AxisEnabledMask;
    hal_u32_t *PEv2_AxisEnabledStatesMask;
    hal_u32_t *PEv2_ExternalRelayOutputs;
    hal_u32_t *PEv2_ExternalOCOutputs;
    hal_u32_t *PEv2_HomingStartMaskSetup;
    hal_u32_t *PEv2_ProbeStartMaskSetup;
    hal_u32_t *PEv2_ProbeStatus;
    hal_float_t *PEv2_ProbeSpeed;
    hal_u32_t *PEv2_BacklashCompensationEnabled;
} PEv2_data_t;

/**
 * @brief Export HAL pins and parameters for the PoKeys Pulse Engine v2 (PEv2).
 *
 * This function creates and registers all necessary HAL pins and parameters
 * for the PoKeys PEv2 device, based on the provided device configuration.
 * It exposes runtime data and control paths for each axis (up to 8) and
 * global system states. If `PEv2_data` is `NULL`, a new memory block is
 * allocated via `hal_malloc`. If `dev` is `NULL`, an error is reported.
 *
 * The exported pins include:
 * - Debugging outputs (`deb_out`, `deb_estop`, axis-specific diagnostics)
 * - Motion control feedback and command pins (`joint-pos-cmd`, `joint-vel-cmd`, etc.)
 * - Axis configuration and position parameters (limits, offsets, scaling, etc.)
 * - Pulse engine state, mode, and configuration
 * - Homing, probing, and limit switch digital input states
 * - Emergency stop and external output controls
 * - Encoder feedback and stepgen configuration values
 * - MPG jogging and switch logic masks
 *
 * This function is typically called during component initialization to ensure
 * all data structures are properly linked to LinuxCNC's HAL framework.
 *
 * @param prefix     The HAL instance name prefix (e.g., "pokeys").
 * @param extra_arg  Unused (reserved for future use).
 * @param comp_id    The HAL component ID.
 * @param Pev2_data  Pointer to PEv2 data structure. If `NULL`, it is allocated.
 * @param dev        Pointer to the PoKeys device descriptor.
 * @return `0` on success, or negative error code if HAL pin/param creation fails.
 */

int PKPEv2_export_pins(char *prefix, long extra_arg, int comp_id, PEv2_data_t *Pev2_data, sPoKeysDevice *dev);
/**
 * @brief Retrieves the value of a specific bit in a byte.
 *
 * Extracts the value of the bit at the specified position from the input byte.
 *
 * @param in_Byte The input byte to read from.
 * @param Bit_Id The bit position to read (0–7).
 *
 * @return `true` if the bit at position `Bit_Id` is set (1), `false` if it is clear (0).
 *
 * @note Bit positions outside the range 0–7 are not checked and may cause undefined behavior.
 *
 * @ingroup PoKeys_BitManipulation
 */
bool Get_BitOfByte(uint8_t in_Byte, int Bit_Id);

/**
 * @brief Sets or clears a specific bit in a byte.
 *
 * Modifies the input byte by setting or clearing the bit at the specified position,
 * depending on the value of the `value` parameter.
 *
 * @param in_Byte The original byte to be modified.
 * @param Bit_Id The bit position to modify (0–7).
 * @param value If `true`, the bit is set to 1; if `false`, the bit is cleared to 0.
 *
 * @return The modified byte with the specified bit updated.
 *
 * @note Bit positions outside the range 0–7 are not checked and may cause undefined behavior.
 *
 * @ingroup PoKeys_BitManipulation
 */
uint8_t Set_BitOfByte(uint8_t in_Byte, int Bit_Id, bool value);

/**
 * @brief Reads and updates the HAL pin states for limit and home switches for a specific axis.
 *
 * This function reads the current status of the limit+ (P), limit- (N), and home switches
 * for a given axis (index `i`) from the bit-mapped status variables. These are typically
 * obtained from `PK_PEv2_StatusGet()` or related functions.
 *
 * The values are interpreted according to the pin configuration:
 * - If a specific pin is assigned (e.g., `PEv2_digin_LimitP_Pin[i] > 0`), the status
 *   is read from the bitfield and written into the HAL pins.
 * - If no pin is assigned but the feature is enabled, the values will be updated elsewhere,
 *   e.g., from dedicated inputs handled in `PEv2_Status2Get()`.
 *
 * Inversion is respected based on the `.invert[]` configuration for each signal type.
 *
 * @param dev Pointer to the PoKeys device instance.
 * @param i   Index of the axis (joint) to process.
 *
 * @note This function does not call `PK_PEv2_StatusGet()` directly; it expects that the
 *       relevant bitfield values have already been populated.
 *
 * @see PEv2_Status2Get()
 * @see PK_PEv2_StatusGet()
 * @ingroup PoKeys_PEv2_IO
 */
void Read_digin_LimitHome_Pins(sPoKeysDevice *dev, int i);

/**
 * @brief Retrieves and processes the current status of the PoKeys Pulse Engine v2 (PEv2).
 *
 * This function calls `PK_PEv2_StatusGet()` to read current state and configuration
 * values from the PoKeys Pulse Engine v2 and updates the corresponding HAL pins
 * and internal variables accordingly.
 *
 * It processes:
 * - General engine configuration (number of axes, max frequency, buffer depth, etc.)
 * - Engine status flags (enabled, activated, charge pump, generator type)
 * - Axis enable mask and limit override
 * - Current engine state (running, stopped, homing, probing, etc.)
 * - Emergency input polarity and status
 *
 * Depending on the machine power state (`HAL_Machine_On`), the function also sets
 * `PEv2_PulseEngineStateSetup` accordingly.
 *
 * Special behavior:
 * - If the state is `peHOME` and homing is active, `Homing_active` will be reset.
 * - In case of `peSTOP_EMERGENCY`, emergency input pins are explicitly set.
 *
 * @param dev Pointer to the PoKeys device structure.
 * @return `PK_OK` on success or the error code from `PK_PEv2_StatusGet()` on failure.
 *
 * @note Updates several HAL pins and internal flags like `Homing_active`.
 * @note Applies sleep delay if `ULAPI` is defined.
 *
 * @see PK_PEv2_StatusGet
 * @see PEv2_data
 * @see ePoKeysPEState
 */
int32_t PEv2_StatusGet(sPoKeysDevice *dev);

/**
 * @brief Reads and updates the state of dedicated limit and home inputs from the PoKeys Pulse Engine v2.
 *
 * This function queries the PoKeys device for the current status of the dedicated
 * negative limit switches, positive limit switches, and home switches using
 * `PK_PEv2_Status2Get()`.
 *
 * For each axis, it checks whether a dedicated pin is assigned for each type of input.
 * If not, it uses software inversion settings and enable flags to update the
 * appropriate HAL pins (input, inverted input, and dedicated input).
 *
 * This function supports both dedicated hardware lines and virtual assignments via HAL.
 * Bitwise decoding is done using `Get_BitOfByte()` per axis and input type.
 *
 * @param dev Pointer to the PoKeys device structure.
 * @return `PK_OK` on success, or error code from `PK_PEv2_Status2Get()` on failure.
 *
 * @note If `ULAPI` is defined, a `usleep()` delay is applied after reading the status.
 *
 * @see PK_PEv2_Status2Get
 * @see Get_BitOfByte
 * @see PEv2_data->PEv2_digin_LimitN_*
 * @see PEv2_data->PEv2_digin_LimitP_*
 * @see PEv2_data->PEv2_digin_Home_*
 */
int32_t PEv2_Status2Get(sPoKeysDevice *dev);

/**
 * @brief Sets the state of external relay and open-collector outputs on the PoKeys device.
 *
 * This function configures the external relay and open-collector (OC) outputs
 * available on Pulse Engine v2–capable PoKeys devices (if extended I/O is enabled).
 *
 * The function first reads the current output states from the device using
 * `PK_PEv2_ExternalOutputsGet()` and stores them in the corresponding HAL pin values.
 *
 * Then, the desired output states are composed based on the values of HAL pins:
 * - `PEv2_digout_ExternalRelay_out[]` for relay outputs
 * - `PEv2_digout_ExternalOC_out[]` for OC outputs
 *
 * Bitwise packing of the 8-bit outputs is done using `Set_BitOfByte()`, following the
 * hardware-specific wiring pattern described in the code comment.
 *
 * If the target states differ from the current values on the device, the new states are
 * set using `PK_PEv2_ExternalOutputsSet()`. The operation is retried once in case of failure.
 *
 * @param dev Pointer to the PoKeys device structure.
 * @return `PK_OK` on success or the error code returned by `PK_PEv2_ExternalOutputsGet()` or `PK_PEv2_ExternalOutputsSet()`.
 *
 * @note Requires `PEv2_PG_extended_io` to be enabled for output access.
 * @note Uses `Set_BitOfByte()` to construct byte values from individual output bits.
 * @note Applies delays using `usleep()` if compiled with `ULAPI`.
 *
 * @see PK_PEv2_ExternalOutputsGet
 * @see PK_PEv2_ExternalOutputsSet
 * @see Set_BitOfByte
 */
int32_t PEv2_ExternalOutputsSet(sPoKeysDevice *dev);

/**
 * @brief Configures and applies Pulse Engine v2 settings to the PoKeys device.
 *
 * This function checks and sets multiple Pulse Engine configuration parameters
 * based on both the current device state (`dev->PEv2`) and user-defined or
 * default settings stored in `PEv2_data`. These include:
 * - Number of enabled axes
 * - Charge pump enable state
 * - Step/dir signal swap
 * - Extended I/O usage
 * - Emergency switch polarity
 * - Axis enabled mask
 *
 * The function determines whether an update to the configuration is needed (`doSetup`),
 * and if so, calls `PK_PEv2_PulseEngineSetup()` to apply it. Upon success, it persists
 * the changes with `PK_SaveConfiguration()`.
 *
 * Additionally, the function sets some device-specific defaults for supported PoKeys
 * device types, such as PoKeys57CNC and PoKeys57U.
 *
 * @param dev Pointer to the PoKeys device structure.
 *
 * @retval PK_OK on success (via `PK_PEv2_PulseEngineSetup` internally)
 * @retval other error codes returned by `PK_PEv2_PulseEngineSetup()` or `PK_SaveConfiguration()` on failure
 *
 * @note Adds delays via `usleep()` when compiled in user-space (`ULAPI`) to ensure communication timing.
 * @note Internally modifies `PEv2_data->PEv2_PulseGeneratorType` using `Set_BitOfByte()` helper for bit flags.
 *
 * @see PK_PEv2_PulseEngineSetup
 * @see PK_SaveConfiguration
 * @see PEv2_AdditionalParametersSet
 * @see Set_BitOfByte
 */
int32_t PEv2_PulseEngineSetup(sPoKeysDevice *dev);

/**
 * @brief Reads additional Pulse Engine v2 parameters from the PoKeys device.
 *
 * This function retrieves the current additional configuration of the Pulse Engine v2,
 * such as the emergency stop input pin, using `PK_PEv2_AdditionalParametersGet()`.
 * If the configuration is successfully read and either `ApplyIniSettings` is false
 * or no HAL configuration is specified for the emergency pin, the value is stored in
 * `PEv2_data->PEv2_digin_Emergency_Pin` after adjusting the index.
 *
 * This allows later logic to determine whether the emergency input pin should be
 * configured or left unchanged.
 *
 * @param dev Pointer to the PoKeys device structure.
 *
 * @retval PK_OK on success (currently not explicitly returned — consider adding `return PK_OK;`)
 * @retval error code if `PK_PEv2_AdditionalParametersGet()` fails (also not currently returned)
 *
 * @note Includes a short delay via `usleep()` if `ULAPI` is defined to accommodate userspace execution timing.
 *
 * @see PK_PEv2_AdditionalParametersGet
 * @see PEv2_AdditionalParametersSet
 */
int32_t PEv2_AdditionalParametersGet(sPoKeysDevice *dev);
/**
 * @brief Updates and sets additional Pulse Engine v2 parameters, such as the emergency input pin.
 *
 * This function checks whether Pulse Engine v2 additional parameters (e.g., the emergency stop input pin)
 * need to be updated on the PoKeys device, and if so, applies the new configuration.
 * It reads the current configuration using `PK_PEv2_AdditionalParametersGet()` and compares it with
 * the desired values defined in `PEv2_data`. If discrepancies are found and `ApplyIniSettings` is true,
 * the values are updated on the device using `PK_PEv2_AdditionalParametersSet()` and saved via
 * `PK_SaveConfiguration()`.
 *
 * If the emergency input pin is mapped to a regular PoKeys pin (ID >= 9), it also ensures that the
 * corresponding pin function is correctly set to `PK_PinCap_digitalInput`.
 *
 * @param dev Pointer to the PoKeys device structure.
 *
 * @retval PK_OK on successful update or no change needed.
 * @retval error code (e.g., PK_ERR_*) if a communication or configuration error occurred.
 *
 * @note The function includes retry logic and short delays (`usleep`) if `ULAPI` is defined,
 *       ensuring compatibility with non-realtime environments.
 *
 * @see PK_PEv2_AdditionalParametersGet
 * @see PK_PEv2_AdditionalParametersSet
 * @see PK_SaveConfiguration
 */
int32_t PEv2_AdditionalParametersSet(sPoKeysDevice *dev);

/**
 * @brief Triggers a synchronized homing state transition for all axes in a given homing sequence.
 *
 * This function checks whether all axes assigned to a given homing sequence have reached a specific
 * required homing state. If all are ready, it transitions them to the specified next homing state
 * and optionally triggers PoKeys Pulse Engine homing actions (e.g., start, finalize, encoder arm, etc.).
 *
 * The function supports transitions through key homing states like:
 * - `PK_Homing_axHOMINGSTART`: Initializes homing for the sequence and sets the start mask.
 * - `PK_Homing_axARMENCODER`: Sets the axis position to zero before the final move.
 * - `PK_Homing_axHOMINGFinalMove`: Triggers a position move to the final home position.
 * - `PK_Homing_axHOMINGFinalize`: Completes the homing procedure and ensures Pulse Engine state is correct.
 *
 * If any axis is not ready (i.e., not in the required state), the function aborts early.
 *
 * @param[in] dev              Pointer to the PoKeys device structure.
 * @param[in] seq              The homing sequence number to process.
 * @param[in] RequiredState    The required state that all joints in the sequence must currently be in.
 * @param[in] NextState        The homing state to transition to for all joints in the sequence.
 *
 * @return int32_t
 * - `0` if the transition was successfully triggered.
 * - `1` if any joint in the sequence is not in the required state or an error occurs.
 *
 * @note The function sets internal flags and masks used by the Pulse Engine API.
 *       Certain transitions involve setting configuration bits or triggering motion commands.
 * @memberof PoKeysHALComponent
 */
int32_t PEv2_HomingStateSyncedTrigger(sPoKeysDevice *dev, int seq, pokeys_home_status_t RequiredState, pokeys_home_status_t NextState);

/**
 * @brief Retrieves and parses the axis configuration from the PoKeys Pulse Engine v2.
 *
 * This function calls `PK_PEv2_AxisConfigurationGet()` to read the configuration for the specified axis
 * and updates the corresponding `PEv2_data` fields based on the retrieved information. It interprets
 * the bitmask flags for axis behavior and switch configuration, and conditionally applies values from
 * the device into `PEv2_data`, unless `ApplyIniSettings` is true (in which case values are preserved).
 *
 * Additionally, this function populates numerous auxiliary parameters such as speeds, filters,
 * soft limits, stepgen scaling, homing algorithm configuration, MPG settings, and enable outputs.
 *
 * @param dev Pointer to the PoKeys device structure.
 * @param AxisId The index of the axis to retrieve configuration for.
 *
 * @return PK_OK (0) on success, or an error code if `PK_PEv2_AxisConfigurationGet()` fails.
 *
 * @note Only fields that are unset (i.e., zero or false) in `PEv2_data` will be overwritten,
 *       unless `ApplyIniSettings == false`, in which case full sync from device is performed.
 *
 * @see PK_PEv2_AxisConfigurationGet
 * @ingroup PEv2_Configuration
 * @ingroup PoKeys_Axis
 * @memberof PoKeysHALComponent
 */
int32_t PEv2_AxisConfigurationGet(sPoKeysDevice *dev, int AxisId);

/**
 * @brief Set the axis configuration of the PoKeys device.
 * @memberof PoKeysHALComponent
 */
int32_t PEv2_AxisConfigurationSet(sPoKeysDevice *dev, int AxisId);

/**
 * @brief Configures the PulseEngine v2 using current settings.
 * @memberof PoKeysHALComponent
 */
int PEv2_PulseEngineSetup(sPoKeysDevice *dev);

/**
 * @brief Applies INI-based configuration to PulseEngine and IO.
 * @memberof PoKeysHALComponent
 */
void PKPEv2_Setup(sPoKeysDevice *dev);

/**
 * @brief Updates PulseEngine feedback and status.
 * @memberof PoKeysHALComponent
 */
void PKPEv2_Update(sPoKeysDevice *dev, bool HAL_Machine_On);
/** @} */ // end of hal_component_pokeys

#ifdef __cplusplus
}
#endif

#endif // _POKEYS_COMP_H_