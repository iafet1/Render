//
// avmCApplication.cpp
//

#include "stdafx.hpp"
#include "avmCLog.hpp"
#include "avmCApplication.hpp"

#include "platform/platforms.hpp"

constexpr char FILE_NAME[] = "avmCApplication.cpp";

using namespace avm;

#define WIN_WIDTH 960
#define WIN_HEIGHT 540

avm::CApplication::CApplication(const char* name) : 
    m_name(name)
{
    assert(name != nullptr && name[0] != '\0');
}

avm::CApplication::~CApplication()
{
    shutdown();
}

void avm::CApplication::Run()
{
    LOG_DEBUG("CApplication::Run()");

    if (!m_initialized) {
        inicilize();
    }
    
    while(!m_exit) {
        getMessageWindow();
    }
 
    return;

}

void avm::CApplication::inicilize()
{
    assert(!m_initialized);

    windowCreate(WIN_WIDTH, WIN_HEIGHT);
    m_initialized = true;

}

void avm::CApplication::shutdown()
{
    assert(m_initialized);

    m_window = {};
    m_initialized = false;
}

void avm::CApplication::windowCreate(uint32_t width, uint32_t height)
{
    if (width > 800 && height > 450) {
        m_window.width = width;
        m_window.height = height;
    }

    platform::WindowCreate(m_window, m_name);
}

void avm::CApplication::getMessageWindow()
{
    if (platform::GetMessageWindow()) {
        SetExitCode(0);
    }
}
