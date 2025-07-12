//
// Created by turiing on 2025/6/29.
//

#ifndef CMDSETSCISSOR_H
#define CMDSETSCISSOR_H

#include "../../../common/common.h"
#include "../CmdBase.h"

USING_GPU_NAMESPACE_BEGIN

class CmdSetScissor final: public CmdBase {
public:
    static constexpr auto CMD_TYPE =  CmdType::SetScissor;
    CmdSetScissor(): CmdBase(CMD_TYPE) {}
    ~CmdSetScissor() override { Reset(); }

    void Init(const Scissor &scissor) {
        m_scissor = scissor;
    }

    void Execute(GlContext* pContext) override {
        CALL_GL(glScissor(m_scissor.offsetX, m_scissor.offsetY, m_scissor.width, m_scissor.height));
        CALL_GL(glEnable(GL_SCISSOR_TEST));
    }

    void Reset() override {
        CALL_GL(glScissor(0, 0, 0, 0));
        CALL_GL(glDisable(GL_SCISSOR_TEST));
    }

private:
    Scissor m_scissor;
};

USING_GPU_NAMESPACE_END
#endif //CMDSETSCISSOR_H
