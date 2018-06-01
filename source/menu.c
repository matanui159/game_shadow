/*
 * menu.c
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
 
#include "menu.h"
#include "res.h"
#include "button.h"
#include "player.h"
#include "game.h"
#include <math.h>

void menu_scene(scene_state_t state, double time) {
	static interp_t fade_in;
	static interp_t fade_out;

	static button_t btn_normal = {"NORMAL", "Handle it yourself", 0, 0};
	static button_t btn_easy = {"EASY", "Get help", 0, -100};

	if (state == SCENE_INIT) {

		interp_init(&fade_in, 1);
		interp_init(&fade_out, 0);

	} else if (state == SCENE_UPDATE) {

		interp_update(&fade_in);
		interp_update(&fade_out);
		fade_in.v -= time;
		if (fade_in.v < 0) {
			fade_in.v = 0;
		}
		if (button_update(&btn_normal, time) || fade_out.v > 0) {
			fade_out.v += time;
			if (fade_out.v >= 1) {
				fade_out.v = 1;
				scene_set(game_scene);
			}
		}
		player_update(time);
		button_update(&btn_easy, time);

	} else if (state == SCENE_DRAW) {

		mintg_color(0, 0, 0, 1);
		mintg_clear();

		mintg_push();
		mintg_translate(0, 200);
		mintg_color(1, 1, 1, 1);
		mintg_font_draw(res_font_menu_large, "Choose your difficulty");
		mintg_pop();

		button_draw(&btn_normal, time);
//		button_draw(&btn_easy, time);

		player_draw(res_image_arrow, 0, time);

		int width, height;
		mintg_size(&width, &height);
		mintg_push();
		mintg_scale(width, height);
		mintg_color(1, 1, 1, sqrt(interp_value(&fade_in, time)));
		mintg_image_draw(res_image_rect, NULL);
		mintg_pop();

		mintg_push();
		mintg_scale(width, height);
		mintg_color(1, 1, 1, sqrt(interp_value(&fade_out, time)));
		mintg_image_draw(res_image_rect, NULL);
		mintg_pop();

	}
}