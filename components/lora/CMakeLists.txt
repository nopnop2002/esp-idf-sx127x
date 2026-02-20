set(component_srcs "lora.c")

idf_component_register(SRCS "${component_srcs}"
                       PRIV_REQUIRES driver esp_driver_spi esp_driver_gpio
                       INCLUDE_DIRS "include")
