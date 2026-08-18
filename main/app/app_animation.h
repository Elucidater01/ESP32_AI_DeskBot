#pragma once

#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// typedef enum {
//     AUDIO_WAKENET_END = 0,
//     AUDIO_WAKENET_START,
//     AUDIO_VAD_END,
//     AUDIO_VAD_START,
//     AUDIO_VAD_WAIT,
// } audio_record_state_t;

esp_err_t app_animation_init();
void app_lvgl_display();
void app_animation_play(int index);
// void maodie_play();

#ifdef __cplusplus
}
#endif