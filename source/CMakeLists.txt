#[[
 * CMakeLists.txt
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
]]

add_executable(shadow "${CMAKE_CURRENT_SOURCE_DIR}/main.c")
set_target_properties(shadow PROPERTIES C_STANDARD 99)
target_compile_options(shadow PRIVATE -Wall -Wpedantic)
target_link_libraries(shadow PRIVATE mint)

if (CMAKE_BUILD_TYPE STREQUAL Release AND WIN32)
	set_target_properties(shadow PROPERTIES LINK_FLAGS -mwindows)
endif()

target_sources(shadow PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/res.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/fade_buffer.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/button.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/player.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/shadow.c"
)

add_subdirectory(scene)