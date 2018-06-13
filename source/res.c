/*
 * res.c
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
 
#include "res.h"

mintg_image_t* res_image_rect;
mintg_image_t* res_image_circle;

mintg_image_t* res_image_tut_hold;
mintg_image_t* res_image_tut_move;
mintg_image_t* res_image_tut_done;

mintg_image_t* res_image_player_cursor;
mintg_image_t* res_image_player_left;
mintg_image_t* res_image_player_right;

mintg_image_t* res_image_eye[5];

mintg_font_t* res_font_clean;
mintg_font_t* res_font_clean_large;
mintg_font_t* res_font_clean_small;

mintg_font_t* res_font_messy;

minta_sound_t* res_sound_down;
minta_sound_t* res_sound_up;
minta_sound_t* res_sound_beat;
minta_sound_t* res_sound_break;

minta_music_t* res_music_game;
minta_music_t* res_music_noise;
minta_music_t* res_music_end;

static int noise_stream(int size, void* data, void* user) {
	uint8_t* raw = data;
	for (int i = 0; i < size; ++i) {
		raw[i] = (uint8_t)mint_random(0, UINT8_MAX);
	}
	return size;
}

void res_init() {
	const uint8_t box_data[] = {0xFF, 0xFF, 0xFF, 0xFF};
	res_image_rect = mintg_image_create(1, 1, box_data);
	res_image_circle = mintg_image_load("res/image/circle.png", MINT_FILE_LOCAL);

	res_image_tut_hold = mintg_image_load("res/image/tut/hold.png", MINT_FILE_LOCAL);
	res_image_tut_move = mintg_image_load("res/image/tut/move.png", MINT_FILE_LOCAL);
	res_image_tut_done = mintg_image_load("res/image/tut/done.png", MINT_FILE_LOCAL);

	res_image_player_cursor = mintg_image_load("res/image/player/cursor.png", MINT_FILE_LOCAL);
	res_image_player_left = mintg_image_load("res/image/player/left.png", MINT_FILE_LOCAL);
	res_image_player_right = mintg_image_load("res/image/player/right.png", MINT_FILE_LOCAL);

	mint_string_t* eye_path = mint_string_create();
	mint_string_add(eye_path, -1, "res/image/eye/");
	int eye_end = mint_string_size(eye_path);
	for (int i = 0; i < 5; ++i) {
		mint_string_replace(eye_path, eye_end, -1, "%i.png", i);
		res_image_eye[i] = mintg_image_load(mint_string_data(eye_path), MINT_FILE_LOCAL);
	}
	mint_string_destroy(eye_path);

	res_font_clean = mintg_font_load("res/font/clean.ttf", MINT_FILE_LOCAL, 30);
	res_font_clean_large = mintg_font_load("res/font/clean.ttf", MINT_FILE_LOCAL, 40);
	res_font_clean_small = mintg_font_load("res/font/clean.ttf", MINT_FILE_LOCAL, 20);

	res_font_messy = mintg_font_load("res/font/messy.ttf", MINT_FILE_LOCAL, 40);

	res_sound_down = minta_sound_load("res/sound/down.ogg", MINT_FILE_LOCAL);
	res_sound_up = minta_sound_load("res/sound/up.ogg", MINT_FILE_LOCAL);
	res_sound_beat = minta_sound_load("res/sound/beat.ogg", MINT_FILE_LOCAL);
	res_sound_break = minta_sound_load("res/sound/break.ogg", MINT_FILE_LOCAL);

	res_music_game = minta_music_load("res/music/game.ogg", MINT_FILE_LOCAL);
	res_music_noise = minta_music_create(MINTA_STEREO8, 44100, noise_stream, NULL);
	res_music_end = minta_music_load("res/music/end.ogg", MINT_FILE_LOCAL);

	mintg_image_t* icon = mintg_image_load("res/image/icon.png", MINT_FILE_LOCAL);
	mintg_image_icon(icon);
	mintg_image_destroy(icon);
}