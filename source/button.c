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

_Bool button_update(button_t* button, double time) {
	interp_update(&button->scale);
	button->time += time;

	int width = 0;
	int height = 20;
	mintg_font_size(res_font_menu, button->text, &width, NULL);
	double x = button->x - width / 2.0;
	double y = button->y - height / 2.0;

	double px, py;
	player_pos(&px, &py);
	mintg_input_state_t state = mintg_input_key_state(MINTG_INPUT_LBUTTON, &button->state);
	if (px > x && px < x + width && py > y && py < y + height) {
		button->hover = 1;
		if (button->state) {
			button->scale.v = 0.9;
		} else {
			button->scale.v = 1;
		}
		if (state == MINTG_INPUT_RELEASED) {
			return 1;
		}
	} else {
		button->hover = 0;
		button->scale.v = sin(button->time * 3) * 0.05 + 0.95;
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