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
#include "death.h"
#include "../res.h"
#include "../player.h"
#include "../shadow.h"


void game_scene(scene_state_t state, double time) {
	static interp_t fade;
	static mintg_image_t* buffer;

	if (state == SCENE_INIT) {

		if (buffer == NULL) {
			buffer = mintg_image_create(mintg_width(), mintg_height(), NULL);
		}
		interp_init(&fade, 1);
		player_init();
		shadow_init();

	} else if (state == SCENE_UPDATE) {

		interp_update(&fade);
		player_update(time);
		player_t* player = shadow_update(time);
		if (player != NULL) {
			death_init(player, buffer);
			scene_set(death_scene);
		}
		fade.v -= time;

	} else if (state == SCENE_DRAW) {

		mintg_image_target(buffer);
		mintg_push();
		mintg_identity();

		mintg_color(0, 0, 0, 0);
		mintg_clear();

		player_draw(1, time);
		shadow_draw(time);

		mintg_pop();
		mintg_image_target(buffer_get());
		mintg_color(1, 1, 1, 1);
		mintg_clear();
		mintg_image_draw(buffer, NULL);

		double alpha = interp_value(&fade, time);
		mintg_push();
		mintg_scale(mintg_width(), mintg_height());
		mintg_color(1, 1, 1, alpha * (2 - alpha));
		mintg_image_draw(res_image_rect, NULL);
		mintg_pop();

	}
}