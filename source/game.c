/*
 * game.c
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
 
#include "game.h"
#include "res.h"
#include <math.h>

void game_scene(scene_state_t state, double time) {
	static interp_t fade_in;
	static interp_t heart_x;
	static interp_t heart_y;
	static interp_t heart_angle;

	if (state == SCENE_INIT) {

		interp_init(&fade_in, 1);

	} else if (state == SCENE_UPDATE) {

		interp_update(&fade_in);
		interp_update(&heart_x);
		interp_update(&heart_y);
		interp_update(&heart_angle);
		fade_in.v -= time;
		if (fade_in.v < 0) {
			fade_in.v = 0;
		}
		mintg_input_cursor(&heart_x.v, &heart_y.v);
		heart_angle.v += (heart_x.v - heart_x.old) / time / 50;
		heart_angle.v = (heart_angle.v / pow(2, time * 50));

	} else if (state == SCENE_DRAW) {

		mintg_color(1, 1, 1, 1);
		mintg_clear();

		mintg_push();
		mintg_translate(interp_value(&heart_x, time), interp_value(&heart_y, time));
		mintg_rotate(interp_value(&heart_angle, time));
		mintg_color(0.6, 0, 0.2, 1);
		mintg_image_draw(res_image_heart, NULL);
		mintg_pop();

		int width, height;
		mintg_size(&width, &height);
		mintg_push();
		mintg_scale(width, height);
		mintg_color(1, 1, 1, sqrt(interp_value(&fade_in, time)));
		mintg_image_draw(res_image_rect, NULL);
		mintg_pop();

	}
}