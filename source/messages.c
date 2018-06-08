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
		"Make it stop"
};
const int messages_normal_count = 5;

const char* const messages_easy[] = {
		"You'll just be annoying",
  		"You're horrible",
		"You're just acting weak",
		"You don't need to drag anyone else down",
		"Just deal with it"
};
const int messages_easy_count = 5;