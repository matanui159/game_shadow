/*
 * shadow.c
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
 
#include "shadow.h"
#include "res.h"
#include "fade_buffer.h"
#include "player.h"

typedef enum shadow_state_t {
	SHADOW_INIT,
	SHADOW_MOVE,
	SHADOW_ATTACK
} shadow_state_t;

typedef struct shadow_t {
	double x;
	double y;
	double vx;
	double vy;
	double tx;
	double ty;
	double time;
	shadow_state_t state;
} shadow_t;

typedef struct rect_t {
	double x;
	double y;
	interp_t alpha;
} rect_t;

static mint_array_t* g_shadows = NULL;
static mintg_image_t* g_buffer;

static double shadow_dist(shadow_t* shadow, double x, double y, double* dx, double* dy) {
	double rx = x - shadow->x;
	double ry = y - shadow->y;
	if (dx != NULL) {
		*dx = rx;
	}
	if (dy != NULL) {
		*dy = ry;
	}
	return sqrt(rx * rx + ry * ry);
}

void shadow_init() {
	if (g_shadows == NULL) {
		g_shadows = mint_array_create(sizeof(shadow_t));
		mint_array_add(g_shadows, -1, 1);
	}
	if (g_buffer == NULL) {
		int width, height;
		mintg_size(&width, &height);
		g_buffer = mintg_image_create(width, height, NULL);
	}

	for (int i = 0; i < mint_array_size(g_shadows); ++i) {
//		double angle = mint_random(0, M_PI * 2);
		shadow_t* shadow = mint_array_get(g_shadows, i);
//		shadow->x = cos(angle) * 500;
//		shadow->y = sin(angle) * 500;
		shadow->x = -400;
		shadow->y = -400;
		shadow->vx = 0;
		shadow->vy = 0;
		shadow->tx = shadow->x;
		shadow->ty = shadow->y;
		shadow->time = 0;
		shadow->state = SHADOW_INIT;
	}
	fade_buffer_init(g_buffer);
}

_Bool shadow_update(double time) {
	_Bool result = 0;
	int width, height;
	mintg_size(&width, &height);
	fade_buffer_update(g_buffer);
	for (int i = 0; i < mint_array_size(g_shadows); ++i) {
		shadow_t* shadow = mint_array_get(g_shadows, i);

		double dx, dy;
		double dist = shadow_dist(shadow, shadow->tx, shadow->ty, &dx, &dy);
		if (dist == 0) {
			dist = 1;
		}

		double vx = dx / dist * 300;
		double vy = dy / dist * 300;
		shadow->vx = pow(0.3, time) * (shadow->vx - vx) + vx;
		shadow->vy = pow(0.3, time) * (shadow->vy - vy) + vy;
		shadow->x += shadow->vx * time;
		shadow->y += shadow->vy * time;

		double px, py;
		player_pos(&px, &py);
		if (dist < 32) {
			switch (shadow->state) {
				case SHADOW_ATTACK:;
					shadow_t *child = mint_array_add(g_shadows, -1, 1);
					shadow = mint_array_get(g_shadows, i);
					*child = *shadow;
					child->state = SHADOW_INIT;

				case SHADOW_INIT:;
					shadow->tx = mint_random(-width / 2.0, width / 2.0);
					shadow->ty = mint_random(-height / 2.0, height / 2.0);
					shadow->state = SHADOW_MOVE;
					break;

				case SHADOW_MOVE:
					shadow->tx = px * 2 - shadow->x;
					shadow->ty = py * 2 - shadow->y;
					shadow->state = SHADOW_ATTACK;
					break;
			}
		}

		if (shadow_dist(shadow, px, py, NULL, NULL) < 24) {
			result = 1;
		}

		const double shadow_clock = 0.001;
		shadow->time += time;
		while (shadow->time > shadow_clock) {
			double angle = mint_random(0, M_PI * 2);
			double offset = mint_random(0, 16);
			mintg_push();
			mintg_translate(
					shadow->x + cos(angle) * offset,
					shadow->y + sin(angle) * offset
			);
			mintg_rotate(mint_random(0, M_PI * 2));
			mintg_scale(8, 8);
			mintg_color(0, 0, 0, 0.5);
			mintg_image_draw(res_image_rect, NULL);
			mintg_pop();
			shadow->time -= shadow_clock;
		}
	}
	mintg_image_target(NULL);
	return result;
}

void shadow_draw() {
	mintg_image_draw(g_buffer, NULL);
}