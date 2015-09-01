#include "project.h"

void
dfu (const char *bdaddr, const char *type, const char *version, uint8_t * dat,
     size_t dat_sz, uint8_t * bin, size_t bin_sz)
{
 BLE *b;
 uint8_t buf[32];


  ble_init();





 do {

  b=ble_open (bdaddr);
  
  if (!b) 
	break;

  if (ble_register_notify(b)) break;

  ble_send_cp(

  ble_close (b);
  return;


} while (0);



  ble_close (b);
	exit(EXIT_FAILURE);

}
