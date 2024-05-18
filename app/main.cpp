/*
 * Entry point of this application.
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

#include <stdio.h>

#include <map>

#include "signal_handling.h"

#include "versions.hpp"
#include "cmdline_args.hpp"
#include "biz_test_camera.hpp"
#include "biz_detect_from_images.hpp"
#include "biz_detect_from_camera.hpp"

typedef struct conf_file
{
#ifdef HAS_CONFIG_FILE
    std::string path;
#endif
} conf_file_t;

static int load_config_file(const char *path, conf_file_t &result)
{
#ifdef HAS_CONFIG_FILE
    todo();
#endif
    return 0;
}

static void unload_config_file(conf_file_t &result)
{
#ifdef HAS_CONFIG_FILE
    todo();
#endif
}

int logger_init(const cmd_args_t &args, const conf_file_t &conf)
{
#ifdef HAS_LOGGER
    todo();
#endif
    return 0;
}

void logger_finalize(void)
{
#ifdef HAS_LOGGER
    todo();
#endif
}

static int register_signals(const cmd_args_t &args, const conf_file_t &conf)
{
#ifdef NEED_OS_SIGNALS
    int err = sig_simple_register();

    if (err < 0)
    {
        fprintf(stderr, "sig_simple_register() failed: %s\n", sig_error(err));
        return EXIT_FAILURE;
    }
#endif

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    cmd_args_t parsed_args = parse_cmdline(argc, argv);
    conf_file_t conf;
    std::map<std::string, std::map<std::string, biz_func_t>> biz_handlers = {
        {
            "normal",
            {
                { "camera", BIZ_FUN(detect_from_camera) },
                { "pic", BIZ_FUN(detect_from_images) },
            }
        },
        {
            "test",
            {
                { "camera", BIZ_FUN(test_camera) },
            }
        },
    };
    biz_func_t biz_func = nullptr;
    int ret;

    assert_parsed_args(parsed_args);

    if ((ret = load_config_file(parsed_args.config_file.c_str(), conf)) < 0)
        return -ret;

    if ((ret = logger_init(parsed_args, conf)) < 0)
        goto lbl_unload_conf;

    if ((ret = register_signals(parsed_args, conf)) < 0)
        goto lbl_finalize_log;

    if (nullptr == (biz_func = biz_handlers[parsed_args.biz][parsed_args.source]))
    {
        ret = -ENOTSUP;
        fprintf(stderr, "*** Combination of biz[%s] and source[%s] is not supported yet!\n",
            parsed_args.biz.c_str(), parsed_args.source.c_str());
    }
    else
    {
        ret = biz_func(argc, argv, parsed_args, conf);
    }

lbl_finalize_log:
    logger_finalize();

lbl_unload_conf:
    unload_config_file(conf);

    return abs(ret);
}

/*
 * ================
 *   CHANGE LOG
 * ================
 *
 * >>> 2024-05-03, Man Hung-Coeng <udc577@126.com>:
 *  01. Create.
 *
 * >>> 2024-05-04, Man Hung-Coeng <udc577@126.com>:
 *  01. Implement a demo of detecting from an image file.
 *
 * >>> 2024-05-10, Man Hung-Coeng <udc577@126.com>:
 *  01. Support detecting from multiple image files.
 *  02. Add a normal biz type of detecting from camera (unimplemented),
 *      and a test biz type of capturing frames from camera.
 *
 * >>> 2024-05-18, Man Hung-Coeng <udc577@126.com>:
 *  01. Register several OS signals.
 */

