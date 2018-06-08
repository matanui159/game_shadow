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
#include "game.h"
#include "../res.h"
#include "../fade_buffer.h"
#include "../button.h"
#include "../player.h"
#include "../messages.h"

static void menu_message(const char* const* messages, int count) {
	mintg_push();
	mintg_translate(
			mint_random(-mintg_width() / 2.0, mintg_width() / 2.0),
			mint_random(-mintg_height() / 2.0, mintg_height() / 2.0)
	);
	mintg_color(0.5, 0.5, 0.5, 1);
	mintg_font_draw(res_font_messy, messages[(int)mint_random(0, count)]);
	mintg_pop();
}

void menu_scene(scene_state_t state, double time) {
	static interp_t fade;
	static _Bool exit;
	static mintg_image_t* buffer;
	static int count;

	static button_t btn_normal = {"NORMAL", "Handle it yourself", 0, 0};
	static button_t btn_easy = {"EASY", "Get help", 0, -100};

	if (state == SCENE_INIT) {

		interp_init(&fade, 1);
		exit = 0;
		if (buffer == NULL) {
			buffer = mintg_image_create(mintg_width(), mintg_height(), NULL);
		}
		fade_buffer_init(buffer);
		count = 0;

		button_init(&btn_normal);
		button_init(&btn_easy);
		player_init();

	} else if (state == SCENE_UPDATE) {

		interp_update(&fade);
		player_update(time);

		fade_buffer_update(buffer, time / 3);
		if (button_update(&btn_normal, time)) {
			if (!player_qld.alive && !player_nsw.alive) {
				menu_message(messages_normal, messages_normal_count);
				++count;
			} else if (!exit) {
				if (fade.v < 0) {
					interp_init(&fade, 0);
				}
				exit = 1;
			}
		}

		if (!player_qld.alive || !player_nsw.alive) {
			if (button_update(&btn_easy, time)) {
				menu_message(messages_easy, messages_easy_count);
				++count;
			}
		}

		if (!player_qld.alive && !player_nsw.alive && count > 10) {
			if (mint_random(0, 1) < 0.5) {
				menu_message(messages_normal, messages_normal_count);
			} else {
				menu_message(messages_easy, messages_easy_count);
			}
		}
		mintg_image_target(NULL);

		if (exit) {
			fade.v += time;
			if (fade.v >= 1) {
				scene_set(game_scene);
			}
		} else {
			fade.v -= time;
		}

	} else if (state == SCENE_DRAW) {

		mintg_color(0, 0, 0, 1);
		mintg_clear();

		fade_buffer_draw(buffer, time / 3);

		mintg_push();
		mintg_translate(0, 200);
		mintg_color(1, 1, 1, 1);
		mintg_font_draw(res_font_clean_large, "Choose your difficulty");
		mintg_pop();

		button_draw(&btn_normal, time);
		if (!player_qld.alive || !player_nsw.alive) {
			button_draw(&btn_easy, time);
		}

		player_draw(0, time);

		double alpha = interp_value(&fade, time);
		mintg_push();
		mintg_scale(mintg_width(), mintg_height());
		mintg_color(1, 1, 1, alpha * (2 - alpha));
		mintg_image_draw(res_image_rect, NULL);
		mintg_pop();

	}
}