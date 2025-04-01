/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/27 21:45
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef BASEDEFINE_H
#define BASEDEFINE_H

#include "Types.h"

using namespace HyperGpu;

/************************************************* Marco ************************************************************/
#define BIT(x) (1 << (x))
#define CASE_FROM_TO(FROM, TO) case FROM: return TO;

/************************************************* Type *************************************************************/

enum class MouseButton {
    None = 0,
    Left = BIT(0),
    Right = BIT(1),
    Middle = BIT(2),
};

struct Point {
    int x = 0;
    int y = 0;
};

struct ImageInfo {
    unsigned char *data = nullptr;
    Size size;
};


/************************************************* Variable *********************************************************/

constexpr auto APP_NAME = "VTEngine";
constexpr Version APP_VERSION = { 1, 0, 0 };
constexpr Size WINDOW_SIZE = { 1200, 1000 };
inline constexpr float FRAME_TIME = 1/60.0;                                                                         // 每秒里每帧的耗时
constexpr std::string SHADER_DIR = "shader/";
constexpr std::string TEXTURE_DIR = "assets/images/";
constexpr std::string MODEL_DIR = "assets/model/";

constexpr const char *VK_LAYER_KHRONOS_VALIDATION = "VK_LAYER_KHRONOS_validation";

#endif //BASEDEFINE_H
