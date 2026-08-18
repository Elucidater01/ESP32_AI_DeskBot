/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"

#include "unity.h"
#include "unity_test_runner.h"
#include "unity_test_utils_memory.h"

#include "mmap_generate_lottie_assets.h"
#include "lv_lottie.h"
#include "lvgl.h"

static const char *TAG = "lottie player";

#define TEST_LCD_H_RES      100
#define TEST_LCD_V_RES      100

static SemaphoreHandle_t flush_sem;
static mmap_assets_handle_t asset_lottie;

static void test_flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    ESP_LOGI(TAG, "flush_cb, 0x%04X, [%d,%d,%d,%d]", color_map[0].full, area->x1, area->y1, area->x2, area->y2);
    xSemaphoreGive(flush_sem);

    lv_disp_flush_ready(drv);
}

static void test_lvgl_init(lv_disp_drv_t **disp_drv, lv_disp_draw_buf_t **disp_buf)
{
    /* LVGL init */
    lv_init();

    *disp_buf = heap_caps_malloc(sizeof(lv_disp_draw_buf_t), MALLOC_CAP_DEFAULT);
    TEST_ASSERT_NOT_NULL(*disp_buf);

    /* Initialize LVGL draw buffers */
    uint32_t buffer_size = TEST_LCD_H_RES * TEST_LCD_V_RES;
    lv_color_t *buf1 = heap_caps_malloc(buffer_size * sizeof(lv_color_t), MALLOC_CAP_DEFAULT);
    TEST_ASSERT_NOT_NULL(buf1);
    lv_disp_draw_buf_init(*disp_buf, buf1, NULL, buffer_size);

    *disp_drv = heap_caps_malloc(sizeof(lv_disp_drv_t), MALLOC_CAP_DEFAULT);
    TEST_ASSERT_NOT_NULL(*disp_drv);
    /* Descriptor of a display driver */
    lv_disp_drv_init(*disp_drv);
    (*disp_drv)->hor_res = TEST_LCD_H_RES;
    (*disp_drv)->ver_res = TEST_LCD_V_RES;
    (*disp_drv)->flush_cb = test_flush_callback;
    (*disp_drv)->draw_buf = *disp_buf;

    /* Finally register the driver */
    lv_disp_drv_register(*disp_drv);

    // Initialize the semaphores
    flush_sem = xSemaphoreCreateBinary();
    TEST_ASSERT_NOT_NULL(flush_sem);
}

static void test_lvgl_deinit(lv_disp_drv_t *disp_drv, lv_disp_draw_buf_t *disp_buf)
{
    free(disp_drv->draw_buf->buf1);
    free(disp_drv->draw_buf);
    free(disp_drv);
    lv_deinit();

    // Delete the semaphores
    vSemaphoreDelete(flush_sem);
}

TEST_CASE("Validate lottie player functionality in LVGL", "[lottie][lv_lottie_set_src_data]")
{
    lv_disp_drv_t *disp_drv = NULL;
    lv_disp_draw_buf_t *disp_buf = NULL;

    const mmap_assets_config_t config_lottie = {
        .partition_label = "animation",
        .max_files = MMAP_LOTTIE_ASSETS_FILES,
        .checksum = MMAP_LOTTIE_ASSETS_CHECKSUM,
        .flags = {
            .mmap_enable = true,
            .app_bin_check = true,
        },
    };

    mmap_assets_new(&config_lottie, &asset_lottie);

    /* Initialize LVGL */
    test_lvgl_init(&disp_drv, &disp_buf);

    lv_obj_t *ui_face_canvas = lv_lottie_create(lv_scr_act());
    lv_obj_set_align(ui_face_canvas, LV_ALIGN_TOP_LEFT);

    uint8_t *canvas_fb = NULL;
    if (canvas_fb == NULL) {
        canvas_fb = malloc(TEST_LCD_H_RES * TEST_LCD_V_RES * 4);
        TEST_ASSERT_TRUE(canvas_fb);
        lv_lottie_set_buffer(ui_face_canvas, TEST_LCD_H_RES, TEST_LCD_V_RES, canvas_fb);
    }

    const void *data = mmap_assets_get_mem(asset_lottie, MMAP_LOTTIE_ASSETS_THINK_JSON);
    size_t size = mmap_assets_get_size(asset_lottie, MMAP_LOTTIE_ASSETS_THINK_JSON);
    lv_lottie_set_src_data(ui_face_canvas, data, size);
    lv_refr_now(NULL);

    TEST_ASSERT_TRUE(xSemaphoreTake(flush_sem, pdMS_TO_TICKS(3 * 1000)));

    lv_obj_clean(lv_scr_act());
    lv_obj_del(lv_scr_act());

    test_lvgl_deinit(disp_drv, disp_buf);
    mmap_assets_del(asset_lottie);

    if (canvas_fb) {
        free(canvas_fb);
    }
}

// Some resources are lazy allocated in the LCD driver, the threadhold is left for that case
#define TEST_MEMORY_LEAK_THRESHOLD  (700)

static size_t before_free_8bit;
static size_t before_free_32bit;

void setUp(void)
{
    before_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    before_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
}

void tearDown(void)
{
    size_t after_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    size_t after_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
    unity_utils_check_leak(before_free_8bit, after_free_8bit, "8BIT", TEST_MEMORY_LEAK_THRESHOLD);
    unity_utils_check_leak(before_free_32bit, after_free_32bit, "32BIT", TEST_MEMORY_LEAK_THRESHOLD);
}

void app_main(void)
{
    printf("ESP FileSystem TEST \n");
    unity_run_menu();
}
