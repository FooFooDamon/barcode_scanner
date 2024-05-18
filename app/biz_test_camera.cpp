/*
 * Biz of camera test.
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

#include "biz_test_camera.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio/registry.hpp>

#include "signal_handling.h"

#include "cmdline_args.hpp"

DECLARE_BIZ_FUN(test_camera)
{
    int cam_id = parsed_args.dev_id;
    cv::VideoCapture vicap;
    cv::VideoCaptureAPIs backend = (cv::VideoCaptureAPIs)backend_name_to_code(parsed_args.backend.c_str());

    //printf("%s\n", cv::getBuildInformation().c_str());
    printf("Specified backend: %s\n", parsed_args.backend.c_str());

    for (int i = cam_id; i < parsed_args.dev_id_max + 1; ++i)
    {
        if (i < 0 || (cam_id >=0 && i != cam_id))
            continue;

        // for backends preferring path string to id integer, V4L2 for example
        std::string path = cv::format("%s%d", parsed_args.dev_prefix.c_str(), i);

        if (((cv::CAP_V4L == backend) ? false : vicap.open(i, backend)) || vicap.open(path, backend) || cam_id >= 0)
            break;
    }

    if (!vicap.isOpened())
    {
        fprintf(stderr, "*** Failed to open camera!\n");
        return -EXIT_FAILURE;
    }
    printf("Actual backend: %s\n", vicap.getBackendName().c_str());

    vicap.set(cv::CAP_PROP_FRAME_WIDTH, parsed_args.width);
    vicap.set(cv::CAP_PROP_FRAME_HEIGHT, parsed_args.height);
    vicap.set(cv::CAP_PROP_FPS, parsed_args.fps);
    if ("auto" != parsed_args.format && "AUTO" != parsed_args.format)
    {
        // vicap.set(cv::CAP_PROP_FORMAT, -1); // TODO: converted from parsed_args.format
        // vicap.set(cv::CAP_PROP_FOURCC, V4L2_PIX_FMT_NV12); // TODO: converted from parsed_args.format
    }

    cv::Mat frame;
    const std::string &WINDOW_NAME = "Camera Test (Press Esc to exit)";
    const int ESC_KEY_CODE = 27;

    cv::namedWindow(WINDOW_NAME);

    while (true)
    {
        if (sig_check_critical_flag())
        {
            fprintf(stderr, "Interrupted by user\n");
            break;
        }

        if (!vicap.read(frame) || frame.empty())
        {
            fprintf(stderr, "*** Failed to capture frame!\n");
            break;
        }

        cv::imshow(WINDOW_NAME, frame);

        // NOTE: The waitKey() is necessary for HighGUI to perform some housekeeping tasks.
        //       Without it, the image won't display and the window might lock up.
        if (cv::waitKey(1) == ESC_KEY_CODE)
            break;
    }

    vicap.release();
    cv::destroyAllWindows();

    return EXIT_SUCCESS;
}

/*
 * ================
 *   CHANGE LOG
 * ================
 *
 * >>> 2024-05-10, Man Hung-Coeng <udc577@126.com>:
 *  01. Create.
 *
 * >>> 2024-05-18, Man Hung-Coeng <udc577@126.com>:
 *  01. Eliminate some runtime errors of V4L2.
 *  02. Check OS signal within biz loop.
 */

