/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 22:27
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

namespace HyperGpu
{
    struct Version {
        int major;
        int minor;
        int patch;
    };

    struct Size {
        uint32_t width = 0;
        uint32_t height = 0;
    };
}
#endif //TYPES_H
