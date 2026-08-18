#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lv_decoder.h"
#include "esp_lv_fs.h"
#include "bsp/esp-bsp.h"
#include "lvgl.h"
#include "app_animation.h"
#include "app_touch.h"
#include "app_audio_record.h"
// #include "lv_gif.h"      // LVGL GIF 控制头文件
// #include "gifdec.h"      // GIF 解码器头文件

#include "mmap_generate_lottie_assets.h"
mmap_assets_handle_t asset_lottie;
esp_lv_fs_handle_t fs_drive_handle;
esp_lv_decoder_handle_t decoder_handle = NULL;
static const char *TAG = "app_animation";

esp_err_t lv_fs_add(void)
{
    fs_cfg_t fs_cfg;
    fs_cfg.fs_letter = 'A';
    fs_cfg.fs_assets = asset_lottie;
    fs_cfg.fs_nums = MMAP_LOTTIE_ASSETS_FILES;

    esp_err_t ret = esp_lv_fs_desc_init(&fs_cfg, &fs_drive_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize FS");
        return ret;
    }
    return ESP_OK;
}


LV_IMG_DECLARE(nature);
LV_IMG_DECLARE(happy);
LV_IMG_DECLARE(sad);
LV_IMG_DECLARE(anger);
LV_IMG_DECLARE(scare);
LV_IMG_DECLARE(disdain);
lv_obj_t *image_obj = NULL;
int g_current_image_index = 0; // 当前显示的图像索引


void app_lvgl_display()
{
    // 必须写在函数里面！
    lv_disp_t *disp = lv_disp_get_default();
    
    // 设置屏幕 180° 翻转
    lv_disp_set_rotation(disp, LV_DISP_ROT_180);

    // 2. 创建一个 对象
    image_obj = lv_gif_create(lv_scr_act()); 
    // image_obj = lv_img_create(lv_scr_act());
    
    // 3. 设置 GIF 图像的源为之前声明的数组
    lv_gif_set_src(image_obj, &happy);
    // lv_img_set_src(image_obj, &shu1);
    
    // 4. 将 GIF 对象对齐到屏幕中心（可选）
    lv_obj_align(image_obj, LV_ALIGN_CENTER, 0, 0);

}

void app_animation_play(int index)
{

    printf("app_animation_play index:%d\n", index);
    if (index == 0)
    {
        lv_gif_set_src(image_obj, &nature);
    }
    else if (index == 1)
    {
        lv_gif_set_src(image_obj, &sad);

    }
    else if (index == 2)
    {
        lv_gif_set_src(image_obj, &happy);
    }
    else if (index == 3)
    {
        lv_gif_set_src(image_obj, &anger);
    }
    else if (index == 4)
    {
        lv_gif_set_src(image_obj, &scare);
    }
    else if (index == 5)
    {
        lv_gif_set_src(image_obj, &disdain);
    }
}

void lvgl_task(void *pvParameters)
{
    uint16_t cnt = 0;
    app_lvgl_display();
    int last_expression = 0;
    
    while (1)
    {

        if (Face_Expression != last_expression)
        {
            app_animation_play(Face_Expression);
            last_expression = Face_Expression;
        }
        
        cnt++;
        if(cnt % 10 == 0)
        {
            lv_timer_handler();
        }
        // if(cnt % 2000 == 0)
        // {
        //     Face_Expression++;
        //     if(Face_Expression >= 6)
        //     {
        //         Face_Expression = 0;
        //     }
        // }
        vTaskDelay(1);
    }
}

esp_err_t app_animation_init()
{
        /* Initialize display and LVGL */
    bsp_display_cfg_t custom_cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = BSP_LCD_H_RES * BSP_LCD_V_RES,
        .trans_size = BSP_LCD_H_RES * 10, // in SRAM, DMA-capable
        .double_buffer = 0,
        .flags = {
            .buff_dma = false,
            .buff_spiram = true,
        }
    };
    custom_cfg.lvgl_port_cfg.task_stack = 1024 * 30;
    custom_cfg.lvgl_port_cfg.task_affinity = 1;
    bsp_display_start_with_config(&custom_cfg);

    /* Turn on display backlight */
    bsp_display_backlight_on();

    // app_mount_mmap_fs();

    // ESP_ERROR_CHECK(lv_fs_add());

    // ESP_ERROR_CHECK(esp_lv_decoder_init(&decoder_handle));
    xTaskCreatePinnedToCore(lvgl_task, "lvgl_task", 1024 * 10, NULL, 10, NULL, 1);
    /* Add and show objects on display */

    return ESP_OK;
}
