/*
 * Common biz definitions.
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

#define todo()                          fprintf(stderr, __FILE__ ":%d %s(): todo ...\n", __LINE__, __func__)

#endif /* #ifndef __BIZ_COMMON_HPP__ */

/*
 * ================
 *   CHANGE LOG
 * ================
 *
 * >>> 2024-05-10, Man Hung-Coeng <udc577@126.com>:
 *  01. Create.
 */

