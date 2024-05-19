/*
 * Common biz declarations and definitions.
 *
 * Copyright (c) 2024 Man Hung-Coeng <udc577@126.com>
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

#ifndef __BIZ_COMMON_HPP__
#define __BIZ_COMMON_HPP__

struct cmd_args;
struct conf_file;

#define BIZ_FUN_ARG_LIST                int argc, char **argv, const struct cmd_args &parsed_args, const struct conf_file &conf
#define DECLARE_BIZ_FUN(name)           int name(BIZ_FUN_ARG_LIST)
#define BIZ_FUN(name)                   name

typedef int (*biz_func_t)(BIZ_FUN_ARG_LIST);

extern DECLARE_BIZ_FUN(test_camera);
extern DECLARE_BIZ_FUN(detect_from_camera);
extern DECLARE_BIZ_FUN(detect_from_images);

#define AUTO_BACKEND                    "ANY"

const char* get_backends(void);
const char* get_camera_backends(void);
const char* get_stream_backends(void);

bool is_valid_backend(const char *name);
bool is_valid_camera_backend(const char *name);
bool is_valid_stream_backend(const char *name);

int backend_name_to_code(const char *name);

#define todo()                          fprintf(stderr, __FILE__ ":%d %s(): todo ...\n", __LINE__, __func__)

#endif /* #ifndef __BIZ_COMMON_HPP__ */

/*
 * ================
 *   CHANGE LOG
 * ================
 *
 * >>> 2024-05-10, Man Hung-Coeng <udc577@126.com>:
 *  01. Create.
 *
 * >>> 2024-05-16, Man Hung-Coeng <udc577@126.com>:
 *  01. Add backend macro and function declarations.
 *
 * >>> 2024-05-19, Man Hung-Coeng <udc577@126.com>:
 *  01. Declare all biz functions in this file.
 */

