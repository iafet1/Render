//
// avmCRender.cpp
//

#include "../stdafx.hpp"
#include "../avmCLog.hpp"

#include "avmCRender.hpp"
#include "vulkan/avmCGxVulkan.hpp"

constexpr char FILE_NAME[] = "avmCRender.cpp";

namespace avm {

    CRender::CRender(GxDeviceType type)
    {
        LOG_DEBUG("CRender::CRender(%d)", type);

        switch(type) {
            case GxDeviceType::VULKAN: {
                m_device = new graphics::CGxVulkan;
                break;
            }
            default: {
                LOG_ERROR("На данном этапе не поддерживается.");
                THROW();
            }
        }
    }

    CRender::~CRender()
    {
        if(m_device != nullptr) {
            delete m_device;
            m_device = nullptr;
        }
        LOG_DEBUG("CRender::~CRender()");
    }

    bool CRender::BeginFrame(float dt)
    {
        return m_device->BeginFrame(dt);
    }

    bool CRender::EndFrame(float dt)
    {
        return m_device->EndFrame(dt);
    }

    bool CRender::Render(float dt)
    {
        if (m_device->BeginFrame(dt)) {
            if (!m_device->EndFrame(dt)) {
                LOG_ERROR("Ошибка отрисовки фрейма.");
                return false;
            }
        }
        return true;
    }
    
    bool CRender::Present(float dt)
    {
        return true;
    }
}