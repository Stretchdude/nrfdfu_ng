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

  int notify_waiting_for_op;
  int notify_code;


  size_t notify_pkts;
  int notify_waiting_for_pkts;

};

typedef struct ble BLE;
