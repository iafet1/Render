//
// avmCGxDevice.cpp
//

#include "../stdafx.hpp"
#include "../avmCLog.hpp"

#include "avmCGxDevice.hpp"

namespace avm::graphics
{
    
    CGxDevice::CGxDevice()
    {
        LOG_DEBUG("CGxDevice::CGxDevice().");
    }
    
    CGxDevice::~CGxDevice()
    {
        m_indexFrame = 0;
        LOG_DEBUG("CGxDevice::~CGxDevice().");
    }
    
}