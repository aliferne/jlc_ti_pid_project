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
set NW_EXECUTOR="E:/TI/sysconfig/nw/nw.exe"
:: Path of TI's sysconfig
set SYSCONF="E:/TI/sysconfig"
:: The path to the syscfg of the project
set SYSCONF_FILE="./empty/empty.syscfg"

:: execute GUI
%NW_EXECUTOR% %SYSCONF% %SYSCONF_FILE%
```

接下来powershell里面运行`./launchGui`就行了，之后配置管脚输出啥的和在Keil里面设置是一样的。

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
            "preLaunchTask": "build and flash" // Build and flash before launch
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

实际上只要你给EIDE配好了最新的JLink驱动，那其实也可以直接可视化配置，不用去改`eide.json`，这是我新近发现的。

然后就可以愉快的调试了！
