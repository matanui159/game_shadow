/*
 * player.h
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

#ifndef SHADOW_PLAYER_H_
#define SHADOW_PLAYER_H_
#include "main.h"

typedef struct player_part_t {
	interp_t x;
	interp_t y;
	interp_t angle;
	double vx;
	double vy;
	double vangle;
} player_part_t;

typedef struct player_t {
	_Bool alive;
	double time;
	interp_t x;
	interp_t y;
	interp_t scale;
	player_part_t left;
	player_part_t right;
} player_t;

extern player_t player_qld;
extern player_t player_nsw;
extern player_t player_act;

void player_init();
_Bool player_update(double time);
void player_draw(_Bool game, double time);
void player_kill(player_t* player);
player_t* player_active();

#endif