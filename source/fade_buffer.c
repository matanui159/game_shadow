/*
 * fade_buffer.c
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
 
#include "fade_buffer.h"
#include "res.h"
#include "hidden.h"

void fade_buffer_init(mintg_image_t* buffer) {
	mintg_image_target(buffer);
	mintg_color(0, 0, 0, 0);
	mintg_clear();
	mintg_image_target(NULL);
}

void fade_buffer_update(mintg_image_t* buffer) {
	mintg_image_target(buffer);
	mint__lib_glBlendFunc(GL_ONE, GL_ONE);
	mint__lib_glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	mintg_push();
	mintg_scale(mintg_width(), mintg_height());
	mintg_color(0.5, 0.5, 0.5, 0.05);
	mintg_image_draw(res_image_rect, NULL);
	mintg_pop();
	mintg__draw_flush();
	mint__lib_glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	mint__lib_glBlendEquation(GL_FUNC_ADD);
}