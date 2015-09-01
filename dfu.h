

#define BLE_DFU_SERVICE_UUID                 0x1530                       /**< The UUID of the DFU Service. */
#define BLE_DFU_PKT_CHAR_UUID                0x1532                       /**< The UUID of the DFU Packet Characteristic. */
#define BLE_DFU_CTRL_PT_UUID                 0x1531                       /**< The UUID of the DFU Control Point. */
#define BLE_DFU_STATUS_REP_UUID              0x1533                       /**< The UUID of the DFU Status Report Characteristic. */
#define BLE_DFU_REV_CHAR_UUID                0x1534                       /**< The UUID of the DFU Revision Characteristic. */

typedef enum
{
  BLE_DFU_RESP_VAL_SUCCESS = 1,                                         /**< Success.*/
  BLE_DFU_RESP_VAL_INVALID_STATE,                                       /**< Invalid state.*/
  BLE_DFU_RESP_VAL_NOT_SUPPORTED,                                       /**< Operation not supported.*/
  BLE_DFU_RESP_VAL_DATA_SIZE,                                           /**< Data size exceeds limit.*/
  BLE_DFU_RESP_VAL_CRC_ERROR,                                           /**< CRC Error.*/
  BLE_DFU_RESP_VAL_OPER_FAILED                                          /**< Operation failed.*/
} ble_dfu_resp_val_t;

enum
{
  OP_CODE_START_DFU = 1,                                                        /**< Value of the Op code field for 'Start DFU' command.*/
  OP_CODE_RECEIVE_INIT = 2,                                                     /**< Value of the Op code field for 'Initialize DFU parameters' command.*/
  OP_CODE_RECEIVE_FW = 3,                                                       /**< Value of the Op code field for 'Receive firmware image' command.*/
  OP_CODE_VALIDATE = 4,                                                         /**< Value of the Op code field for 'Validate firmware' command.*/
  OP_CODE_ACTIVATE_N_RESET = 5,                                                 /**< Value of the Op code field for 'Activate & Reset' command.*/
  OP_CODE_SYS_RESET = 6,                                                        /**< Value of the Op code field for 'Reset System' command.*/
  OP_CODE_IMAGE_SIZE_REQ = 7,                                                   /**< Value of the Op code field for 'Report received image size' command.*/
  OP_CODE_PKT_RCPT_NOTIF_REQ = 8,                                               /**< Value of the Op code field for 'Request packet receipt notification.*/
  OP_CODE_RESPONSE = 16,                                                        /**< Value of the Op code field for 'Response.*/
  OP_CODE_PKT_RCPT_NOTIF = 17                                                   /**< Value of the Op code field for 'Packets Receipt Notification'.*/
};

#define DFU_UPDATE_SD                   0x01                                                            /**< Bit field indicating update of SoftDevice is ongoing. */
#define DFU_UPDATE_BL                   0x02                                                            /**< Bit field indicating update of bootloader is ongoing. */
#define DFU_UPDATE_APP                  0x04                                                            /**< Bit field indicating update of application is ongoing. */

#define DFU_INIT_RX                     0x00                                                            /**< Op Code identifies for receiving init packet. */
#define DFU_INIT_COMPLETE               0x01                                                            /**< Op Code identifies for transmission complete of init packet. */
