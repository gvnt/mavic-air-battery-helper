#include <Arduino.h>
#include <Wire.h>
#include "utility.h"
#include <string.h>  // For strcmp

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
const MBACommandInfo* getMBACommandInfoByName(const char* name) {
    if (name == NULL) {
        return NULL;
    }
    
    // Number of commands in the array
    size_t count = sizeof(MBACommandsInfo) / sizeof(MBACommandsInfo[0]);
    
    for (size_t i = 0; i < count; ++i) {
        if (strcmp(MBACommandsInfo[i].name, name) == 0) {
            return &MBACommandsInfo[i];
        }
    }
    
    // No matching command found
    return NULL;
}

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
bool sendMBACommand(const uint8_t address, const MBACommandInfo* cmdInfo) {
  // Begin I2C transmission to device
  Wire.beginTransmission(address);
  // Write the ManufacturerBlockAccess command byte
  Wire.write(MANUFACTURER_BLOCK_ACCESS_COMMAND);
  // 2 for subcommand + dataLength
  Wire.write(2 + cmdInfo->dataLength);
  // Sending LSB command byte
  Wire.write(lowByte(cmdInfo->cmd));
  // Sending MSB command byte
  Wire.write(highByte(cmdInfo->cmd));
  // Sending all data we want to send
  for (uint8_t i = 0; i < cmdInfo->dataLength; i++) {
    Wire.write(cmdInfo->data[i]);
  }
  // End transmission and get result
  int result = Wire.endTransmission();  
  // Short delay to allow device processing
  delay(20);  

  // Transmission successful
  if (result == 0) {
    return true;  
  } 
  // Print appropriate error message
  else {
    // Transmission failed
    printMBACommandError(result);
    return false;
  }
}

/**
 * @brief Reads a response block from a device over I2C using the Manufacturer Block Access (MBA) command.
 *
 * This function sends a Manufacturer Block Access command to a device at the specified I2C address,
 * then reads the resulting data block into the provided buffer. It checks for basic transmission
 * errors, handles timeouts, and verifies buffer sizes to avoid overflows.
 *
 * Additionally, the function:
 * - Warns if the response length exceeds the buffer capacity.
 * - Prints the received data using the format specified in `cmdInfo->displayFormat`.
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
bool readMBACommand(uint8_t address, const MBACommandInfo* cmdInfo, uint8_t* buffer, size_t bufferSize) {
  // Check if our buffer is larger than the lib limit
  if (bufferSize > SOFTWAREWIRE_BUFSIZE) {
    Serial.print(F("⚠️  Warning: bufferSize exceeds Wire buffer max ("));
    Serial.print(SOFTWAREWIRE_BUFSIZE);
    Serial.println(F(" bytes)."));
  }

  // Begin I2C transmission to device
  Wire.beginTransmission(address);
  // Write the ManufacturerBlockAccess command byte
  Wire.write(MANUFACTURER_BLOCK_ACCESS_COMMAND);
  // Repeated start for read
  int result = Wire.endTransmission(false);

  if (result != 0) {
    // Transmission failed
    printMBACommandError(result);
    return false;
  }

  // Requesting result
  // Max number of bytes we will try to read : bufferSize (limited by the Wire.available())
  Wire.requestFrom(address, (uint8_t)(bufferSize));

  // Check if we have at least 3 entry to read (we should at least have 1 byte to length and 2 for command reprint)
  // Note that ManufacturerBlockAccess command reprint the MBACommandInfo cmd before sending the result
  if (Wire.available() < 3) {
    Serial.println(F("No data available to read"));
    return false;
  }

  // First byte is the block length
  uint8_t len = Wire.read();  

   // Wait until all bytes are available (rarely needed but safest)
  unsigned long start = millis();
  // SOFTWAREWIRE_BUFSIZE - 1 because we have already read a byte
  // Note : len doesn't count his own byte
  while (Wire.available() < len && Wire.available() < SOFTWAREWIRE_BUFSIZE-1 ) {
    if (millis() - start > 10000) {
      Serial.print(F("Timeout waiting for full data block, only "));
      Serial.print(Wire.available());
      Serial.println(F(" bytes readable."));
      return false;
    }
    delay(10);
  }

  // If response is bigger than our buffer we send a warning
  if (len > bufferSize) {
    Serial.print(F("⚠️  Warning: Block length ("));
    Serial.print(len);
    Serial.print(F(" bytes) exceeds buffer limit ("));
    Serial.print(bufferSize);
    Serial.println(F(" bytes). Truncation may occur."));
  }
  // Check len OK
  else{
    Serial.print(F("Response length: "));
    Serial.print(len);
    Serial.println(F(" bytes"));
  }

  int available_bytes = Wire.available();
  // Writing response into buffer
  for (uint8_t i = 0; i < available_bytes; i++) {
    buffer[i] = Wire.read();
  }

  // Print received data
  printBuffer(&buffer[0], min(available_bytes, len), cmdInfo->displayFormat);

  // Little edian to big edian to reorder proprely
  reverseBufferEndian(buffer, len);

  return true;
}

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
bool runMBACommand(uint8_t address, const char* cmdName) {
    // Lookup command info
    const MBACommandInfo* cmdInfo = getMBACommandInfoByName(cmdName);
    if (cmdInfo == NULL) {
        Serial.print(F("Command not found: "));
        Serial.println(cmdName);
        Serial.println();
        return false;
    } else{
        Serial.print(F("Starting command "));
        printMBACommandInfo(cmdInfo);
    }

    // Send the ManufacturerBlockAccess command
    if (!sendMBACommand(address, cmdInfo)) {
        Serial.println(F("Failed to send command."));
        Serial.println();
        return false;
    }

    // Only print result of readable commands
    if(cmdInfo->access != "W"){
      // Where we store response
      uint8_t buffer[SOFTWAREWIRE_BUFSIZE];

      // Send command and read the response
      if (!readMBACommand(address, cmdInfo, buffer, sizeof(buffer))) {
          Serial.println(F("Failed to read command response"));
          Serial.println();
          return false;
      } 
      else{
        if (cmdInfo->bitfields && cmdInfo->bitfieldCount > 0) {
          printBitFields(&buffer[0], sizeof(buffer)-2, cmdInfo->bitfields, cmdInfo->bitfieldCount);
        }
      }
    }

    Serial.println();
    delay(100);
    return true;
}
