/*
 * res.c
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
 
#include "res.h"

mintg_image_t* res_image_rect;

mintg_image_t* res_image_tut_hold;
mintg_image_t* res_image_tut_move;
mintg_image_t* res_image_tut_done;

mintg_image_t* res_image_player_cursor;
mintg_image_t* res_image_player_left;
mintg_image_t* res_image_player_right;

mintg_font_t* res_font_menu;
mintg_font_t* res_font_menu_large;
mintg_font_t* res_font_menu_small;

void res_init() {
	const uint8_t box_data[] = {0xFF, 0xFF, 0xFF, 0xFF};
	res_image_rect = mintg_image_create(1, 1, box_data);

	res_image_tut_hold = mintg_image_load("res/tut/hold.png", MINT_FILE_LOCAL);
	res_image_tut_move = mintg_image_load("res/tut/move.png", MINT_FILE_LOCAL);
	res_image_tut_done = mintg_image_load("res/tut/done.png", MINT_FILE_LOCAL);

	res_image_player_cursor = mintg_image_load("res/player/cursor.png", MINT_FILE_LOCAL);
	res_image_player_left = mintg_image_load("res/player/left.png", MINT_FILE_LOCAL);
	res_image_player_right = mintg_image_load("res/player/right.png", MINT_FILE_LOCAL);

	res_font_menu = mintg_font_load("res/menu.ttf", MINT_FILE_LOCAL, 30);
	res_font_menu_large = mintg_font_load("res/menu.ttf", MINT_FILE_LOCAL, 40);
	res_font_menu_small = mintg_font_load("res/menu.ttf", MINT_FILE_LOCAL, 20);
}