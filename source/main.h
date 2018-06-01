/*
 * scene.h
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

#ifndef SHADOW_MAIN_H_
#define SHADOW_MAIN_H_
#include <mint.h>
#include <math.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct interp_t {
	double v;
	double old;
} interp_t;

typedef enum scene_state_t {
	SCENE_INIT,
	SCENE_UPDATE,
	SCENE_DRAW,
	SCENE_EXIT
} scene_state_t;

typedef void (*scene_t)(scene_state_t state, double time);

void interp_init(interp_t* interp, double value);
void interp_update(interp_t* interp);
double interp_value(interp_t* interp, double time);
void scene_set(scene_t scene);

#endif