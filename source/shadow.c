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

static mint_array_t* g_shadows = NULL;
static mintg_image_t* g_buffer = NULL;

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
	}
	if (g_buffer == NULL) {
		g_buffer = mintg_image_create(mintg_width(), mintg_height(), NULL);
	}

	shadow_t* shadow = mint_array_replace(g_shadows, 0, -1, 1);
	shadow->x = -mintg_width();
	shadow->y = -mintg_height();
	shadow->vx = 0;
	shadow->vy = 0;
	shadow->tx = shadow->x;
	shadow->ty = shadow->y;
	shadow->time = 0;
	shadow->state = SHADOW_INIT;
	fade_buffer_init(g_buffer);
}

player_t* shadow_update(double time) {
	fade_buffer_update(g_buffer, time * 2);
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

		double qld_dist = shadow_dist(shadow, player_qld.x.v, player_qld.y.v, NULL, NULL);
		double nsw_dist = shadow_dist(shadow, player_nsw.x.v, player_nsw.y.v, NULL, NULL);

		if (dist < 24) {
			switch (shadow->state) {
				case SHADOW_ATTACK:;
					shadow_t *child = mint_array_add(g_shadows, -1, 1);
					shadow = mint_array_get(g_shadows, i);
					*child = *shadow;
					child->state = SHADOW_INIT;

				case SHADOW_INIT:;
					shadow->tx = mint_random(-mintg_width() / 2.0, mintg_width() / 2.0);
					shadow->ty = mint_random(-mintg_height() / 2.0, mintg_height() / 2.0);
					shadow->state = SHADOW_MOVE;
					break;

				case SHADOW_MOVE:
					if (player_qld.alive && qld_dist < nsw_dist) {
						shadow->tx = player_qld.x.v * 2 - shadow->x;
						shadow->ty = player_qld.y.v * 2 - shadow->y;
					} else {
						shadow->tx = player_nsw.x.v * 2 - shadow->x;
						shadow->ty = player_nsw.y.v * 2 - shadow->y;
					}
					shadow->state = SHADOW_ATTACK;
					break;
			}
		}

		if (player_qld.alive && qld_dist < 24) {
			return &player_qld;
		}
		if (player_nsw.alive && nsw_dist < 24) {
			return &player_nsw;
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
	return NULL;
}

void shadow_draw(double time) {
	fade_buffer_draw(g_buffer, time * 2);
}