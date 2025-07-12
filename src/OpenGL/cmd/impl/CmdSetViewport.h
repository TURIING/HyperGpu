//
// Created by turiing on 2025/6/29.
//

#ifndef CMDSETVIEWPORT_H
#define CMDSETVIEWPORT_H

#include "../../../common/common.h"
#include "../CmdBase.h"

USING_GPU_NAMESPACE_BEGIN

class CmdSetViewport final: public CmdBase {
public:
    static constexpr auto CMD_TYPE =  CmdType::SetViewport;
    CmdSetViewport(): CmdBase(CMD_TYPE) {}
    ~CmdSetViewport() override { Reset(); }

    void Init(const Viewport &viewport) {
        m_viewport = viewport;
    }

    void Execute(GlContext* pContext) override {
        CALL_GL(glViewport(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height));
    }

    void Reset() override {
        CALL_GL(glViewport(0, 0, 0, 0));
    }

private:
    Viewport m_viewport;
};

USING_GPU_NAMESPACE_END
#endif //CMDSETVIEWPORT_H
