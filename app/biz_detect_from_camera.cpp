/*
 * Biz of detection from camera.
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

#include "signal_handling.h"

#include <set>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <ZXing/DecodeHints.h>
#include <ZXing/ReadBarcode.h>
#include <ZXing/Result.h>
#include <QtCore/QString>

#include "cmdline_args.hpp"
#include "biz_common.hpp"

#define MAX_FRAME_RATE                  30.0
#define MAX_FRAME_RATE_FOR_GUI          15.0
#define MAX_FRAME_WIDTH                 1920
#define MAX_FRAME_HEIGHT                1080

static bool validate_several_args_again(const cmd_args_t &args)
{
    float max_fps = args.use_gui ? MAX_FRAME_RATE_FOR_GUI : MAX_FRAME_RATE;

    if (args.fps > max_fps && args.fps - max_fps > 0.01)
    {
        fprintf(stderr, "*** Frame rate should not be greater than %f!\n", max_fps);
        return false;
    }

    if (args.width > MAX_FRAME_WIDTH)
    {
        fprintf(stderr, "*** Frame width should not be greater than %d (px)!\n", MAX_FRAME_WIDTH);
        return false;
    }

    if (args.height > MAX_FRAME_HEIGHT)
    {
        fprintf(stderr, "*** Frame height should not be greater than %d (px)!\n", MAX_FRAME_HEIGHT);
        return false;
    }

    return true;
}

static int open_camera(const cmd_args_t &args, cv::VideoCapture &vicap)
{
    int cam_id = args.dev_id;
    cv::VideoCaptureAPIs backend = (cv::VideoCaptureAPIs)backend_name_to_code(args.backend.c_str());

    fprintf(stderr, "Specified backend: %s\n", args.backend.c_str());

    for (int i = cam_id; i < args.dev_id_max + 1; ++i)
    {
        if (i < 0 || (cam_id >= 0 && i != cam_id))
            continue;

        // for backends preferring path string to id integer, V4L2 for example
        // TODO: pipeline string for GStreamer
        std::string path = cv::format("%s%d", args.dev_prefix.c_str(), i);

        if (((/*cv::CAP_ANY == backend || */cv::CAP_V4L == backend) ? false : vicap.open(i, backend))
            || vicap.open(path, backend) || cam_id >= 0)
            break;
    }

    if (!vicap.isOpened())
    {
        fprintf(stderr, "*** Failed to open camera!\n");
        return -EXIT_FAILURE;
    }
    fprintf(stderr, "Actual backend: %s\n", vicap.getBackendName().c_str());

    vicap.set(cv::CAP_PROP_FRAME_WIDTH, args.width);
    vicap.set(cv::CAP_PROP_FRAME_HEIGHT, args.height);
    vicap.set(cv::CAP_PROP_FPS, args.fps);
    if ("auto" != cv::toLowerCase(args.format))
    {
        // vicap.set(cv::CAP_PROP_FORMAT, ...); // TODO: converted from parsed_args.format
        // vicap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc(...)); // TODO: converted from parsed_args.format
    }

    return EXIT_SUCCESS;
}

static ZXing::Result detect_barcode(const cv::Mat &frame)
{
    auto img_view = ZXing::ImageView(frame.data, frame.cols, frame.rows, ZXing::ImageFormat::BGR);
    ZXing::DecodeHints hints;
    auto result = ZXing::ReadBarcode(img_view, hints.setFormats(ZXing::BarcodeFormat::Any));

    return result;
}

static bool do_nothing_to_frame(const std::string &window_name, ZXing::Result &barcode_info, cv::Mat &frame)
{
    return true;
}

static bool mark_and_display_frame(const std::string &window_name, ZXing::Result &barcode_info, cv::Mat &frame)
{
    const int ESC_KEY_CODE = 27;

    if (ZXing::DecodeStatus::NoError == barcode_info.status())
    {
        const auto &pos = barcode_info.position();
        const auto &top_left = pos.topLeft();
        const auto &bottom_right = pos.bottomRight();
        auto center = ZXing::Position::Point(
            std::min(top_left.x, bottom_right.x) + abs(bottom_right.x - top_left.x) / 2,
            std::min(top_left.y, bottom_right.y) + abs(bottom_right.y - top_left.y) / 2
        );
        const cv::Scalar color(0, 0, 255);
        const int thickness = 2;

        for (const auto &p : { top_left, pos.topRight(), pos.bottomLeft(), bottom_right, center })
        {
            cv::drawMarker(frame, cv::Point(p.x, p.y), color, cv::MarkerTypes::MARKER_DIAMOND,
                /* markerSize = */20, thickness);
        }
    }

    cv::imshow(window_name, frame);

    // NOTE: The waitKey() is necessary for HighGUI to perform some housekeeping tasks.
    //       Without it, the image won't display and the window might lock up.
    if (cv::waitKey(1) == ESC_KEY_CODE)
        return false;

    return true;
}

DECLARE_BIZ_FUN(detect_from_camera)
{
    cv::VideoCapture vicap;
    int ret = validate_several_args_again(parsed_args) ? open_camera(parsed_args, vicap) : -EINVAL;

    if (ret < 0)
        return ret;

    cv::Mat frame;
    std::set<std::string> barcode_items;
    const std::string &WINDOW_NAME = "Barcode Scanner (Press Esc to exit)";
    auto display_func = parsed_args.use_gui ? mark_and_display_frame : do_nothing_to_frame;

    fprintf(stderr, "Scanner started, press Ctrl+C whenever you want to stop\n");
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

        auto barcode_result = detect_barcode(frame);
        const std::string &text = QString::fromStdWString(barcode_result.text()).toStdString();

        if (ZXing::DecodeStatus::NoError == barcode_result.status()
            && barcode_items.end() == barcode_items.find(text))
        {
            printf("%s\n", text.c_str());
            barcode_items.insert(std::move(text));
            if (barcode_items.size() > 10000/* FIXME: Specified through command-line. */)
                barcode_items.clear();
        }

        if (!display_func(WINDOW_NAME, barcode_result, frame))
            break;

        // TODO: --oneshot, or --mode=oneshot|forever, or --max-detects=0|1|N
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
 * >>> 2024-05-21, Man Hung-Coeng <udc577@126.com>:
 *  01. Implement a basic version.
 *
 * >>> 2024-11-11, Man Hung-Coeng <udc577@126.com>:
 *  01. Change the window title to Barcode Scanner.
 */

