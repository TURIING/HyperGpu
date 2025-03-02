/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 16:00
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef GPUPIPELINE_H
#define GPUPIPELINE_H
#include <vector>
#include <memory>

struct RenderEnvInfo {
    std::vector<char> vertexShader;
    std::vector<char> fragShader;

};

class Pipeline {
public:
    virtual ~Pipeline() = default;
};

class PipelineManager {
public:
    virtual ~PipelineManager() = default;
    virtual std::shared_ptr<Pipeline> CreateRenderPipeline(const RenderEnvInfo& renderEnvInfo) = 0;
};


#endif //GPUPIPELINE_H
