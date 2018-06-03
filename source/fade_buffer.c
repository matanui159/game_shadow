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

void fade_buffer_init(fade_buffer_t* buffer) {
	int width, height;
	mintg_size(&width, &height);
	buffer->front = mintg_image_create(width, height, NULL);
	buffer->back = mintg_image_create(width, height, NULL);
	buffer->alpha.old = 1;
	buffer->alpha.v = 0.9;
	mintg_image_target(buffer->front);
	mintg_color(0, 0, 0, 0);
	mintg_clear();
	mintg_image_target(NULL);
}

void fade_buffer_update(fade_buffer_t* buffer) {
	mintg_image_t* tmp = buffer->front;
	buffer->front = buffer->back;
	buffer->back = tmp;
	mintg_image_target(buffer->front);
	mintg_color(0, 0, 0, 0);
	mintg_clear();
	mintg_color(1, 1, 1, 0.9);
	mintg_image_draw(buffer->back, NULL);
}

void fade_buffer_finish() {
	mintg_image_target(NULL);
}

void fade_buffer_draw(fade_buffer_t* buffer, double time) {
	mintg_color(1, 1, 1, interp_value(&buffer->alpha, time));
	mintg_image_draw(buffer->front, NULL);
}

void fade_buffer_exit(fade_buffer_t* buffer) {
	mintg_image_destroy(buffer->front);
	mintg_image_destroy(buffer->back);
}