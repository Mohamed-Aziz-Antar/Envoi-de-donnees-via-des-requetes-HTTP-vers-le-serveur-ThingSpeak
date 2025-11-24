idf_component_register(SRCS "hello_world_main.c"
                       PRIV_REQUIRES spi_flash
                       INCLUDE_DIRS ""
                           REQUIRES freertos driver esp_wifi esp_netif esp_http_client nvs_flash dht
)
