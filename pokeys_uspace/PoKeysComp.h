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

int PKEncoder_export_pins(char *prefix, long extra_arg, int id, int njoints, all_encoder_data_t *Encoder_data)
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
void PKPoExtBus_Setup(sPoKeysDevice *dev)
    // ========================== PoNet Support =====================================
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
void PKPoNet_Setup(sPoKeysDevice *dev)

    // ========================== PulseEngine v2 ===================================

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

/**
  * @brief Triggers a synchronized homing event.
  * @memberof PoKeysHALComponent
  */
void PEv2_HomingStateSyncedTrigger(sPoKeysDevice *dev, int home_seq, int old_state, int new_state);

/**
  * @brief Reads axis configuration from the device.
  * @memberof PoKeysHALComponent
  */
void PEv2_AxisConfigurationGet(sPoKeysDevice *dev, int axis);

/**
  * @brief Writes axis configuration to the device.
  * @memberof PoKeysHALComponent
  */
void PEv2_AxisConfigurationSet(sPoKeysDevice *dev, int axis);

/** @} */ // end of hal_component_pokeys

#ifdef __cplusplus
}
#endif

#endif // _POKEYS_COMP_H_