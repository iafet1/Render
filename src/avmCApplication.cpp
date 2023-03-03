//
// avmCApplication.cpp
//

#include "stdafx.hpp"
#include "avmCLog.hpp"
#include "avmCGxDevice.hpp"
#include "platform/platforms.hpp"

#include "avmCApplication.hpp"
#include "avmEvent.hpp"

constexpr char FILE_NAME[] = "avmCApplication.cpp";

using namespace avm;

#define WIN_WIDTH 960
#define WIN_HEIGHT 540

avm::CApplication::CApplication(const char *name) : m_name(name)
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

    if (!m_initialized)
    {
        inicilize();
    }

    while (!m_exit)
    {
        event::HandlingEvents();
        
        getMessageWindow();
    }

    return;
}

void avm::CApplication::inicilize()
{
    assert(!m_initialized);

    // создание графического устройства
    m_gxDevice = new graphics::CGxDevice();

    // создание основного окна приложения
    windowCreate(WIN_WIDTH, WIN_HEIGHT);

    // TODO: test event QUIT: регистрация события
    event::RegisterHandler(event::AppTypes::EVENT_APP_QUIT, this,
                           [](void *object, Variant param1, Variant param2) -> bool
                           {
                               CApplication *app = static_cast<CApplication *>(object);
                               app->SetExitCode(param1.u32);
                               return false;
                           });

    m_initialized = true;
}

void avm::CApplication::shutdown()
{
    assert(m_initialized);

    // TODO: test event QUIT
    event::UnRegisterHandler(event::AppTypes::EVENT_APP_QUIT, this);

    // удаление графического устройства
    if (m_gxDevice != nullptr)
    {
        delete m_gxDevice;
        m_gxDevice = nullptr;
    }

    m_window = {};
    m_initialized = false;
}

void avm::CApplication::windowCreate(uint32_t width, uint32_t height)
{
    if (width > 800 && height > 450)
    {
        m_window.width = width;
        m_window.height = height;
    }

    platform::WindowCreate(m_window, m_name);
}

void avm::CApplication::getMessageWindow()
{
    if (platform::GetMessageWindow())
    {
        SetExitCode(0);
    }
}
