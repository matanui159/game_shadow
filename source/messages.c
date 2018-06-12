/*
 * messages.c
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
 
#include "messages.h"

const char* const messages_normal[] = {
		"I can't go back there",
		"I've had enough",
		"This is too much",
		"I have to get out",
		"Make it stop",
		"I can't deal with this"
};
const int messages_normal_count = 6;

const char* const messages_easy[] = {
		"You'll just be annoying",
  		"You're horrible",
		"You're just acting weak",
		"You'll just drag people down",
		"Just deal with it",
		"Be a man"
};
const int messages_easy_count = 6;

const char* const messages_end[] = {
		"Note to self",
		"I am not being annoying",
		"I am not being weak",
		"I am not bringing others down",
		"Sometimes people need help",
		"It is three simple words",
		"\"I'm not ok\""
};
const int messages_end_count = 7;