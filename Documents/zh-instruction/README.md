# 硬件构建手册

这个部分主要记录硬件的搭建过程，以及在搭建过程中可能遇到的问题和解决方法。

## 传感器

本项目的传感器如下表所示：

| 项目  | 传感器名称         | 型号    | 描述                         | 数量 (pcs)     | 规格书      |
|:----:|------------------|:--------:|-------------------------------|:---------:|:-----------:|
|  1   | 摄像头           | WebCam   | 采集面部图像和照片              | 1         |             |
|  2   | 脉搏传感器       | MAX30102 | 采集ECG心电图、脉搏、血氧数据    | 1         |             |
|  3   | 湿度、温度传感器 | AHT20    | 采集手指的湿度和温度变化         | 1         |             |

## 传感器与 Raspberry Pi 连接说明

<p align="center">
  <img src="../pictures/sensors_wire_connection.png" width="500"/>
</p>

该电路连接图通过 *Frizing* 绘制，你可以按上述说明连接电路，确保所有传感器通过正确的线路连接到 Raspberry Pi。如果有多个传感器，只需在 I2C 总线（SDA 和 SCL）扩展连接即可。*Frizing* 绘制的电路图文件已经上传到项目仓库中，名称为 `sensors_connection_instruction.fzz` 你可以使用 *Frizing* 软件进行查看。

### Raspberry Pi
 - **GND**（地线）：黑色线连接到 Raspberry Pi 的 **GND** 引脚
 - **VIN**（电源输入）：红色线连接到 Raspberry Pi 的 **5V** 引脚或 **3.3V** 引脚，**注意**：具体取决于传感器的电压需求（通常 5V 或 3.3V）
 - **SDA**（数据线）：橙色线连接到 Raspberry Pi 的 **GPIO 2**（SDA）
 - **SCL**（时钟线）：蓝色线连接到 Raspberry Pi 的 **GPIO 3**（SCL）

### 传感器（多个 I2C 传感器）
 - 每个传感器的 **GND** 引脚连接到 Raspberry Pi 的 **GND** 引脚（黑色线）
 - 每个传感器的 **VIN** 引脚连接到 Raspberry Pi 的 **5V** 或 **3.3V** 引脚（红色线），确保电压一致
 - 每个传感器的 **SDA** 引脚连接到 **SDA** 总线上，即 Raspberry Pi 的 **GPIO 2**（橙色线）
 - 每个传感器的 **SCL** 引脚连接到 **SCL** 总线上，即 Raspberry Pi 的 **GPIO 3**（蓝色线）

> [!NOTE]  
> 1. **I2C 地址冲突**：如果连接多个 I2C 设备，请确保它们的地址不冲突。如果有冲突，可以通过设置传感器的地址（通过跳线或软件配置）来解决。
> 2. **电压匹配**：确保传感器的电源电压与 Raspberry Pi 提供的电压匹配，通常传感器支持 3.3V 或 5V，具体取决于传感器型号。
> 3. **I2C 引脚**：确保使用 Raspberry Pi 上的 **GPIO 2**（SDA）和 **GPIO 3**（SCL）作为数据传输线路。

## 传感器开发指南

该项目使用了 `pigpio` 库，为 Raspberry Pi 提供高效的 GPIO 控制和与外部传感器的交互。`pigpio` 是一个在 Raspberry Pi 上运行的 C 语言库，它提供了强大的 GPIO 控制功能，其中包括数字输入、输出、PWM、I2C、SPI 和串行通信等。本项目使用 `pigpio` 库的原因是因为该库能够为传感器提供精准时间控制，并且支持多线程操作，可以在并发环境中稳定运行，适用于复杂的传感器数据采集和处理任务。

如果还未安装 `pigpio` 库，请通过以下代码进行安装。

```bash
sudo apt install pigpio
```

通过运行以下命令来验证 `pigpio` 是否正确安装.

```bash
pigpiod -v
```

<br>为了在 CMake 中正确地链接 `pigpio` 库，你需要按照以下步骤对 CMakeLists.txt 进行配置

```bash
# 设置 pigpio 库的路径
find_library(PIGPIO_LIB pigpio)

# 链接 pigpio 库
target_link_libraries(Project_Name ${PIGPIO_LIB})
```

<br>下面给出一个如何使用 `pigpio` 库调用 I2C 的简单例子：

```C++
#include <pigpio.h>

// 初始化 pigpio 库
if (gpioInitialise() < 0) {
    std::cerr << "pigpio initialization failed!" << std::endl;
    return -1; // 初始化失败，退出程序
}

int i2cHandle = i2cOpen(1, 0x40, 0);  // 打开 I2C 总线 1，设备地址为 0x40
if (i2cHandle < 0) {
    std::cerr << "Failed to open I2C bus!" << std::endl;
    return -1;
}

char data[] = {0x01, 0xFF};  // 发送数据，给 0x01 寄存器写入 0xFF 值
i2cWrite(i2cHandle, data, sizeof(data));

char buffer[10];
int readBytes = i2cRead(i2cHandle, buffer, sizeof(buffer)); // 读取数据
if (readBytes >= 0) {
    std::cout << "I2C read data: ";
    for (int i = 0; i < readBytes; i++) {
        std::cout << (int)buffer[i] << " ";
    }
    std::cout << std::endl;
}

gpioTerminate();  // 关闭 pigpio 库
```

### 脉搏传感器

### 湿度、温度传感器

### 摄像头