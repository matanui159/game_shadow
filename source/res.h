/*
 * res.h
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

#ifndef SHADOW_RES_H_
#define SHADOW_RES_H_
#include "main.h"

extern mintg_image_t* res_image_rect;
extern mintg_image_t* res_image_ellipse;

extern mintg_image_t* res_image_arrow;
extern mintg_image_t* res_image_heart;

extern mintg_font_t* res_font_menu;
extern mintg_font_t* res_font_menu_large;
extern mintg_font_t* res_font_menu_small;

void res_init();

#endif