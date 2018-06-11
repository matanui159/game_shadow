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
player_t player_act;
static _Bool g_init = 0;
static mintg_input_state_t g_state;

static void player_init1(player_t* player, double x, double r, double g, double b) {
	player->alive = 1;
	player->time = 0;
	interp_init(&player->x, x);
	interp_init(&player->y, -100);
	interp_init(&player->scale, 0);
	player->r = r;
	player->g = g;
	player->b = b;
}

void player_init() {
	if (!g_init) {
		player_init1(&player_qld, -100, 0.6, 0, 0.2);
		player_init1(&player_nsw, 100, 0.2, 0, 0.6);
		player_init1(&player_act, 0, 1, 1, 1);
		g_init = 1;
	}
}

static void player_update1(player_t* player, double time, _Bool active) {
	if (player->alive) {
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

		player->time += time;
		player->scale.v = pow(0.001, time) * player->scale.v;
		if (player->time >= 0.6 && player->time < 0.6 + time) {
			player->scale.v = 0.2;
		}
		if (player->time >= 0.8 && player->time < 0.8 + time) {
			player->scale.v = 0.2;
		}
		if (player->time >= 1) {
			player->time = 0;
		}
	}
}

void player_update(double time) {
	mintg_input_key(MINTG_INPUT_RBUTTON, &g_state);
	switch (g_state) {
		case MINTG_INPUT_KEYUP_EVENT:
			if (player_qld.alive) {
				mintg_input_set(player_qld.x.v, player_qld.y.v);
			}
		case MINTG_INPUT_KEYUP:
			player_update1(&player_qld, time, 1);
			player_update1(&player_nsw, time, 0);
			break;

		case MINTG_INPUT_KEYDOWN_EVENT:
			if (player_nsw.alive) {
				mintg_input_set(player_nsw.x.v, player_nsw.y.v);
			}
		case MINTG_INPUT_KEYDOWN:
			player_update1(&player_qld, time, 0);
			player_update1(&player_nsw, time, 1);
			break;
	}

	if (!player_qld.alive && !player_nsw.alive) {
		player_update1(&player_act, time, 1);
	}

	double timer = 0;
	if (player_qld.alive) {
		timer = player_qld.time;
	}
	if (player_nsw.alive) {
		timer = player_nsw.time;
	}

	if (timer >= 0.6 && timer < 0.6 + time) {
		minta_sound_play(res_sound_beat);
	}
	if (timer >= 0.8 && timer < 0.8 + time) {
		minta_sound_play(res_sound_beat);
	}
}

static void player_draw1(player_t* player, _Bool game, double time) {
	if (player->alive) {
		mintg_push();
		mintg_translate(interp_value(&player->x, time), interp_value(&player->y, time));
		mintg_color(player->r, player->g, player->b, 1);
		if (game) {
			double scale = 0.8 + interp_value(&player->scale, time);
			mintg_scale(scale, scale);
			mintg_image_draw(res_image_player_left, NULL);
			mintg_image_draw(res_image_player_right, NULL);
		} else {
			mintg_image_draw(res_image_player_cursor, NULL);
		}
		mintg_pop();
	}
}

void player_draw(_Bool game, double time) {
	player_draw1(&player_qld, game, time);
	player_draw1(&player_nsw, game, time);
	if (!player_qld.alive && !player_nsw.alive) {
		player_draw1(&player_act, game, time);
	}
}

player_t* player_active() {
	if (!player_qld.alive && !player_nsw.alive) {
		return &player_act;
	}
	if (g_state == MINTG_INPUT_KEYUP || g_state == MINTG_INPUT_KEYUP_EVENT) {
		return &player_qld;
	} else {
		return &player_nsw;
	}
}