#include "project.h"
#include "dfu.h"
#include "bluez/mainloop.h"


int update(char *updatefilename, char *pkg_fn, int retryCount)
{
  int opt;
  bdaddr_t dst;
  struct zip *zip;
  char *m_str;
  struct manifest *m;

  uint8_t *bin;
  size_t bin_size;

  uint8_t *dat;
  size_t dat_size;

  ble_init();

  if (str2ba (bdaddr, &dst) < 0) {
    printf ("Invalid remote address: %s\n", bdaddr);
    return EXIT_FAILURE;
  }

  zip = open_zip (pkg_fn);

  read_file_from_zip (zip, "manifest.json", &m_str);

  m = parse_manifest (m_str);

  if (m->hasSDBootloader) {
    dat_size = read_file_from_zip (zip, m->sdBootloaderDatFileName, &dat);
    bin_size = read_file_from_zip (zip, m->sdBootloaderBinFileName, &bin);
    
    printf ("%u bytes init_data, %u bytes SD+bootloader\n\n", (unsigned) dat_size, (unsigned) bin_size);

    if (dfu(&dst, dat, dat_size, bin, bin_size, retryCount) != BLE_DFU_RESP_VAL_SUCCESS){
      mainloop_finish ();
      return EXIT_FAILURE;  
    }
    sleep(5);
  }

  if (m->hasBootloader) {
    dat_size = read_file_from_zip (zip, m->bootloaderDatFileName, &dat);
    bin_size = read_file_from_zip (zip, m->bootloaderBinFileName, &bin);
    
    printf ("%u bytes init_data, %u bytes bootloader\n\n", (unsigned) dat_size, (unsigned) bin_size);

    if (dfu(&dst, dat, dat_size, bin, bin_size, retryCount) != BLE_DFU_RESP_VAL_SUCCESS){
      mainloop_finish();
      return EXIT_FAILURE;  
    }
    sleep(5);
  }


  if (m->hasApplication) {
    dat_size = read_file_from_zip (zip, m->applicationDatFileName, &dat);
    bin_size = read_file_from_zip (zip, m->applicationBinFileName, &bin);
    
    printf ("%u bytes init_data, %u bytes firmware\n\n", (unsigned) dat_size, (unsigned) bin_size);

    if (dfu(&dst, dat, dat_size, bin, bin_size, retryCount) != BLE_DFU_RESP_VAL_SUCCESS){
      mainloop_finish ();
      return EXIT_FAILURE;  
    }
  }

  mainloop_finish ();
  return EXIT_SUCCESS;
}
