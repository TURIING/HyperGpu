/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 14:05
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulRenderPass.h"
#include "../device/VulLogicDevice.h"

USING_GPU_NAMESPACE_BEGIN

VulRenderPass::VulRenderPass(VulLogicDevice* device, VulRenderPassCreateInfo &createInfo): m_pDevice(device) {
    m_pDevice->AddRef();

    VkSubpassDescription subpass {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,                                       // Vulkan在未来也可能会支持计算子流程，所以，我们还需要显式地指定这是一个图形渲染的子流程
    };

    std::vector<VkAttachmentDescription> attachments;
    std::vector<VkAttachmentReference> vecColorAttachmentRef;
    VkAttachmentReference depthAttachmentRef;

    for(const auto & [type, attachmentIndex, format] : createInfo.attachment) {
        // 颜色附着
        if(type == AttachmentType::COLOR) {
            const VkAttachmentDescription colorAttachment = {
                .format = format,
                .samples = VK_SAMPLE_COUNT_1_BIT,

                // 对颜色和深度缓冲起效
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,                                      // 指定在渲染之前对附着中的数据进行的操作,使用一个常量值来清除附着的内容
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,                                    // 渲染之后对附着中的数据进行的操作,渲染的内容会被存储起来，以便之后读取

                // 对模板缓冲起效
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,                           // 设置对模板缓冲不关心
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

                // 图像的像素数据在内存中的分布取决于我们要对图像进行的操作
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,             // 用于指定渲染流程开始前的图像布局方式,我们不关心之前的图像布局方式
                .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL // 用于指定渲染流程结束后的图像布局方式,图像被用在交换链中进行呈现操作
            };

            VkAttachmentReference colorAttachmentRef = {
                .attachment = attachmentIndex,                                              // 用于指定要引用的附着在附着描述结构体数组中的索引
                .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL                          // 用于指定进行子流程时引用的附着使用的布局方式，Vulkan会在子流程开始时自动将引用的附着转换到layout成员变量指定的图像布局
            };

            attachments.push_back(colorAttachment);
            vecColorAttachmentRef.push_back(colorAttachmentRef);
        }

        // 深度附着
        else if(type == AttachmentType::DEPTH) {
            const VkAttachmentDescription depthAttachment = {
                .format = format,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
            };

            depthAttachmentRef = {
                .attachment = attachmentIndex,
                .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
            };
            attachments.push_back(depthAttachment);
        }

        subpass.colorAttachmentCount = vecColorAttachmentRef.size();
        subpass.pColorAttachments = vecColorAttachmentRef.data();
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
    }

    VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    };

    const VkRenderPassCreateInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };
    CALL_VK(vkCreateRenderPass(m_pDevice->GetHandle(), &renderPassInfo, nullptr, &m_pHandle));
    LOG_INFO("RenderPass created");
}

VulRenderPass::~VulRenderPass() {
    vkDestroyRenderPass(m_pDevice->GetHandle(), m_pHandle, nullptr);
	m_pDevice->SubRef();
}

VulRenderPassBuilder& VulRenderPassBuilder::AddAttachment(const VulAttachmentInfo& attachmentInfo) {
    m_renderPassCreateInfo.attachment.push_back(attachmentInfo);
    return *this;
}

VulRenderPass* VulRenderPassBuilder::Build() {
    return new VulRenderPass(m_pDevice, m_renderPassCreateInfo);
}

USING_GPU_NAMESPACE_END