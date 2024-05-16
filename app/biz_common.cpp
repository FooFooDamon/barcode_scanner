/*
 * Common biz utils.
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

#include "biz_common.hpp"

#include <string>
#include <map>

#include <opencv2/videoio/registry.hpp>

class backend_info_fetcher_c
{
public:
    backend_info_fetcher_c()
    {
        struct
        {
            get_backend_func_t get_func;
            std::map<std::string, std::string> &holder;
        } RULES[] = {
            { cv::videoio_registry::getBackends, this->backends },
            { cv::videoio_registry::getCameraBackends, this->camera_backends },
            { cv::videoio_registry::getStreamBackends, this->stream_backends },
        };

        for (auto &rule : RULES)
        {
            std::string supported_backends(AUTO_BACKEND);

            for (const auto &b : rule.get_func())
            {
                const std::string &name = cv::videoio_registry::getBackendName(b);

                rule.holder[name] = cv::format("%d", b);
                supported_backends.append(",").append(name);
            }

            rule.holder["__ALL__"] = supported_backends;
            rule.holder[AUTO_BACKEND] = "0";
        }
    }

    ~backend_info_fetcher_c()
    {
        backends.clear();
        camera_backends.clear();
        stream_backends.clear();
    }

public:
    std::map<std::string, std::string> backends;
    std::map<std::string, std::string> camera_backends;
    std::map<std::string, std::string> stream_backends;

private:
    typedef std::vector<cv::VideoCaptureAPIs> (*get_backend_func_t)();
};

static backend_info_fetcher_c s_auto_fetcher;

const char* get_backends(void)
{
    return s_auto_fetcher.backends["__ALL__"].c_str();
}

const char* get_camera_backends(void)
{
    return s_auto_fetcher.camera_backends["__ALL__"].c_str();
}

const char* get_stream_backends(void)
{
    return s_auto_fetcher.stream_backends["__ALL__"].c_str();
}

bool is_valid_backend(const char *name)
{
    return s_auto_fetcher.backends.end() != s_auto_fetcher.backends.find(name);
}

bool is_valid_camera_backend(const char *name)
{
    return s_auto_fetcher.camera_backends.end() != s_auto_fetcher.camera_backends.find(name);
}

bool is_valid_stream_backend(const char *name)
{
    return s_auto_fetcher.stream_backends.end() != s_auto_fetcher.stream_backends.find(name);
}

int backend_name_to_code(const char *name)
{
    const auto &iter = s_auto_fetcher.backends.find(name);

    return (s_auto_fetcher.backends.end() == iter) ? -1 : atoi(iter->second.c_str());
}

/*
 * ================
 *   CHANGE LOG
 * ================
 *
 * >>> 2024-05-16, Man Hung-Coeng <udc577@126.com>:
 *  01. Create.
 */

