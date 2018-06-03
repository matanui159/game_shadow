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
#include "player.h"
#include "shadow.h"

void game_scene(scene_state_t state, double time) {
	static interp_t fade;

	if (state == SCENE_INIT) {

		interp_init(&fade, 1);
		shadow_init();

	} else if (state == SCENE_UPDATE) {

		interp_update(&fade);
		fade.v -= time;
		player_update(time);
		shadow_update(time);

	} else if (state == SCENE_DRAW) {

		mintg_color(1, 1, 1, 1);
		mintg_clear();

		player_draw(res_image_heart, 1, time);
		shadow_draw(time);

		int width, height;
		double alpha = interp_value(&fade, time);
		mintg_size(&width, &height);
		mintg_push();
		mintg_scale(width, height);
		mintg_color(1, 1, 1, alpha * (2 - alpha));
		mintg_image_draw(res_image_rect, NULL);
		mintg_pop();

	} else if (state == SCENE_EXIT) {

		shadow_exit();

	}
}