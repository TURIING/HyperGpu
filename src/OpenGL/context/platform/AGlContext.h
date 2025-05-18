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

namespace HyperGpu
{
class AGlContext final: public IContext {
public:
    ~AGlContext() override;
    void MakeCurrent() override;
    void ClearCurrent() override;
    void BindLayer(void *layer);
    void SwapBuffer();
    static void init();
    static AGlContext* CreateContext(AGlContext* shareContext);
    static AGlContext* GetLastContext();

private:
    void createContext(void *shareContext);

private:
    void* m_pContext = nullptr;
};
}
#endif //HYPERRENDER_AGLCONTEXT_H