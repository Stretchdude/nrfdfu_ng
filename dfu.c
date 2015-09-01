#include "project.h"
#include "dfu.h"

void
dfu (const char *bdaddr, const char *type, const char *version, uint8_t * dat,
     size_t dat_sz, uint8_t * bin, size_t bin_sz)
{
  BLE *b;
  uint8_t buf[32];
  uint32_t u32;
  uint16_t u16;

  if (strcmp (type, "application"))
    {
      fprintf (stderr, "only know how to flash applications\n");
      exit (EXIT_FAILURE);
    }

  ble_init ();


  do
    {

      b = ble_open (bdaddr);

      if (!b)
        break;

      if (ble_register_notify (b))
        break;

      buf[0] = OP_CODE_START_DFU;
      buf[1] = DFU_UPDATE_APP;  /*bit field */

      ble_wait_setup (b, OP_CODE_START_DFU);

      if (ble_send_cp (b, buf, 2))
        break;

      /*4 bytes sd size, 4 bytes bl size, 4 bytes app size */

      memset (buf, 0, 12);
      u32 = bin_sz;
      memcpy (&buf[8], &u32, 4);

      if (ble_send_data (b, buf, 12))
        break;

      if (ble_wait_run (b) != BLE_DFU_RESP_VAL_SUCCESS)
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


//  ble_wait_setup(b,OP_CODE_PKT_RCPT_NOTIF_REQ);
      buf[0] = OP_CODE_PKT_RCPT_NOTIF_REQ;
      u16 = 0x10;
      memcpy (&buf[1], &u16, 2);

      if (ble_send_cp (b, buf, 3))
        break;

//  if (ble_wait_run(b)!=BLE_DFU_RESP_VAL_SUCCESS)
// break;



      ble_wait_setup (b, OP_CODE_RECEIVE_FW);

      buf[0] = OP_CODE_RECEIVE_FW;
      if (ble_send_cp (b, buf, 1))
        break;

      if (ble_send_data (b, bin, bin_sz))
        break;

      if (ble_wait_run (b) != BLE_DFU_RESP_VAL_SUCCESS)
        break;

      ble_wait_setup (b, OP_CODE_VALIDATE);
      buf[0] = OP_CODE_VALIDATE;
      if (ble_send_cp (b, buf, 1))
        break;
      if (ble_wait_run (b) != BLE_DFU_RESP_VAL_SUCCESS)
        break;

      buf[0] = OP_CODE_ACTIVATE_N_RESET;
      if (ble_send_cp (b, buf, 1))
        break;

      ble_close (b);
      return;


    }
  while (0);

  if (b)
    {
      buf[0] = OP_CODE_SYS_RESET;
      ble_send_cp (b, buf, 1);
    }



  ble_close (b);
  exit (EXIT_FAILURE);

}
