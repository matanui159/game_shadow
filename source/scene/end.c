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
#include "game.h"
#include "../res.h"
#include "../button.h"
#include "../player.h"
#include "../messages.h"
#include <stdlib.h>

void end_scene(scene_state_t state, double time) {
	static button_t btn_message = {"", "", 0, -200};
	static button_t btn_exit = {"Quit", "", 0, -300};
	static int message;
	static double timer;
	static interp_t final;
	static int beats;

	if (state == SCENE_INIT) {

		message = 0;
		btn_message.text = messages_end[message];
		timer = 0;
		button_init(&btn_message);
		button_init(&btn_exit);
		interp_init(&final, 0);
		beats = 0;
		player_init();

	} else if (state == SCENE_UPDATE) {

		interp_update(&final);

		timer += time;
		if (timer >= 0.1 && beats < 1) {
			minta_sound_play(res_sound_beat);
			++beats;
		}

		if (timer >= 0.3) {
			if (beats < 2) {
				minta_sound_play(res_sound_beat);
				minta_music_play(res_music_end);
				++beats;
			}

			if (message < messages_end_count - 1) {
				if (button_update(&btn_message, time)) {
					++message;
					btn_message.text = messages_end[message];
				}
			} else {
				final.v += time;
				if (final.v > 5 && button_update(&btn_exit, time)) {
					exit(0);
				}
			}

			player_update(0, time);
		}

		minta_music_update(res_music_end);

	} else if (state == SCENE_DRAW) {

		mintg_color(0, 0, 0, 1);
		mintg_clear();

		if (timer >= 0.3) {
			double f = interp_value(&final, time);
			mintg_image_t* eye = NULL;
			if (f < 0.6) {
				eye = res_image_eye[0];
			} else if (f > 1) {
				eye = res_image_eye[4];
			} else {
				eye = res_image_eye[(int)((f - 0.6) / 0.1)];
			}

			mintg_color(1, 1, 1, 1);
			mintg_push();
			mintg_translate(-100, 0);
			mintg_image_draw(eye, NULL);
			mintg_pop();

			mintg_push();
			mintg_translate(100, 0);
			mintg_image_draw(eye, NULL);
			mintg_pop();

			if (message < messages_end_count - 1) {
				button_draw(&btn_message, time);
			}

			if (f > 1) {
				mintg_push();
				mintg_translate(0, -200);
				mintg_color(1, 1, 1, (f - 1) * (f - 1));
				mintg_font_draw(res_font_clean, messages_end[message]);
				mintg_pop();
			}

			if (f > 3) {
				mintg_color(1, 1, 1, (f - 3) * (f - 3));
				mintg_push();
				mintg_translate(-200, -250);
				mintg_font_draw(res_font_clean_small, "Made by Joshua Minter");
				mintg_pop();

				mintg_push();
				mintg_translate(200, -250);
				mintg_font_draw(res_font_clean_small, "Music by Bensound.com");
				mintg_pop();
			}

			if (f > 5) {
				btn_exit.alpha = (f - 5) * (f - 5);
				button_draw(&btn_exit, time);
			}

			if (f > 7) {
				mintg_push();
				mintg_translate(0, 300);
				mintg_color(1, 1, 1, (f - 7) * (f - 7));
				mintg_font_draw(res_font_clean_small, "You survived a total of %.1f seconds", game_time());
				mintg_pop();
			}

			player_draw(0, time);
		}

	} else if (state == SCENE_EXIT) {

		minta_music_stop(res_music_end);

	}
}