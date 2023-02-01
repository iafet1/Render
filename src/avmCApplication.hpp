//
// avmCApplication.hpp - определение основного класса приложения
//

#pragma once

#include "stdafx.hpp"

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

        // рендер
        CRender* m_render{nullptr};

        // окно
        #if defined(_WIN32)
            HINSTANCE m_hInst{nullptr}; // дескриптор приложения
            HWND      m_hWnd{nullptr};  // дескриптор главного окна приложения
        #else
            #error "Not support"   
        #endif // 

        uint32_t m_width {800}; // ширина окна
        uint32_t m_heght {450}; // высота окна

        // приложение
        const char* m_name {nullptr}; // имя программы
        uint32_t m_exitCode {0}; // код завершения приложения

        bool m_initialized {false}; // флаг иницилизации
        bool m_exit {false}; // флаг завершения приложения
        
    };
}

// extern avm::CApplication* g_App;