# 条形码扫描器 | Barcode Scanner

## 简介 | Introduction

* 一款可检测`一维`和`二维`条形码的小工具。
    > A tool being able to detect `one-dimensional` and `two-dimensional` barcode.

* 本项目是探索减少使用安卓手机时所需的一个环节——由于众所周知的原因，
某大国境内绝大多数安卓手机是流氓软件的集中营。
    > This project is one of steps of reaching a goal that
    we can use the Android phones as little as possible. As we all know,
    Android phones are the main carriers of rogue apps in some big country.

* 本项目**仅供学习和演示**！**强烈不建议在生产环境使用，尤其是商用**！
    > This project is **for STUDYINIG and ILLUSTRATION ONLY**!
    **Use in PRODUCTION environment (especially for commercial purpose) is STRONGLY DISCOURAGED**!

## 组成 | Components

### 应用程序（核心） | Application Program (The Core)

* 硬件要求 | Hardware Requirement
    * 台式/笔记本电脑或开发板 | A desktop/laptop computer or a development board
    * 带`USB`、`MIPI-CSI`或`DVP`接口的`摄像头` | A `camera` with `USB`, `MIPI-CSI` or `DVP` interface

* 软件要求 | Software Requirement
    * `Operating System`: `Linux` (`Ubuntu 22.04` is recommended)
    * `Libraries` (with install commands available on `Ubuntu 22.04`):
        * [OpenCV](https://github.com/opencv/opencv.git): `sudo apt install libopencv-dev`
        * [ZXing-C++](https://github.com/zxing-cpp/zxing-cpp.git): `sudo apt install libzxingcore-dev`
        * `Qt`: `sudo apt install qtchooser qt6-base-dev qt6-base-dev-tools`

* 编译 | Compilation
    ````
    $ cd app
    $
    $ make seeds && make init # Only needed at the first time
    $
    $ make # If targeting at the desktop/laptop computer
    $ # Or:
    $ make arm-release # Or "make aarch64-release", if targeting at a development board (usually ARM platform)
    ````

* 使用示例 | Usage Examples
    ````
    $ cd app
    $
    $ ./barcode_scanner.elf -h # Show help info.
    $
    $ ./barcode_scanner.elf -W 1920 -H 1080 # Detect frames captured by camera with bigger resolution but without GUI window
    $
    $ ./barcode_scanner.elf -W 1024 -H 768 --gui # Detect frames captured by camera with smaller resolution and with GUI window
    $
    $ ./barcode_scanner.elf -s pic demo1.jpg demo2.png # Detect images. The --gui is still available but only for the final image
    ````

* `GIF`:

    ![HOW_TO_USE](HOW_TO_USE.gif)

### 内核系统（可选） | Kernel (Optional)

* 若想将应用程序部署在开发板上，则内核需要定制。
此处提供一个样例帮助你更轻松地定制自己的内核，
但切记只能参考不能生搬硬套，因为大家的板不大可能是同一款。
    > If the application program is expected to run on a development board,
    then a customized kernel is needed.
    This is a demo to make your customization easier, but note that
    it's for reference only since your board is possibly not the same as mine.

* 详见`kernel`目录。
    > Enter `kernel` directory to see more details.

### 下位机（可选） | Slave Machine (Optional)

* 若想自制`USB`摄像头，则可参考此样例（仍未开工）。
    > If you want to make an `USB` camera yourself,
    you'll find this part helps (but it's not developed yet).

* 详见`mcu`目录。
    > Enter `mcu` directory to see more details.

## 许可证 | License

`Apache License Version 2.0`

