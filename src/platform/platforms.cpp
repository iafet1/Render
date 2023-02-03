//
// platforms.cpp
// 

#include "platforms.hpp"
#include "../avmCLog.hpp"

constexpr char FILE_NAME[] = "platforms.cpp";

namespace avm {
namespace platform {

#if defined(WIN32) || defined(_WIN32)

    // оконная процедура для WIN32
    static LRESULT WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        switch (Msg)
        {
            case WM_CLOSE:
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }

            // TEMP: временно для закрытия окна
            case WM_KEYDOWN:
            {
                if (wParam == VK_ESCAPE)
                    PostQuitMessage(1);

                return 0;
            }

            default:
                break;
        }

        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    void WindowCreate(WindowDesc& window, const char* name)
    {
        assert(window.hWnd == nullptr);

        window.hInstance = GetModuleHandle(NULL);   // получить дескриптор приложения

        WNDCLASSEX  wcex = {   // Содержит информацию о классе окна.
            .cbSize         = sizeof(WNDCLASSEX),    // Размер в байтах этой структуры.
            .style          = CS_DBLCLKS,            // Стиль класса.
            .lpfnWndProc    = WndProc,               // Указатель на оконную процедуру.
            .cbClsExtra     = 0,                     // Количество дополнительных байтов, выделяемых в соответствии со структурой оконного класса.
            .cbWndExtra     = sizeof(void*),         // Количество дополнительных байтов, выделяемых после экземпляра окна.
            .hInstance      = window.hInstance,      // Дескриптор экземпляра, содержащего оконную процедуру для класса.
            .hIcon          = NULL,                  // Дескриптор значка класса.
            .hCursor        = NULL,                  // Дескриптор курсора класса.
            .hbrBackground  = NULL,                  // (HBRUSH)COLOR_BACKGROUND;  // Дескриптор фоновой кисти класса.
            .lpszMenuName   = NULL,                  // Указатель на символьную строку с завершающим нулем, которая определяет имя ресурса меню класса.
            .lpszClassName  = name,                  // Указатель на строку с завершающим нулем или является атомом.
            .hIconSm        = NULL                   // Дескриптор маленького значка, связанного с классом окна.
        };

        if (0 == RegisterClassEx(&wcex)) {
            THROW();
        }

        // расширенный стиль окна
        DWORD exStyle = WS_EX_APPWINDOW;  // нет расширенного стиля
        // основной стиль окна
        DWORD style = WS_THICKFRAME | WS_CAPTION;    // окно имеет размерную рамку.

        RECT rc = { 
            .left   = 0,
            .top    = 0,
            .right  = ((LONG)window.width),
            .bottom = ((LONG)window.height)
        };

        AdjustWindowRectEx(&rc, style, FALSE, exStyle);

        window.width = rc.right - rc.left;
        window.height = rc.bottom - rc.top;

        // создание главного окна приложения
        window.hWnd = CreateWindowEx(exStyle, name, name, style, CW_USEDEFAULT, CW_USEDEFAULT,
                                    window.width, window.height, NULL, NULL, window.hInstance, NULL);

        if (NULL == window.hWnd) {
            LOG_ERROR("Не могу создать главное окно приложения.");
            THROW();
        }
        
        GetClientRect(window.hWnd, &rc);
        LOG_DEBUG("Ширина - %d, Высота - %d.", rc.right, rc.bottom);

        ShowWindow(window.hWnd, SW_SHOW);
    }

    bool GetMessageWindow()
    {
        MSG msg;

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            switch (msg.message)
            {
                case WM_QUIT:
                    return true;

                default:
                    break;;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return false;
    }

#else
    #error "Платформа пока не поддерживается"

    void WindowCreate(WindowDesc& window, const char* name) {}
    bool GetMessageWindow() {}

#endif
}
}