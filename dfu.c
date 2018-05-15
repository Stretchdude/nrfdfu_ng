#include "project.h"
#include "dfu.h"
#include "crc32.h"

struct ErrorDescription {
  int code;
  const char *description;
};

struct ErrorDescription errorDescriptions[] = {
  {0x00, "Invalid code: The provided opcode was missing or malformed."},
  {0x01, "Success: The operation completed successfully."},
  {0x02, "Opcode not supported: The provided opcode was invalid."},
  {0x03, "Invalid parameter: A parameter for the opcode was missing."},
  {0x04, "Insufficient resources: There was not enough memory for the data object."},
  {0x05, "Invalid object: The data object did not match the firmware and hardware requirements, the signature was missing, or parsing the command failed."},
  {0x07, "Unsupported type: The provided object type was not valid for a Create or Read operation."},
  {0x08, "Operation not permitted: The state of the DFU process did not allow this operation."},
  {0x0A, "Operation failed: The operation failed."},
  {-1  , "Unknown code: ???"}
};

void dfuPrintHumanReadableError(int responceCode){
  struct ErrorDescription *errorDescription = errorDescriptions;
  while (errorDescription->code != -1){
    if (errorDescription->code == responceCode){
      break;
    }
    errorDescription++;
  }
  printf("The operation failed \"%s (code: %d)\"\n", errorDescription->description, responceCode);
}

int dfuSendPackage(BLE * ble, uint8_t *packageData, size_t packageDataLength, BleObjType packageType){
  uint8_t buffer[MAX_BLE_PACKAGE_SIZE];
  int returnCode;
  size_t dataSend = 0;
  size_t chunkLength;

  uint32_t returnedOffset;
  uint32_t returnedCRC32;
  uint32_t calculatedCRC32 = crc32_compute(packageData, packageDataLength, 0);
  
  //Create a new object (No attempt at reusing existing data)
  ble_wait_setup(ble, OP_CODE_CREATE);
  buffer[0]         = OP_CODE_CREATE;
  buffer[1]         = (uint8_t)packageType;
  buffer[2]         = (packageDataLength>> 0) & 0xFF;
  buffer[3]         = (packageDataLength>> 8) & 0xFF;
  buffer[4]         = (packageDataLength>>16) & 0xFF;
  buffer[5]         = (packageDataLength>>32) & 0xFF;  
  ble_send_cp(ble, buffer, 6);
  returnCode = ble_wait_run(ble);
  if (returnCode != BLE_DFU_RESP_VAL_SUCCESS){
    dfuPrintHumanReadableError(returnCode);
    return returnCode;
  }

  //Transfer data over BLE link
  while (dataSend<packageDataLength){
    chunkLength = 20;
    if (chunkLength > packageDataLength - dataSend) {
      chunkLength = packageDataLength - dataSend;
    }

    memcpy(buffer, packageData+dataSend, chunkLength);
    if (ble_send_data_noresp(ble, buffer, chunkLength)!= EXIT_SUCCESS){
      return -1;
    }
    dataSend += chunkLength;
  }

  //Request checksum
  ble_wait_setup(ble, OP_CODE_CALCULATE_CHECKSUM);
  buffer[0]         = OP_CODE_CALCULATE_CHECKSUM;
  ble_send_cp(ble, buffer, 1);
  returnCode = ble_wait_run(ble);
  if (returnCode != BLE_DFU_RESP_VAL_SUCCESS){
    dfuPrintHumanReadableError(returnCode);
    return returnCode;
  }
  if (ble->last_notification_package_size == 11 && ble->last_notification_package[1] == OP_CODE_CALCULATE_CHECKSUM){
    returnedOffset  = ble->last_notification_package[3+0]<<0;
    returnedOffset |= ble->last_notification_package[3+1]<<8;
    returnedOffset |= ble->last_notification_package[3+2]<<16;
    returnedOffset |= ble->last_notification_package[3+3]<<24;

    returnedCRC32   = ble->last_notification_package[7+0]<<0;
    returnedCRC32  |= ble->last_notification_package[7+1]<<8;
    returnedCRC32  |= ble->last_notification_package[7+2]<<16;
    returnedCRC32  |= ble->last_notification_package[7+3]<<24;

    if (returnedCRC32==calculatedCRC32 && returnedOffset==packageDataLength){
      printf("Data uploaded successfully\n");
    }
    else {
      printf("Data upload failed\nUploaded CRC=%u File CRC=%u\nUploaded length=%u File length=%lu\n", returnedCRC32, calculatedCRC32, returnedOffset, packageDataLength);
      return -1;
    }
  }
  else {
    printf("Unexpected notification from the peripheral\n");
    return -1;
  }

  //Request command execution
  ble_wait_setup(ble, OP_CODE_EXECUTE);
  buffer[0]         = OP_CODE_EXECUTE;
  ble_send_cp(ble, buffer, 1);
  returnCode = ble_wait_run(ble);
  if (returnCode != BLE_DFU_RESP_VAL_SUCCESS){
    dfuPrintHumanReadableError(returnCode);
    return returnCode;
  }
  
  return BLE_DFU_RESP_VAL_SUCCESS;
}




int dfu (const char *bdaddr, const char *type, uint8_t * dat,
     size_t dat_sz, uint8_t * bin, size_t bin_sz)
{
  BLE *ble;
  uint8_t retries;
  uint8_t maxRetries=3;
  uint8_t done = 0;
  

  ble_init ();

  for (retries=0; retries<maxRetries && (!done); retries++){
    ble = ble_open(bdaddr);
    if (ble == 0){
      printf("ERROR: Unable to ble_open with address=%s\n", bdaddr);
      break;
    }

    if (ble_register_notify(ble)){
      printf("ERROR: Unable to ble_register_notify\n");
      break;
    }

    printf("=== Sending INIT package  ===\n");
    if (dfuSendPackage(ble, dat, dat_sz, BLE_OBJ_TYPE_COMMAND) == BLE_DFU_RESP_VAL_SUCCESS){
      printf("=== Sending DATA package  ===\n");
      if (dfuSendPackage(ble, bin, bin_sz, BLE_OBJ_TYPE_DATA) == BLE_DFU_RESP_VAL_SUCCESS){
	done = 1;
      }
    }



    ble_close(ble);

    if (retries<maxRetries && (!done)) {
      printf("Operation failed, retrying in 3 seconds\n");
      sleep (3);
    }
  }

  
  if (retries == maxRetries){
    printf("Too many retries, the operation failed!!!\n");
    return -1;
  }
  else {
    return 0;
  }

}
