# IMX6ULL裸机程序DEMO（使用NXP库）

## 开发环境（Windows平台）
* 操作系统：Windows11
* 开发环境：VSCode
* 编译器：gcc-linaro-7.5.0-2019.12-i686-mingw32_arm-eabi


## SD卡烧录方法（Windows平台）
* 首先下载安装SDK_2.2_MCIM6ULL
  * 本文安装在C:\nxp\SDK_2.2_MCIM6ULL路径下
* 安装镜像烧写工具软件balenaEtcher
* 将编译生成的bin文件拷贝至\SDK_2.2_MCIM6ULL\tools\imgutil\evkmcimx6ull路径下并改名为sdk20-app.bin
* 在mingw32 shell中运行上述路径下的mkimage.sh脚本
    * 借用Git的bash，运行C:\Program Files\Git\git-bash.exe
    * 切换至脚本路径：`cd c/nxp/SDK_2.2_MCIM6ULL/tools/imgutil/evkmcimx6ull/`
    * 运行脚本：`./mkimage.sh sd`
    * 得到镜像文件sdk20-app.img
* 使用balenaEtcher软件将镜像文件烧录至TF卡

