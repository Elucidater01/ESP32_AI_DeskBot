#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sparkbot_bsp.h"
#include "app_imu.h"
#include "app_datafusion.h"
#include "app_touch.h"
#include "nvs_flash.h"

#include "esp_log.h"
#include "app_power.h"
#include "app_wifi.h"
#include "app_audio_record.h"
#include "app_animation.h"
// #include "app_AI_chat.h"
#include "app_eeprom.h"

int power_voltage = 0;
uint16_t cnt = 0;

void memory_monitor()
{
    static char buffer[128];    /* Make sure buffer is enough for `sprintf` */
    if (1) {
        sprintf(buffer, "   Biggest /     Free /    Total\n"
                "\t  SRAM : [%8d / %8d / %8d]\n"
                "\t PSRAM : [%8d / %8d / %8d]",
                heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
                heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                heap_caps_get_total_size(MALLOC_CAP_INTERNAL),
                heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM),
                heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
                heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
        ESP_LOGE("MEM", "%s", buffer);
    }
}
bool is_first = true;
void app_main(void)
{

    bsp_i2c_init();
    app_imu_init();
    app_animation_init();
    app_touch_init();
    app_wifi_init();
    power_adc_init();
    // app_ai_chat_init();
    // uint8_t buffer = 4;
    // at24c08_write_bytes(0x00, &buffer, 1); //默认声音为4号
    // buffer = 2;
    // at24c08_write_bytes(0x01, &buffer, 1);
    uint8_t buf = 0;

    while (1)
    {
        // app_imu_read();
        if(app_wifi_is_connected && is_first)
        {
            ESP_ERROR_CHECK(app_sr_start());
            is_first = false;
        }
 
        if(cnt % 5000 == 0)
        {
            
            printf("app_wifi:%d\n", app_wifi_is_connected);
            power_voltage = get_power_value();
            ESP_LOGI("POWER", "Power Voltage: %d %%", power_voltage);
            // at24c08_read_bytes(0x00, &buf, 1);
            // ESP_LOGI("EEPROM", "read person_tone: %d", buf);
            // memory_monitor();
        }
        // if(cnt % 20 == 0)
        // {
        //     lv_task_handler();
        //     lv_timer_handler();
        // }
        cnt++;
        vTaskDelay(1);
        
    }
    
}
