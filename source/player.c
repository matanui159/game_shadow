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

static void player_init1(player_t* player, double x) {
	player->alive = 1;
	player->time = 0;
	interp_init(&player->x, x);
	interp_init(&player->y, -100);
	interp_init(&player->scale, 1);
}

void player_init() {
	if (!g_init) {
		player_init1(&player_qld, -100);
		player_init1(&player_nsw, 100);
		player_init1(&player_act, 0);
		g_init = 1;
	}
}

static void part_update(player_part_t* part, double time) {
	interp_update(&part->x);
	interp_update(&part->y);
	interp_update(&part->angle);
	part->vy -= 1000 * time;
	part->x.v += part->vx * time;
	part->y.v += part->vy * time;
	part->angle.v += part->vangle * time;
}

static _Bool player_update1(player_t* player, double time, _Bool active) {
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
		double s0 = cos(player->time * 20 + M_PI);
		double s1 = cos(player->time * 10);
		player->scale.v = (s0 + 1) * (s1 + 1) / 15 + 0.8;

	} else {

		part_update(&player->left, time);
		part_update(&player->right, time);
		double bottom = -mintg_height() / 2.0 - 32;
		if (player->left.y.old >= bottom && player->left.y.v < bottom) {
			if (player->right.y.old >= bottom && player->left.y.v < bottom) {
				return 1;
			}
		}

	}
	return 0;
}

_Bool player_update(double time) {
	mintg_input_key(MINTG_INPUT_RBUTTON, &g_state);
	_Bool active = 0;
	switch (g_state) {
		case MINTG_INPUT_KEYUP_EVENT:
			if (player_qld.alive) {
				mintg_input_set(player_qld.x.v, player_qld.y.v);
			}
		case MINTG_INPUT_KEYUP:
			active = 1;
			break;
		case MINTG_INPUT_KEYDOWN_EVENT:
			if (player_nsw.alive) {
				mintg_input_set(player_nsw.x.v, player_nsw.y.v);
			}
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
	if (!player_qld.alive && !player_nsw.alive) {
		player_update1(&player_act, time, 1);
	}
	return result;
}

static void part_draw(player_part_t* part, double time, mintg_image_t* image) {
	mintg_push();
	mintg_translate(interp_value(&part->x, time), interp_value(&part->y, time));
	mintg_rotate(interp_value(&part->angle, time));
	mintg_image_draw(image, NULL);
	mintg_pop();
}

static void player_draw1(player_t* player, _Bool game, double time, double r, double g, double b) {
	if (player->alive) {

		mintg_push();
		mintg_translate(interp_value(&player->x, time), interp_value(&player->y, time));
		mintg_color(r, g, b, 1);
		if (game) {
			double scale = interp_value(&player->scale, time);
			mintg_scale(scale, scale);
			mintg_image_draw(res_image_heart_left, NULL);
			mintg_image_draw(res_image_heart_right, NULL);
		} else {
			mintg_image_draw(res_image_arrow, NULL);
		}
		mintg_pop();

	} else {

		mintg_color(r, g, b, 1);
		part_draw(&player->left, time, res_image_heart_left);
		part_draw(&player->right, time, res_image_heart_right);

	}
}

void player_draw(_Bool game, double time) {
	player_draw1(&player_qld, game, time, 0.6, 0, 0.2);
	player_draw1(&player_nsw, game, time, 0.2, 0, 0.6);
	if (!game && !player_qld.alive && !player_nsw.alive) {
		player_draw1(&player_act, game, time, 1, 1, 1);
	}
}

void player_kill(player_t* player) {
	player->alive = 0;

	interp_init(&player->left.x, player->x.v);
	interp_init(&player->left.y, player->y.v);
	interp_init(&player->left.angle, 0);
	player->left.vx = -100;
	player->left.vy = 300;
	player->left.vangle = 3;

	interp_init(&player->right.x, player->x.v);
	interp_init(&player->right.y, player->y.v);
	interp_init(&player->right.angle, 0);
	player->right.vx = 100;
	player->right.vy = 300;
	player->right.vangle = -3;
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