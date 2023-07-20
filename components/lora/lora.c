#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

/*
 * Register definitions
 */
#define REG_FIFO                       0x00
#define REG_OP_MODE                    0x01
#define REG_FRF_MSB                    0x06
#define REG_FRF_MID                    0x07
#define REG_FRF_LSB                    0x08
#define REG_PA_CONFIG                  0x09
#define REG_LNA                        0x0c
#define REG_FIFO_ADDR_PTR              0x0d
#define REG_FIFO_TX_BASE_ADDR          0x0e
#define REG_FIFO_RX_BASE_ADDR          0x0f
#define REG_FIFO_RX_CURRENT_ADDR       0x10
#define REG_IRQ_FLAGS                  0x12
#define REG_RX_NB_BYTES                0x13
#define REG_PKT_SNR_VALUE              0x19
#define REG_PKT_RSSI_VALUE             0x1a
#define REG_MODEM_CONFIG_1             0x1d
#define REG_MODEM_CONFIG_2             0x1e
#define REG_PREAMBLE_MSB               0x20
#define REG_PREAMBLE_LSB               0x21
#define REG_PAYLOAD_LENGTH             0x22
#define REG_MODEM_CONFIG_3             0x26
#define REG_RSSI_WIDEBAND              0x2c
#define REG_DETECTION_OPTIMIZE         0x31
#define REG_DETECTION_THRESHOLD        0x37
#define REG_SYNC_WORD                  0x39
#define REG_DIO_MAPPING_1              0x40
#define REG_DIO_MAPPING_2              0x41
#define REG_VERSION                    0x42

/*
 * Transceiver modes
 */
#define MODE_LONG_RANGE_MODE           0x80
#define MODE_SLEEP                     0x00
#define MODE_STDBY                     0x01
#define MODE_TX                        0x03
#define MODE_RX_CONTINUOUS             0x05
#define MODE_RX_SINGLE                 0x06

/*
 * PA configuration
 */
#define PA_BOOST                       0x80

/*
 * IRQ masks
 */
#define IRQ_TX_DONE_MASK               0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK     0x20
#define IRQ_RX_DONE_MASK               0x40

#define PA_OUTPUT_RFO_PIN              0
#define PA_OUTPUT_PA_BOOST_PIN         1

#define TIMEOUT_RESET                  100

// SPI Stuff
#if CONFIG_SPI2_HOST
#define HOST_ID SPI2_HOST
#elif CONFIG_SPI3_HOST
#define HOST_ID SPI3_HOST
#endif


#define TAG "LORA"

static spi_device_handle_t __spi;

static int __implicit;
static long __frequency;
static int __send_packet_lost = 0;

// use spi_device_transmit
#define SPI_TRANSMIT 1

// use buffer io
// A little faster
#define BUFFER_IO 1


/**
 * Write a value to a register.  * @param reg Register index.
 * @param val Value to write.
 */
void 
lora_write_reg(int reg, int val)
{
   uint8_t out[2] = { 0x80 | reg, val };
   uint8_t in[2];

   spi_transaction_t t = {
      .flags = 0,
      .length = 8 * sizeof(out),
      .tx_buffer = out,
      .rx_buffer = in  
   };

   //gpio_set_level(CONFIG_CS_GPIO, 0);
#if SPI_TRANSMIT
   spi_device_transmit(__spi, &t);
#else
   spi_device_polling_transmit(__spi, &t);
#endif
   //gpio_set_level(CONFIG_CS_GPIO, 1);
}

/**
 * Write a buffer to a register.
 * @param reg Register index.
 * @param val Value to write.
 * @param len Byte length to write.
 */
void
lora_write_reg_buffer(int reg, uint8_t *val, int len)
{
   uint8_t *out;
   out = (uint8_t *)malloc(len+1);
   out[0] = 0x80 | reg;
   for (int i=0;i<len;i++) {
      out[i+1] = val[i];
   }

   spi_transaction_t t = {
      .flags = 0,
      .length = 8 * (len+1),
      .tx_buffer = out,
      .rx_buffer = NULL
   };

   //gpio_set_level(CONFIG_CS_GPIO, 0);
#if SPI_TRANSMIT
   spi_device_transmit(__spi, &t);
#else
   spi_device_polling_transmit(__spi, &t);
#endif
   //gpio_set_level(CONFIG_CS_GPIO, 1);
   free(out);
}

/**
 * Read the current value of a register.
 * @param reg Register index.
 * @return Value of the register.
 */
int
lora_read_reg(int reg)
{
   uint8_t out[2] = { reg, 0xff };
   uint8_t in[2];

   spi_transaction_t t = {
      .flags = 0,
      .length = 8 * sizeof(out),
      .tx_buffer = out,
      .rx_buffer = in
   };

   //gpio_set_level(CONFIG_CS_GPIO, 0);
#if SPI_TRANSMIT
   spi_device_transmit(__spi, &t);
#else
   spi_device_polling_transmit(__spi, &t);
#endif
   //gpio_set_level(CONFIG_CS_GPIO, 1);
   return in[1];
}

/**
 * Read the current value of a register.
 * @param reg Register index.
 * @return Value of the register.
 * @param len Byte length to read.
 */
void
lora_read_reg_buffer(int reg, uint8_t *val, int len)
{
   uint8_t *out;
   uint8_t *in;
   out = (uint8_t *)malloc(len+1);
   in = (uint8_t *)malloc(len+1);
   out[0] = reg;
   for (int i=0;i<len;i++) {
      out[i+1] = 0xff;
   }

   spi_transaction_t t = {
      .flags = 0,
      .length = 8 * (len+1),
      .tx_buffer = out,
      .rx_buffer = in
   };

   //gpio_set_level(CONFIG_CS_GPIO, 0);
#if SPI_TRANSMIT
   spi_device_transmit(__spi, &t);
#else
   spi_device_polling_transmit(__spi, &t);
#endif
   //gpio_set_level(CONFIG_CS_GPIO, 1);
   for (int i=0;i<len;i++) {
      val[i] = in[i+1];
   }
   free(out);
   free(in);
}

/**
 * Perform physical reset on the Lora chip
 */
void 
lora_reset(void)
{
   gpio_set_level(CONFIG_RST_GPIO, 0);
   vTaskDelay(pdMS_TO_TICKS(1));
   gpio_set_level(CONFIG_RST_GPIO, 1);
   vTaskDelay(pdMS_TO_TICKS(10));
}

/**
 * Configure explicit header mode.
 * Packet size will be included in the frame.
 */
void 
lora_explicit_header_mode(void)
{
   __implicit = 0;
   lora_write_reg(REG_MODEM_CONFIG_1, lora_read_reg(REG_MODEM_CONFIG_1) & 0xfe);
}

/**
 * Configure implicit header mode.
 * All packets will have a predefined size.
 * @param size Size of the packets.
 */
void 
lora_implicit_header_mode(int size)
{
   __implicit = 1;
   lora_write_reg(REG_MODEM_CONFIG_1, lora_read_reg(REG_MODEM_CONFIG_1) | 0x01);
   lora_write_reg(REG_PAYLOAD_LENGTH, size);
}

/**
 * Sets the radio transceiver in idle mode.
 * Must be used to change registers and access the FIFO.
 */
void 
lora_idle(void)
{
   lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}

/**
 * Sets the radio transceiver in sleep mode.
 * Low power consumption and FIFO is lost.
 */
void 
lora_sleep(void)
{ 
   lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

/**
 * Sets the radio transceiver in receive mode.
 * Incoming packets will be received.
 */
void 
lora_receive(void)
{
   lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
}

/**
 * Configure power level for transmission
 * @param level 2-17, from least to most power
 */
void 
lora_set_tx_power(int level)
{
   // RF9x module uses PA_BOOST pin
   if (level < 2) level = 2;
   else if (level > 17) level = 17;
   lora_write_reg(REG_PA_CONFIG, PA_BOOST | (level - 2));
}

/**
 * Set carrier frequency.
 * @param frequency Frequency in Hz
 */
void 
lora_set_frequency(long frequency)
{
   __frequency = frequency;

   uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

   lora_write_reg(REG_FRF_MSB, (uint8_t)(frf >> 16));
   lora_write_reg(REG_FRF_MID, (uint8_t)(frf >> 8));
   lora_write_reg(REG_FRF_LSB, (uint8_t)(frf >> 0));
}

/**
 * Set spreading factor.
 * @param sf 6-12, Spreading factor to use.
 */
void 
lora_set_spreading_factor(int sf)
{
   if (sf < 6) sf = 6;
   else if (sf > 12) sf = 12;

   if (sf == 6) {
      lora_write_reg(REG_DETECTION_OPTIMIZE, 0xc5);
      lora_write_reg(REG_DETECTION_THRESHOLD, 0x0c);
   } else {
      lora_write_reg(REG_DETECTION_OPTIMIZE, 0xc3);
      lora_write_reg(REG_DETECTION_THRESHOLD, 0x0a);
   }

   lora_write_reg(REG_MODEM_CONFIG_2, (lora_read_reg(REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));
}

/**
 * Get spreading factor.
 */
int 
lora_get_spreading_factor(void)
{
   return (lora_read_reg(REG_MODEM_CONFIG_2) >> 4);
}

/**
 * Set Mapping of pins DIO0 to DIO5
 * @param dio Number of DIO(0 to 5)
 * @param mode mode of DIO(0 to 3)
 */
void 
lora_set_dio_mapping(int dio, int mode)
{
   if (dio < 4) {
      int _mode = lora_read_reg(REG_DIO_MAPPING_1);
      if (dio == 0) {
         _mode = _mode & 0x3F;
         _mode = _mode | (mode << 6);
      } else if (dio == 1) {
         _mode = _mode & 0xCF;
         _mode = _mode | (mode << 4);
      } else if (dio == 2) {
         _mode = _mode & 0xF3;
         _mode = _mode | (mode << 2);
      } else if (dio == 3) {
         _mode = _mode & 0xFC;
         _mode = _mode | mode;
      }
      lora_write_reg(REG_DIO_MAPPING_1, _mode);
      ESP_LOGD(TAG, "REG_DIO_MAPPING_1=0x%02x", _mode);
   } else if (dio < 6) {
      int _mode = lora_read_reg(REG_DIO_MAPPING_2);
      if (dio == 4) {
         _mode = _mode & 0x3F;
         _mode = _mode | (mode << 6);
      } else if (dio == 5) {
         _mode = _mode & 0xCF;
         _mode = _mode | (mode << 4);
      }
      ESP_LOGD(TAG, "REG_DIO_MAPPING_2=0x%02x", _mode);
      lora_write_reg(REG_DIO_MAPPING_2, _mode);
   }
}

/**
 * Get Mapping of pins DIO0 to DIO5
 * @param dio Number of DIO(0 to 5)
 */
int 
lora_get_dio_mapping(int dio)
{
   if (dio < 4) {
      int _mode = lora_read_reg(REG_DIO_MAPPING_1);
      ESP_LOGD(TAG, "REG_DIO_MAPPING_1=0x%02x", _mode);
      if (dio == 0) {
         return ((_mode >> 6) & 0x03);
      } else if (dio == 1) {
         return ((_mode >> 4) & 0x03);
      } else if (dio == 2) {
         return ((_mode >> 2) & 0x03);
      } else if (dio == 3) {
         return (_mode & 0x03);
      }
   } else if (dio < 6) {
      int _mode = lora_read_reg(REG_DIO_MAPPING_2);
      ESP_LOGD(TAG, "REG_DIO_MAPPING_2=0x%02x", _mode);
      if (dio == 4) {
         return ((_mode >> 6) & 0x03);
      } else if (dio == 5) {
         return ((_mode >> 4) & 0x03);
      }
   }
   return 0;
}

/**
 * Set bandwidth (bit rate)
 * @param sbw Signal bandwidth(0 to 9)
 */
void 
lora_set_bandwidth(int sbw)
{
   if (sbw < 10) {
      lora_write_reg(REG_MODEM_CONFIG_1, (lora_read_reg(REG_MODEM_CONFIG_1) & 0x0f) | (sbw << 4));
   }
}

/**
 * Get bandwidth (bit rate)
 * @param sbw Signal bandwidth(0 to 9)
 */
int 
lora_get_bandwidth(void)
{
   //int bw;
   //bw = lora_read_reg(REG_MODEM_CONFIG_1) & 0xf0;
   //ESP_LOGD(TAG, "bw=0x%02x", bw);
   //bw = bw >> 4;
   //return bw;
   return ((lora_read_reg(REG_MODEM_CONFIG_1) & 0xf0) >> 4);
}

/**
 * Set coding rate 
 * @param denominator 5-8, Denominator for the coding rate 4/x
 */ 
void 
lora_set_coding_rate(int denominator)
{
   if (denominator < 5) denominator = 5;
   else if (denominator > 8) denominator = 8;

   int cr = denominator - 4;
   lora_write_reg(REG_MODEM_CONFIG_1, (lora_read_reg(REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));
}

/**
 * Get coding rate 
 */ 
int 
lora_get_coding_rate(void)
{
   return ((lora_read_reg(REG_MODEM_CONFIG_1) & 0x0E) >> 1);
}

/**
 * Set the size of preamble.
 * @param length Preamble length in symbols.
 */
void 
lora_set_preamble_length(long length)
{
   lora_write_reg(REG_PREAMBLE_MSB, (uint8_t)(length >> 8));
   lora_write_reg(REG_PREAMBLE_LSB, (uint8_t)(length >> 0));
}

/**
 * Get the size of preamble.
 */
long
lora_get_preamble_length(void)
{
   long preamble;
   preamble = lora_read_reg(REG_PREAMBLE_MSB) << 8;
   preamble = preamble + lora_read_reg(REG_PREAMBLE_LSB);
   return preamble;
}

/**
 * Change radio sync word.
 * @param sw New sync word to use.
 */
void 
lora_set_sync_word(int sw)
{
   lora_write_reg(REG_SYNC_WORD, sw);
}

/**
 * Enable appending/verifying packet CRC.
 */
void 
lora_enable_crc(void)
{
   lora_write_reg(REG_MODEM_CONFIG_2, lora_read_reg(REG_MODEM_CONFIG_2) | 0x04);
}

/**
 * Disable appending/verifying packet CRC.
 */
void 
lora_disable_crc(void)
{
   lora_write_reg(REG_MODEM_CONFIG_2, lora_read_reg(REG_MODEM_CONFIG_2) & 0xfb);
}

/**
 * Perform hardware initialization.
 */
int 
lora_init(void)
{
   esp_err_t ret;

   /*
    * Configure CPU hardware to communicate with the radio chip
    */
   gpio_reset_pin(CONFIG_RST_GPIO);
   gpio_set_direction(CONFIG_RST_GPIO, GPIO_MODE_OUTPUT);
   gpio_reset_pin(CONFIG_CS_GPIO);
   gpio_set_direction(CONFIG_CS_GPIO, GPIO_MODE_OUTPUT);
   gpio_set_level(CONFIG_CS_GPIO, 1);

   spi_bus_config_t bus = {
      .miso_io_num = CONFIG_MISO_GPIO,
      .mosi_io_num = CONFIG_MOSI_GPIO,
      .sclk_io_num = CONFIG_SCK_GPIO,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 0
   };
           
   //ret = spi_bus_initialize(VSPI_HOST, &bus, 0);
   ret = spi_bus_initialize(HOST_ID, &bus, SPI_DMA_CH_AUTO);
   assert(ret == ESP_OK);

   spi_device_interface_config_t dev = {
      .clock_speed_hz = 9000000,
      .mode = 0,
      .spics_io_num = CONFIG_CS_GPIO,
      .queue_size = 7,
      .flags = 0,
      .pre_cb = NULL
   };
   //ret = spi_bus_add_device(VSPI_HOST, &dev, &__spi);
   ret = spi_bus_add_device(HOST_ID, &dev, &__spi);
   assert(ret == ESP_OK);

   /*
    * Perform hardware reset.
    */
   lora_reset();

   /*
    * Check version.
    */
   uint8_t version;
   uint8_t i = 0;
   while(i++ < TIMEOUT_RESET) {
      version = lora_read_reg(REG_VERSION);
      ESP_LOGD(TAG, "version=0x%02x", version);
      if(version == 0x12) break;
      vTaskDelay(2);
   }
   ESP_LOGD(TAG, "i=%d, TIMEOUT_RESET=%d", i, TIMEOUT_RESET);
   if (i == TIMEOUT_RESET + 1) return 0; // Illegal version
   //assert(i < TIMEOUT_RESET + 1); // at the end of the loop above, the max value i can reach is TIMEOUT_RESET + 1

   /*
    * Default configuration.
    */
   lora_sleep();
   lora_write_reg(REG_FIFO_RX_BASE_ADDR, 0);
   lora_write_reg(REG_FIFO_TX_BASE_ADDR, 0);
   lora_write_reg(REG_LNA, lora_read_reg(REG_LNA) | 0x03);
   lora_write_reg(REG_MODEM_CONFIG_3, 0x04);
   lora_set_tx_power(17);

   lora_idle();
   return 1;
}

/**
 * Send a packet.
 * @param buf Data to be sent
 * @param size Size of data.
 */
void 
lora_send_packet(uint8_t *buf, int size)
{
   /*
    * Transfer data to radio.
    */
   lora_idle();
   lora_write_reg(REG_FIFO_ADDR_PTR, 0);

#if BUFFER_IO
   lora_write_reg_buffer(REG_FIFO, buf, size);
#else
   for(int i=0; i<size; i++) 
      lora_write_reg(REG_FIFO, *buf++);
#endif
   
   lora_write_reg(REG_PAYLOAD_LENGTH, size);
   
   /*
    * Start transmission and wait for conclusion.
    */
   lora_write_reg(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);
#if 0
   while((lora_read_reg(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0)
      vTaskDelay(2);
#endif
   int loop = 0;
   while(1) {
      int irq = lora_read_reg(REG_IRQ_FLAGS);
      ESP_LOGD(TAG, "lora_read_reg=0x%x", irq);
      if ((irq & IRQ_TX_DONE_MASK) == IRQ_TX_DONE_MASK) break;
      loop++;
      if (loop == 10) break;
      vTaskDelay(2);
   }
   if (loop == 10) {
      __send_packet_lost++;
      ESP_LOGE(TAG, "lora_send_packet Fail");
   }
   lora_write_reg(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
}

/**
 * Read a received packet.
 * @param buf Buffer for the data.
 * @param size Available size in buffer (bytes).
 * @return Number of bytes received (zero if no packet available).
 */
int 
lora_receive_packet(uint8_t *buf, int size)
{
   int len = 0;

   /*
    * Check interrupts.
    */
   int irq = lora_read_reg(REG_IRQ_FLAGS);
   lora_write_reg(REG_IRQ_FLAGS, irq);
   if((irq & IRQ_RX_DONE_MASK) == 0) return 0;
   if(irq & IRQ_PAYLOAD_CRC_ERROR_MASK) return 0;

   /*
    * Find packet size.
    */
   if (__implicit) len = lora_read_reg(REG_PAYLOAD_LENGTH);
   else len = lora_read_reg(REG_RX_NB_BYTES);

   /*
    * Transfer data from radio.
    */
   lora_idle();   
   lora_write_reg(REG_FIFO_ADDR_PTR, lora_read_reg(REG_FIFO_RX_CURRENT_ADDR));
   if(len > size) len = size;
#if BUFFER_IO
   lora_read_reg_buffer(REG_FIFO, buf, len);
#else
   for(int i=0; i<len; i++) 
      *buf++ = lora_read_reg(REG_FIFO);
#endif

   return len;
}

/**
 * Returns non-zero if there is data to read (packet received).
 */
int
lora_received(void)
{
   if(lora_read_reg(REG_IRQ_FLAGS) & IRQ_RX_DONE_MASK) return 1;
   return 0;
}

/**
 * Returns RegIrqFlags.
 */
int
lora_get_irq(void)
{
   return (lora_read_reg(REG_IRQ_FLAGS));
}


/**
 * Return lost send packet count.
 */
int 
lora_packet_lost(void)
{
   return (__send_packet_lost);
}

/**
 * Return last packet's RSSI.
 */
int 
lora_packet_rssi(void)
{
   return (lora_read_reg(REG_PKT_RSSI_VALUE) - (__frequency < 868E6 ? 164 : 157));
}


/**
 * Return last packet's SNR (signal to noise ratio).
 */
float 
lora_packet_snr(void)
{
   return ((int8_t)lora_read_reg(REG_PKT_SNR_VALUE)) * 0.25;
}

/**
 * Shutdown hardware.
 */
void 
lora_close(void)
{
   lora_sleep();
//   close(__spi);  FIXME: end hardware features after lora_close
//   close(__cs);
//   close(__rst);
//   __spi = -1;
//   __cs = -1;
//   __rst = -1;
}

void 
lora_dump_registers(void)
{
   int i;
   printf("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
   for(i=0; i<0x40; i++) {
      printf("%02X ", lora_read_reg(i));
      if((i & 0x0f) == 0x0f) printf("\n");
   }
   printf("\n");
}

