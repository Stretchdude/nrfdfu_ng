#include "project.h"

static void
usage (char *name)
{
  fprintf (stderr, "Usage: %s -b bdaddr -p pkg_file\n", name);
  exit (EXIT_FAILURE);
}











int
main (int argc, char *argv[])
{
  char *bdaddr = NULL;
  char *pkg_fn = NULL;
  int opt;
  struct zip *zip;
  char *m_str;
  struct manifest *m;

  uint8_t *bin;
  size_t bin_size;

  uint8_t *dat;
  size_t dat_size;



  while ((opt = getopt (argc, argv, "b:p:")) != -1)
    {
      switch (opt)
        {
        case 'b':
          bdaddr = optarg;
          break;
        case 'p':
          pkg_fn = optarg;
          break;
        default:               /* '?' */
          usage (argv[0]);
        }
    }

  if ((!bdaddr) || (!pkg_fn))
    usage (argv[0]);


  zip = open_zip (pkg_fn);

  read_file_from_zip (zip, "manifest.json", &m_str);

  m = parse_manifest (m_str);

  dat_size = read_file_from_zip (zip, m->dat_file, &dat);

  bin_size = read_file_from_zip (zip, m->bin_file, &bin);


  printf ("%u bytes init_data, %u bytes firmware\n",(unsigned) dat_size, (unsigned) bin_size);


  dfu (bdaddr, m->type, m->dfu_version, dat, dat_size, bin, bin_size);


return EXIT_SUCCESS;
}
