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

template <typename T>
uint32_t TO_U32(T value) {
    static_assert(std::is_arithmetic<T>::value, "T must be numeric");
    return static_cast<uint32_t>(value);
}

template <typename T>
int32_t TO_I32(T value) {
    static_assert(std::is_arithmetic<T>::value, "T must be numeric");
    return static_cast<int32_t>(value);
}

inline void HASH_COMBINE(size_t &seed, size_t hash) {
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;
}

template <class T>
inline void HASH_COMBINE(size_t &seed, const T &v) {
    std::hash<T> hasher;
    HASH_COMBINE(seed, hasher(v));
}

using u32 = uint32_t;
using i32 = int32_t;
/************************************************* Type *************************************************************/
struct Point {
    int x = 0;
    int y = 0;
};

struct ImageInfo {
    unsigned char *data = nullptr;
    Size size;
};


/************************************************* Variable *********************************************************/
constexpr const char *VK_LAYER_KHRONOS_VALIDATION = "VK_LAYER_KHRONOS_validation";

#endif //BASEDEFINE_H
