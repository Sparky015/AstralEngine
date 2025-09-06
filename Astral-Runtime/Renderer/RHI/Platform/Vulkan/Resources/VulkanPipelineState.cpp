/**
* @file VulkanPipelineState.cpp
* @author Andrew Fagan
* @date 5/17/2025
*/

#include "VulkanPipelineState.h"

#include "VulkanVertexBuffer.h"
#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Error.h"
#include "Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h"

namespace Astral {

    VulkanPipelineState::VulkanPipelineState(const VulkanGraphicsPipelineStateDesc& desc) :
        m_GraphicsDescription(desc),
        m_Device(desc.Device),
        m_ViewportDimensions()
    {
        CreateGraphicsPipelineStateObject();
    }


    VulkanPipelineState::~VulkanPipelineState()
    {
        DestroyPipelineLayout();
        DestroyPipelineState();
    }


    void VulkanPipelineState::CreateGraphicsPipelineStateObject()
    {
        CreateGraphicsPipelineLayout();

        SetGraphicsShaderStages();
        SetVertexInputState();
        SetInputAssemblyState();
        SetViewportState();
        SetRasterizerState();
        SetMultisampleState();
        SetDepthStencilState();
        SetColorBlendState();
        SetDynamicState();


        VkRenderPass renderPass = (VkRenderPass)m_GraphicsDescription.RenderPass->GetNativeHandle();

        VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = sizeof(m_PipelineCreateInfos.ShaderStates) / sizeof(VkPipelineShaderStageCreateInfo),
            .pStages = m_PipelineCreateInfos.ShaderStates,
            .pVertexInputState = &m_PipelineCreateInfos.VertexInputState,
            .pInputAssemblyState = &m_PipelineCreateInfos.InputAssemblyState,
            .pViewportState = &m_PipelineCreateInfos.ViewportState,
            .pRasterizationState = &m_PipelineCreateInfos.RasterizationState,
            .pMultisampleState = &m_PipelineCreateInfos.MultisampleState,
            .pDepthStencilState = &m_PipelineCreateInfos.DepthStencilState,
            .pColorBlendState = &m_PipelineCreateInfos.ColorBlendState,
            .pDynamicState = &m_PipelineCreateInfos.DynamicState,
            .layout = m_PipelineLayout,
            .renderPass = renderPass,
            .subpass = m_GraphicsDescription.SubpassIndex,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = -1
        };

        VkResult result = vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineCreateInfo,
                                                nullptr, &m_Pipeline);
        ASSERT(result == VK_SUCCESS, "Failed to create graphics pipeline!");
    }


    void VulkanPipelineState::DestroyPipelineState()
    {
        vkDestroyPipeline(m_Device, m_Pipeline, nullptr);
    }


    void VulkanPipelineState::SetGraphicsShaderStages()
    {
        ASSERT(m_GraphicsDescription.VertexShader, "Vertex shader can't be null!")
        ASSERT(m_GraphicsDescription.FragmentShader, "Fragment shader can't be null!")

        VkShaderModule vertexShaderModule = (VkShaderModule)m_GraphicsDescription.VertexShader->GetNativeHandle();
        VkShaderModule fragmentShaderModule = (VkShaderModule)m_GraphicsDescription.FragmentShader->GetNativeHandle();

        VkPipelineShaderStageCreateInfo shaderStageCreateInfo[2] = {
         {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage = VK_SHADER_STAGE_VERTEX_BIT,
                .module = vertexShaderModule,
                .pName = "main",
            },
         {
                 .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                 .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                 .module = fragmentShaderModule,
                 .pName = "main",
            }
        };

        m_PipelineCreateInfos.ShaderStates[0] = shaderStageCreateInfo[0];
        m_PipelineCreateInfos.ShaderStates[1] = shaderStageCreateInfo[1];
    }


    static VkFormat ConvertShaderDataTypeToVkFormat(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:  return VK_FORMAT_R32_SFLOAT;
            case ShaderDataType::Float2: return VK_FORMAT_R32G32_SFLOAT;
            case ShaderDataType::Float3: return VK_FORMAT_R32G32B32_SFLOAT;
            case ShaderDataType::Float4: return VK_FORMAT_R32G32B32A32_SFLOAT;
            case ShaderDataType::Int:    return VK_FORMAT_R32_SINT;
            case ShaderDataType::Int2:   return VK_FORMAT_R32G32_SINT;
            case ShaderDataType::Int3:   return VK_FORMAT_R32G32B32_SINT;
            case ShaderDataType::Int4:   return VK_FORMAT_R32G32B32A32_SINT;
            default:                     ASTRAL_ERROR("Unknown vertex buffer data type!");
        }
    }


    void VulkanPipelineState::SetVertexInputState()
    {
        VkVertexInputBindingDescription bindingDescription = {
            .binding = 0,
            .stride = m_GraphicsDescription.VertexBufferLayout.GetStride(),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        };
        m_PipelineCreateInfos.VertexBindingDescription = bindingDescription;


        uint32 location = 0;
        uint32 offset = 0;

        for (VertexBufferAttribute& attribute : m_GraphicsDescription.VertexBufferLayout)
        {
            VkVertexInputAttributeDescription attributeDescription = {
                .location = location,
                .binding = 0,
                .format = ConvertShaderDataTypeToVkFormat(attribute.DataType),
                .offset = offset
            };
            m_PipelineCreateInfos.VertexAttributeDescriptions.push_back(attributeDescription);

            offset += attribute.GetAttributeSize();
            location++;
        }

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount = 1,
            .pVertexBindingDescriptions = &m_PipelineCreateInfos.VertexBindingDescription,
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(m_PipelineCreateInfos.VertexAttributeDescriptions.size()),
            .pVertexAttributeDescriptions = m_PipelineCreateInfos.VertexAttributeDescriptions.data(),
        };

        m_PipelineCreateInfos.VertexInputState = vertexInputInfo;
    }


    void VulkanPipelineState::SetInputAssemblyState()
    {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
        };

        m_PipelineCreateInfos.InputAssemblyState = inputAssembly;
    }


    void VulkanPipelineState::SetViewportState()
    {
        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float)m_GraphicsDescription.WindowWidth,
            .height = (float)m_GraphicsDescription.WindowHeight,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };

        VkRect2D scissor = {
            .offset = {0, 0},
            .extent = {(uint32)m_GraphicsDescription.WindowWidth, (uint32)m_GraphicsDescription.WindowHeight}
        };

        m_PipelineCreateInfos.Viewport = viewport;
        m_PipelineCreateInfos.Scissor = scissor;

        VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports = &m_PipelineCreateInfos.Viewport,
            .scissorCount = 1,
            .pScissors = &m_PipelineCreateInfos.Scissor,
        };

        m_PipelineCreateInfos.ViewportState = viewportStateCreateInfo;
    }


    void VulkanPipelineState::SetRasterizerState()
    {
        VkPipelineRasterizationStateCreateInfo rasterizer = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_NONE,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .lineWidth = 1.0f
        };

        m_PipelineCreateInfos.RasterizationState = rasterizer;
    }


    void VulkanPipelineState::SetMultisampleState()
    {
        bool shouldEnableMSAA = m_GraphicsDescription.MSAASamples != SampleCount::SAMPLE_1_BIT;
        VkSampleCountFlagBits msaaSamples = ConvertSampleCountToVkSampleCountBit(m_GraphicsDescription.MSAASamples);

        VkPipelineMultisampleStateCreateInfo multisample = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = msaaSamples,
            .sampleShadingEnable = shouldEnableMSAA,
            .minSampleShading = 1.0f
        };

        m_PipelineCreateInfos.MultisampleState = multisample;
    }


    void VulkanPipelineState::SetDepthStencilState()
    {
        VkPipelineDepthStencilStateCreateInfo depthStencilState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthTestEnable = true,
            .depthWriteEnable = true,
            .depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
            .depthBoundsTestEnable = VK_FALSE,
            .stencilTestEnable = VK_FALSE,
            .front = {},
            .back = {},
            .minDepthBounds = 0.0f,
            .maxDepthBounds = 1.0f,
        };

        m_PipelineCreateInfos.DepthStencilState = depthStencilState;
    }


    void VulkanPipelineState::SetColorBlendState()
    {
        VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
            .blendEnable = m_GraphicsDescription.IsAlphaBlended == true ? VK_TRUE : VK_FALSE,
            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            .colorBlendOp = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp = VK_BLEND_OP_ADD,
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT, // <--- Enable writing to Alpha channel
        };

        uint32 numColorAttachments = m_GraphicsDescription.RenderPass->GetNumColorAttachments(m_GraphicsDescription.SubpassIndex);
        m_PipelineCreateInfos.ColorBlendAttachmentStates.reserve(numColorAttachments);

        for (int i = 0; i < numColorAttachments; i++)
        {
            m_PipelineCreateInfos.ColorBlendAttachmentStates.push_back(colorBlendAttachmentState);
        }

        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = (uint32)m_PipelineCreateInfos.ColorBlendAttachmentStates.size(),
            .pAttachments = m_PipelineCreateInfos.ColorBlendAttachmentStates.data(),
        };

        m_PipelineCreateInfos.ColorBlendState = colorBlendStateCreateInfo;
    }


    void VulkanPipelineState::SetDynamicState()
    {
        m_PipelineCreateInfos.DynamicStateSpecs[0] = VK_DYNAMIC_STATE_VIEWPORT;
        m_PipelineCreateInfos.DynamicStateSpecs[1] = VK_DYNAMIC_STATE_SCISSOR;

        VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .dynamicStateCount = 2,
            .pDynamicStates = m_PipelineCreateInfos.DynamicStateSpecs,
        };

        m_PipelineCreateInfos.DynamicState = dynamicStateCreateInfo;
    }


    void VulkanPipelineState::CreateGraphicsPipelineLayout()
    {
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        descriptorSetLayouts.reserve(m_GraphicsDescription.DescriptorSets.size());
        m_DescriptorSetLayout.reserve(m_GraphicsDescription.DescriptorSets.size());
        for (DescriptorSetHandle descriptorSet : m_GraphicsDescription.DescriptorSets)
        {
            descriptorSetLayouts.push_back((VkDescriptorSetLayout)descriptorSet->GetNativeLayout());
            m_DescriptorSetLayout.push_back(descriptorSet->GetDescriptorSetLayout());
        }

        m_PushConstantRange.stageFlags = VK_SHADER_STAGE_ALL;
        m_PushConstantRange.offset = 0;
        m_PushConstantRange.size = MaxPushConstantRange;

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .setLayoutCount = (uint32)descriptorSetLayouts.size(),
            .pSetLayouts = descriptorSetLayouts.data(),
            .pushConstantRangeCount = 1,
            .pPushConstantRanges = &m_PushConstantRange,
        };

        VkResult result = vkCreatePipelineLayout(m_Device, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout);
        ASSERT(result == VK_SUCCESS, "Failed to create pipeline layout!");
    }


    void VulkanPipelineState::DestroyPipelineLayout()
    {
        vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
    }

}
