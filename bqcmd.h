#ifndef BQCMD_H
#define BQCMD_H
#define MANUFACTURER_BLOCK_ACCESS_COMMAND 0x44
// https://github.com/Testato/SoftwareWire/blob/master/SoftwareWire.h
#define SOFTWAREWIRE_BUFSIZE  32

#include <Arduino.h>
#include <Wire.h>
#include <avr/pgmspace.h>

// Flash-resident (PROGMEM) string constants used by the BitFieldInfo / MBACommandInfo tables below.
// Declared individually (instead of inline PSTR()) because PSTR() expands to a GCC statement-
// expression, which is not a valid constant-expression inside a static array initializer.
static const char FSTR_CUV[] PROGMEM = "CUV";
static const char FSTR_CELL_UNDERVOLTAGE[] PROGMEM = "Cell Undervoltage";
static const char FSTR_DETECTED[] PROGMEM = "Detected";
static const char FSTR_NOT_DETECTED[] PROGMEM = "Not Detected";
static const char FSTR_COV[] PROGMEM = "COV";
static const char FSTR_CELL_OVERVOLTAGE[] PROGMEM = "Cell Overvoltage";
static const char FSTR_OCC1[] PROGMEM = "OCC1";
static const char FSTR_OVERCURRENT_DURING_CHARGE_1[] PROGMEM = "Overcurrent During Charge 1";
static const char FSTR_OCC2[] PROGMEM = "OCC2";
static const char FSTR_OVERCURRENT_DURING_CHARGE_2[] PROGMEM = "Overcurrent During Charge 2";
static const char FSTR_OCD1[] PROGMEM = "OCD1";
static const char FSTR_OVERCURRENT_DURING_DISCHARGE_1[] PROGMEM = "Overcurrent During Discharge 1";
static const char FSTR_OCD2[] PROGMEM = "OCD2";
static const char FSTR_OVERCURRENT_DURING_DISCHARGE_2[] PROGMEM = "Overcurrent During Discharge 2";
static const char FSTR_RSVD[] PROGMEM = "RSVD";
static const char FSTR_RESERVED[] PROGMEM = "Reserved";
static const char FSTR_EMPTY[] PROGMEM = "";
static const char FSTR_AOLDL[] PROGMEM = "AOLDL";
static const char FSTR_OVERLOAD_DURING_DISCHARGE_LATCH[] PROGMEM = "Overload During Discharge Latch";
static const char FSTR_ASCCL[] PROGMEM = "ASCCL";
static const char FSTR_SHORT_CIRCUIT_DURING_CHARGE_LATCH[] PROGMEM = "Short-Circuit During Charge Latch";
static const char FSTR_ASCDL[] PROGMEM = "ASCDL";
static const char FSTR_SHORT_CIRCUIT_DURING_DISCHARGE_LATCH[] PROGMEM = "Short-Circuit During Discharge Latch";
static const char FSTR_OTC[] PROGMEM = "OTC";
static const char FSTR_OVERTEMPERATURE_DURING_CHARGE[] PROGMEM = "Overtemperature During Charge";
static const char FSTR_OTD[] PROGMEM = "OTD";
static const char FSTR_OVERTEMPERATURE_DURING_DISCHARGE[] PROGMEM = "Overtemperature During Discharge";
static const char FSTR_CUVC[] PROGMEM = "CUVC";
static const char FSTR_CELL_UNDERVOLTAGE_COMPENSATED[] PROGMEM = "Cell Undervoltage Compensated";
static const char FSTR_OTF[] PROGMEM = "OTF";
static const char FSTR_OVERTEMPERATURE_FET[] PROGMEM = "Overtemperature FET";
static const char FSTR_PTO[] PROGMEM = "PTO";
static const char FSTR_PRECHARGE_TIMEOUT[] PROGMEM = "Precharge Timeout";
static const char FSTR_PTOS[] PROGMEM = "PTOS";
static const char FSTR_PRECHARGE_TIMEOUT_SUSPEND[] PROGMEM = "Precharge Timeout Suspend";
static const char FSTR_CTO[] PROGMEM = "CTO";
static const char FSTR_CHARGE_TIMEOUT[] PROGMEM = "Charge Timeout";
static const char FSTR_CTOS[] PROGMEM = "CTOS";
static const char FSTR_CHARGE_TIMEOUT_SUSPEND[] PROGMEM = "Charge Timeout Suspend";
static const char FSTR_OC[] PROGMEM = "OC";
static const char FSTR_OVERCHARGE[] PROGMEM = "Overcharge";
static const char FSTR_CHGC[] PROGMEM = "CHGC";
static const char FSTR_OVERCHARGING_CURRENT[] PROGMEM = "Overcharging Current";
static const char FSTR_CHGV[] PROGMEM = "CHGV";
static const char FSTR_OVERCHARGING_VOLTAGE[] PROGMEM = "Overcharging Voltage";
static const char FSTR_PCHGC[] PROGMEM = "PCHGC";
static const char FSTR_OVER_PRECHARGE_CURRENT[] PROGMEM = "Over-Precharge Current";
static const char FSTR_UTC[] PROGMEM = "UTC";
static const char FSTR_UNDERTEMPERATURE_DURING_CHARGE[] PROGMEM = "Undertemperature During Charge";
static const char FSTR_UTD[] PROGMEM = "UTD";
static const char FSTR_UNDERTEMPERATURE_DURING_DISCHARGE[] PROGMEM = "Undertemperature During Discharge";
static const char FSTR_COVL[] PROGMEM = "COVL";
static const char FSTR_CELL_OVERVOLTAGE_LATCH[] PROGMEM = "Cell Overvoltage Latch";
static const char FSTR_OCDL[] PROGMEM = "OCDL";
static const char FSTR_OVERCURRENT_IN_DISCHARGE[] PROGMEM = "Overcurrent in Discharge";
static const char FSTR_AOLD[] PROGMEM = "AOLD";
static const char FSTR_OVERLOAD_DURING_DISCHARGE[] PROGMEM = "Overload During Discharge";
static const char FSTR_ASCC[] PROGMEM = "ASCC";
static const char FSTR_SHORT_CIRCUIT_DURING_CHARGE[] PROGMEM = "Short-circuit During Charge";
static const char FSTR_ASCD[] PROGMEM = "ASCD";
static const char FSTR_SHORT_CIRCUIT_DURING_DISCHARGE[] PROGMEM = "Short-circuit During Discharge";
static const char FSTR_SUV[] PROGMEM = "SUV";
static const char FSTR_SAFETY_CELL_UNDERVOLTAGE_FAILURE[] PROGMEM = "Safety Cell Undervoltage Failure";
static const char FSTR_SOV[] PROGMEM = "SOV";
static const char FSTR_SAFETY_CELL_OVERVOLTAGE_FAILURE[] PROGMEM = "Safety Cell Overvoltage Failure";
static const char FSTR_SOCC[] PROGMEM = "SOCC";
static const char FSTR_SAFETY_OVERCURRENT_IN_CHARGE[] PROGMEM = "Safety Overcurrent in Charge";
static const char FSTR_SOCD[] PROGMEM = "SOCD";
static const char FSTR_SAFETY_OVERCURRENT_IN_DISCHARGE[] PROGMEM = "Safety Overcurrent in Discharge";
static const char FSTR_SOT[] PROGMEM = "SOT";
static const char FSTR_SAFETY_OVERTEMPERATURE_CELL_FAILURE[] PROGMEM = "Safety Overtemperature Cell Failure";
static const char FSTR_SOTF[] PROGMEM = "SOTF";
static const char FSTR_SAFETY_OVERTEMPERATURE_FET_FAILURE[] PROGMEM = "Safety Overtemperature FET Failure";
static const char FSTR_QIM[] PROGMEM = "QIM";
static const char FSTR_QMAX_IMBALANCE_FAILURE[] PROGMEM = "QMax Imbalance Failure";
static const char FSTR_CB[] PROGMEM = "CB";
static const char FSTR_CELL_BALANCING_FAILURE[] PROGMEM = "Cell Balancing Failure";
static const char FSTR_IMP[] PROGMEM = "IMP";
static const char FSTR_IMPEDANCE_FAILURE[] PROGMEM = "Impedance Failure";
static const char FSTR_CD[] PROGMEM = "CD";
static const char FSTR_CAPACITY_DEGRADATION_FAILURE[] PROGMEM = "Capacity Degradation Failure";
static const char FSTR_VIMR[] PROGMEM = "VIMR";
static const char FSTR_VOLTAGE_IMBALANCE_AT_REST[] PROGMEM = "Voltage Imbalance At Rest";
static const char FSTR_VIMA[] PROGMEM = "VIMA";
static const char FSTR_VOLTAGE_IMBALANCE_WHILE_ACTIVE[] PROGMEM = "Voltage Imbalance While Active";
static const char FSTR_OVERLOAD_IN_DISCHARGE[] PROGMEM = "Overload in Discharge";
static const char FSTR_SHORT_CIRCUIT_IN_CHARGE[] PROGMEM = "Short Circuit in Charge";
static const char FSTR_SHORT_CIRCUIT_IN_DISCHARGE[] PROGMEM = "Short Circuit in Discharge";
static const char FSTR_CFETF[] PROGMEM = "CFETF";
static const char FSTR_CHARGE_FET_FAILURE[] PROGMEM = "Charge FET Failure";
static const char FSTR_DFETF[] PROGMEM = "DFETF";
static const char FSTR_DISCHARGE_FET_FAILURE[] PROGMEM = "Discharge FET Failure";
static const char FSTR_FUSE[] PROGMEM = "FUSE";
static const char FSTR_CHEMICAL_FUSE_FAILURE[] PROGMEM = "Chemical Fuse Failure";
static const char FSTR_AFER[] PROGMEM = "AFER";
static const char FSTR_AFE_REGISTER_FAILURE[] PROGMEM = "AFE Register Failure";
static const char FSTR_AFEC[] PROGMEM = "AFEC";
static const char FSTR_AFE_COMMUNICATION_FAILURE[] PROGMEM = "AFE Communication Failure";
static const char FSTR_2LVL[] PROGMEM = "2LVL";
static const char FSTR_SECOND_LEVEL_PROTECTOR_FAILURE[] PROGMEM = "Second Level Protector Failure";
static const char FSTR_TS1[] PROGMEM = "TS1";
static const char FSTR_OPEN_THERMISTOR_TS1_FAILURE[] PROGMEM = "Open Thermistor TS1 Failure";
static const char FSTR_TS2[] PROGMEM = "TS2";
static const char FSTR_OPEN_THERMISTOR_TS2_FAILURE[] PROGMEM = "Open Thermistor TS2 Failure";
static const char FSTR_TS3[] PROGMEM = "TS3";
static const char FSTR_OPEN_THERMISTOR_TS3_FAILURE[] PROGMEM = "Open Thermistor TS3 Failure";
static const char FSTR_TS4[] PROGMEM = "TS4";
static const char FSTR_OPEN_THERMISTOR_TS4_FAILURE[] PROGMEM = "Open Thermistor TS4 Failure";
static const char FSTR_PTC[] PROGMEM = "PTC";
static const char FSTR_PTC_FAILURE[] PROGMEM = "PTC Failure";
static const char FSTR_IFC[] PROGMEM = "IFC";
static const char FSTR_INSTRUCTION_FLASH_CHECKSUM_FAILURE[] PROGMEM = "Instruction Flash Checksum Failure";
static const char FSTR_DFW[] PROGMEM = "DFW";
static const char FSTR_DATA_FLASH_WEAROUT_FAILURE[] PROGMEM = "Data Flash Wearout Failure";
static const char FSTR_PRES[] PROGMEM = "PRES";
static const char FSTR_SYSTEM_PRESENT_LOW[] PROGMEM = "System Present (low)";
static const char FSTR_ACTIVE[] PROGMEM = "Active";
static const char FSTR_INACTIVE[] PROGMEM = "Inactive";
static const char FSTR_DSG[] PROGMEM = "DSG";
static const char FSTR_DISCHARGE_FET_STATUS[] PROGMEM = "Discharge FET status";
static const char FSTR_CHG[] PROGMEM = "CHG";
static const char FSTR_CHARGE_FET_STATUS[] PROGMEM = "Charge FET status";
static const char FSTR_PCHG[] PROGMEM = "PCHG";
static const char FSTR_PRECHARGE_FET_STATUS[] PROGMEM = "Precharge FET status";
static const char FSTR_FUSE_STATUS[] PROGMEM = "Fuse status";
static const char FSTR_BTP_INT[] PROGMEM = "BTP_INT";
static const char FSTR_BATTERY_TRIP_POINT_INTERRUPT[] PROGMEM = "Battery Trip Point Interrupt";
static const char FSTR_SEC0[] PROGMEM = "SEC0";
static const char FSTR_SECURITY_MODE_BIT_0_00_RESERVED_01_FULLA[] PROGMEM = "Security Mode Bit 0 (00-Reserved 01-FullAccess 10-Unsealed 11-Sealed)";
static const char FSTR_SEC1[] PROGMEM = "SEC1";
static const char FSTR_SECURITY_MODE_BIT_1_00_RESERVED_01_FULLA[] PROGMEM = "Security Mode Bit 1 (00-Reserved 01-FullAccess 10-Unsealed 11-Sealed)";
static const char FSTR_SDV[] PROGMEM = "SDV";
static const char FSTR_SHUTDOWN_DUE_TO_LOW_PACK_VOLTAGE[] PROGMEM = "Shutdown due to low pack voltage";
static const char FSTR_SS[] PROGMEM = "SS";
static const char FSTR_SAFETY_STATUS_OR_OF_ALL_SAFETY_BITS[] PROGMEM = "Safety Status (OR of all safety bits)";
static const char FSTR_PF[] PROGMEM = "PF";
static const char FSTR_PERMANENT_FAILURE_MODE[] PROGMEM = "Permanent Failure mode";
static const char FSTR_XDSG[] PROGMEM = "XDSG";
static const char FSTR_DISCHARGING_DISABLED[] PROGMEM = "Discharging disabled";
static const char FSTR_XCHG[] PROGMEM = "XCHG";
static const char FSTR_CHARGING_DISABLED[] PROGMEM = "Charging disabled";
static const char FSTR_SLEEP[] PROGMEM = "SLEEP";
static const char FSTR_SLEEP_MODE_CONDITIONS_MET[] PROGMEM = "Sleep mode conditions met";
static const char FSTR_SDM[] PROGMEM = "SDM";
static const char FSTR_SHUTDOWN_VIA_COMMAND[] PROGMEM = "Shutdown via command";
static const char FSTR_LED[] PROGMEM = "LED";
static const char FSTR_LED_DISPLAY_STATUS[] PROGMEM = "LED Display status";
static const char FSTR_ON[] PROGMEM = "On";
static const char FSTR_OFF[] PROGMEM = "Off";
static const char FSTR_AUTH[] PROGMEM = "AUTH";
static const char FSTR_AUTHENTICATION_IN_PROGRESS[] PROGMEM = "Authentication in progress";
static const char FSTR_CALM[] PROGMEM = "CALM";
static const char FSTR_AUTO_CC_OFFSET_CALIBRATION_MAC[] PROGMEM = "Auto CC Offset Calibration (MAC)";
static const char FSTR_CAL[] PROGMEM = "CAL";
static const char FSTR_CALIBRATION_OUTPUT_ADC_CC[] PROGMEM = "Calibration output (ADC/CC)";
static const char FSTR_AVAILABLE[] PROGMEM = "Available";
static const char FSTR_NOT_AVAILABLE[] PROGMEM = "Not available";
static const char FSTR_CAL_OFFSET[] PROGMEM = "CAL_OFFSET";
static const char FSTR_CALIBRATION_OUTPUT_SHORTED_CC[] PROGMEM = "Calibration Output (Shorted CC)";
static const char FSTR_XL[] PROGMEM = "XL";
static const char FSTR_400_KHZ_SMBUS_MODE[] PROGMEM = "400-kHz SMBus mode";
static const char FSTR_SLEEPM[] PROGMEM = "SLEEPM";
static const char FSTR_SLEEP_MODE_VIA_COMMAND[] PROGMEM = "SLEEP mode via command";
static const char FSTR_INIT[] PROGMEM = "INIT";
static const char FSTR_INITIALIZATION_AFTER_FULL_RESET[] PROGMEM = "Initialization after full reset";
static const char FSTR_SMBLCAL[] PROGMEM = "SMBLCAL";
static const char FSTR_AUTO_CC_CALIBRATION_BUS_LOW[] PROGMEM = "Auto CC Calibration (bus low)";
static const char FSTR_STARTED[] PROGMEM = "Started";
static const char FSTR_NOT_STARTED[] PROGMEM = "Not started";
static const char FSTR_SLPAD[] PROGMEM = "SLPAD";
static const char FSTR_ADC_MEASUREMENT_IN_SLEEP[] PROGMEM = "ADC Measurement in Sleep";
static const char FSTR_SLPCC[] PROGMEM = "SLPCC";
static const char FSTR_CC_MEASUREMENT_IN_SLEEP[] PROGMEM = "CC Measurement in Sleep";
static const char FSTR_CELL_BALANCING_STATUS[] PROGMEM = "Cell Balancing status";
static const char FSTR_EMSHUT[] PROGMEM = "EMSHUT";
static const char FSTR_EMERGENCY_FET_SHUTDOWN[] PROGMEM = "Emergency FET Shutdown";
static const char FSTR_PRECHARGE_FET_TEST[] PROGMEM = "Precharge FET Test.";
static const char FSTR_DISABLED[] PROGMEM = "Disabled";
static const char FSTR_CHARGE_FET_TEST[] PROGMEM = "Charge FET Test.";
static const char FSTR_DISCHARGE_FET_TEST[] PROGMEM = "Discharge FET Test.";
static const char FSTR_GAUGE[] PROGMEM = "GAUGE";
static const char FSTR_GAS_GAUGING[] PROGMEM = "Gas Gauging.";
static const char FSTR_ENABLED[] PROGMEM = "Enabled";
static const char FSTR_FET[] PROGMEM = "FET";
static const char FSTR_ALL_FET_ACTION[] PROGMEM = "All FET Action.";
static const char FSTR_LF[] PROGMEM = "LF";
static const char FSTR_LIFETIME_DATA_COLLECTION[] PROGMEM = "Lifetime data collection.";
static const char FSTR_PERMANENT_FAILURE_FUNCTIONALITY[] PROGMEM = "Permanent Failure functionality.";
static const char FSTR_BBR[] PROGMEM = "BBR";
static const char FSTR_BLACK_BOX_RECORDER[] PROGMEM = "Black box recorder.";
static const char FSTR_FUSE_ACTION[] PROGMEM = "FUSE action.";
static const char FSTR_LED_DISPLAY[] PROGMEM = "LED Display.";
static const char FSTR_LT_TS[] PROGMEM = "LT_TS";
static const char FSTR_LIFETIME_SPEED_UP_MODE[] PROGMEM = "Lifetime Speed Up mode.";
static const char FSTR_CALTS[] PROGMEM = "CALTS";
static const char FSTR_CAL_ADC_OR_CC_OUTPUT_ON_MANUFACTURERDATA[] PROGMEM = "CAL ADC or CC output on ManufacturerData().";
static const char FSTR_DEVICETYPE[] PROGMEM = "DeviceType";
static const char FSTR_R[] PROGMEM = "R";
static const char FSTR_IDENTIFIES_THE_BATTERY_DEVICE_TYPE_TO_VE[] PROGMEM = "Identifies the battery device type to verify model and family compatibility.";
static const char FSTR_FIRMWAREVERSION[] PROGMEM = "FirmwareVersion";
static const char FSTR_REPORTS_THE_FIRMWARE_VERSION_RUNNING_ON_[] PROGMEM = "Reports the firmware version running on the battery controller, useful for compatibility and updates.";
static const char FSTR_HARDWAREVERSION[] PROGMEM = "HardwareVersion";
static const char FSTR_INDICATES_THE_HARDWARE_REVISION_OF_THE_D[] PROGMEM = "Indicates the hardware revision of the device to identify physical variations or improvements.";
static const char FSTR_PERMANENTFAILURE[] PROGMEM = "PermanentFailure";
static const char FSTR_W[] PROGMEM = "W";
static const char FSTR_THIS_COMMAND_ENABLES_DISABLES_PERMANENT_[] PROGMEM = "This command enables/disables Permanent Failure to help streamline production testing.";
static const char FSTR_LIFETIMEDATARESET[] PROGMEM = "LifetimeDataReset";
static const char FSTR_RESETS_ACCUMULATED_LIFETIME_DATA_SUCH_AS[] PROGMEM = "Resets accumulated lifetime data such as cycle count and usage statistics.";
static const char FSTR_PERMANENTFAILUREDATARESET[] PROGMEM = "PermanentFailureDataReset";
static const char FSTR_RESETS_PERMANENT_FAILURE_DATA_FLAGS_TO_C[] PROGMEM = "Resets permanent failure data flags to clear fault status.";
static const char FSTR_BLACKBOXRECORDERRESET[] PROGMEM = "BlackBoxRecorderReset";
static const char FSTR_RESETS_THE_BLACK_BOX_EVENT_RECORDER_TO_C[] PROGMEM = "Resets the black box event recorder to clear logged fault history.";
static const char FSTR_SEALDEVICE[] PROGMEM = "SealDevice";
static const char FSTR_SEALS_THE_DEVICE_TO_PREVENT_FURTHER_MODI[] PROGMEM = "Seals the device to prevent further modifications to configuration or data.";
static const char FSTR_DEVICERESET[] PROGMEM = "DeviceReset";
static const char FSTR_COMMAND_TO_RESET_THE_DEVICE_REINITIALIZI[] PROGMEM = "Command to reset the device, reinitializing all registers and states.";
static const char FSTR_SAFETYALERT[] PROGMEM = "SafetyAlert";
static const char FSTR_RETURNS_CURRENT_SAFETY_ALERT_FLAGS_INDIC[] PROGMEM = "Returns current safety alert flags indicating critical conditions such as overvoltage or overtemperature.";
static const char FSTR_SAFETYSTATUS[] PROGMEM = "SafetyStatus";
static const char FSTR_REPORTS_THE_CURRENT_SAFETY_STATUS_OF_THE[] PROGMEM = "Reports the current safety status of the device, showing ongoing safety-related events.";
static const char FSTR_PFALERT[] PROGMEM = "PFAlert";
static const char FSTR_INDICATES_PERMANENT_FAILURE_ALERTS_THAT_[] PROGMEM = "Indicates permanent failure alerts that require immediate attention or servicing.";
static const char FSTR_PFSTATUS[] PROGMEM = "PFStatus";
static const char FSTR_REPORTS_THE_STATUS_OF_PERMANENT_FAILURE_[] PROGMEM = "Reports the status of permanent failure flags for battery health monitoring.";
static const char FSTR_OPERATIONSTATUS[] PROGMEM = "OperationStatus";
static const char FSTR_GENERAL_OPERATIONAL_STATUS_REPORTING_THE[] PROGMEM = "General operational status reporting the current mode and condition of the device.";
static const char FSTR_MANUFACTURINGSTATUS[] PROGMEM = "ManufacturingStatus";
static const char FSTR_CONTAINS_INFORMATIONS_ABOUT_ACTIVATED_MO[] PROGMEM = "Contains informations about activated modes (PF, etc ..)";
static const char FSTR_UNSEALKEY1[] PROGMEM = "UnsealKey1";
static const char FSTR_KEY_TO_CHANGE_SECURITY_MODE_FROM_SEALED_[] PROGMEM = "Key to change security mode from SEALED to UNSEALED 1/2. The two words must be sent within 4 s.";
static const char FSTR_UNSEALKEY2[] PROGMEM = "UnsealKey2";
static const char FSTR_KEY_TO_CHANGE_SECURITY_MODE_FROM_SEALED__2[] PROGMEM = "Key to change security mode from SEALED to UNSEALED 2/2. The two words must be sent within 4 s.";
static const char FSTR_PF2REGISTERREAD[] PROGMEM = "PF2RegisterRead";
static const char FSTR_CUSTOM_DJI_REGISTER_KEY_WHERE_WE_CAN_FIN[] PROGMEM = "Custom DJI register key where we can find the PF2 flag.";
static const char FSTR_CLEARPF2[] PROGMEM = "ClearPF2";
static const char FSTR_OVERWRITE_THE_CUSTOM_DJI_REGISTER_KEY_WH[] PROGMEM = "Overwrite the custom DJI register key where we can find the PF2 flag.";


enum DisplayFormat {
  FORMAT_DECIMAL,
  FORMAT_HEX,
  FORMAT_BINARY,
  FORMAT_TEXT,
  FORMAT_MIXED,
};

// Represents a single bit in a status or response byte
struct BitFieldInfo {
  uint8_t bitIndex;        // Index (0-7)
  const char* label;       // Name of the bit (e.g. "PF Alert")
  const char* description; // Description or meaning
  const char* activeValue; // What does it mean when the bit is 1?
  const char* inactiveValue; // Optional: what does it mean when bit is 0?
};

typedef struct {
    uint16_t cmd;
    uint8_t data[8];
    uint8_t dataLength;
    const char* name; 
    const char* access;
    const DisplayFormat displayFormat;
    const BitFieldInfo* bitfields;
    uint8_t bitfieldCount;
    const char* description;
} MBACommandInfo;

/**
 * @brief Retrieves a pointer to a ManufacturerBlockAccess command by its name.
 *
 * This function searches the static MBACommandsInfo array for a command whose `name` field
 * matches the specified string (case-sensitive), and returns a pointer to its corresponding
 * MBACommandInfo structure.
 *
 * @param name  The name of the command to search for (e.g., "DeviceType").
 *              Must not be NULL.
 *
 * @return A pointer to the matching MBACommandInfo struct if found, 
 *         or NULL if no matching command is found or if `name` is NULL.
 *
 * @note The name comparison is case-sensitive.
 */
const MBACommandInfo* getMBACommandInfoByName(const char* name);

/**
 * @brief Sends a ManufacturerBlockAccess (MBA) command to a BQ battery device over I2C.
 *
 * This function writes a ManufacturerBlockAccess (command 0x44) followed by a 2-byte subcommand 
 * and optional data bytes to the device. The subcommand and data are specified via a 
 * MBACommandInfo structure.
 *
 * @param address     I2C address of the target battery device.
 * @param cmdInfo     Reference to an MBACommandInfo struct that holds the subcommand, data bytes, 
 *                    and metadata for the command.
 *
 * @return true if the command was sent successfully (no I2C transmission error), 
 *         false otherwise.
 *
 * @note This function assumes the device uses the standard SMBus block write format, 
 *       where the first byte after the command indicates the total number of data bytes to follow.
 *       It includes a small delay (20 ms) after transmission to allow the device to process the command.
 */
bool sendMBACommand(const uint8_t address, const MBACommandInfo* cmdInfo);

/**
 * @brief Reads a response block from a device over I2C using the Manufacturer Block Access (MBA) command.
 *
 * This function sends a Manufacturer Block Access command to a device at the specified I2C address,
 * then reads the resulting data block into the provided buffer. It checks for basic transmission
 * errors, handles timeouts, and verifies buffer sizes to avoid overflows.
 *
 * Additionally, the function:
 * - Warns if the response length exceeds the buffer capacity.
 * - Prints the received data using the format specified in `cmdInfo.displayFormat`.
 * - Converts the data from little-endian to big-endian using `reverseEndian`.
 *
 * @param address I2C address of the target device.
 * @param cmdInfo Command metadata containing display format and other related info.
 * @param buffer Pointer to the buffer where the read data will be stored.
 * @param bufferSize Size of the buffer (maximum number of bytes to read).
 *
 * @return `true` if the data was successfully read and processed; `false` on transmission failure, timeout,
 *         or if no sufficient data was available.
 *
 * @note The function assumes that the device echoes back the command before the result block,
 *       and that the first byte of the response indicates the payload length.
 *
 * Example usage:
 * @code
 * uint8_t data[32];
 * if (readMBACommand(0x50, myCmdInfo, data, sizeof(data))) {
 *     // Process the data
 * }
 * @endcode
 */
bool readMBACommand(uint8_t address, const MBACommandInfo* cmdInfo, uint8_t* buffer, size_t bufferSize);

/**
 * @brief Run a BQ ManufacturerBlockAccess command by name: send the sub-command and read back data.
 * 
 * This function looks up the command info by its name, sends the
 * ManufacturerBlockAccess command with the sub-command,
 * reads the response from the device, and prints it according to the
 * command's expected data format.
 * 
 * @param address I2C device address
 * @param cmdName Name of the command to run (case-sensitive)
 * @param format Display format for the read data (decimal, hex, binary, text)
 * 
 * @return true if command executed successfully, false on failure.
 */
bool runMBACommand(uint8_t address, const char* cmdName);

static const BitFieldInfo safetyAlertBits[] PROGMEM = {
  // Bits 0–7
  {  0, FSTR_CUV,     FSTR_CELL_UNDERVOLTAGE,                          FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  1, FSTR_COV,     FSTR_CELL_OVERVOLTAGE,                           FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  2, FSTR_OCC1,    FSTR_OVERCURRENT_DURING_CHARGE_1,                FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  3, FSTR_OCC2,    FSTR_OVERCURRENT_DURING_CHARGE_2,                FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  4, FSTR_OCD1,    FSTR_OVERCURRENT_DURING_DISCHARGE_1,             FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  5, FSTR_OCD2,    FSTR_OVERCURRENT_DURING_DISCHARGE_2,             FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  6, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  {  7, FSTR_AOLDL,   FSTR_OVERLOAD_DURING_DISCHARGE_LATCH,            FSTR_DETECTED, FSTR_NOT_DETECTED },

  // Bits 8–15
  {  8, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  {  9, FSTR_ASCCL,   FSTR_SHORT_CIRCUIT_DURING_CHARGE_LATCH,          FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 10, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  { 11, FSTR_ASCDL,   FSTR_SHORT_CIRCUIT_DURING_DISCHARGE_LATCH,       FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 12, FSTR_OTC,     FSTR_OVERTEMPERATURE_DURING_CHARGE,              FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 13, FSTR_OTD,     FSTR_OVERTEMPERATURE_DURING_DISCHARGE,           FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 14, FSTR_CUVC,    FSTR_CELL_UNDERVOLTAGE_COMPENSATED,              FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 15, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },

  // Bits 16–23
  { 16, FSTR_OTF,     FSTR_OVERTEMPERATURE_FET,                        FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 17, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  { 18, FSTR_PTO,     FSTR_PRECHARGE_TIMEOUT,                          FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 19, FSTR_PTOS,    FSTR_PRECHARGE_TIMEOUT_SUSPEND,                  FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 20, FSTR_CTO,     FSTR_CHARGE_TIMEOUT,                             FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 21, FSTR_CTOS,    FSTR_CHARGE_TIMEOUT_SUSPEND,                     FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 22, FSTR_OC,      FSTR_OVERCHARGE,                                 FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 23, FSTR_CHGC,    FSTR_OVERCHARGING_CURRENT,                       FSTR_DETECTED, FSTR_NOT_DETECTED },

  // Bits 24–31
  { 24, FSTR_CHGV,    FSTR_OVERCHARGING_VOLTAGE,                       FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 25, FSTR_PCHGC,   FSTR_OVER_PRECHARGE_CURRENT,                     FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 26, FSTR_UTC,     FSTR_UNDERTEMPERATURE_DURING_CHARGE,             FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 27, FSTR_UTD,     FSTR_UNDERTEMPERATURE_DURING_DISCHARGE,          FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 28, FSTR_COVL,    FSTR_CELL_OVERVOLTAGE_LATCH,                      FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 29, FSTR_OCDL,    FSTR_OVERCURRENT_IN_DISCHARGE,                    FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 30, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  { 31, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
};

static const BitFieldInfo safetyStatusBits[] PROGMEM = {
  // Bits 0–7
  {  0, FSTR_CUV,     FSTR_CELL_UNDERVOLTAGE,                          FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  1, FSTR_COV,     FSTR_CELL_OVERVOLTAGE,                           FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  2, FSTR_OCC1,    FSTR_OVERCURRENT_DURING_CHARGE_1,                FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  3, FSTR_OCC2,    FSTR_OVERCURRENT_DURING_CHARGE_2,                FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  4, FSTR_OCD1,    FSTR_OVERCURRENT_DURING_DISCHARGE_1,             FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  5, FSTR_OCD2,    FSTR_OVERCURRENT_DURING_DISCHARGE_2,             FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  6, FSTR_AOLD,    FSTR_OVERLOAD_DURING_DISCHARGE,                  FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  7, FSTR_AOLDL,   FSTR_OVERLOAD_DURING_DISCHARGE_LATCH,            FSTR_DETECTED, FSTR_NOT_DETECTED },

  // Bits 8–15
  {  8, FSTR_ASCC,    FSTR_SHORT_CIRCUIT_DURING_CHARGE,                FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  9, FSTR_ASCCL,   FSTR_SHORT_CIRCUIT_DURING_CHARGE_LATCH,          FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 10, FSTR_ASCD,    FSTR_SHORT_CIRCUIT_DURING_DISCHARGE,             FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 11, FSTR_ASCDL,   FSTR_SHORT_CIRCUIT_DURING_DISCHARGE_LATCH,       FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 12, FSTR_OTC,     FSTR_OVERTEMPERATURE_DURING_CHARGE,              FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 13, FSTR_OTD,     FSTR_OVERTEMPERATURE_DURING_DISCHARGE,           FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 14, FSTR_CUVC,    FSTR_CELL_UNDERVOLTAGE_COMPENSATED,              FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 15, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },

  // Bits 16–23
  { 16, FSTR_OTF,     FSTR_OVERTEMPERATURE_FET,                        FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 17, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  { 18, FSTR_PTO,     FSTR_PRECHARGE_TIMEOUT,                          FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 19, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  { 20, FSTR_CTO,     FSTR_CHARGE_TIMEOUT,                             FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 21, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  { 22, FSTR_OC,      FSTR_OVERCHARGE,                                 FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 23, FSTR_CHGC,    FSTR_OVERCHARGING_CURRENT,                       FSTR_DETECTED, FSTR_NOT_DETECTED },

  // Bits 24–31
  { 24, FSTR_CHGV,    FSTR_OVERCHARGING_VOLTAGE,                       FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 25, FSTR_PCHGC,   FSTR_OVER_PRECHARGE_CURRENT,                     FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 26, FSTR_UTC,     FSTR_UNDERTEMPERATURE_DURING_CHARGE,             FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 27, FSTR_UTD,     FSTR_UNDERTEMPERATURE_DURING_DISCHARGE,          FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 28, FSTR_COVL,    FSTR_CELL_OVERVOLTAGE_LATCH,                      FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 29, FSTR_OCDL,    FSTR_OVERCURRENT_IN_DISCHARGE,                    FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 30, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  { 31, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
};

static const BitFieldInfo pfAlertBits[] PROGMEM = {
  // Bit 0–7
  { 0,  FSTR_SUV,    FSTR_SAFETY_CELL_UNDERVOLTAGE_FAILURE,       FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 1,  FSTR_SOV,    FSTR_SAFETY_CELL_OVERVOLTAGE_FAILURE,        FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 2,  FSTR_SOCC,   FSTR_SAFETY_OVERCURRENT_IN_CHARGE,           FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 3,  FSTR_SOCD,   FSTR_SAFETY_OVERCURRENT_IN_DISCHARGE,        FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 4,  FSTR_SOT,    FSTR_SAFETY_OVERTEMPERATURE_CELL_FAILURE,     FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 5,  FSTR_COVL,   FSTR_CELL_OVERVOLTAGE_LATCH,                 FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 6,  FSTR_SOTF,   FSTR_SAFETY_OVERTEMPERATURE_FET_FAILURE,     FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 7,  FSTR_QIM,    FSTR_QMAX_IMBALANCE_FAILURE,                 FSTR_DETECTED, FSTR_NOT_DETECTED },

  // Bit 8–15
  { 8,  FSTR_CB,     FSTR_CELL_BALANCING_FAILURE,                 FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 9,  FSTR_IMP,    FSTR_IMPEDANCE_FAILURE,                      FSTR_DETECTED, FSTR_NOT_DETECTED },
  {10,  FSTR_CD,     FSTR_CAPACITY_DEGRADATION_FAILURE,           FSTR_DETECTED, FSTR_NOT_DETECTED },
  {11,  FSTR_VIMR,   FSTR_VOLTAGE_IMBALANCE_AT_REST,              FSTR_DETECTED, FSTR_NOT_DETECTED },
  {12,  FSTR_VIMA,   FSTR_VOLTAGE_IMBALANCE_WHILE_ACTIVE,         FSTR_DETECTED, FSTR_NOT_DETECTED },
  {13,  FSTR_AOLDL,  FSTR_OVERLOAD_IN_DISCHARGE,                  FSTR_DETECTED, FSTR_NOT_DETECTED },
  {14,  FSTR_ASCCL,  FSTR_SHORT_CIRCUIT_IN_CHARGE,                FSTR_DETECTED, FSTR_NOT_DETECTED },
  {15,  FSTR_ASCDL,  FSTR_SHORT_CIRCUIT_IN_DISCHARGE,             FSTR_DETECTED, FSTR_NOT_DETECTED },

  // Bit 16–23
  {16,  FSTR_CFETF,  FSTR_CHARGE_FET_FAILURE,                     FSTR_DETECTED, FSTR_NOT_DETECTED },
  {17,  FSTR_DFETF,  FSTR_DISCHARGE_FET_FAILURE,                  FSTR_DETECTED, FSTR_NOT_DETECTED },
  {18,  FSTR_OCDL,   FSTR_OVERCURRENT_IN_DISCHARGE,               FSTR_DETECTED, FSTR_NOT_DETECTED },
  {19,  FSTR_FUSE,   FSTR_CHEMICAL_FUSE_FAILURE,                  FSTR_DETECTED, FSTR_NOT_DETECTED },
  {20,  FSTR_AFER,   FSTR_AFE_REGISTER_FAILURE,                   FSTR_DETECTED, FSTR_NOT_DETECTED },
  {21,  FSTR_AFEC,   FSTR_AFE_COMMUNICATION_FAILURE,              FSTR_DETECTED, FSTR_NOT_DETECTED },
  {22,  FSTR_2LVL,   FSTR_SECOND_LEVEL_PROTECTOR_FAILURE,         FSTR_DETECTED, FSTR_NOT_DETECTED },
  {23, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },

  // Bits 24–31
  {24, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  {25, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  {26, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  {27, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  {28,  FSTR_TS1,    FSTR_OPEN_THERMISTOR_TS1_FAILURE,            FSTR_DETECTED, FSTR_NOT_DETECTED },
  {29,  FSTR_TS2,    FSTR_OPEN_THERMISTOR_TS2_FAILURE,            FSTR_DETECTED, FSTR_NOT_DETECTED },
  {30,  FSTR_TS3,    FSTR_OPEN_THERMISTOR_TS3_FAILURE,            FSTR_DETECTED, FSTR_NOT_DETECTED },
  {31,  FSTR_TS4,    FSTR_OPEN_THERMISTOR_TS4_FAILURE,            FSTR_DETECTED, FSTR_NOT_DETECTED }
};

static const BitFieldInfo pfStatusBits[] PROGMEM = {
  // Bits 0–7
  {  0, FSTR_SUV,    FSTR_SAFETY_CELL_UNDERVOLTAGE_FAILURE,            FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  1, FSTR_SOV,    FSTR_SAFETY_CELL_OVERVOLTAGE_FAILURE,             FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  2, FSTR_SOCC,   FSTR_SAFETY_OVERCURRENT_IN_CHARGE,                FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  3, FSTR_SOCD,   FSTR_SAFETY_OVERCURRENT_IN_DISCHARGE,             FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  4, FSTR_SOT,    FSTR_SAFETY_OVERTEMPERATURE_CELL_FAILURE,         FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  5, FSTR_COVL,   FSTR_CELL_OVERVOLTAGE_LATCH,                      FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  6, FSTR_SOTF,   FSTR_SAFETY_OVERTEMPERATURE_FET_FAILURE,          FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  7, FSTR_QIM,    FSTR_QMAX_IMBALANCE_FAILURE,                      FSTR_DETECTED, FSTR_NOT_DETECTED },

  // Bits 8–15
  {  8, FSTR_CB,     FSTR_CELL_BALANCING_FAILURE,                      FSTR_DETECTED, FSTR_NOT_DETECTED },
  {  9, FSTR_IMP,    FSTR_IMPEDANCE_FAILURE,                           FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 10, FSTR_CD,     FSTR_CAPACITY_DEGRADATION_FAILURE,                FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 11, FSTR_VIMR,   FSTR_VOLTAGE_IMBALANCE_AT_REST,                   FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 12, FSTR_VIMA,   FSTR_VOLTAGE_IMBALANCE_WHILE_ACTIVE,              FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 13, FSTR_AOLDL,  FSTR_OVERLOAD_IN_DISCHARGE,                       FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 14, FSTR_ASCCL,  FSTR_SHORT_CIRCUIT_IN_CHARGE,                     FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 15, FSTR_ASCDL,  FSTR_SHORT_CIRCUIT_IN_DISCHARGE,                  FSTR_DETECTED, FSTR_NOT_DETECTED },

  // Bits 16–23
  { 16, FSTR_CFETF,  FSTR_CHARGE_FET_FAILURE,                          FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 17, FSTR_DFETF,  FSTR_DISCHARGE_FET_FAILURE,                       FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 18, FSTR_OCDL,   FSTR_OVERCURRENT_IN_DISCHARGE,                    FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 19, FSTR_FUSE,   FSTR_CHEMICAL_FUSE_FAILURE,                       FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 20, FSTR_AFER,   FSTR_AFE_REGISTER_FAILURE,                        FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 21, FSTR_AFEC,   FSTR_AFE_COMMUNICATION_FAILURE,                   FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 22, FSTR_2LVL,   FSTR_SECOND_LEVEL_PROTECTOR_FAILURE,              FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 23, FSTR_PTC,    FSTR_PTC_FAILURE,                                 FSTR_DETECTED, FSTR_NOT_DETECTED },

  // Bits 24–31
  { 24, FSTR_IFC,    FSTR_INSTRUCTION_FLASH_CHECKSUM_FAILURE,          FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 25, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  { 26, FSTR_DFW,    FSTR_DATA_FLASH_WEAROUT_FAILURE,                  FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 27, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  { 28, FSTR_TS1,    FSTR_OPEN_THERMISTOR_TS1_FAILURE,                 FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 29, FSTR_TS2,    FSTR_OPEN_THERMISTOR_TS2_FAILURE,                 FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 30, FSTR_TS3,    FSTR_OPEN_THERMISTOR_TS3_FAILURE,                 FSTR_DETECTED, FSTR_NOT_DETECTED },
  { 31, FSTR_TS4,    FSTR_OPEN_THERMISTOR_TS4_FAILURE,                 FSTR_DETECTED, FSTR_NOT_DETECTED }
};

static const BitFieldInfo operationStatusBits[] PROGMEM = {
  // Bits 0–7
  {  0, FSTR_PRES,     FSTR_SYSTEM_PRESENT_LOW,                       FSTR_ACTIVE, FSTR_INACTIVE },
  {  1, FSTR_DSG,      FSTR_DISCHARGE_FET_STATUS,                      FSTR_ACTIVE, FSTR_INACTIVE },
  {  2, FSTR_CHG,      FSTR_CHARGE_FET_STATUS,                         FSTR_ACTIVE, FSTR_INACTIVE },
  {  3, FSTR_PCHG,     FSTR_PRECHARGE_FET_STATUS,                      FSTR_ACTIVE, FSTR_INACTIVE },
  {  4, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  {  5, FSTR_FUSE,     FSTR_FUSE_STATUS,                               FSTR_ACTIVE, FSTR_INACTIVE },
  {  6, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  {  7, FSTR_BTP_INT,  FSTR_BATTERY_TRIP_POINT_INTERRUPT,              FSTR_ACTIVE, FSTR_INACTIVE },

  // Bits 8–15
  {  8,  FSTR_SEC0,    FSTR_SECURITY_MODE_BIT_0_00_RESERVED_01_FULLA,                       FSTR_EMPTY, FSTR_EMPTY },
  {  9,  FSTR_SEC1,    FSTR_SECURITY_MODE_BIT_1_00_RESERVED_01_FULLA,                       FSTR_EMPTY, FSTR_EMPTY },
  { 10, FSTR_SDV,      FSTR_SHUTDOWN_DUE_TO_LOW_PACK_VOLTAGE,         FSTR_ACTIVE, FSTR_INACTIVE },
  { 11, FSTR_SS,       FSTR_SAFETY_STATUS_OR_OF_ALL_SAFETY_BITS,     FSTR_ACTIVE, FSTR_INACTIVE },
  { 12, FSTR_PF,       FSTR_PERMANENT_FAILURE_MODE,                    FSTR_ACTIVE, FSTR_INACTIVE },
  { 13, FSTR_XDSG,     FSTR_DISCHARGING_DISABLED,                      FSTR_ACTIVE, FSTR_INACTIVE },
  { 14, FSTR_XCHG,     FSTR_CHARGING_DISABLED,                         FSTR_ACTIVE, FSTR_INACTIVE },
  { 15, FSTR_SLEEP,    FSTR_SLEEP_MODE_CONDITIONS_MET,                 FSTR_ACTIVE, FSTR_INACTIVE },

  // Bits 16–23
  { 16, FSTR_SDM,      FSTR_SHUTDOWN_VIA_COMMAND,                      FSTR_ACTIVE, FSTR_INACTIVE },
  { 17, FSTR_LED,      FSTR_LED_DISPLAY_STATUS,                        FSTR_ON,     FSTR_OFF },
  { 18, FSTR_AUTH,     FSTR_AUTHENTICATION_IN_PROGRESS,                FSTR_ACTIVE, FSTR_INACTIVE },
  { 19, FSTR_CALM,     FSTR_AUTO_CC_OFFSET_CALIBRATION_MAC,          FSTR_ACTIVE, FSTR_INACTIVE },
  { 20, FSTR_CAL,      FSTR_CALIBRATION_OUTPUT_ADC_CC,               FSTR_AVAILABLE, FSTR_NOT_AVAILABLE },
  { 21, FSTR_CAL_OFFSET, FSTR_CALIBRATION_OUTPUT_SHORTED_CC,         FSTR_AVAILABLE, FSTR_NOT_AVAILABLE },
  { 22, FSTR_XL,       FSTR_400_KHZ_SMBUS_MODE,                        FSTR_ACTIVE, FSTR_INACTIVE },
  { 23, FSTR_SLEEPM,   FSTR_SLEEP_MODE_VIA_COMMAND,                    FSTR_ACTIVE, FSTR_INACTIVE },

  // Bits 24–31
  { 24, FSTR_INIT,     FSTR_INITIALIZATION_AFTER_FULL_RESET,           FSTR_ACTIVE, FSTR_INACTIVE },
  { 25, FSTR_SMBLCAL,  FSTR_AUTO_CC_CALIBRATION_BUS_LOW,             FSTR_STARTED, FSTR_NOT_STARTED },
  { 26, FSTR_SLPAD,    FSTR_ADC_MEASUREMENT_IN_SLEEP,                  FSTR_ACTIVE, FSTR_INACTIVE },
  { 27, FSTR_SLPCC,    FSTR_CC_MEASUREMENT_IN_SLEEP,                   FSTR_ACTIVE, FSTR_INACTIVE },
  { 28, FSTR_CB,       FSTR_CELL_BALANCING_STATUS,                     FSTR_ACTIVE, FSTR_INACTIVE },
  { 29, FSTR_EMSHUT,   FSTR_EMERGENCY_FET_SHUTDOWN,                    FSTR_ACTIVE, FSTR_INACTIVE },
  { 30, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
  { 31, FSTR_RSVD,    FSTR_RESERVED,                                   FSTR_EMPTY,          FSTR_EMPTY },
};

static const BitFieldInfo ManufacturingStatusBits[] PROGMEM = {
  // Bits 0–7
  {  0, FSTR_PCHG,     FSTR_PRECHARGE_FET_TEST,                          FSTR_ACTIVE, FSTR_DISABLED },
  {  1, FSTR_CHG,      FSTR_CHARGE_FET_TEST,                             FSTR_ACTIVE, FSTR_DISABLED },
  {  2, FSTR_DSG,      FSTR_DISCHARGE_FET_TEST,                          FSTR_ACTIVE, FSTR_DISABLED },
  {  3, FSTR_GAUGE,    FSTR_GAS_GAUGING,                                 FSTR_ENABLED, FSTR_DISABLED },
  {  4, FSTR_FET,      FSTR_ALL_FET_ACTION,                              FSTR_ENABLED, FSTR_DISABLED },
  {  5, FSTR_LF,       FSTR_LIFETIME_DATA_COLLECTION,                    FSTR_ENABLED, FSTR_DISABLED },
  {  6, FSTR_PF,       FSTR_PERMANENT_FAILURE_FUNCTIONALITY,             FSTR_ENABLED, FSTR_DISABLED },
  {  7, FSTR_BBR,      FSTR_BLACK_BOX_RECORDER,                          FSTR_ENABLED, FSTR_DISABLED },

  // Bits 8–15
  {  8, FSTR_FUSE,     FSTR_FUSE_ACTION,                                 FSTR_ENABLED, FSTR_DISABLED },
  {  9, FSTR_LED,      FSTR_LED_DISPLAY,                                 FSTR_ON, FSTR_OFF },
  { 10, FSTR_RSVD,     FSTR_RESERVED,                                     FSTR_ENABLED, FSTR_DISABLED },
  { 11, FSTR_RSVD,     FSTR_RESERVED,                                     FSTR_ENABLED, FSTR_DISABLED },
  { 12, FSTR_RSVD,     FSTR_RESERVED,                                     FSTR_ENABLED, FSTR_DISABLED },
  { 13, FSTR_RSVD,     FSTR_RESERVED,                                     FSTR_ENABLED, FSTR_DISABLED },
  { 14, FSTR_LT_TS,    FSTR_LIFETIME_SPEED_UP_MODE,                      FSTR_ENABLED, FSTR_DISABLED },
  { 15, FSTR_CALTS,    FSTR_CAL_ADC_OR_CC_OUTPUT_ON_MANUFACTURERDATA,  FSTR_ENABLED, FSTR_DISABLED },
};

// List of ManufacturerBlockAccess commands () (data from bq40z50-R2 Technical Reference)
static const MBACommandInfo MBACommandsInfo[] PROGMEM = {
    {0x0001, {}, 0,FSTR_DEVICETYPE, FSTR_R, FORMAT_HEX, NULL, 0, FSTR_IDENTIFIES_THE_BATTERY_DEVICE_TYPE_TO_VE},
    {0x0002, {}, 0,FSTR_FIRMWAREVERSION, FSTR_R, FORMAT_HEX, NULL, 0, FSTR_REPORTS_THE_FIRMWARE_VERSION_RUNNING_ON_},
    {0x0003, {}, 0,FSTR_HARDWAREVERSION, FSTR_R, FORMAT_HEX, NULL, 0, FSTR_INDICATES_THE_HARDWARE_REVISION_OF_THE_D},
    {0x0024, {}, 0,FSTR_PERMANENTFAILURE, FSTR_W, FORMAT_HEX, NULL, 0, FSTR_THIS_COMMAND_ENABLES_DISABLES_PERMANENT_},
    {0x0028, {}, 0,FSTR_LIFETIMEDATARESET, FSTR_W, FORMAT_HEX, NULL, 0, FSTR_RESETS_ACCUMULATED_LIFETIME_DATA_SUCH_AS},
    {0x0029, {}, 0,FSTR_PERMANENTFAILUREDATARESET, FSTR_W, FORMAT_HEX, NULL, 0, FSTR_RESETS_PERMANENT_FAILURE_DATA_FLAGS_TO_C},
    {0x002A, {}, 0,FSTR_BLACKBOXRECORDERRESET, FSTR_W, FORMAT_HEX, NULL, 0, FSTR_RESETS_THE_BLACK_BOX_EVENT_RECORDER_TO_C},
    {0x0030, {}, 0,FSTR_SEALDEVICE, FSTR_W, FORMAT_HEX, NULL, 0, FSTR_SEALS_THE_DEVICE_TO_PREVENT_FURTHER_MODI},
    {0x0041, {}, 0,FSTR_DEVICERESET, FSTR_W, FORMAT_HEX, NULL, 0, FSTR_COMMAND_TO_RESET_THE_DEVICE_REINITIALIZI},
    {0x0050, {}, 0,FSTR_SAFETYALERT, FSTR_R, FORMAT_BINARY, safetyAlertBits, 32, FSTR_RETURNS_CURRENT_SAFETY_ALERT_FLAGS_INDIC},
    {0x0051, {}, 0,FSTR_SAFETYSTATUS, FSTR_R, FORMAT_BINARY, safetyStatusBits, 32, FSTR_REPORTS_THE_CURRENT_SAFETY_STATUS_OF_THE},
    {0x0052, {}, 0,FSTR_PFALERT, FSTR_R, FORMAT_BINARY, pfAlertBits, 32, FSTR_INDICATES_PERMANENT_FAILURE_ALERTS_THAT_},
    {0x0053, {}, 0,FSTR_PFSTATUS, FSTR_R, FORMAT_BINARY, pfStatusBits, 32, FSTR_REPORTS_THE_STATUS_OF_PERMANENT_FAILURE_},
    {0x0054, {}, 0,FSTR_OPERATIONSTATUS, FSTR_R, FORMAT_BINARY, operationStatusBits, 32, FSTR_GENERAL_OPERATIONAL_STATUS_REPORTING_THE},
    {0x0057, {}, 0,FSTR_MANUFACTURINGSTATUS, FSTR_R, FORMAT_BINARY, ManufacturingStatusBits, 16, FSTR_CONTAINS_INFORMATIONS_ABOUT_ACTIVATED_MO},
    {0x7EE0, {}, 0,FSTR_UNSEALKEY1, FSTR_W, FORMAT_HEX, NULL, 0, FSTR_KEY_TO_CHANGE_SECURITY_MODE_FROM_SEALED_},
    {0xCCDF, {}, 0,FSTR_UNSEALKEY2, FSTR_W, FORMAT_HEX, NULL, 0, FSTR_KEY_TO_CHANGE_SECURITY_MODE_FROM_SEALED__2},
    {0x4062, {}, 0,FSTR_PF2REGISTERREAD, FSTR_R, FORMAT_HEX, NULL, 0, FSTR_CUSTOM_DJI_REGISTER_KEY_WHERE_WE_CAN_FIN},
    // Why write 0x01234567 to clear PF ? Saw it with DJI battery recovery tool so i simply reproduce it and it worked well
    {0x4062, {0x01, 0x23, 0x45, 0x67}, 4,FSTR_CLEARPF2, FSTR_W, FORMAT_HEX, NULL, 0, FSTR_OVERWRITE_THE_CUSTOM_DJI_REGISTER_KEY_WH},
};

#endif // BQCMD_H
