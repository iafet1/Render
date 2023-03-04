//
// avmCApplication.cpp
//

#include "stdafx.hpp"
#include "avmCLog.hpp"
#include "avmCGxDevice.hpp"
#include "platform/platforms.hpp"

#include "avmCApplication.hpp"
#include "avmEvent.hpp"
#include "avmCommands.hpp"
#include "avmCInput.hpp"
#include "key_code.hpp"


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

    // создание системы ввода
    m_input = new CInput;

    // создание графического устройства
    m_gxDevice = new graphics::CGxDevice;

    // создание основного окна приложения
    windowCreate(WIN_WIDTH, WIN_HEIGHT);

    // регистрация комманды QUIT
    command::RegisterHandler(command::Commands::COMMAND_QUIT, this, onQuit);

    // TODO: test event QUIT: регистрация события
    event::RegisterHandler(event::AppTypes::EVENT_APP_QUIT, this,
                           [](void *object, Variant param1, Variant param2) -> bool
                           {
                               CApplication *app = static_cast<CApplication *>(object);
                               app->setExitCode(param1.u32);
                               return true;
                           });

    // TODO: test event pressed key ESC: регистрация события
    event::RegisterHandler(event::AppTypes::EVENT_APP_KEY_PRESSED, this,
                           [](void *object, Variant param1, Variant param2) -> bool
                           {
                               if ( param1.u8 == static_cast<uint8_t>(KeyCode::KEY_CODE_ESCAPE) )
                               {
                                   Variant p1{.u32 = 1};
                                   Variant p2{.u32 = 0};
                                   event::SetEvent(event::AppTypes::EVENT_APP_QUIT, p1, p2);
                                   return true;
                               }

                               return false;
                           });

    m_initialized = true;
}

void avm::CApplication::shutdown()
{
    assert(m_initialized);

    // TODO: test event QUIT
    event::UnRegisterHandler(event::AppTypes::EVENT_APP_QUIT, this);

    // отмена обработчика комманды QUIT
    command::UnregisterHandler(command::Commands::COMMAND_QUIT, this);

    // удаление графического устройства
    if (m_gxDevice != nullptr)
    {
        delete m_gxDevice;
        m_gxDevice = nullptr;
    }

    // удаление графического устройства
    if (m_input != nullptr)
    {
        delete m_input;
        m_input = nullptr;
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
        setExitCode(0);
    }
}

Variant avm::CApplication::onQuit(void *object, Variant param1, Variant param2)
{
    //CApplication *app = static_cast<CApplication *>(object);
    event::SetEvent(event::AppTypes::EVENT_APP_QUIT, param1, param2);
    return {0};
}
