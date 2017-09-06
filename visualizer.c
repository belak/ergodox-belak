/*
Copyright 2017 Fred Sundvik

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Currently we are assuming that both the backlight and LCD are enabled
// But it's entirely possible to write a custom visualizer that use only
// one of them
#ifndef LCD_BACKLIGHT_ENABLE
#error This visualizer needs that LCD backlight is enabled
#endif

#ifndef LCD_ENABLE
#error This visualizer needs that LCD is enabled
#endif

#include "visualizer.h"
#include "lcd_keyframes.h"
#include "lcd_backlight_keyframes.h"
#include "led.h"
#include "animations.h"

static const uint32_t initial_color = LCD_COLOR(0, 0, 0);
static const uint32_t initial_target_color = LCD_COLOR(0, 0, 0xFF);

static bool initial_update = true;

// Experiment to cycle through a rainbow on startup.
bool backlight_keyframe_color_cycle(keyframe_animation_t* animation, visualizer_state_t* state) {
    int frame_length = animation->frame_lengths[animation->current_frame];
    int current_pos = frame_length - animation->time_left_in_frame;

    //uint8_t p_h = LCD_HUE(state->prev_lcd_color);
    uint8_t p_s = 255;
    uint8_t p_i = 100;

    lcd_backlight_color(255 * current_pos / frame_length, p_s, p_i);

    return true;
}

static keyframe_animation_t startup_animation = {
    .num_frames = 3,
    .loop = false,
    .frame_lengths = {gfxMillisecondsToTicks(5000), 0, 0},
    .frame_functions = {
        // keyframe_enable and fade in need to happen so text can be displayed
        keyframe_enable,
        backlight_keyframe_color_cycle,
        keyframe_fade_in,
    },
};

static keyframe_animation_t lcd_layer_display = {
    .num_frames = 1,
    .loop = false,
    .frame_lengths = {0},
    .frame_functions = {lcd_keyframe_display_layer_and_led_states}
};

static keyframe_animation_t color_set = {
    .num_frames = 1,
    .loop = false,
    .frame_lengths = {0},
    .frame_functions = {backlight_keyframe_set_color},
};

void initialize_user_visualizer(visualizer_state_t* state) {
    // The brightness will be dynamically adjustable in the future
    // But for now, change it here.
    lcd_backlight_brightness(130);
    state->current_lcd_color = initial_color;
    state->target_lcd_color = initial_target_color;
    initial_update = true;
    start_keyframe_animation(&startup_animation);
}

void update_user_visualizer_state(visualizer_state_t* state, visualizer_keyboard_status_t* prev_status) {
    uint32_t prev_color = state->target_lcd_color;
    const char* prev_layer_text = state->layer_text;

    // My code
    uint8_t saturation = 60;
    if (state->status.leds & (1u << USB_LED_CAPS_LOCK)) {
        saturation = 255;
    }

    if (state->status.layer & 0x4) {
        state->target_lcd_color = LCD_COLOR(0, saturation, 0xFF);
        state->layer_text = "3. Numpad";
    } else if (state->status.layer & 0x2) {
        state->target_lcd_color = LCD_COLOR(168, saturation, 0xFF);
        state->layer_text = "2. Symbols";
    } else {
        state->target_lcd_color = LCD_COLOR(84, saturation, 0xFF);
        state->layer_text = "1. Base";
    }

    // Run animations if they need to be run
    if (initial_update || prev_color != state->target_lcd_color) {
        start_keyframe_animation(&color_set);
    }

    if (initial_update || prev_layer_text != state->layer_text) {
        start_keyframe_animation(&lcd_layer_display);
    }
}

void user_visualizer_suspend(visualizer_state_t* state) {
    state->layer_text = "Suspending...";
    uint8_t hue = LCD_HUE(state->current_lcd_color);
    uint8_t sat = LCD_SAT(state->current_lcd_color);
    state->target_lcd_color = LCD_COLOR(hue, sat, 0);
    start_keyframe_animation(&default_suspend_animation);
}

void user_visualizer_resume(visualizer_state_t* state) {
    state->current_lcd_color = initial_color;
    state->target_lcd_color = initial_target_color;
    initial_update = true;
    start_keyframe_animation(&startup_animation);
}
