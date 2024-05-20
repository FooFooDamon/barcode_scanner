/*
 * Biz of detection from image files.
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

#include <iostream>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <ZXing/BarcodeFormat.h>
#include <ZXing/DecodeHints.h>
#include <ZXing/ReadBarcode.h>
#include <ZXing/Result.h>
#include <ZXing/DecodeStatus.h>
#include <QtCore/QString>
#include <QtGui/QScreen>
#include <QtGui/QGuiApplication>

#include "cmdline_args.hpp"
#include "biz_common.hpp"

DECLARE_BIZ_FUN(detect_from_images)
{
    int ret = -EXIT_FAILURE;
    int total = parsed_args.img_files->size();
    int successes = 0;
    bool has_multi_files = (total > 1);
    const char *indent = has_multi_files ? "  " : "";

    for (const std::string &img_file : *parsed_args.img_files)
    {
        cv::Mat image = cv::imread(img_file, cv::IMREAD_COLOR);

        if (image.cols <= 0 || image.rows <= 0)
        {
            fprintf(stderr, "\n*** Image file does not exist, or failed to parse it: %s\n", img_file.c_str());
            ret = -EXIT_FAILURE;
            continue;
        }

        auto img_view = ZXing::ImageView(image.data, image.cols, image.rows, ZXing::ImageFormat::BGR);
        ZXing::DecodeHints hints;
        const auto &result = ZXing::ReadBarcode(img_view, hints.setFormats(ZXing::BarcodeFormat::Any));

        if (ZXing::DecodeStatus::NoError != result.status())
        {
            fprintf(stderr, "\n%s: *** Failed to detect: %s\n", img_file.c_str(), ZXing::ToString(result.status()));
            ret = -EXIT_FAILURE;
            continue;
        }

        ++successes;
        ret = EXIT_SUCCESS;

        if (has_multi_files)
            printf("\n%s:\n", img_file.c_str());

        std::cout << indent << "Type: " << ZXing::ToString(result.format()) << std::endl
            << indent <<"Text: " << QString::fromStdWString(result.text()).toStdString() << std::endl
            << indent <<"Orientation: " << result.orientation() << std::endl
            << indent <<"Error Correction Level: " << QString::fromStdWString(result.ecLevel()).toStdString() << std::endl
            << indent <<"Bits: " << result.numBits() << std::endl;

        if (!parsed_args.use_gui || img_file != parsed_args.img_files->back())
            continue;

        QGuiApplication app(argc, argv);
        const auto &screen_size = app.primaryScreen()->size(); // Will crash if using QGuiApplication::primaryScreen()
        const auto &pos = result.position();
        const auto &top_left = pos.topLeft();
        const auto &bottom_right = pos.bottomRight();
        auto center = ZXing::Position::Point(std::min(top_left.x, bottom_right.x) + abs(bottom_right.x - top_left.x) / 2,
            std::min(top_left.y, bottom_right.y) + abs(bottom_right.y - top_left.y) / 2);
        const cv::Scalar color(0, 0, 255);
        const int thickness = 2;

#if 0
        cv::rectangle(image, cv::Point(top_left.x, top_left.y), cv::Point(bottom_right.x, bottom_right.y),
            color, thickness, cv::LineTypes::LINE_AA);
#endif
        for (const auto &p : { top_left, pos.topRight(), pos.bottomLeft(), bottom_right, center })
        {
            cv::drawMarker(image, cv::Point(p.x, p.y), color, cv::MarkerTypes::MARKER_DIAMOND,
                /* markerSize = */20, thickness);
        }
        if (image.cols > screen_size.width() || image.rows > screen_size.height())
        {
            float ratio = std::max(((float)image.cols) / screen_size.width(), ((float)image.rows) / screen_size.height());

            cv::resize(image, image, cv::Size(), 1.0f / ratio, 1.0f / ratio);
        }
        cv::imshow("Barcode Scanner", image);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    if (has_multi_files)
    {
        std::cout << "\n>>> [Summary] <<<\n"
            << indent << "Total: " << total << std::endl
            << indent << "OK: " << successes << std::endl
            << indent << "Failed: " << total - successes << std::endl
            << std::endl;
    }

    return ret;
}

/*
 * ================
 *   CHANGE LOG
 * ================
 *
 * >>> 2024-05-10, Man Hung-Coeng <udc577@126.com>:
 *  01. Create.
 *
 * >>> 2024-05-20, Man Hung-Coeng <udc577@126.com>:
 *  01. Improve some error messages.
 */

