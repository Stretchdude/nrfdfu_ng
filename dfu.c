#include "project.h"
#include "dfu.h"


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
  //No attempt at reusing existing data

  int returnCode;

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


    if (dfuSendPackage(ble, dat, dat_sz, BLE_OBJ_TYPE_COMMAND) == BLE_DFU_RESP_VAL_SUCCESS){
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

  /*do
    {

      b = ble_open (bdaddr);

      if (!b)
        break;

      if (ble_register_notify (b))
        break;

           buf[0] = OP_CODE_START_DFU;
      buf[1] = dfu_type;

      ble_wait_setup (b, OP_CODE_START_DFU);

      if (ble_send_cp (b, buf, 2))
        break;

      //4 bytes sd size, 4 bytes bl size, 4 bytes app size 


      if (ble_send_data (b, (uint8_t *) start_data, sizeof (start_data)))
        break;

      if (ble_wait_run
 (b) != BLE_DFU_RESP_VAL_SUCCESS)
        break;

      buf[0] = OP_CODE_RECEIVE_INIT;
      buf[1] = DFU_INIT_RX;

      if (ble_send_cp (b, buf, 2))
        break;

      ble_wait_setup (b, OP_CODE_RECEIVE_INIT);
      if (ble_send_data (b, dat, dat_sz))
        break;

      buf[0] = OP_CODE_RECEIVE_INIT;
      buf[1] = DFU_INIT_COMPLETE;
      if (ble_send_cp (b, buf, 2))
        break;

      if (ble_wait_run (b) != BLE_DFU_RESP_VAL_SUCCESS)
        break;


#if 1
//  ble_wait_setup(b,OP_CODE_PKT_RCPT_NOTIF_REQ);
//
      buf[0] = OP_CODE_PKT_RCPT_NOTIF_REQ;
      u16 = 0x40;
      memcpy (&buf[1], &u16, 2);

      if (ble_send_cp (b, buf, 3))
        break;

//  if (ble_wait_run(b)!=BLE_DFU_RESP_VAL_SUCCESS)
// break;

      ble_wait_setup (b, OP_CODE_RECEIVE_FW);

      buf[0] = OP_CODE_RECEIVE_FW;
      if (ble_send_cp (b, buf, 1))
        break;

      if (send_data_quickly (b, bin, bin_sz, u16) != EXIT_SUCCESS)
        break;


      if (ble_wait_run (b) != BLE_DFU_RESP_VAL_SUCCESS)
        break;
#else

      ble_wait_setup (b, OP_CODE_RECEIVE_FW);

      buf[0] = OP_CODE_RECEIVE_FW;
      if (ble_send_cp (b, buf, 1))
        break;

      if (send_data_slowly (b, bin, bin_sz) != EXIT_SUCCESS)
        break;


      if (ble_wait_run (b) != BLE_DFU_RESP_VAL_SUCCESS)
        break;
#endif

      buf[0] = OP_CODE_IMAGE_SIZE_REQ;
      ble_send_cp (b, buf, 1);

      ble_wait_setup (b, OP_CODE_VALIDATE);
      buf[0] = OP_CODE_VALIDATE;
      if (ble_send_cp (b, buf, 1))
        break;
      if (ble_wait_run (b) != BLE_DFU_RESP_VAL_SUCCESS)
        break;

      buf[0] = OP_CODE_ACTIVATE_N_RESET;
      if (ble_send_cp_noresp (b, buf, 1))
        break;

      printf ("Success....\n");

      ble_close (b);
      return 0;


      }*/
  //  while (0);

  /*  if (b)
    {
      buf[0] = OP_CODE_SYS_RESET;
      ble_send_cp_noresp (b, buf, 1);
      sleep (1);
      ble_close (b);
    }
  */
