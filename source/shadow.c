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
#include "player.h"

typedef enum shadow_state_t {
	SHADOW_INIT,
	SHADOW_MOVE,
	SHADOW_ATTACK
} shadow_state_t;

typedef struct shadow_t {
	interp_t x;
	interp_t y;
	double vx;
	double vy;
	double tx;
	double ty;
	shadow_state_t state;
} shadow_t;

typedef struct rect_t {
	double x;
	double y;
	interp_t alpha;
} rect_t;

static mint_array_t* g_shadows = NULL;
static mint_array_t* g_rects;

static double shadow_dist(shadow_t* shadow, double x, double y, double* dx, double* dy) {
	double rx = x - shadow->x.v;
	double ry = y - shadow->y.v;
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
		mint_array_add(g_shadows, -1, 64);
	}

	for (int i = 0; i < mint_array_size(g_shadows); ++i) {
		double angle = mint_random(0, M_PI * 2);
		shadow_t* shadow = mint_array_get(g_shadows, i);
		interp_init(&shadow->x, cos(angle) * 600);
		interp_init(&shadow->y, sin(angle) * 600);
		shadow->vx = 0;
		shadow->vy = 0;
		shadow->tx = shadow->x.v;
		shadow->ty = shadow->y.v;
		shadow->state = SHADOW_INIT;
	}
	g_rects = mint_array_create(sizeof(rect_t));
}

void shadow_update(double time) {
	for (int i = 0; i < mint_array_size(g_shadows); ++i) {
		shadow_t* shadow = mint_array_get(g_shadows, i);
		interp_update(&shadow->x);
		interp_update(&shadow->y);

		double dx, dy;
		double dist = shadow_dist(shadow, shadow->tx, shadow->ty, &dx, &dy);
		if (dist == 0) {
			dist = 1;
		}
		double accel = time * 300 / dist;
		shadow->vx = min(shadow->vx + dx * accel, 300);
		shadow->vy = min(shadow->vy + dy * accel, 300);
		shadow->vx *= pow(0.3, time);
		shadow->vy *= pow(0.3, time);
		shadow->x.v += shadow->vx * time;
		shadow->y.v += shadow->vy * time;

		if (dist < 32) {
			double px, py;
			player_pos(&px, &py);
			switch (shadow->state) {
				case SHADOW_ATTACK:
					if (mint_array_size(g_shadows) < 30) {
						shadow_t *child = mint_array_add(g_shadows, -1, 1);
						shadow = mint_array_get(g_shadows, i);
						interp_init(&child->x, shadow->x.v);
						interp_init(&child->y, shadow->y.v);
						child->vx = shadow->vx;
						child->vy = shadow->vx;
						child->tx = child->x.v;
						child->ty = child->y.v;
						child->state = SHADOW_INIT;
					}

				case SHADOW_INIT:;
					double angle = mint_random(0, M_PI * 2);
					shadow->tx = px + cos(angle) * 100;
					shadow->ty = py + sin(angle) * 100;
					shadow->state = SHADOW_MOVE;
					break;

				case SHADOW_MOVE:
					shadow->tx = px * 2 - shadow->x.v;
					shadow->ty = py * 2 - shadow->y.v;
					shadow->state = SHADOW_ATTACK;
					break;
			}
		}

		const int rect_count = 10;
		for (int j = 0; j < rect_count; ++j) {
			double interp = time / rect_count * j;
			double angle = mint_random(0, M_PI * 2);
			dist = mint_random(0, 16);

			rect_t* rect = mint_array_add(g_rects, -1, 1);
			rect->x = interp_value(&shadow->x, interp) + cos(angle) * dist;
			rect->y = interp_value(&shadow->y, interp) + sin(angle) * dist;
			interp_init(&rect->alpha, 0.5);
		}
	}

	for (int i = 0; i < mint_array_size(g_rects); ++i) {
		rect_t* rect = mint_array_get(g_rects, i);
		interp_update(&rect->alpha);

		if (rect->alpha.v == 0) {
			mint_array_remove(g_rects, i, 1);
			--i;
			continue;
		}
		rect->alpha.v = max(rect->alpha.v - time, 0);
	}
}

void shadow_draw(double time) {
	for (int i = 0; i < mint_array_size(g_rects); ++i) {
		rect_t* rect = mint_array_get(g_rects, i);
		mintg_push();
		mintg_translate(rect->x, rect->y);
		mintg_scale(8, 8);
		mintg_color(0, 0, 0, interp_value(&rect->alpha, time));
		mintg_image_draw(res_image_rect, NULL);
		mintg_pop();
	}
}

void shadow_exit() {
	mint_array_destroy(g_rects);
}