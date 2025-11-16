# HyperGpu

## Features

`资源类型`

|       类型       | OpenGL | Vulkan |
| :--------------: | :----: | :----: |
|    `Image2D`     |   ✅    |   ✅    |
| `Uniform Buffer` |   ✅    |   ✅    |
| `Vertex Buffer`  |   ✅    |   ✅    |
|  `Index Buffer`  |   ✅    |   ✅    |
| `Storage Image`  |   ✅    |   ✅    |
|      `SSBO`      |   ✅    |   ✅    |
|     `Mipmap`     |   ✅    |   ✅    |
|  `TextureArray`  |   ✅    |   ✅    |

`特性`

|                 特性                  | OpenGL | Vulkan |
| :-----------------------------------: | :----: | :----: |
|                实例化                 |   ✅    |   ✅    |
|              计算着色器               |   ✅    |   ✅    |
|               混合方程                |   ✅    |   ✅    |
|              多窗口渲染               |   ✅    |   ✅    |
|               多重采样                |        |   ✅    |
| `Debug Util Label`、`Debug Util Name` |   ✅    |   ✅    |

## 架构

### Shader反射

|   API    |                             思路                             |
| :------: | :----------------------------------------------------------: |
| `OpenGL` |                  运行时动态查询参数及绑定点                  |
| `Vulkan` | 通过`SPIRV-Reflect`运行时反射SPV文件得到绑定点信息，从而生成`VkDescriptorSetLayoutBinding`信息 |

### 线程

`OpenGL`方面通过内部创建多个渲染线程，每个线程持有一个`Context`，所有上下文共享一个基类上下文，轮询争夺渲染任务实现模拟Vulkan的多线程

## 平台支持

|    OS     |    GRAPHICS API    |
| :-------: | :----------------: |
| `Windows` | `OpenGL`& `Vulkan` |
|  `MacOS`  | `OpenGL`& `Vulkan` |
|   `Ios`   | `OpenGL`& `Vulkan` |
| `Android` | `OpenGL`& `Vulkan` |
|   `Web`   |      `OpenGL`      |

注：安卓支持`SurfaceView`、`AHarwareBuffer`两种上屏方式
