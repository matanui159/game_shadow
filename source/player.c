/*
 * player.c
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
 
#include "player.h"
#include "res.h"

player_t player_qld;
player_t player_nsw;
static _Bool g_init = 0;
static mintg_input_state_t g_state;

static void player_init1(player_t* player, double x) {
	player->time = 0;
	interp_init(&player->x, x);
	interp_init(&player->y, -100);
	interp_init(&player->scale, player->health);
	player->health = 1;
}

void player_init() {
	if (!g_init) {
		player_init1(&player_qld, -100);
		player_init1(&player_nsw, 100);
		g_init = 1;
	}
}

static _Bool player_update1(player_t* player, double time, _Bool active) {
	interp_update(&player->x);
	interp_update(&player->y);
	interp_update(&player->scale);
	if (active) {
		mintg_input_cursor(&player->x.v, &player->y.v);
	}

	double half_width = mintg_width() / 2.0;
	double half_height = mintg_height() / 2.0;
	if (player->x.v < -half_width) {
		player->x.v = -half_width;
	}
	if (player->x.v > half_width) {
		player->x.v = half_width;
	}
	if (player->y.v < -half_height) {
		player->y.v = -half_height;
	}
	if (player->y.v > half_height) {
		player->y.v = half_height;
	}

	_Bool result = 0;
	player->time += time;
	if (player->health > 0 && player->health < 1) {
		player->health -= time * 2;
		if (player->health < 0) {
			result = 1;
		}
	} else if (player->health < 0) {
		player->health = 0;
	}

	double s0 = cos(player->time * 20 + M_PI);
	double s1 = cos(player->time * 10);
	player->scale.v = (s0 + 1) * (s1 + 1) / 15 + 0.8;
//	player->scale.v *= player->health * (2 - player->health);
	return result;
}

_Bool player_update(double time) {
	mintg_input_key(MINTG_INPUT_RBUTTON, &g_state);
	_Bool active = 0;
	switch (g_state) {
		case MINTG_INPUT_KEYUP_EVENT:
			mintg_input_set(player_qld.x.v, player_qld.y.v);
		case MINTG_INPUT_KEYUP:
			active = 1;
			break;
		case MINTG_INPUT_KEYDOWN_EVENT:
			mintg_input_set(player_nsw.x.v, player_nsw.y.v);
		case MINTG_INPUT_KEYDOWN:
			active = 0;
			break;
	}

	_Bool result = 0;
	if (player_update1(&player_qld, time, active)) {
		result = 1;
	}
	if (player_update1(&player_nsw, time, !active)) {
		result = 1;
	}
	return result;
}

void player_draw1(player_t* player, _Bool game, double time, double red, double blue) {
	mintg_push();
	mintg_translate(interp_value(&player->x, time), interp_value(&player->y, time));
	mintg_color(red, 0, blue, player->health);
	if (game) {
		double scale = interp_value(&player->scale, time);
		mintg_scale(scale, scale);
		mintg_image_draw(res_image_heart, NULL);
	} else if (player->health > 0) {
		mintg_image_draw(res_image_arrow, NULL);
	}
	mintg_pop();
}

void player_draw(_Bool game, double time) {
	player_draw1(&player_qld, game, time, 0.6, 0.2);
	player_draw1(&player_nsw, game, time, 0.2, 0.6);
}

void player_kill(player_t* player, double time) {
	player->health -= time;
}

player_t* player_active() {
	if (g_state == MINTG_INPUT_KEYUP || g_state == MINTG_INPUT_KEYUP_EVENT) {
		return &player_qld;
	} else {
		return &player_nsw;
	}
}