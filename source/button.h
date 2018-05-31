/*
 * button.h
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

#ifndef SHADOW_BUTTON_H_
#define SHADOW_BUTTON_H_
#include "main.h"

typedef struct button_t {
	const char* text;
	const char* info;
	double x;
	double y;
	interp_t scale;
	double time;
	_Bool hover;
	_Bool state;
} button_t;

_Bool button_update(button_t* button, double time);
void button_draw(button_t* button, double time);

#endif