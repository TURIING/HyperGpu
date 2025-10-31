//
// Created by turiing on 2025/11/1.
//

#ifndef CMDENDDEBUGUTILSLABEL_H
#define CMDENDDEBUGUTILSLABEL_H

#include "../../../common/common.h"
#include "../CmdBase.h"

USING_GPU_NAMESPACE_BEGIN

class CmdEndDebugUtilsLabel final: public CmdBase {
public:
    static constexpr auto CMD_TYPE =  CmdType::EndDebugUtilsLabel;
    CmdEndDebugUtilsLabel(): CmdBase(CMD_TYPE) {}
    ~CmdEndDebugUtilsLabel() override { Reset(); }

    void Init() {}

    void Execute(GlContext* pContext) override {
#if !defined(PLATFORM_MACOS) && !defined(PLATFORM_IOS)
        glPopDebugGroup();
#endif
    }

    void Reset() override {}
};

USING_GPU_NAMESPACE_END

#endif //CMDENDDEBUGUTILSLABEL_H
