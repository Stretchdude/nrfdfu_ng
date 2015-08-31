#include "project.h"

static int verbose = 1;

#define ATT_CID 4



static int
l2cap_le_att_connect (bdaddr_t * src, bdaddr_t * dst, uint8_t dst_type,
                      int sec)
{
  int sock;
  struct sockaddr_l2 srcaddr, dstaddr;
  struct bt_security btsec;

  if (verbose)
    {
      char srcaddr_str[18], dstaddr_str[18];

      ba2str (src, srcaddr_str);
      ba2str (dst, dstaddr_str);

      printf ("btgatt-client: Opening L2CAP LE connection on ATT "
              "channel:\n\t src: %s\n\tdest: %s\n", srcaddr_str, dstaddr_str);
    }

  sock = socket (PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
  if (sock < 0)
    {
      perror ("Failed to create L2CAP socket");
      return -1;
    }

  /* Set up source address */
  memset (&srcaddr, 0, sizeof (srcaddr));
  srcaddr.l2_family = AF_BLUETOOTH;
  srcaddr.l2_cid = htobs (ATT_CID);
  srcaddr.l2_bdaddr_type = 0;
  bacpy (&srcaddr.l2_bdaddr, src);

  if (bind (sock, (struct sockaddr *) &srcaddr, sizeof (srcaddr)) < 0)
    {
      perror ("Failed to bind L2CAP socket");
      close (sock);
      return -1;
    }

  /* Set the security level */
  memset (&btsec, 0, sizeof (btsec));
  btsec.level = sec;
  if (setsockopt (sock, SOL_BLUETOOTH, BT_SECURITY, &btsec,
                  sizeof (btsec)) != 0)
    {
      fprintf (stderr, "Failed to set L2CAP security level\n");
      close (sock);
      return -1;
    }

  /* Set up destination address */
  memset (&dstaddr, 0, sizeof (dstaddr));
  dstaddr.l2_family = AF_BLUETOOTH;
  dstaddr.l2_cid = htobs (ATT_CID);
  dstaddr.l2_bdaddr_type = dst_type;
  bacpy (&dstaddr.l2_bdaddr, dst);

  printf ("Connecting to device...");
  fflush (stdout);

  if (connect (sock, (struct sockaddr *) &dstaddr, sizeof (dstaddr)) < 0)
    {
      perror (" Failed to connect");
      close (sock);
      return -1;
    }

  printf (" Done\n");

  return sock;
}



void ble_finish(struct ble *ble)
{
if (!ble) return;

if (ble->att) bt_att_unref(ble->att);
if (ble->fd>0) close(ble->fd);

free(ble);
}


struct ble *ble_start (const char *bdaddr)
{
  struct ble *ble;

  ble=xmalloc(sizeof(*ble));
  memset(ble,0,sizeof(*ble));

  ble->sec = BT_SECURITY_LOW;
  ble->dst_type = BDADDR_LE_RANDOM;
  bacpy (&ble->src_addr, BDADDR_ANY);

  if (str2ba (bdaddr, &ble->dst_addr) < 0)
    {
      fprintf (stderr, "Invalid remote address: %s\n", bdaddr);
      ble_finish(ble);
      return NULL;
    }


  ble->fd = l2cap_le_att_connect (&ble->src_addr, &ble->dst_addr, ble->dst_type, ble->sec);
  if (ble->fd < 0)
    {
      perror ("l2cap_le_att_connect");
      ble_finish(ble);
      return NULL;
    }


    ble->att = bt_att_new(ble->fd);

        if (!ble->att) {
                fprintf(stderr, "Failed to initialze ATT transport layer\n");
		ble_finish(ble);
                return NULL;
        }

        if (!bt_att_set_close_on_unref(ble->att, true)) {
                fprintf(stderr, "Failed to set up ATT transport layer\n");
		ble_finish(ble);
                return NULL;
        }

        if (!bt_att_register_disconnect(ble->att, att_disconnect_cb, NULL,
                                                                NULL)) {
                fprintf(stderr, "Failed to set ATT disconnect handler\n");
		ble_finish(ble);
                return NULL;
        }



  printf ("Connected!\n");
}
