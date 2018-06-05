/*
 * light.c
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
 
#include "light.h"
#include "res.h"
#include "fade_buffer.h"
#include "player.h"

static struct {
	double time;
	interp_t x;
	interp_t y;
	interp_t scale;
	interp_t angle;
	double tx;
	double ty;
	double ex;
	double ey;
	double radius;
} g_light;

static mintg_image_t* g_buffer;

static void light_trans(double* value, double target, double time) {
	*value = pow(0.1, time) * (*value - target) + target;
}

void light_init() {
	if (g_buffer == NULL) {
		int width, height;
		mintg_size(&width, &height);
		g_buffer = mintg_image_create(width, height, NULL);
	}
	g_light.time = 0;
	interp_init(&g_light.x, 0);
	interp_init(&g_light.y, 0);
	interp_init(&g_light.scale, 1);
	interp_init(&g_light.angle, 0);
	g_light.tx = 200;
	g_light.ty = 200;
	g_light.radius = 0;
}

void light_update(double time) {
	double px, py;
	player_pos(&px, &py);
	if (pos_dist(g_light.tx, g_light.ty, px, py, NULL, NULL) < 24) {
		int width, height;
		mintg_size(&width, &height);
		g_light.ex = g_light.tx;
		g_light.ey = g_light.ty;
		g_light.tx = mint_random(-width / 2.0, width / 2.0);
		g_light.ty = mint_random(-height / 2.0, height / 2.0);
		interp_init(&g_light.angle, 0);
		g_light.radius += time;
	}

	g_light.time += time;
	interp_update(&g_light.x);
	interp_update(&g_light.y);
	interp_update(&g_light.scale);
	interp_update(&g_light.angle);
	light_trans(&g_light.x.v, g_light.tx, time);
	light_trans(&g_light.y.v, g_light.ty, time);
	light_trans(&g_light.angle.v, M_PI * 2, time);
	g_light.scale.v = cos(g_light.time * 3) * 0.05 + 0.95;

	fade_buffer_update(g_buffer);
	if (g_light.radius > 0) {

		g_light.radius += time;
		double radius = g_light.radius * (2 - g_light.radius);
		mintg_push();
		mintg_translate(g_light.ex, g_light.ey);
		mintg_scale(radius, radius);
		mintg_color(0.8, 0.8, 0.2, 1);
		mintg_image_draw(res_image_wave, NULL);
		mintg_pop();

		if (g_light.radius > 1) {
			g_light.radius = 0;
		}

	}
	mintg_image_target(NULL);
}

void light_draw(double time) {
	mintg_color(1, 1, 1, 1);
	mintg_image_draw(g_buffer, NULL);

	double scale = interp_value(&g_light.scale, time);
	mintg_push();
	mintg_translate(interp_value(&g_light.x, time), interp_value(&g_light.y, time));
	mintg_scale(scale, scale);
	mintg_rotate(interp_value(&g_light.angle, time));
	mintg_color(0.8, 0.8, 0.2, 1);
	mintg_image_draw(res_image_light, NULL);
	mintg_pop();
}