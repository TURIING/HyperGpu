//
// Created by turiing on 2025/10/31.
//

#ifndef CMDBEGINDEBUGUTILSLABEL_H
#define CMDBEGINDEBUGUTILSLABEL_H

#include "../../../common/common.h"
#include "../CmdBase.h"

USING_GPU_NAMESPACE_BEGIN

class CmdBeginDebugUtilsLabel final: public CmdBase {
public:
    static constexpr auto CMD_TYPE =  CmdType::BeginDebugUtilsLabel;
    CmdBeginDebugUtilsLabel(): CmdBase(CMD_TYPE) {}
    ~CmdBeginDebugUtilsLabel() override { Reset(); }

    void Init(const char *name, const Color &color) {
        m_name = name;
        m_color = color;
    }

    void Execute(GlContext* pContext) override {
#if !defined(PLATFORM_MACOS) && !defined(PLATFORM_IOS)
        static int id = 0;
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, ++id, -1, m_name);
#endif
    }

    void Reset() override {}

private:
    const char *m_name = nullptr;
    Color m_color;
};

USING_GPU_NAMESPACE_END

#endif //CMDBEGINDEBUGUTILSLABEL_H
