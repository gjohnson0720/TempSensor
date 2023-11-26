#include "driver/i2c.h"
#include "esp_err.h"
#include "hal/i2c_ll.h"

#define I2C_MASTER_FREQ_HZ          100000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */

static bool initialized = false;

#define kGpi__i2c_sda      	GPIO_NUM_18
#define kGpo__i2c_scl      	GPIO_NUM_19
#define kI2c__port      0
#define kI2c_master_tx_timeout_ms       0
#define kI2c_master_rx_timeout_ms       0

esp_err_t init_i2c_master()
{
	esp_err_t err = ESP_OK;
	if (!initialized)
	{
		i2c_config_t conf = {
			.mode = I2C_MODE_MASTER,
			.sda_io_num = kGpi__i2c_sda,
			.scl_io_num = kGpo__i2c_scl,
			.sda_pullup_en = GPIO_PULLUP_ENABLE,
			.scl_pullup_en = GPIO_PULLUP_ENABLE,
			.master.clk_speed = I2C_MASTER_FREQ_HZ,
		};

		esp_err_t err = i2c_param_config(kI2c__port, &conf);
		if (err != ESP_OK)
		{
			printf("Failed i2c_param_config with error %s\n", esp_err_to_name(err));
		}
		err = i2c_driver_install(kI2c__port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
		if (err != ESP_OK)
		{
			printf("Failed i2c_driver_install with error %s\n", esp_err_to_name(err));
		}
		i2c_reset_rx_fifo(kI2c__port);
		i2c_reset_tx_fifo(kI2c__port);
		i2c_set_timeout(kI2c__port, I2C_LL_MAX_TIMEOUT);
		initialized = true;
	}
    return err;
}

esp_err_t i2c_register_read(uint8_t device_address, uint8_t reg_addr, uint8_t *data, size_t len)
{
	return i2c_master_write_read_device(kI2c__port, device_address, &reg_addr, 1, data, len, kI2c_master_rx_timeout_ms / portTICK_PERIOD_MS);
}

esp_err_t i2c_register_read_bytes(uint8_t device_address, uint8_t *data, size_t len)
{
	return i2c_master_read_from_device(kI2c__port, device_address, data, len, kI2c_master_rx_timeout_ms / portTICK_PERIOD_MS);
}

esp_err_t i2c_register_write_byte(uint8_t device_address, uint8_t reg_addr, uint8_t data)
{
    uint8_t write_buf[2] = {reg_addr, data};

    esp_err_t ret = i2c_master_write_to_device(kI2c__port, device_address, write_buf, sizeof(write_buf), portMAX_DELAY);//kI2c_master_timeout_ms / portTICK_PERIOD_MS);

    //printf("W reg 0x%x value 0x%x\n", reg_addr, data);
    return ret;
}

esp_err_t i2c_register_write(uint8_t device_address, uint8_t reg_addr, uint8_t data[], size_t len)
{
    uint8_t write_buf[len+1];
    write_buf[0] = reg_addr;
    for (int i = 0; i < len; i++)
    {
    	write_buf[i+1] = data[i];
    }

    esp_err_t ret = i2c_master_write_to_device(kI2c__port, device_address, write_buf, sizeof(write_buf), kI2c_master_tx_timeout_ms / portTICK_PERIOD_MS);

    //printf("W reg 0x%x value 0x%x\n", reg_addr, data);
    return ret;
}

esp_err_t i2c_register_write_bytes(uint8_t device_address, const uint8_t* write_buffer, size_t write_size)
{
	return i2c_master_write_to_device(kI2c__port, device_address, write_buffer, write_size, kI2c_master_tx_timeout_ms / portTICK_PERIOD_MS);
}

