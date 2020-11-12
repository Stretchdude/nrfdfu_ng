#define GATT_HEADER_LEN	3
#define GATT_PAYLOAD(mtu)	((mtu) - GATT_HEADER_LEN)

struct ble
{
  bdaddr_t src_addr, dst_addr;
  int sec;
  uint8_t dst_type;

  uint16_t mtu;

  int fd;

  struct bt_att *att;
  struct gatt_db *db;
  struct bt_gatt_client *gatt;

  unsigned notify_id;

  uint16_t cp_handle;
  uint16_t cccd_handle;
  uint16_t data_handle;
  uint16_t btnless_handle;

  int notify_waiting_for_op;
  int notify_code;
  int extended_notify_code;


  size_t notify_pkts;
  int notify_waiting_for_pkts;

  size_t last_notification_package_size;
  uint8_t last_notification_package[BT_ATT_DEFAULT_LE_MTU];

  ///If true the ble code is allowed to emit debug messages
  uint8_t debug;
};

typedef struct ble BLE;
