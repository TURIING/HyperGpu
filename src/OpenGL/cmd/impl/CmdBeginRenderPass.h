//
// Created by turiing on 2025/7/5.
//

#ifndef CMDBEGINRENDERPASS_H
#define CMDBEGINRENDERPASS_H

#include "../../../common/common.h"
#include "../CmdBase.h"

USING_GPU_NAMESPACE_BEGIN

class CmdBeginRenderPass final: public CmdBase {
public:
    static constexpr auto CMD_TYPE =  CmdType::BeginRenderPass;
    CmdBeginRenderPass(): CmdBase(CMD_TYPE) {}
    ~CmdBeginRenderPass() override { Reset(); }

    void Init(const BeginRenderInfo &info) {
        m_beginRenderInfo = info;
    }

    void Execute(GlContext* pContext) override {}
    void Reset() override {}
    const BeginRenderInfo& GetBeginRenderInfo() const { return m_beginRenderInfo; }

private:
    BeginRenderInfo m_beginRenderInfo {};
};

USING_GPU_NAMESPACE_END
#endif //CMDBEGINRENDERPASS_H
