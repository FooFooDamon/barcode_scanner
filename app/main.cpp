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

#include <string>
#include <algorithm>
#include <iostream>

#include <opencv2/core/types.hpp>
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

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " /path/to/barcode/image" << std::endl;
        return EXIT_FAILURE;
    }

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);
    auto img_view = ZXing::ImageView(image.data, image.cols, image.rows, ZXing::ImageFormat::BGR);
    ZXing::DecodeHints hints;
    const auto &result = ZXing::ReadBarcode(img_view, hints.setFormats(ZXing::BarcodeFormat::Any));

    if (ZXing::DecodeStatus::NoError != result.status())
    {
        std::cerr << "Failed to detect: " << ZXing::ToString(result.status()) << std::endl;
        return EXIT_FAILURE;
    }

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
    cv::imshow("Barcode Test", image);
    cv::waitKey(0);
    std::cout << "Type: " << ZXing::ToString(result.format()) << std::endl
        << "Text: " << QString::fromStdWString(result.text()).toStdString() << std::endl
        << "Orientation: " << result.orientation() << std::endl
        << "Error Correction Level: " << QString::fromStdWString(result.ecLevel()).toStdString() << std::endl
        << "Bits: " << result.numBits() << std::endl;

    return EXIT_SUCCESS;
}

/*
 * ================
 *   CHANGE LOG
 * ================
 *
 * >>> 2024-05-03, Man Hung-Coeng <udc577@126.com>:
 *  01. Create.
 */

