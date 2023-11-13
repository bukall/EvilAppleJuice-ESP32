# EvilAppleJuice ESP32

苹果手机上的蓝牙垃圾广播！

|iPhone 15s (latest)|Older iPhones|
|-------------------|-------------|
|<video controls width="250" src="https://user-images.githubusercontent.com/6680615/274864225-53ed6d7c-0569-4f22-b55b-bc9973c4bc93.mp4"></video>|<video controls width="250" src="https://user-images.githubusercontent.com/6680615/274864287-c6e871fd-9fdf-4507-ae21-a566beead5cc.mp4"></video>|

基于 [ronaldstoner](https://github.com/ronaldstoner) 的 [AppleJuice repository](https://github.com/ECTO-1A/AppleJuice/blob/e6a61f6a199075f5bb5b1a00768e317571d25bb9/ESP32-Arduino/applejuice.ino)工作.

通过随机化优化，它可以使用单个 ESP32让苹果手机几乎无法使用（只要关闭旧弹窗，就会出现新弹窗）。

工作于:
* iPhone 14 Pro (运行 iOS 16.6.1)
* iPhone 13 Pro (iOS 版本待定)
* iPhone 11 (运行 iOS 16.6.1)
* iPhone X (运行 iOS 14.8 (18H17)) - 仅 "AppleTV 键盘", "TV Color 色彩平衡", "AppleTV 设置", "AppleTV Homekit 设置", "AppleTV 新用户".

不工作于:
* iPhone 4S (运行 iOS 10.3 (14E277))

特殊情况:
* 如果键盘打开/相机打开，似乎不会产生弹窗

### 视频演示

单个 ESP32 vs. iPhone 14 Pro @ iOS 16.6.1

https://github.com/ECTO-1A/AppleJuice/assets/6680615/47466ed6-03c9-43b2-a0d0-aac2e2aaa228

## 值得注意的区别

此实例进行了以下更改：

* 随机源 MAC 地址（包括`BLE_ADDR_TYPE_RANDOM`）
* 随机选择蓝牙广播类型 ([这可能会带来更大的成功](https://github.com/ECTO-1A/AppleJuice/pull/25))
* 随机选择一个可能的设备

每次运行时都会做出这些随机选择（默认每秒重新发送一次）

给定 29 个设备和 3 种广播类型，总共有 87 个独一无二的广播类型（忽略随机源 MAC），其中每秒广播一个。

## 使用

克隆存储库，使用 VS Code 中的 PlatformIO 插件将程序烧写至ESP32
详细以后再填
