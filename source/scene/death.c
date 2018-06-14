/*
 * death.c
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

#include "death.h"
#include "menu.h"
#include "../res.h"

typedef struct half_t {
	interp_t x;
	interp_t y;
	interp_t angle;
	double vx;
	double vy;
	double vangle;
} half_t;

static player_t* g_player;
static mintg_image_t* g_buffer;

void death_init(player_t* player, mintg_image_t* buffer) {
	g_player = player;
	g_buffer = buffer;
}

static void half_init(half_t* half, player_t* player, double vx) {
	interp_init(&half->x, player->x.old);
	interp_init(&half->y, player->y.old);
	interp_init(&half->angle, 0);
	half->vx = 300 * vx;
	half->vy = 300;
	half->vangle = -3 * vx;
}

static void half_update(half_t* half, double time) {
	interp_update(&half->x);
	interp_update(&half->y);
	interp_update(&half->angle);
	half->vx = pow(0.01, time) * half->vx;
	half->vy -= 1500 * time;
	half->vangle = pow(0.01, time) * half->vangle;
	half->x.v += half->vx * time;
	half->y.v += half->vy * time;
	half->angle.v += half->vangle * time;
}

static void half_draw(half_t* half, double time, mintg_image_t* image) {
	mintg_push();
	mintg_translate(interp_value(&half->x, time), interp_value(&half->y, time));
	mintg_rotate(interp_value(&half->angle, time));
	mintg_image_draw(image, NULL);
	mintg_pop();
}

void death_scene(scene_state_t state, double time) {
	static interp_t fade;
	static half_t left;
	static half_t right;

	if (state == SCENE_INIT) {

		interp_init(&fade, 2);
		half_init(&left, g_player, -1);
		half_init(&right, g_player, 1);

	} else if (state == SCENE_UPDATE) {

		interp_update(&fade);
		fade.v -= time;
		if (fade.v <= 1) {
			if (fade.old >= 1) {
				minta_sound_play(res_sound_break);
			}
			half_update(&left, time);
			half_update(&right, time);
		}
		if (fade.v <= 0) {
			g_player->alive = 0;
			scene_set(menu_scene);
		}
		double volume = fade.v - 1;
		if (volume < 0) {
			volume = 0;
		}
		minta_music_volume(res_music_game, volume * volume);
		minta_music_update(res_music_game);
	} else if (state == SCENE_DRAW) {

		double alpha = interp_value(&fade, time);

		mintg_color(1, 1, 1, 1);
		mintg_clear();

		mintg_color(g_player->r, g_player->g, g_player->b, alpha * alpha);
		half_draw(&left, time, res_image_player_left);
		half_draw(&right, time, res_image_player_right);

		if (alpha >= 1) {
			mintg_color(1, 1, 1, (alpha - 1) * (alpha - 1));
			mintg_image_draw(g_buffer, NULL);
		}

	} else if (state == SCENE_EXIT) {

		mintg_image_destroy(g_buffer);
		minta_music_stop(res_music_game);

	}
}