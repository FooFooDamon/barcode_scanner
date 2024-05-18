/*
 * APIs for parsing and validating command-line arguments.
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

#ifndef __CMDLINE_ARGS_HPP__
#define __CMDLINE_ARGS_HPP__

#include <string>
#include <vector>

typedef struct cmd_args
{
    std::vector<std::string> orphan_args;
    std::string biz;
    std::string config_file;
#ifdef HAS_LOGGER
    std::string log_file;
    std::string log_level;
#else
    bool verbose;
    bool debug;
#endif
    std::string source;
    std::string format;
    std::string backend;
    std::string dev_prefix;
    std::vector<std::string> *img_files;
    float fps;
    int dev_id;
    int dev_id_max;
    int width;
    int height;
    int detect_threads;
    bool use_gui;
} cmd_args_t;

cmd_args_t parse_cmdline(int argc, char **argv);

void assert_parsed_args(const cmd_args_t &args);

#endif /* #ifndef __CMDLINE_ARGS_HPP__ */

/*
 * ================
 *   CHANGE LOG
 * ================
 *
 * >>> 2024-05-10, Man Hung-Coeng <udc577@126.com>:
 *  01. Create.
 *
 * >>> 2024-05-18, Man Hung-Coeng <udc577@126.com>:
 *  01. struct cmd_args: Rename camera_id* to dev_id*;
 *      add dev_prefix, detect_threads and backend.
 */

