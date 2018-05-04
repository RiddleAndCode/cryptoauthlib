
#include "hal_i2c_stm32l475.h"

extern I2C_HandleTypeDef hi2c3;

/** \brief initialize an I2C interface using given config
 * \param[in] hal  opaque ptr to HAL data
 * \param[in] cfg  pointer to interface configuration
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS hal_i2c_init(void *hal, ATCAIfaceCfg *cfg) { return ATCA_SUCCESS; }

/** \brief HAL implementation of I2C post init
 * \param[in] iface  instance
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS hal_i2c_post_init(ATCAIface iface) { return ATCA_SUCCESS; }

/** \brief HAL implementation of I2C send over ASF
 * \param[in] iface     instance
 * \param[in] txdata    pointer to space to bytes to send
 * \param[in] txlength  number of bytes to send
 * \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS hal_i2c_send(ATCAIface iface, uint8_t *txdata, int txlength) {
  // for this implementation of I2C with CryptoAuth chips, txdata is assumed to
  // have ATCAPacket format

  // other device types that don't require i/o tokens on the front end of a
  // command need a different hal_i2c_send and wire it up instead of this one
  // this covers devices such as ATSHA204A and ATECCx08A that require a word
  // address value pre-pended to the packet

  uint32_t status = HAL_ERROR;
  txdata[0] = 0x03;
  txlength++;

  do {
    status = HAL_I2C_Master_Transmit(&hi2c3, 0xC0, txdata, txlength, 1000);
  } while (HAL_I2C_GetError(&hi2c3) == HAL_I2C_ERROR_AF);

  // free(tmp);
  if (status != HAL_OK)
    return ATCA_COMM_FAIL;
  return ATCA_SUCCESS;
}
ATCA_STATUS hal_i2c_receive(ATCAIface iface, uint8_t *rxdata,
                            uint16_t *rxlength) {
  uint32_t status = HAL_ERROR;

  do {
    status = HAL_I2C_Master_Receive(&hi2c3, 0xC0, rxdata, *rxlength, 1000);
  } while (HAL_I2C_GetError(&hi2c3) == HAL_I2C_ERROR_AF);
  if (status != HAL_OK)
    return ATCA_COMM_FAIL;

  return ATCA_SUCCESS;
}
ATCA_STATUS hal_i2c_wake(ATCAIface iface) {

  uint8_t data[4], expected[4] = {0x04, 0x11, 0x33, 0x43};
  uint16_t data_size = 4;
  do {
    HAL_I2C_Master_Transmit(&hi2c3, 0xC0, 0x00, 1, 1000);
  } while (HAL_I2C_GetError(&hi2c3) == HAL_I2C_ERROR_AF);
  
  hal_i2c_receive(iface, data, &data_size);

  if (memcmp(data, expected, 4) != 0) {
    return ATCA_COMM_FAIL;
  }

  return ATCA_SUCCESS;
}
ATCA_STATUS hal_i2c_idle(ATCAIface iface) {
  uint8_t data[4];
  data[0] = 0x02; // idle word address value

  do {
    HAL_I2C_Master_Transmit(&hi2c3, 0xC0, data, 1, 1000);
  } while (HAL_I2C_GetError(&hi2c3) == HAL_I2C_ERROR_AF);
  return ATCA_SUCCESS;
}
ATCA_STATUS hal_i2c_sleep(ATCAIface iface) {
  uint8_t data[4];
  data[0] = 0x01; // sleep word address value

  do {
    HAL_I2C_Master_Transmit(&hi2c3, 0xC0, data, 1, 1000);
  } while (HAL_I2C_GetError(&hi2c3) == HAL_I2C_ERROR_AF);
  return ATCA_SUCCESS;
}
ATCA_STATUS hal_i2c_release(void *hal_data) { return ATCA_SUCCESS; }
ATCA_STATUS hal_i2c_discover_buses(int i2c_buses[], int max_buses) {
  return ATCA_SUCCESS;
}
ATCA_STATUS hal_i2c_discover_devices(int bus_num, ATCAIfaceCfg *cfg,
                                     int *found) {
  ATCAIfaceCfg discoverCfg = {.iface_type = ATCA_I2C_IFACE,
                              .devtype = ATECC608A,
                              .atcai2c.slave_address = 0xC0,
                              .atcai2c.bus = 1,
                              .atcai2c.baud = 400000,
                              .wake_delay = 1500,
                              .rx_retries = 3};
  hal_i2c_init(NULL, &discoverCfg);

  return ATCA_SUCCESS;
}