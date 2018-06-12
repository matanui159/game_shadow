/*
 * res.h
 *
 * Copyright 2018 Joshua Michael Minter
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SHADOW_RES_H_
#define SHADOW_RES_H_
#include "main.h"

extern mintg_image_t* res_image_rect;
extern mintg_image_t* res_image_circle;
extern mintg_image_t* res_image_eye;

extern mintg_image_t* res_image_tut_hold;
extern mintg_image_t* res_image_tut_move;
extern mintg_image_t* res_image_tut_done;

extern mintg_image_t* res_image_player_cursor;
extern mintg_image_t* res_image_player_left;
extern mintg_image_t* res_image_player_right;

extern mintg_font_t* res_font_clean;
extern mintg_font_t* res_font_clean_large;
extern mintg_font_t* res_font_clean_small;

extern mintg_font_t* res_font_messy;

extern minta_sound_t* res_sound_down;
extern minta_sound_t* res_sound_up;
extern minta_sound_t* res_sound_beat;
extern minta_sound_t* res_sound_break;

extern minta_music_t* res_music_game;
extern minta_music_t* res_music_noise;

void res_init();

#endif