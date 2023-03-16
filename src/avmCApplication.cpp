//
// avmCApplication.cpp
//

#include "stdafx.hpp"
#include "platform/platforms.hpp"
#include "avmCLog.hpp"
#include "avmCTimer.hpp"
#include "render/avmCRender.hpp"

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
        initialization();
        m_initialized = true;
    }

    const float target_deltaTime = 1.0f / m_targetFrameRate;

    CTimer timer;

    while (!m_exit)
    {
        // обработать события
        event::HandlingEvents();

        m_deltaTime = timer.Elapsed();

        // задержка для фиксации кадров с частотой 'm_targetFrameRate' в секунду
        if (m_frameRateLock && m_deltaTime < target_deltaTime)
        {
            platform::Sleep((target_deltaTime - m_deltaTime) * 1000);
            m_deltaTime += timer.Elapsed();
        }

        // Фиксированное время обновления
        {
            if (m_frameSkip)
            {
                m_deltaTimeAccumulator += m_deltaTime;
                if (m_deltaTimeAccumulator > 10)
                {
                    // приложение, вероятно, потеряло контроль, исправленное обновление заняло бы слишком много времени
                    m_deltaTimeAccumulator = 0;
                }

                const float targetFrameRateInv = 1.0f / m_targetFrameRate;
                while (m_deltaTimeAccumulator >= targetFrameRateInv)
                {
                    FixedUpdate();
                    m_deltaTimeAccumulator -= targetFrameRateInv;
                }
            }
            else
            {
                FixedUpdate();
            }
        }

        // Обновление с переменным временем
        Update(m_deltaTime);

        // TODO: отрисовка
        Render(m_deltaTime);

        // TODO: обработать композицию
        Compose();

        // представить изображение на экран
        m_render->Present(m_deltaTime);

        // обработать пришедшие сообщения окна
        getMessageWindow();

    }

    return;
}

void avm::CApplication::Initialize()
{
}

void avm::CApplication::Update(float dt)
{
}

void avm::CApplication::FixedUpdate()
{
}

void avm::CApplication::Render(float dt)
{
    m_render->Render(dt);
}

void avm::CApplication::Compose()
{
}

void avm::CApplication::initialization()
{
    assert(!m_initialized);

    // создание системы ввода
    m_input = new CInput;

    // создание рендера (отрисовщика)
    GxDeviceType type = GxDeviceType::VULKAN; // TODO: перенести в настройки приложения
    m_render = new CRender(type);

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

    // установка поверхности (холста) для вывода изображения
    m_render->SetCanvas(&m_window);

    m_initialized = true;
}

void avm::CApplication::shutdown()
{
    assert(m_initialized);

    // TODO: test event QUIT
    event::UnRegisterHandler(event::AppTypes::EVENT_APP_QUIT, this);

    // отмена обработчика комманды QUIT
    command::UnregisterHandler(command::Commands::COMMAND_QUIT, this);

    // удаление рендера
    if (m_render != nullptr)
    {
        delete m_render;
        m_render = nullptr;
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
