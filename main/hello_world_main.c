#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "driver/gpio.h"
#include "esp_http_client.h"
#include "dht.h"

#define WIFI_SSID "Ahmed"
#define WIFI_PASS "ahmed123"

#define DHT_GPIO 4

#define THINGSPEAK_API_KEY "6SCTLCWCMYXSAHGN"
#define THINGSPEAK_URL "http://api.thingspeak.com/update?api_key=" THINGSPEAK_API_KEY

static const char *TAG = "DHT11_ThingSpeak";

static bool wifi_connected = false;

// Wi-Fi event handler
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Disconnected, retrying...");
        wifi_connected = false;
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        wifi_connected = true;
    }
}

// Wi-Fi initialization
void wifi_init(void)
{
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "Wi-Fi initialization finished. Connecting...");
}

// Send data to ThingSpeak
void send_to_thingspeak(float temperature, float humidity)
{
    char url[256];
    sprintf(url, "%s&field1=%.1f&field2=%.1f", THINGSPEAK_URL, temperature, humidity);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Data sent successfully: Temp %.1f°C, Hum %.1f%%", temperature, humidity);
    } else {
        ESP_LOGE(TAG, "Failed to send data: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

void app_main(void)
{
    // Start Wi-Fi
    wifi_init();

    // Wait until Wi-Fi is connected
    while (!wifi_connected) {
        ESP_LOGI(TAG, "Waiting for Wi-Fi...");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    int16_t temperature = 0;
    int16_t humidity = 0;

    while (1) {
        // Read DHT11
        if (dht_read_data(DHT_TYPE_DHT11, DHT_GPIO, &humidity, &temperature) == ESP_OK) {
            float temp_c = temperature / 10.0;
            float hum_percent = humidity / 10.0;

            ESP_LOGI(TAG, "Temperature: %.1f°C, Humidity: %.1f%%", temp_c, hum_percent);

            // Send to ThingSpeak
            send_to_thingspeak(temp_c, hum_percent);
        } else {
            ESP_LOGE(TAG, "Failed to read DHT11");
        }

        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

