/*
 * end.c
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
 
#include "end.h"
#include "../res.h"
#include "../button.h"
#include "../player.h"
#include "../messages.h"

void end_scene(scene_state_t state, double time) {
	static button_t btn_message = {"", "", 0, -200};
	static int message;
	static double timer;

	if (state == SCENE_INIT) {

		message = 0;
		btn_message.text = messages_end[message];
		timer = 0;
		player_init();

	} else if (state == SCENE_UPDATE) {

		timer += time;
		if (timer >= 0.1 && timer < 0.1 + time) {
			minta_sound_play(res_sound_beat);
		}
		if (timer >= 0.3) {
			if (timer < 0.3 + time) {
				minta_sound_play(res_sound_beat);
			}
			if (button_update(&btn_message, time)) {
				++message;
				if (message == messages_end_count) {
					message = messages_end_count - 1;
				}
				btn_message.text = messages_end[message];
			}
			player_update(0, time);
		}

	} else if (state == SCENE_DRAW) {

		mintg_color(0, 0, 0, 1);
		mintg_clear();

		if (timer >= 0.3) {
			mintg_color(1, 1, 1, 1);

			mintg_push();
			mintg_translate(-100, 0);
			mintg_image_draw(res_image_eye, NULL);
			mintg_pop();

			mintg_push();
			mintg_translate(100, 0);
			mintg_image_draw(res_image_eye, NULL);
			mintg_pop();

			button_draw(&btn_message, time);
			player_draw(0, time);
		}

	} else if (state == SCENE_EXIT) {



	}
}