#include "project.h"
#include <string.h>
#if 0
json_object *
_json_object_object_get (json_object * obj, const char *name)
{
  json_object *sub;
  return json_object_object_get_ex (obj, name, &sub) ? sub : NULL;
}
#endif

static void dump_manifest (struct manifest *m)
{
  printf ("Manifest:\n");
  if (m->hasSDBootloader){
    printf("  == SD+Bootloader ==\n");
    printf("  dat_file : %s\n", m->sdBootloaderDatFileName);
    printf("  bin_file : %s\n", m->sdBootloaderBinFileName);
    printf("\n");
  }
  if (m->hasBootloader){
    printf("  == Bootloader ==\n");
    printf("  dat_file : %s\n", m->bootloaderDatFileName);
    printf("  bin_file : %s\n", m->bootloaderBinFileName);
    printf("\n");
  }
  if (m->hasApplication){
    printf("  == Application ==\n");
    printf("  dat_file : %s\n", m->applicationDatFileName);
    printf("  bin_file : %s\n", m->applicationBinFileName);
    printf("\n");
  }
}


const char *DFU_VERSION="0";

static void fake_parser(char *out,const char *haystack, const  char *first, const char *second) 
{
        char *ptr = strstr(haystack, first);
	if (NULL == ptr) {
		return;
	}
        char *p = strstr(haystack, second);
	if (NULL == p) {
		return;
	}
        ptr = strstr(p, "\"");
        ptr++;
        p = strstr(ptr, "\"");
        p++;
        ptr = strstr(p, "\"");

        char name[256] = {0};
        int i = 0;
        while(p < ptr) {
                name[i] = *p++;
                i++;
		if (i >= sizeof(name)) {
			break;
		}
        }   
    
        strcpy(out, name);
}


static void parse_value(const char **oDat, const char **oBin, char *oHas, const char *haystack, const char *image)
{
  char name[64] = {0};
  if(0 < (*oHas = (!!strstr(haystack, image)))) {
    fake_parser(name, haystack, image, "dat_file" );
    *oDat = strdup(name);
    fake_parser(name, haystack, image, "bin_file");
    *oBin = strdup(name);
  }
}


struct manifest parse_manifest (const char *str)
{
  struct manifest m = {0};

  parse_value(&m.applicationDatFileName, &m.applicationBinFileName, &m.hasApplication, str, "application");
  parse_value(&m.sdBootloaderDatFileName, &m.sdBootloaderBinFileName, &m.hasSDBootloader, str, "softdevice_bootloader");
  parse_value(&m.bootloaderDatFileName, &m.bootloaderBinFileName, &m.hasBootloader, str, "bootloader");

  dump_manifest(&m);

  return m; 
}
