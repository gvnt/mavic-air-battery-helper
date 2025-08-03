#ifndef BQCMD_H
#define BQCMD_H
#define MANUFACTURER_BLOCK_ACCESS_COMMAND 0x44
// https://github.com/Testato/SoftwareWire/blob/master/SoftwareWire.h
#define SOFTWAREWIRE_BUFSIZE  32

#include <Arduino.h>
#include <Wire.h>

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

static const BitFieldInfo safetyAlertBits[] = {
  // Bits 0–7
  {  0, "CUV",     "Cell Undervoltage",                          "Detected", "Not Detected" },
  {  1, "COV",     "Cell Overvoltage",                           "Detected", "Not Detected" },
  {  2, "OCC1",    "Overcurrent During Charge 1",                "Detected", "Not Detected" },
  {  3, "OCC2",    "Overcurrent During Charge 2",                "Detected", "Not Detected" },
  {  4, "OCD1",    "Overcurrent During Discharge 1",             "Detected", "Not Detected" },
  {  5, "OCD2",    "Overcurrent During Discharge 2",             "Detected", "Not Detected" },
  {  6, "RSVD",    "Reserved",                                   "",          "" },
  {  7, "AOLDL",   "Overload During Discharge Latch",            "Detected", "Not Detected" },

  // Bits 8–15
  {  8, "RSVD",    "Reserved",                                   "",          "" },
  {  9, "ASCCL",   "Short-Circuit During Charge Latch",          "Detected", "Not Detected" },
  { 10, "RSVD",    "Reserved",                                   "",          "" },
  { 11, "ASCDL",   "Short-Circuit During Discharge Latch",       "Detected", "Not Detected" },
  { 12, "OTC",     "Overtemperature During Charge",              "Detected", "Not Detected" },
  { 13, "OTD",     "Overtemperature During Discharge",           "Detected", "Not Detected" },
  { 14, "CUVC",    "Cell Undervoltage Compensated",              "Detected", "Not Detected" },
  { 15, "RSVD",    "Reserved",                                   "",          "" },

  // Bits 16–23
  { 16, "OTF",     "Overtemperature FET",                        "Detected", "Not Detected" },
  { 17, "RSVD",    "Reserved",                                   "",          "" },
  { 18, "PTO",     "Precharge Timeout",                          "Detected", "Not Detected" },
  { 19, "PTOS",    "Precharge Timeout Suspend",                  "Detected", "Not Detected" },
  { 20, "CTO",     "Charge Timeout",                             "Detected", "Not Detected" },
  { 21, "CTOS",    "Charge Timeout Suspend",                     "Detected", "Not Detected" },
  { 22, "OC",      "Overcharge",                                 "Detected", "Not Detected" },
  { 23, "CHGC",    "Overcharging Current",                       "Detected", "Not Detected" },

  // Bits 24–31
  { 24, "CHGV",    "Overcharging Voltage",                       "Detected", "Not Detected" },
  { 25, "PCHGC",   "Over-Precharge Current",                     "Detected", "Not Detected" },
  { 26, "UTC",     "Undertemperature During Charge",             "Detected", "Not Detected" },
  { 27, "UTD",     "Undertemperature During Discharge",          "Detected", "Not Detected" },
  { 28, "COVL",    "Cell Overvoltage Latch",                      "Detected", "Not Detected" },
  { 29, "OCDL",    "Overcurrent in Discharge",                    "Detected", "Not Detected" },
  { 30, "RSVD",    "Reserved",                                   "",          "" },
  { 31, "RSVD",    "Reserved",                                   "",          "" },
};

static const BitFieldInfo safetyStatusBits[] = {
  // Bits 0–7
  {  0, "CUV",     "Cell Undervoltage",                          "Detected", "Not Detected" },
  {  1, "COV",     "Cell Overvoltage",                           "Detected", "Not Detected" },
  {  2, "OCC1",    "Overcurrent During Charge 1",                "Detected", "Not Detected" },
  {  3, "OCC2",    "Overcurrent During Charge 2",                "Detected", "Not Detected" },
  {  4, "OCD1",    "Overcurrent During Discharge 1",             "Detected", "Not Detected" },
  {  5, "OCD2",    "Overcurrent During Discharge 2",             "Detected", "Not Detected" },
  {  6, "AOLD",    "Overload During Discharge",                  "Detected", "Not Detected" },
  {  7, "AOLDL",   "Overload During Discharge Latch",            "Detected", "Not Detected" },

  // Bits 8–15
  {  8, "ASCC",    "Short-circuit During Charge",                "Detected", "Not Detected" },
  {  9, "ASCCL",   "Short-circuit During Charge Latch",          "Detected", "Not Detected" },
  { 10, "ASCD",    "Short-circuit During Discharge",             "Detected", "Not Detected" },
  { 11, "ASCDL",   "Short-circuit During Discharge Latch",       "Detected", "Not Detected" },
  { 12, "OTC",     "Overtemperature During Charge",              "Detected", "Not Detected" },
  { 13, "OTD",     "Overtemperature During Discharge",           "Detected", "Not Detected" },
  { 14, "CUVC",    "Cell Undervoltage Compensated",              "Detected", "Not Detected" },
  { 15, "RSVD",    "Reserved",                                   "",          "" },

  // Bits 16–23
  { 16, "OTF",     "Overtemperature FET",                        "Detected", "Not Detected" },
  { 17, "RSVD",    "Reserved",                                   "",          "" },
  { 18, "PTO",     "Precharge Timeout",                          "Detected", "Not Detected" },
  { 19, "RSVD",    "Reserved",                                   "",          "" },
  { 20, "CTO",     "Charge Timeout",                             "Detected", "Not Detected" },
  { 21, "RSVD",    "Reserved",                                   "",          "" },
  { 22, "OC",      "Overcharge",                                 "Detected", "Not Detected" },
  { 23, "CHGC",    "Overcharging Current",                       "Detected", "Not Detected" },

  // Bits 24–31
  { 24, "CHGV",    "Overcharging Voltage",                       "Detected", "Not Detected" },
  { 25, "PCHGC",   "Over-Precharge Current",                     "Detected", "Not Detected" },
  { 26, "UTC",     "Undertemperature During Charge",             "Detected", "Not Detected" },
  { 27, "UTD",     "Undertemperature During Discharge",          "Detected", "Not Detected" },
  { 28, "COVL",    "Cell Overvoltage Latch",                      "Detected", "Not Detected" },
  { 29, "OCDL",    "Overcurrent in Discharge",                    "Detected", "Not Detected" },
  { 30, "RSVD",    "Reserved",                                   "",          "" },
  { 31, "RSVD",    "Reserved",                                   "",          "" },
};

static const BitFieldInfo pfAlertBits[] = {
  // Bit 0–7
  { 0,  "SUV",    "Safety Cell Undervoltage Failure",       "Detected", "Not Detected" },
  { 1,  "SOV",    "Safety Cell Overvoltage Failure",        "Detected", "Not Detected" },
  { 2,  "SOCC",   "Safety Overcurrent in Charge",           "Detected", "Not Detected" },
  { 3,  "SOCD",   "Safety Overcurrent in Discharge",        "Detected", "Not Detected" },
  { 4,  "SOT",    "Safety Overtemperature Cell Failure",     "Detected", "Not Detected" },
  { 5,  "COVL",   "Cell Overvoltage Latch",                 "Detected", "Not Detected" },
  { 6,  "SOTF",   "Safety Overtemperature FET Failure",     "Detected", "Not Detected" },
  { 7,  "QIM",    "QMax Imbalance Failure",                 "Detected", "Not Detected" },

  // Bit 8–15
  { 8,  "CB",     "Cell Balancing Failure",                 "Detected", "Not Detected" },
  { 9,  "IMP",    "Impedance Failure",                      "Detected", "Not Detected" },
  {10,  "CD",     "Capacity Degradation Failure",           "Detected", "Not Detected" },
  {11,  "VIMR",   "Voltage Imbalance At Rest",              "Detected", "Not Detected" },
  {12,  "VIMA",   "Voltage Imbalance While Active",         "Detected", "Not Detected" },
  {13,  "AOLDL",  "Overload in Discharge",                  "Detected", "Not Detected" },
  {14,  "ASCCL",  "Short Circuit in Charge",                "Detected", "Not Detected" },
  {15,  "ASCDL",  "Short Circuit in Discharge",             "Detected", "Not Detected" },

  // Bit 16–23
  {16,  "CFETF",  "Charge FET Failure",                     "Detected", "Not Detected" },
  {17,  "DFETF",  "Discharge FET Failure",                  "Detected", "Not Detected" },
  {18,  "OCDL",   "Overcurrent in Discharge",               "Detected", "Not Detected" },
  {19,  "FUSE",   "Chemical Fuse Failure",                  "Detected", "Not Detected" },
  {20,  "AFER",   "AFE Register Failure",                   "Detected", "Not Detected" },
  {21,  "AFEC",   "AFE Communication Failure",              "Detected", "Not Detected" },
  {22,  "2LVL",   "Second Level Protector Failure",         "Detected", "Not Detected" },
  {23, "RSVD",    "Reserved",                                   "",          "" },

  // Bits 24–31
  {24, "RSVD",    "Reserved",                                   "",          "" },
  {25, "RSVD",    "Reserved",                                   "",          "" },
  {26, "RSVD",    "Reserved",                                   "",          "" },
  {27, "RSVD",    "Reserved",                                   "",          "" },
  {28,  "TS1",    "Open Thermistor TS1 Failure",            "Detected", "Not Detected" },
  {29,  "TS2",    "Open Thermistor TS2 Failure",            "Detected", "Not Detected" },
  {30,  "TS3",    "Open Thermistor TS3 Failure",            "Detected", "Not Detected" },
  {31,  "TS4",    "Open Thermistor TS4 Failure",            "Detected", "Not Detected" }
};

static const BitFieldInfo pfStatusBits[] = {
  // Bits 0–7
  {  0, "SUV",    "Safety Cell Undervoltage Failure",            "Detected", "Not Detected" },
  {  1, "SOV",    "Safety Cell Overvoltage Failure",             "Detected", "Not Detected" },
  {  2, "SOCC",   "Safety Overcurrent in Charge",                "Detected", "Not Detected" },
  {  3, "SOCD",   "Safety Overcurrent in Discharge",             "Detected", "Not Detected" },
  {  4, "SOT",    "Safety Overtemperature Cell Failure",         "Detected", "Not Detected" },
  {  5, "COVL",   "Cell Overvoltage Latch",                      "Detected", "Not Detected" },
  {  6, "SOTF",   "Safety Overtemperature FET Failure",          "Detected", "Not Detected" },
  {  7, "QIM",    "QMax Imbalance Failure",                      "Detected", "Not Detected" },

  // Bits 8–15
  {  8, "CB",     "Cell Balancing Failure",                      "Detected", "Not Detected" },
  {  9, "IMP",    "Impedance Failure",                           "Detected", "Not Detected" },
  { 10, "CD",     "Capacity Degradation Failure",                "Detected", "Not Detected" },
  { 11, "VIMR",   "Voltage Imbalance At Rest",                   "Detected", "Not Detected" },
  { 12, "VIMA",   "Voltage Imbalance While Active",              "Detected", "Not Detected" },
  { 13, "AOLDL",  "Overload in Discharge",                       "Detected", "Not Detected" },
  { 14, "ASCCL",  "Short Circuit in Charge",                     "Detected", "Not Detected" },
  { 15, "ASCDL",  "Short Circuit in Discharge",                  "Detected", "Not Detected" },

  // Bits 16–23
  { 16, "CFETF",  "Charge FET Failure",                          "Detected", "Not Detected" },
  { 17, "DFETF",  "Discharge FET Failure",                       "Detected", "Not Detected" },
  { 18, "OCDL",   "Overcurrent in Discharge",                    "Detected", "Not Detected" },
  { 19, "FUSE",   "Chemical Fuse Failure",                       "Detected", "Not Detected" },
  { 20, "AFER",   "AFE Register Failure",                        "Detected", "Not Detected" },
  { 21, "AFEC",   "AFE Communication Failure",                   "Detected", "Not Detected" },
  { 22, "2LVL",   "Second Level Protector Failure",              "Detected", "Not Detected" },
  { 23, "PTC",    "PTC Failure",                                 "Detected", "Not Detected" },

  // Bits 24–31
  { 24, "IFC",    "Instruction Flash Checksum Failure",          "Detected", "Not Detected" },
  { 25, "RSVD",    "Reserved",                                   "",          "" },
  { 26, "DFW",    "Data Flash Wearout Failure",                  "Detected", "Not Detected" },
  { 27, "RSVD",    "Reserved",                                   "",          "" },
  { 28, "TS1",    "Open Thermistor TS1 Failure",                 "Detected", "Not Detected" },
  { 29, "TS2",    "Open Thermistor TS2 Failure",                 "Detected", "Not Detected" },
  { 30, "TS3",    "Open Thermistor TS3 Failure",                 "Detected", "Not Detected" },
  { 31, "TS4",    "Open Thermistor TS4 Failure",                 "Detected", "Not Detected" }
};

static const BitFieldInfo operationStatusBits[] = {
  // Bits 0–7
  {  0, "PRES",     "System Present (low)",                       "Active", "Inactive" },
  {  1, "DSG",      "Discharge FET status",                      "Active", "Inactive" },
  {  2, "CHG",      "Charge FET status",                         "Active", "Inactive" },
  {  3, "PCHG",     "Precharge FET status",                      "Active", "Inactive" },
  {  4, "RSVD",    "Reserved",                                   "",          "" },
  {  5, "FUSE",     "Fuse status",                               "Active", "Inactive" },
  {  6, "RSVD",    "Reserved",                                   "",          "" },
  {  7, "BTP_INT",  "Battery Trip Point Interrupt",              "Active", "Inactive" },

  // Bits 8–15
  {  8,  "SEC0",    "Security Mode Bit 0 (00-Reserved 01-FullAccess 10-Unsealed 11-Sealed)",                       "", "" },
  {  9,  "SEC1",    "Security Mode Bit 1 (00-Reserved 01-FullAccess 10-Unsealed 11-Sealed)",                       "", "" },
  { 10, "SDV",      "Shutdown due to low pack voltage",         "Active", "Inactive" },
  { 11, "SS",       "Safety Status (OR of all safety bits)",     "Active", "Inactive" },
  { 12, "PF",       "Permanent Failure mode",                    "Active", "Inactive" },
  { 13, "XDSG",     "Discharging disabled",                      "Active", "Inactive" },
  { 14, "XCHG",     "Charging disabled",                         "Active", "Inactive" },
  { 15, "SLEEP",    "Sleep mode conditions met",                 "Active", "Inactive" },

  // Bits 16–23
  { 16, "SDM",      "Shutdown via command",                      "Active", "Inactive" },
  { 17, "LED",      "LED Display status",                        "On",     "Off" },
  { 18, "AUTH",     "Authentication in progress",                "Active", "Inactive" },
  { 19, "CALM",     "Auto CC Offset Calibration (MAC)",          "Active", "Inactive" },
  { 20, "CAL",      "Calibration output (ADC/CC)",               "Available", "Not available" },
  { 21, "CAL_OFFSET", "Calibration Output (Shorted CC)",         "Available", "Not available" },
  { 22, "XL",       "400-kHz SMBus mode",                        "Active", "Inactive" },
  { 23, "SLEEPM",   "SLEEP mode via command",                    "Active", "Inactive" },

  // Bits 24–31
  { 24, "INIT",     "Initialization after full reset",           "Active", "Inactive" },
  { 25, "SMBLCAL",  "Auto CC Calibration (bus low)",             "Started", "Not started" },
  { 26, "SLPAD",    "ADC Measurement in Sleep",                  "Active", "Inactive" },
  { 27, "SLPCC",    "CC Measurement in Sleep",                   "Active", "Inactive" },
  { 28, "CB",       "Cell Balancing status",                     "Active", "Inactive" },
  { 29, "EMSHUT",   "Emergency FET Shutdown",                    "Active", "Inactive" },
  { 30, "RSVD",    "Reserved",                                   "",          "" },
  { 31, "RSVD",    "Reserved",                                   "",          "" },
};

static const BitFieldInfo ManufacturingStatusBits[] = {
  // Bits 0–7
  {  0, "PCHG",     "Precharge FET Test.",                          "Active", "Disabled" },
  {  1, "CHG",      "Charge FET Test.",                             "Active", "Disabled" },
  {  2, "DSG",      "Discharge FET Test.",                          "Active", "Disabled" },
  {  3, "GAUGE",    "Gas Gauging.",                                 "Enabled", "Disabled" },
  {  4, "FET",      "All FET Action.",                              "Enabled", "Disabled" },
  {  5, "LF",       "Lifetime data collection.",                    "Enabled", "Disabled" },
  {  6, "PF",       "Permanent Failure functionality.",             "Enabled", "Disabled" },
  {  7, "BBR",      "Black box recorder.",                          "Enabled", "Disabled" },

  // Bits 8–15
  {  8, "FUSE",     "FUSE action.",                                 "Enabled", "Disabled" },
  {  9, "LED",      "LED Display.",                                 "On", "Off" },
  { 10, "RSVD",     "Reserved",                                     "Enabled", "Disabled" },
  { 11, "RSVD",     "Reserved",                                     "Enabled", "Disabled" },
  { 12, "RSVD",     "Reserved",                                     "Enabled", "Disabled" },
  { 13, "RSVD",     "Reserved",                                     "Enabled", "Disabled" },
  { 14, "LT_TS",    "Lifetime Speed Up mode.",                      "Enabled", "Disabled" },
  { 15, "CALTS",    "CAL ADC or CC output on ManufacturerData().",  "Enabled", "Disabled" },
};

// List of ManufacturerBlockAccess commands () (data from bq40z50-R2 Technical Reference)
static const MBACommandInfo MBACommandsInfo[] = {
    {0x0001, {}, 0,"DeviceType", "R", FORMAT_HEX, NULL, 0, "Identifies the battery device type to verify model and family compatibility."},
    {0x0002, {}, 0,"FirmwareVersion", "R", FORMAT_HEX, NULL, 0, "Reports the firmware version running on the battery controller, useful for compatibility and updates."},
    {0x0003, {}, 0,"HardwareVersion", "R", FORMAT_HEX, NULL, 0, "Indicates the hardware revision of the device to identify physical variations or improvements."},
    {0x0024, {}, 0,"PermanentFailure", "W", FORMAT_HEX, NULL, 0, "This command enables/disables Permanent Failure to help streamline production testing."},
    {0x0028, {}, 0,"LifetimeDataReset", "W", FORMAT_HEX, NULL, 0, "Resets accumulated lifetime data such as cycle count and usage statistics."},
    {0x0029, {}, 0,"PermanentFailureDataReset", "W", FORMAT_HEX, NULL, 0, "Resets permanent failure data flags to clear fault status."},
    {0x002A, {}, 0,"BlackBoxRecorderReset", "W", FORMAT_HEX, NULL, 0, "Resets the black box event recorder to clear logged fault history."},
    {0x0030, {}, 0,"SealDevice", "W", FORMAT_HEX, NULL, 0, "Seals the device to prevent further modifications to configuration or data."},
    {0x0041, {}, 0,"DeviceReset", "W", FORMAT_HEX, NULL, 0, "Command to reset the device, reinitializing all registers and states."},
    {0x0050, {}, 0,"SafetyAlert", "R", FORMAT_BINARY, safetyAlertBits, 32, "Returns current safety alert flags indicating critical conditions such as overvoltage or overtemperature."},
    {0x0051, {}, 0,"SafetyStatus", "R", FORMAT_BINARY, safetyStatusBits, 32, "Reports the current safety status of the device, showing ongoing safety-related events."},
    {0x0052, {}, 0,"PFAlert", "R", FORMAT_BINARY, pfAlertBits, 32, "Indicates permanent failure alerts that require immediate attention or servicing."},
    {0x0053, {}, 0,"PFStatus", "R", FORMAT_BINARY, pfStatusBits, 32, "Reports the status of permanent failure flags for battery health monitoring."},
    {0x0054, {}, 0,"OperationStatus", "R", FORMAT_BINARY, operationStatusBits, 32, "General operational status reporting the current mode and condition of the device."},
    {0x0057, {}, 0,"ManufacturingStatus", "R", FORMAT_BINARY, ManufacturingStatusBits, 16, "Contains informations about activated modes (PF, etc ..)"},
    {0x7EE0, {}, 0,"UnsealKey1", "W", FORMAT_HEX, NULL, 0, "Key to change security mode from SEALED to UNSEALED 1/2. The two words must be sent within 4 s."},
    {0xCCDF, {}, 0,"UnsealKey2", "W", FORMAT_HEX, NULL, 0, "Key to change security mode from SEALED to UNSEALED 2/2. The two words must be sent within 4 s."},
    {0x4062, {}, 0,"PF2RegisterRead", "R", FORMAT_HEX, NULL, 0, "Custom DJI register key where we can find the PF2 flag."},
    // Why write 0x01234567 to clear PF ? Saw it with DJI battery recovery tool so i simply reproduce it and it worked well
    {0x4062, {0x01, 0x23, 0x45, 0x67}, 4,"ClearPF2", "W", FORMAT_HEX, NULL, 0, "Overwrite the custom DJI register key where we can find the PF2 flag."},
};

#endif // BQCMD_H