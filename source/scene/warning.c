/*
 * warning.c
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
 
#include "warning.h"
#include "tutorial.h"
#include "../res.h"

void warning_scene(scene_state_t state, double time) {
	static interp_t timer;

	if (state == SCENE_INIT) {

		interp_init(&timer, 0);

	} else if (state == SCENE_UPDATE) {

		interp_update(&timer);
		timer.v += time;
		if (timer.v >= 4) {
			scene_set(tutorial_scene);
		}

	} else if (state == SCENE_DRAW) {

		mintg_color(1, 1, 1, 1);
		mintg_clear();

		mintg_color(0, 0, 0, 1);
		mintg_push();
		mintg_translate(0, 15);
		mintg_font_draw(res_font_clean, "This game contains loud noises");
		mintg_pop();

		mintg_push();
		mintg_translate(0, -15);
		mintg_font_draw(res_font_clean, "and themes of depression");
		mintg_pop();

		double t = interp_value(&timer, time);
		double alpha = 0;
		if (t < 1) {
			alpha = 1 - t;
		} else if (t > 3) {
			alpha = t - 3;
		}
		mintg_push();
		mintg_scale(mintg_width(), mintg_height());
		mintg_color(1, 1, 1, alpha * (2 - alpha));
		mintg_image_draw(res_image_rect, NULL);
		mintg_pop();

	}
}