struct ble {
    bdaddr_t src_addr, dst_addr;
    int sec;
    uint8_t dst_type;

    int fd;

    struct bt_att *att;
    struct gatt_db *db;
    struct bt_gatt_client *gatt;
};


