/*
 * button.c
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
 
#include "button.h"
#include "res.h"
#include "player.h"

void button_init(button_t* button) {
	button->time = 0;
	interp_init(&button->scale, 1);
}

_Bool button_update(button_t* button, double time) {
	interp_update(&button->scale);
	button->time += time;

	int width = mintg_font_width(res_font_menu, button->text);
	int height = 20;
	double x = button->x - width / 2.0;
	double y = button->y - height / 2.0;

	mintg_input_key(MINTG_INPUT_LBUTTON, &button->state);
	player_t* player = player_active();
	if (player->x.v > x && player->x.v < x + width && player->y.v > y && player->y.v < y + height) {
		button->hover = 1;
		if (button->state == MINTG_INPUT_KEYUP || button->state == MINTG_INPUT_KEYUP_EVENT) {
			interp_init(&button->scale, 1);
		} else {
			interp_init(&button->scale, 0.9);
		}
		if (button->state == MINTG_INPUT_KEYUP_EVENT) {
			return 1;
		}
	} else {
		button->hover = 0;
		button->scale.v = cos(button->time * 3) * 0.05 + 0.95;
	}
	return 0;
}

void button_draw(button_t* button, double time) {
	double scale = interp_value(&button->scale, time);
	mintg_push();
	mintg_translate(button->x, button->y);
	mintg_scale(scale, scale);
	mintg_translate(0, 5);
	mintg_color(1, 1, 1, 1);
	mintg_font_draw(res_font_menu, button->text);
	mintg_pop();

	if (button->hover) {
		mintg_push();
		mintg_translate(0, -200);
		mintg_font_draw(res_font_menu_small, button->info);
		mintg_pop();
	}
}