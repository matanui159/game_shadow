/*
 * main.c
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

#include "main.h"
#include "res.h"
#include "menu.h"

#define GAME_CLOCK 0.02

static scene_t g_scene = menu_scene;

void interp_init(interp_t* interp, double value) {
	interp->v = value;
	interp->old = value;
}

void interp_update(interp_t* interp) {
	interp->old = interp->v;
}

double interp_value(interp_t* interp, double time) {
	return (time / GAME_CLOCK) * (interp->v - interp->old) + interp->old;
}

void scene_set(scene_t scene) {
	g_scene(SCENE_EXIT, 0);
	g_scene = scene;
	g_scene(SCENE_INIT, 0);
}

double pos_dist(double x0, double y0, double x1, double y1, double* dx, double* dy) {
	double rx = x1 - x0;
	double ry = y1 - y0;
	if (dx != NULL) {
		*dx = rx;
	}
	if (dy != NULL) {
		*dy = ry;
	}
	return sqrt(rx * rx + ry * ry);
}

int main(int argc, char* argv[]) {
	const int window_size = 720;
	mintg_init("Kage", window_size, window_size);
	mintg_input_hide(1);
	res_init();

	mintg_image_t* buffer = mintg_image_create(window_size * 2, window_size * 2, NULL);
	mintg_input_state_t key_f1;
	_Bool ssaa = 1;

	mint_timer_t timer;
	mint_timer_set(&timer, 0);
	double time = 0;

	int fps_count = 0;
	mint_timer_t fps_timer;
	mint_timer_set(&fps_timer, 0);
	char fps_buffer[16] = "FPS: 0";

	g_scene(SCENE_INIT, 0);
	while (mintg_update()) {
		if (mint_timer_get(&fps_timer) >= 1) {
			snprintf(fps_buffer, sizeof(fps_buffer), "FPS: %i", fps_count);
			fps_count = 0;
			mint_timer_set(&fps_timer, 0);
		}
		++fps_count;

		time += mint_timer_set(&timer, 0);
		if (time >= GAME_CLOCK) {
			g_scene(SCENE_UPDATE, GAME_CLOCK);
			time -= GAME_CLOCK;
			if (time > GAME_CLOCK) {
				time = GAME_CLOCK;
			}
		}

		if (mintg_input_key(MINTG_INPUT_F1, &key_f1) == MINTG_INPUT_KEYDOWN_EVENT) {
			ssaa = !ssaa;
		}
		if (ssaa) {
			mintg_image_target(buffer);
			mintg_push();
			mintg_scale(2, 2);
		}

		g_scene(SCENE_DRAW, time);

		if (ssaa) {
			mintg_pop();
			mintg_image_target(NULL);
			mintg_push();
			mintg_scale(0.5, 0.5);
			mintg_color(1, 1, 1, 1);
			mintg_image_draw(buffer, NULL);
			mintg_pop();
		}

		mintg_push();
		mintg_translate(0, -200);
		mintg_color(0.5, 0.5, 0.5, 1);
		mintg_font_draw(res_font_menu_small, fps_buffer);
		mintg_pop();
	}
}