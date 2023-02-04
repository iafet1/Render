//
// avmCApplication.hpp - определение основного класса приложения
//

#pragma once

#include "stdafx.hpp"
#include "platform/platforms.hpp"
#include "avmCGxDevice.hpp"

namespace avm {

    class CRender;

    class CApplication
    {
    public:
        CApplication(const char* name);
        ~CApplication();

        // удаление
        CApplication(const CApplication &) = delete;
        CApplication &operator=(const CApplication &) = delete;

        // Интерфейс API

        // основной цикл
        void Run();

        // установка кода и флага завершения
        inline void SetExitCode(uint32_t exitCode) { m_exitCode = exitCode; m_exit = true;}
        // запрос кода завершения
        inline uint32_t GetExitCode() { return m_exitCode;}


    private:
        /* functions */

        // иницилизация
        void inicilize();
        // завершение
        void shutdown();

        // создание основного окна приложения
        void windowCreate(uint32_t width, uint32_t height);

        // чтение сообщений окна приложения
        void getMessageWindow();


    private:
        /* data */

        // графическое устройство
        graphics::CGxDevice* m_gxDevice {nullptr};

        // рендер
        CRender* m_render{nullptr};

        // приложение
        const char* m_name {nullptr}; // имя программы
        uint32_t m_exitCode {0}; // код завершения приложения

        bool m_initialized {false}; // флаг иницилизации
        bool m_exit {false}; // флаг завершения приложения
        
        // окно
        platform::WindowDesc m_window; // свойства окна


    };
}

// extern avm::CApplication* g_App;