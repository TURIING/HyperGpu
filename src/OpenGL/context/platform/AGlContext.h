/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 10:20
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_AGLCONTEXT_H
#define HYPERRENDER_AGLCONTEXT_H

#include "IContext.h"
#include "../../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class AGlContext final: public IContext {
public:
    ~AGlContext() override;
    void MakeCurrent() override;
    void ClearCurrent() override;
    static void init();
    static AGlContext* CreateContext(AGlContext* shareContext);
    static AGlContext* GetLastContext();

private:
    void createContext(void *shareContext);

private:
    void* m_pContext = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //HYPERRENDER_AGLCONTEXT_H