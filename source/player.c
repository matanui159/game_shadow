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

static struct {
	double time;
	interp_t x;
	interp_t y;
	interp_t scale;
	interp_t shield;
} g_player;

void player_update(double time) {
	interp_update(&g_player.x);
	interp_update(&g_player.y);
	interp_update(&g_player.scale);
	interp_update(&g_player.shield);
	mintg_input_cursor(&g_player.x.v, &g_player.y.v);

	int width, height;
	mintg_size(&width, &height);
	if (g_player.x.v < -width / 2.0) {
		g_player.x.v = -width / 2.0;
	}
	if (g_player.x.v > width / 2.0) {
		g_player.x.v = width / 2.0;
	}
	if (g_player.y.v < -height / 2.0) {
		g_player.y.v = -height / 2.0;
	}
	if (g_player.y.v > height / 2.0) {
		g_player.y.v = height / 2.0;
	}

	g_player.time += time;
	double s0 = cos(g_player.time * 20 + M_PI);
	double s1 = cos(g_player.time * 10);
	g_player.scale.v = (s0 + 1) * (s1 + 1) / 15 + 0.8;

	double vx = g_player.x.v - g_player.x.old;
	double vy = g_player.y.v - g_player.y.old;
	if (vx != 0 || vy != 0) {
		double angle = atan2(vy, vx);
		while (angle - g_player.shield.v < -M_PI) {
			angle += M_PI * 2;
		}
		while (angle - g_player.shield.v > M_PI) {
			angle -= M_PI * 2;
		}
		g_player.shield.v = pow(0.05, time) * (g_player.shield.v - angle) + angle;
	}
}

void player_draw(_Bool game, double time) {
	double scale = interp_value(&g_player.scale, time);
	if (!game) {
		scale = 1;
	}

	mintg_push();
	mintg_translate(interp_value(&g_player.x, time), interp_value(&g_player.y, time));
	mintg_scale(scale, scale);
	mintg_color(0.6, 0, 0.2, 1);
	if (game) {
		mintg_image_draw(res_image_heart, NULL);
		mintg_push();
		mintg_rotate(interp_value(&g_player.shield, time));
		mintg_translate(-32, 0);
		mintg_scale(4, 32);
		mintg_color(0.5, 0.5, 0.5, 1);
		mintg_image_draw(res_image_rect, NULL);
		mintg_pop();
	} else {
		mintg_image_draw(res_image_arrow, NULL);
	}
	mintg_pop();
}

void player_pos(double* x, double* y) {
	*x = g_player.x.v;
	*y = g_player.y.v;
}