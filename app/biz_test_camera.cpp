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
#include <opencv2/videoio.hpp>

#include "cmdline_args.hpp"

DECLARE_BIZ_FUN(test_camera)
{
    int cam_id = parsed_args.camera_id;
    cv::VideoCapture vicap;

    if (parsed_args.camera_id < 0)
    {
        for (int i = 0; i < parsed_args.camera_id_max + 1; ++i)
        {
            if (vicap.open(i)) // NOTE: "/dev/videoX" does not work.
                break;
        }
    }
    else
        vicap.open(cam_id);

    if (!vicap.isOpened())
    {
        fprintf(stderr, "*** Failed to open camera!\n");
        return -EXIT_FAILURE;
    }
    printf("Camera backend name: %s\n", vicap.getBackendName().c_str());

    vicap.set(cv::CAP_PROP_FRAME_WIDTH, parsed_args.width);
    vicap.set(cv::CAP_PROP_FRAME_HEIGHT, parsed_args.height);
    vicap.set(cv::CAP_PROP_FPS, parsed_args.fps);
    /*vicap.set(cv::CAP_PROP_FORMAT, -1);
    vicap.set(cv::CAP_PROP_FOURCC, V4L2_PIX_FMT_NV12); // V4L2_PIX_FMT_GREY */

    cv::Mat frame;
    const std::string &WINDOW_NAME = "Camera Test (Press Esc to exit)";
    const int ESC_KEY_CODE = 27;

    cv::namedWindow(WINDOW_NAME);

    while (true)
    {
        vicap >> frame;
        if (frame.empty())
        {
            fprintf(stderr, "*** Failed to capture frame!\n");
            break;
        }

        cv::imshow(WINDOW_NAME, frame);

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
 */

