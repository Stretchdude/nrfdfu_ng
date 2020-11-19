
#include "libnrfdfu.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>






static void usage (char *name)
{
	fprintf (stderr, "Usage: %s -b bdaddr -p pkg_file\n [-c retry_count]", name);
	exit (EXIT_FAILURE);
}

int main (int argc, char *argv[])
{
	char *bdaddr = NULL;
	char *pkg_fn = NULL;
	int opt;
	struct zip *zip;
	char *m_str;

	uint8_t *bin;
	size_t bin_size;

	uint8_t *dat;
	size_t dat_size;

	int retryCount = 0;

	while ((opt = getopt (argc, argv, "b:p:c:")) != -1)
	{
		switch (opt)
		{
			case 'b':
				bdaddr = optarg;
				break;
			case 'p':
				pkg_fn = optarg;
				break;
			case 'c':
				retryCount = atoi(optarg);
				if (retryCount < 0) { retryCount = 0; }
				break;
			default:               /* '?' */
				usage (argv[0]);
		}
	}

	if ((!bdaddr) || (!pkg_fn))
		usage (argv[0]);


	int ret =  update(bdaddr, pkg_fn, retryCount);


	return ret;

}
