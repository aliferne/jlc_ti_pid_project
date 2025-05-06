# 前言

开发的是TI的MSPM0G3507芯片，立创开发板，EIDE配置较为麻烦，跟着下面走一遍。

# 开发环境搭建

见[此文](https://wiki.lckfb.com/zh-hans/tmx-mspm0g3507/keil-beginner/install.html)

不过这个教的是Keil上开发，我们要在Vscode上开发

需要修改几处地方

为了确保到时候keil上也能用，需要先按照上面文档的步骤走一遍，然后我们来专门说说怎么配置Vscode，你可以选择EIDE插件，然后修改构建配置中的构建器选项，也可以直接修改 `eide.json`，因为有个步骤需要两个都修改，我们索性就只讲 `eide.json` 的改法：

```json
"beforeBuildTasks": [
    {
        "name": "Linking syscfg",
        "command": "cd \"${projectRoot}\" && cmd.exe /C \"${projectRoot}\\syscfg.bat '${projectRoot}\\empty' empty.syscfg\"",
        "disable": false,
        "abortAfterFailed": true,
        "stopBuildAfterFailed": true
    }
],
```

修改里面的 `command` 属性即可，由于我这个文件夹的工作目录为 `TI-PID`，且 Keil 中表示项目根目录的 `$P` 不适用于 EIDE，所以需要改为 `${projectRoot}`，然后经过我debug发现`syscfg.bat`传入的参数是`empty`这个目录以及里面的`empty.syscfg`，所以需要配置为 `'${projectRoot}\\empty'` 和 `empty.syscfg`。
而你也可以配置一下`name`，从而使得build时显示此名称。

linker里面的misc-controls修改为此路径：

```json
"linker": {
    "output-format": "elf",
    "misc-controls": "source/ti/driverlib/lib/keil/m0p/mspm0g1x0x_g3x0x/driverlib.a"
}
```

（因为我们的 `${projectRoot}` 的子文件夹里面有`source`，不需要像keil里面配置那样子退回到父级的父级文件夹）

然后是GUI界面的配置：

```batch
@echo off

:: Path of TI's nw.exe
set NW_EXECUTOR="E:\TI\sysconfig\nw\nw.exe"
:: Path of TI's sysconfig
set SYSCONF="E:\TI\sysconfig"
:: The path to the syscfg of the project
set SYSCONF_FILE="E:\Interests\Code\micro_chip\jlc_training_course\course_1_pid_pcb_ti_project\ti-pid\empty\empty.syscfg"

:: execute GUI
%NW_EXECUTOR% %SYSCONF% %SYSCONF_FILE%
```

接下来powershell里面运行`./launchGui.bat`就行了，之后配置管脚输出啥的和在Keil里面设置是一样的。

有个究极大坑！！！

**EIDE自带的JLink工具包是V6.90版本的**（它的实用工具里面只有V6.90版本的Jlink），要去 `${userRoot}/.eide/tools/`目录下把JLink删掉！然后再装最新版的JLink，接下来去设置里面配置下JLink的路径就行了。不然V6.90的不支持新版芯片，各种烧录问题能把你脑子干烧🙂。

我这里`${userRoot}`等价于`C:/Users/a3230`

最后我选择装到`C:/Users/a3230/.eide/tools`去，然后Jlink目录就成了JLink_V830（这是JLink驱动自己设置的）

Cortex-Debug插件需要自己去设置一下，首先是在 `settings.json` 里面设置一些工具依赖的路径，比如armToolChain（gcc-arm-none-eabi啥的），这个自己去找教程安装，Keil不提供的。

然后在.vscode文件夹里面创建个`launch.json`文件，内容如下：

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug (gdbserver)",
            "cwd": "${workspaceFolder}",
            "executable": "${workspaceFolder}/build/empty_LP_MSPM0G3507_nortos_keil/ti-pid.elf",    // Path to the elf file
            // Waring! HEX file can't be assigned for debug!
            "request": "launch",
            "type": "cortex-debug",
            "servertype": "jlink",
            "device": "MSPM0G3507",
            "serverpath": "C:/Users/a3230/.eide/tools/JLink_V830/JLinkGDBServerCL.exe", // Path to JLinkGDBServer
            "interface": "swd", // Use SWD interface
            // NOTE: 建议先烧录再Debug，这个插件对双任务的支持不算很好，build完没flash就开始debug了
            // "preLaunchTask": "build and flash" // Build and flash before launch
        }
    ]
}
```

然后最好再次设置一下`.eide.json`的一些信息，你可以先在EIDE插件里面随便选一个芯片型号，然后再去文件里面改成这块芯片的型号（因为EIDE可能更新频率不太经常，没办法涵盖所有芯片）

```json
"cpuInfo": {
    "vendor": "TI", // manufactor
    "cpuName": "MSPM0G3507" // chip name
},
```

**配置完之后会更方便一些，就不用再每次都设置一下芯片型号了**（烧录时JLink驱动会打开，让你设置芯片型号）。

然后就可以愉快的调试了！

# 简单的延时函数

延时函数是编程中常用的函数，用于让程序暂停执行一段时间。在嵌入式系统中，延时函数通常用于等待硬件设备完成某个操作，或者实现用户交互中的延迟效果。

# 点亮LED

点LED原理还需要废话吗？制造电势差。

开发板PB22引脚接了LED的正极（小样输出能力还挺彪悍）。

工程已经搞定了，powershell切到根目录跑一下 `./launchGui.bat`，然后配置下GPIO啥的，再把 `ti_msp_dl_config.h` 头文件和 `ti_msp_dl_config.c` 源文件的代码用GUI生成的文件覆盖即可，具体的看立创文档。

## 控制引脚状态的dl库函数

在`dl_gpio.h`库函数文件中，有三个函数可以控制引脚状态

**相关函数说明**

> `__STATIC_INLINE void DL_GPIO_setPins(GPIO_Regs* gpio, uint32_t pins)`

该函数**为控制引脚输出高电平**，按照本例程中LED的引脚，则可以写为 `DL_GPIO_setPins(LED1_PORT,LED1_PIN_22_PIN);`

> `__STATIC_INLINE void DL_GPIO_clearPins(GPIO_Regs* gpio, uint32_t pins)`

该函数**为控制引脚输出低电平**，按照本例程中LED的引脚，则可以写为 `DL_GPIO_clearPins(LED1_PORT,LED1_PIN_22_PIN);`

> `__STATIC_INLINE void DL_GPIO_togglePins(GPIO_Regs* gpio, uint32_t pins)`

该函数**为控制引脚的电平翻转**，如果原来是高电平则变为低电平，如果原来是低电平则变为高电平。按照本例程中LED的引脚，则可以写为`DL_GPIO_togglePins(LED1_PORT,LED1_PIN_22_PIN);`

```c
#include "ti_msp_dl_config.h"
#include "type.h"

void delay(uint ms)
{
    uint i, j;
    /*
     * 下面的嵌套循环次数是由主控频率和编译器生成的指令周期大致计算出来的
     * 实际开发需要通过测试调整来达到所需时间。
     */
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 8000; j++) {
            __asm__("nop"); // NOP指令，空操作
        }
    }
}

int main(void)
{
    SYSCFG_DL_init();   // 这一步会初始化所有的必要系统设置

    while (1) {
        DL_GPIO_clearPins(GREEN_PORT, GREEN_PIN_22_PIN);
        delay(1000);    // 延时大体1s
        DL_GPIO_setPins(GREEN_PORT, GREEN_PIN_22_PIN);
        delay(1000);

        /*
            * 或写成
            DL_GPIO_togglePins(GREEN_PORT, GREEN_PIN_22_PIN);
            delay(1000);
         */
    }
}
```

- 引脚名称命名规则：`GUI设置的引脚名_PORT`
- 引脚编号命名规则：`GUI设置的引脚名_GUI设置的引脚编号_PIN`

# 系统延时

## 延时的作用

在MSPM0或其他微控制器的编程中，延时被广泛使用，主要有以下一些原因：

1. **处理硬件**：**许多硬件都需要一些时间来响应某个命令**。例如，如果在一个程序中你启动一个电动机然后立即检查其状态，你可能会得到一个错误的读数，因为电动机可能还没有足够的时间开始旋转。此时你需要使用 `delay()` 函数让系统等待一段时间，使得电动机有足够的时间响应。
2. **用户交互**：我们常常**需要在用户交互中实现延迟效果**。例如，在蜂鸣器播放音乐时，音符之间需要一段沉默的时间。或者，在闪烁LED灯的情况下，"开"和"关"状态之间需要延时以控制闪烁的速度。
3. **节省能源**：在一些应用中，比如电池供电的系统，如果不在需要的时候长期保持系统的高速运转，那么电池的寿命会大大缩短。在此情况下，我们可以让系统在一段时间后进入待机或低功耗模式，直到下一个理周期到来。
4. **定时操作**：在许多项目中，我们常常需要实现一些特定时间点的操作。例如，在自动灌溉系统中，我们可能需要在每天的特定时间点进行灌溉。在间隔测量中，我们可能每隔一段时间采集一次数据。

尽管**延时函数在很多情况下非常有用，但也需要注意其阻塞性质**。

**过度依赖阻塞延时可能会导致程序对其他事件的响应不及时**。为了更好的在MSPM0上进行多任务编程，我们**还可以学习一些非阻塞延时的编程技术**。

> ❓什么是阻塞延时？
> **阻塞延时是在程序执行过程中，当某个操作或函数需要一定时间才能完成时，程序会暂停执行直至该操作完成，这段时间程序被阻塞了**。阻塞延时可能会导致程序运行速度变慢或出现**假死**现象。 举个例子，假设你想要煮开水来泡茶。通常情况下，你会将水壶放在炉灶上加热，等待水烧开后才能用。在这个过程中，存在阻塞延时。 当你将水壶放在火上时，程序可以看作是“等待”水烧开的操作。在这个等待过程中，你不能立即得到热水来泡茶，需要耐心等待水煮沸。期间，你可能无法做他与烧水无关的事情，因为你需要留意水壶，并等待时机。即便家里着火了，你也还是在等待烧水。

## 实现延时的方式

有很多，但只介绍三种：`空代码延时`，`TI工程自带的延时`，`使用滴答定时器`

### 空代码延时

又名NOP延时，就是个机器指令，表示不执行任何操作，但是会消耗时钟周期。

应用场景有以下几个：

1. **时序延时**，在SPI或者I2C通信中会用到NOP，在某些硬件相关的编程中（E2PROM等），精确的时序控制非常重要。通过插入NOP指令，可以确保其他硬件操作有足够的时间来完成。
2. **调试**：在调试程序时，开发者可能会插入NOP指令来暂时“冻结”程序，以便观察程序的某个特定状态。
3. 占位符：在开发过程中，开发者可能暂时不知道要在某个位置放置什么代码，此时可以用NOP作为占位符（这个太难绷了，还不如Python的`pass`呢）

简单空延时如下：

```c
void delay(void)
{
    for (int i = 0; i < 1000; i++) {
        // 不执行任何操作
        // 你也可以放一个 __asm__("nop");
    }
}
```

优点是能用，缺点是不准。

### TI工程自带的延时

提供的函数API为`delay_cycles(cycles)`，一个可以根据主控频率进行计算时间的延时函数。

源代码：

```c
/**
 * @brief Alias for DL_Common_delayCycles
 */
#define delay_cycles(cycles) DL_Common_delayCycles(cycles)

/**
 * @brief   Consumes the number of CPU cycles specified.
 *
 * @param[in] cycles  Floor number of cycles to delay.
 *                    Specifying zero will result in the maximum
 *                    possible delay. Note that guarantees at least
 *                    this number of cycles will be delayed,
 *                    not that exactly this number of cycles will be
 *                    delayed. If a more precise number of cycle delay value
 *                    is needed, GPTimer is recommended.
 *
 *                    Typical variance from this function is 10 cycles or
 *                    less assuming that the function is located in flash and
 *                    that caching is enabled. Disabling caching may result in
 *                    wait-states when fetching from flash.
 *                    Other variance occurs due:
 *                      - Amount of register stacking/unstacking around API entry/exit
 *                      - Value of cycles relative to 4-cycle loop counter
 *                      - Placement of code on a 2- or 4-byte aligned boundary
 */
void DL_Common_delayCycles(uint32_t cycles);

void DL_Common_delayCycles(uint32_t cycles)
{
    /* this is a scratch register for the compiler to use */
    uint32_t scratch;

    /* There will be a 2 cycle delay here to fetch & decode instructions
     * if branch and linking to this function */

    /* Subtract 2 net cycles for constant offset: +2 cycles for entry jump,
     * +2 cycles for exit, -1 cycle for a shorter loop cycle on the last loop,
     * -1 for this instruction */

    __asm volatile(
#ifdef __GNUC__
        ".syntax unified\n\t"
#endif
        "SUBS %0, %[numCycles], #2; \n"
        "%=: \n\t"
        "SUBS %0, %0, #4; \n\t"
        "NOP; \n\t"
        "BHS  %=b;" /* branches back to the label defined above if number > 0 */
        /* Return: 2 cycles */
        : "=&r"(scratch)
        : [ numCycles ] "r"(cycles));
}
```

**通过汇编语言的NOP指令，通过主频计算执行代码的时间周期，去实现的延时。而时间周期TI团队也已经为大家计算好了，可以直接在工程中调用`delay_cycles(主频hz);`函数即可**。

实现一秒延时： 

```c
// 我们工程模板设为32Mhz
// 有修改则另说
delay_cycles(32000000);
``` 

实现一毫秒延时：

```c
// 同上
delay_cycles(32000000/1000);
```

### 使用滴答定时器

**基本所有arm内核芯片都有的功能，可以精准延时**

#### 什么是滴答定时器

**`SysTick`定时器可用作标准的下行计数器，是一个24位向下计数器，有自动重新装载能力，可屏蔽系统中断发生器**。

Cortex-M0处理器内部包含了一个简单的定时器，所有基于M0内核的控制器都带有`SysTick`定时器,这样就**方便了程序在不同的器件之间的移植**。**`SysTick`定时器可用于操作系统，提供系统必要的时钟节拍，为操作系统的任务调度提供一个有节奏的“心跳”**。正因为所有的M0内核的芯片都有`SysTick`定时器，这在移植的时候不像普通定时器那样难以移植。

学51的时候学过了，本质就是个计数器，但这里直接就能重装了，不需要像51一样配几个寄存器才能计时。

RCU 通过 `MCLK` 作为 Cortex 系统定时器（`SysTick`）的外部时钟，即使用`MCLK`计时，MCLK默认为32MHz。通过对 `SysTick` 控制和状态寄存器的设置，即可控制或读取。关于系统时钟的介绍可参考用户手册的第128页。

**`SysTick`定时器设定初值并使能之后，每经过1个系统时钟周期，计数值就减1，减到0时，SysTick计数器自动重新装载初值并继续计数，同时内部的COUNTFLAG标志位被置位，触发中断**（前提是开启中断）。

#### 配置 `SysTick`

[教程](https://wiki.lckfb.com/zh-hans/tmx-mspm0g3507/keil-beginner/delay.html#_4-3-2-%E5%BC%80%E5%90%AFsysconfig%E9%85%8D%E7%BD%AE%E5%B7%A5%E5%85%B7)

很简单，就是去SysTick那一栏开个初始化定时器周期，然后设置周期计数值，然后设置下中断优先级（可以不设置），和开启定时器并启动。

设置周期为32000就可以做到定时1ms。

然后一个简单的使用代码大体如此：

```c
#include "ti_msp_dl_config.h"

volatile uint32_t delay_times = 0;

// 和SysTick一起实现精准ms计时
void delay_ms(uint32_t ms) {
    delay_times = ms;
    while(delay_times); // > 0 时卡死在这里
}

int main(void) {
    SYSCFG_DL_init();

    while (1) {
        // 暂时为空循环
    }
}

// 滴答定时器中断服务函数
void SysTick_Handler(void) {
    if (delay_times) {
        delay_times--;
    }
}
```

> 🔅C语言扩展
> **在C语言中，`volatile` 是一个关键字，用来告诉编译器不要对它所修饰的变量做任何优化，因为这个变量的值可能会随时被意想不到的因素改变，比如硬件中断、多线程操作等**。`volatile` 告诉编译器不要将对这个变量的读写操作优化掉，每次访问都要从变量地址中读取或写入。在多线程或与硬件相关的编程中，**`volatile` 经常用来声明那些程序之外其他实体可能会修改的变量，以确保每次访问都能获取最新的值，避免编译器优化导致的意想不到的问题**。

上面的代码 `SysTick_Handler` 和我们之前学51的时候写的 `void Interrupt_ISR() interrupt 1` 是一样的，就是在系统触发中断之后就会进入这个函数，然后执行函数内的代码。简单点的逻辑就是，先设置好一个初始值（ms），然后定时1ms，每次都去减这个值，最后就定时完毕。

#### LED灯闪烁实验

相当简单，间隔1s闪烁即可

```c
#include "ti_msp_dl_config.h"
#include "type.h"

// 和提供的例程略有不同
// volatile 依然不能少
// 但为了避免 delay_times 定义为 uint时
// 减到0后还往下减，导致未定义行为
// 这里设置为有符号的int
volatile int delay_times = 0;

void delay_ms(int ms)
{
    delay_times = ms;
    while (delay_times >= 0);
}

int main(void)
{
    SYSCFG_DL_init();

    while (1) {
        // 每1s反转一次电平
        DL_GPIO_togglePins(LED1_PORT, LED1_PIN_22_PIN);
        delay_ms(1000);
    }
}

void SysTick_Handler(void)
{
    if (delay_times >= 0) {
        delay_times--;
    } else {
        delay_times = 0; // 小于0则归0
    }
}
```

# 按键

## 结构、原理

独立按键的主要结构组成包括：按钮、外壳、弹簧、触点、导电片和引脚。**当按下按键时，导电片触碰到触点，从而形成一个闭合电路**。

独立按键原理主要是基于机械触点和电气触点之间的关系。**当按键未被按下时，通常触点是分开的，电路是断开的**。当用户**按下按键时，触点在弹簧和导电片的作用下形成闭合，这时电路连通，微控制器能够读取到该按键触发的信号**。

## 消抖

按键消抖分为硬件消抖和软件消抖

软件消抖就是加延时（延后检测）和多次检测，确保确实是按下的。

板子上三个按键，RST，BSL，B21

- RST是芯片复位功能按键，不能通过软件的方式去控制该按键
- BSL是特殊按键，它接到了PA18引脚，该引脚是用于进入升级模式进行串口下载等操作的特殊引脚，不建议使用
- B21按键就是用户的可以操控的按键，将PB21引脚接入了一个按键，通过按键接到GND

[原理图](https://wiki.lckfb.com/zh-hans/tmx-mspm0g3507/keil-beginner/key.html#_5-6-%E7%8B%AC%E7%AB%8B%E6%8C%89%E9%94%AE%E5%8E%9F%E7%90%86%E5%9B%BE)

B21由于内部上拉电阻的存在，是高电平的，按键按下后会变成低电平。

> `PA18`引脚为MSPM0G3507芯片的BSL引脚，**用于启动芯片的BootLoader模式**，非特殊情况，**请确保在给开发板通电时，PA18引脚为低电平，否则将会启动BootLoader模式**，导致**无法正常执行之前所下载的代码**。

## 按键点灯实验

在打开GUI界面配置时，需要注意把PB21配置成输入模式（检测外部电压变化）和上拉电阻（确保高电平）

我们要借助函数 `DL_GPIO_readPins`，它的原型是 

`__STATIC_INLINE uint32_t DL_GPIO_readPins(GPIO_Regs* gpio, uint32_t pins)`

也就是说它会返回一个 `uint32_t`（`static`的），也就是对应引脚的状态位，所以需要查数据手册，比如以PA18为例。当PA18引脚检测到低电平时，返回的值是0；当PA18引脚检测到高电平时，对应的32位寄存器中PA18的位将会被置1，返回的值为0x40000，换为2进制就是：100 0000 0000 0000 0000。

因此，我们有种简单的方法——只要判断引脚电平是不是为0就行，这样子就不用去找数据手册数数了。

最简单的示例如下：

```c
#include "ti_msp_dl_config.h"

int main() {
    // static可加可不加、你也可以使用volatile来避免编译器优化
    uint32_t last_state = 0;
    while (1) {
        last_state = DL_GPIO_readPins(BTN_PORT, BTN_PIN_21_PIN);
        if (last_state == 0) {
            // 按下就亮，松开就灭
            DL_GPIO_setPins(LED1_PORT, LED1_PIN_22_PIN);
        } else {
            DL_GPIO_clearPins(LED1_PORT, LED1_PIN_22_PIN);
        }
    }
}
```

