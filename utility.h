#ifndef UTILITY_H
#define UTILITY_H

#include <Arduino.h>
#include "bqcmd.h"

/**
 * @brief Reverses the byte order of a buffer (endianness).
 *
 * This function swaps the bytes of the given buffer in-place, effectively reversing the order.
 * It is typically used to convert between little-endian and big-endian representations.
 *
 * @param buffer Pointer to the byte buffer to reverse.
 * @param bufferSize Number of bytes in the buffer.
 *
 * For example, if the input buffer contains {0x12, 0x34, 0x56, 0x78}, the result after calling
 * this function will be {0x78, 0x56, 0x34, 0x12}.
 */
void reverseBufferEndian(uint8_t *buffer, size_t bufferSize);

/**
 * @brief Prints the contents of a byte buffer to the Serial monitor in multiple formats.
 *
 * This function first prints the buffer in hexadecimal format regardless of the selected display format.
 * Then, depending on the value of the `format` parameter, it additionally prints the buffer in one of
 * the following formats: decimal, binary, or ASCII text.
 *
 * @param buffer Pointer to the byte buffer to be printed.
 * @param bufferSize Number of bytes in the buffer.
 * @param displayFormat Display format to use in addition to hexadecimal. Can be one of:
 *               - FORMAT_DECIMAL: Print values in decimal format.
 *               - FORMAT_BINARY: Print values in binary format (8-bit).
 *               - FORMAT_TEXT: Print printable ASCII characters, replacing non-printable ones with '.'.
 *
 * Example output:
 * @code
 * Data (hex): 0x48 0x65 0x6C 0x6C 0x6F 
 * Data (txt): Hello
 * @endcode
 */
void printBuffer(const uint8_t* buffer, size_t bufferSize, DisplayFormat displayFormat);

/**
 * @brief Prints the state of individual bit fields from a buffer based on provided metadata.
 *
 * This function reads specific bits from a byte buffer and prints a human-readable description
 * for each, based on an array of `BitFieldInfo` structures. Bits are accessed in MSB-first order.
 *
 * @param buffer Pointer to the byte buffer containing the bit fields.
 * @param bufferSize Number of bytes in the buffer.
 * @param bitfields Pointer to an array of `BitFieldInfo` structures defining each bit field.
 * @param bitfieldsCount Number of bit fields to process from the `bitfields` array.
 *
 * Each `BitFieldInfo` entry defines the bit index, a label, the value meaning when the bit is set (`activeValue`),
 * the meaning when cleared (`inactiveValue`), and an optional description. The function prints these to the Serial monitor.
 *
 * The function performs safe bounds checking to avoid reading beyond the buffer.
 *
 * Example output:
 * @code
 * Bit 5 (Power): 1 = On
 * Bit 6 (Error): 0 = No Error
 * Bit 7 (Ready): 1 = Ready to start
 * @endcode
 */
void printBitFields(uint8_t* buffer, size_t bufferSize, const BitFieldInfo* bitfields, uint8_t bitfieldsCount);

/**
 * @brief Prints a human-readable error message corresponding to an I2C transmission result code.
 *
 * This function decodes standard `Wire.endTransmission()` error codes and prints a 
 * descriptive error message to the Serial monitor. It is used for debugging failed 
 * ManufacturerBlockAccess transmissions.
 *
 * @param result  The return code from `Wire.endTransmission()`. Expected values:
 *                - 0: Success (not handled here)
 *                - 1: Data too long for transmit buffer
 *                - 2: Received NACK on transmit of address
 *                - 3: Received NACK on transmit of data
 *                - 4: Other error
 *                - 5: Timeout
 *                - Default: Unknown error code
 *
 * @note A return value of 0 (success) is not handled in this function and should be checked separately.
 */

void printMBACommandError(int result);

/**
 * @brief Prints detailed information about a ManufacturerBlockAccess (MBA) command.
 *
 * This function displays the command name, main command code (0x44), subcommand (as a 2-byte word),
 * and any associated data bytes in hexadecimal format via the Serial monitor. It is primarily used
 * for debugging or logging purposes during development.
 *
 * @param cmdInfo  Reference to a `MBACommandInfo` structure containing the subcommand,
 *                 data, and metadata for a given MBA command.
 *
 * @note The subcommand is displayed as two bytes (LSB and MSB), and data bytes (if any)
 *       are printed in 0xXX format. The `dataLength` field must be properly initialized.
 */

void printMBACommandInfo(const MBACommandInfo* cmdInfo);

#endif // UTILITY_H