# HyperGpu
## Features

`资源类型`

|       类型       | OpenGL | Vulkan |
| :--------------: | :----: | :----: |
|    `Image2D`     |   ✅    |   ✅    |
| `Uniform Buffer` |        |   ✅    |
| `Vertex Buffer`  |        |   ✅    |
|  `Index Buffer`  |        |   ✅    |
| `Storage Image`  |        |   ✅    |
|      `SSBO`      |        |        |
|     `Mipmap`     |        |        |
|  `TextureArray`  |        |        |

`特性`

|                 特性                  | OpenGL | Vulkan |
| :-----------------------------------: | :----: | :----: |
|                实例化                 |        |   ✅    |
|              计算着色器               |        |   ✅    |
|               混合方程                |        |   ✅    |
|              多窗口渲染               |   ✅    |   ✅    |
|               多重采样                |        |        |
| `Debug Util Label`、`Debug Util Name` |        |   ✅    |

## 平台支持

|    OS     |    GRAPHICS API    |
| :-------: | :----------------: |
| `Windows` | `OpenGL`& `Vulkan` |
|  `MacOS`  | `OpenGL`& `Vulkan` |
|   `Ios`   | `OpenGL`& `Vulkan` |
| `Android` | `OpenGL`& `Vulkan` |
|   `Web`   |      `OpenGL`      |

注：安卓支持`SurfaceView`、`AHarwareBuffer`两种上屏方式
