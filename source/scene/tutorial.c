/*
 * tutorial.c
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
 
#include "tutorial.h"
#include "menu.h"
#include "../res.h"
#include "../player.h"

void tutorial_scene(scene_state_t state, double time) {
	static interp_t fade;
	static _Bool exit;
	static double dist;

	if (state == SCENE_INIT) {

		interp_init(&fade, 1);
		exit = 0;
		dist = 0;
		player_init();

	} else if (state == SCENE_UPDATE) {

		interp_update(&fade);
		player_update(0, time);

		if (!exit) {
			double dx = player_nsw.x.v - player_nsw.x.old;
			double dy = player_nsw.y.v - player_nsw.y.old;
			dist += sqrt(dx * dx + dy * dy);
			if (dist > 300) {
				if (fade.v < -1) {
					interp_init(&fade, -1);
				}
				exit = 1;
			}
		}

		if (exit) {
			fade.v += time;
			if (fade.v >= 1) {
				scene_set(menu_scene);
			}
		} else {
			fade.v -= time;
		}

	} else if (state == SCENE_DRAW) {

		mintg_color(1, 1, 1, 1);
		mintg_clear();

		mintg_image_t* image = NULL;
		const char* text = NULL;
		if (exit) {
			image = res_image_tut_done;
			text = "WELL DONE";
		} else if (mintg_input_key(MINTG_INPUT_RBUTTON, NULL) == MINTG_INPUT_KEYDOWN) {
			image = res_image_tut_move;
			text = "MOVE";
		} else {
			image = res_image_tut_hold;
			text = "HOLD";
		}

		mintg_color(0, 0, 0, 1);
		mintg_image_draw(image, NULL);
		mintg_push();
		mintg_translate(0, -50);
		mintg_font_draw(res_font_clean, text);
		mintg_pop();

		player_draw(0, time);

		double alpha = interp_value(&fade, time);
		mintg_push();
		mintg_scale(mintg_width(), mintg_height());
		mintg_color(1, 1, 1, alpha * (2 - alpha));
		mintg_image_draw(res_image_rect, NULL);
		mintg_pop();

	}
}