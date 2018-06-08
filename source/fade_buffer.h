/*
 * fade_buffer.h
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

#ifndef SHADOW_FADE_BUFFER_H_
#define SHADOW_FADE_BUFFER_H_
#include "main.h"

void fade_buffer_init(mintg_image_t* buffer);
void fade_buffer_update(mintg_image_t* buffer, double time);
void fade_buffer_draw(mintg_image_t* buffer, double time);

#endif