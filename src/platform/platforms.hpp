//
// planforms.hpp - Платформозависимый код
//

#pragma once

#include "../stdafx.hpp"

namespace avm {
namespace platform {

    struct WindowDesc
    {
        #if defined(WIN32) || defined(_WIN32)
            HINSTANCE hInstance {nullptr}; // дескриптор приложения
            HWND hWnd {nullptr}; // дескриптор окна приложения
        #else
            #error "Платформа пока не поддерживается"
        #endif

        uint32_t width  {800}; // ширина окна
        uint32_t height {450}; // высота окна

        bool minimized {false}; // минимизация окна
        bool active {true}; // окно активно
    };

    // создание окна приложения
    void WindowCreate(WindowDesc& window, const char* name);

    // чтение сообщений очереди окна (return true = WM_QUIT)
    bool GetMessageWindow();
    
}
}