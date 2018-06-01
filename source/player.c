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

static struct {
	double time;
	interp_t x;
	interp_t y;
	interp_t scale;
} g_player;

void player_update(double time) {
	interp_update(&g_player.x);
	interp_update(&g_player.y);
	interp_update(&g_player.scale);
	mintg_input_cursor(&g_player.x.v, &g_player.y.v);

	g_player.time += time;
	double s0 = cos(g_player.time * 20 + M_PI);
	double s1 = cos(g_player.time * 10);
	g_player.scale.v = (s0 + 1) * (s1 + 1) / 30 + 0.9;
}

void player_draw(mintg_image_t* image, _Bool beat, double time) {
	double scale = interp_value(&g_player.scale, time);
	if (!beat) {
		scale = 1;
	}

	mintg_push();
	mintg_translate(interp_value(&g_player.x, time), interp_value(&g_player.y, time));
	mintg_scale(scale, scale);
	mintg_color(0.6, 0, 0.2, 1);
	mintg_image_draw(image, NULL);
	mintg_pop();
}

void player_pos(double* x, double* y) {
	*x = g_player.x.v;
	*y = g_player.y.v;
}