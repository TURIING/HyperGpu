/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:33
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef CMDBASE_H
#define CMDBASE_H
#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class GlContext;

class CmdBase : public GpuObject {
public:
    explicit CmdBase(CmdType cmdType): m_cmdType(cmdType) {}
    virtual void Execute(GlContext* pContext) = 0;
    [[nodiscard]] CmdType GetCmdType() const { return m_cmdType; }
    virtual void Reset() = 0;

protected:
    CmdType m_cmdType =  CmdType::None;
};

USING_GPU_NAMESPACE_END
#endif //CMDBASE_H