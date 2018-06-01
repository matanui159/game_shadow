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

#define RECT_COUNT 10

typedef struct shadow_t {
	interp_t x;
	interp_t y;
} shadow_t;

typedef struct rect_t {
	double x;
	double y;
	interp_t alpha;
} rect_t;

static mint_array_t* g_shadows = NULL;
static mint_array_t* g_rects;

void shadow_init() {
	if (g_shadows == NULL) {
		g_shadows = mint_array_create(sizeof(rect_t));
		mint_array_add(g_shadows, -1, 1);
	}

	for (int i = 0; i < mint_array_size(g_shadows); ++i) {
		shadow_t* shadow = mint_array_get(g_shadows, i);
		interp_init(&shadow->x, 0);
		interp_init(&shadow->y, 0);
	}
	g_rects = mint_array_create(sizeof(rect_t));
}

void shadow_update(double time) {
	for (int i = 0; i < mint_array_size(g_shadows); ++i) {
		shadow_t* shadow = mint_array_get(g_shadows, i);
		interp_update(&shadow->x);
		interp_update(&shadow->y);
		mintg_input_cursor(&shadow->x.v, &shadow->y.v);

		for (int j = 0; j < RECT_COUNT; ++j) {
			double interp = time / RECT_COUNT * j;
			double angle = mint_random(0, M_PI * 2);
			double dist = mint_random(0, 16);

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
	for (int i = 0; i < mint_array_size(g_rects); ++i) {\
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