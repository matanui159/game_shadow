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
#include "scene/tutorial.h"

#define GAME_CLOCK 0.02

static scene_t g_scene = tutorial_scene;
static mintg_image_t* g_buffer;

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

mintg_image_t* buffer_get() {
	return g_buffer;
}

int main(int argc, char* argv[]) {
	mintg_init("Kage", 720, 720);
	mintg_input_hide(1);
	minta_init();
	res_init();

	mintg_image_t* buffer = mintg_image_create(mintg_width() * 2, mintg_height() * 2, NULL);
	mintg_input_state_t key_f1;
	_Bool ssaa = 1;

	mint_timer_t timer;
	mint_timer_set(&timer, 0);
	double time = 0;

	g_scene(SCENE_INIT, 0);
	while (mintg_update()) {
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
			g_buffer = buffer;
		} else {
			g_buffer = NULL;
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
	}
}