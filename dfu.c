#include "project.h"
#include "dfu.h"

static int
send_data_quickly (BLE * b, uint8_t * d, size_t sz, int pkts)
{
  char bs[64];
  size_t mtu = 16;
  size_t off = 0;
  size_t tx, rx;
  int sent = 0;

  ble_notify_pkts_start (b);

  memset (bs, '\b', sizeof (bs));

  while (off != sz)
    {
      fwrite (bs, 1, sizeof (bs), stdout);
      printf ("%6u/%6u bytes", (unsigned) off, (unsigned) sz);
      fflush (stdout);

      tx = (sz - off);
      if (tx > mtu)
        tx = mtu;

      if (ble_send_data_noresp (b, d + off, tx))
        {
          printf ("\nFailed\n");
          ble_notify_pkts_stop (b);
          return EXIT_FAILURE;
        }

      off += tx;
      sent++;

      if (sent == pkts)
        {
          rx = ble_notify_get_pkts (b);

          if (rx != off)
            {
              printf ("\nFailed -Lost a packet\n");
              ble_notify_pkts_stop (b);
              return EXIT_FAILURE;
            }
          sent = 0;
        }

    }

  ble_notify_pkts_stop (b);

  printf ("\nDone\n");
  return EXIT_SUCCESS;

}


#if 0
static int
send_data_slowly (BLE * b, uint8_t * d, size_t sz)
{
  char bs[64];
  size_t mtu = 16;
  size_t off = 0;
  size_t tx;

  memset (bs, '\b', sizeof (bs));

  while (off != sz)
    {
      fwrite (bs, 1, sizeof (bs), stdout);
      printf ("%6u/%6u bytes", (unsigned) off, (unsigned) sz);
      fflush (stdout);

      tx = (sz - off);
      if (tx > mtu)
        tx = mtu;

      if (ble_send_data (b, d + off, tx))
        {
          printf ("\nFailed\n");
          return EXIT_FAILURE;
        }

      off += tx;
    }


  printf ("\nDone\n");
  return EXIT_SUCCESS;

}
#endif

int
dfu (const char *bdaddr, const char *type, const char *version, uint8_t * dat,
     size_t dat_sz, uint8_t * bin, size_t bin_sz)
{
  BLE *b;
  uint8_t buf[32];
  uint16_t u16;

  uint32_t start_data[3];
  uint8_t dfu_type;

  if (!strcmp (type, "application"))
    {
      dfu_type = DFU_UPDATE_APP; /*bit field */
      start_data[0] = 0;
      start_data[1] = 0;
      start_data[2] = bin_sz;

    }
  else if (!strcmp (type, "bootloader"))
    {
      dfu_type = DFU_UPDATE_BL; /*bit field */
      start_data[0] = 0;
      start_data[1] = bin_sz;
      start_data[2] = 0;
    }
  else
    {
      fprintf (stderr, "No idea how to upload %s\n", type);
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
      buf[1] = dfu_type;        /*bit field */

      ble_wait_setup (b, OP_CODE_START_DFU);

      if (ble_send_cp (b, buf, 2))
        break;

      /*4 bytes sd size, 4 bytes bl size, 4 bytes app size */


      if (ble_send_data (b, (uint8_t *) start_data, sizeof (start_data)))
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


    }
  while (0);

  if (b)
    {
      buf[0] = OP_CODE_SYS_RESET;
      ble_send_cp_noresp (b, buf, 1);
      sleep (1);
    }



  ble_close (b);
  return -1;

}
