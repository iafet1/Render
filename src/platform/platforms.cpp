//
// platforms.cpp
// 

#include "platforms.hpp"
#include "../avmCLog.hpp"
#include "../avmEvent.hpp"
#include "../avmCInput.hpp"


constexpr char FILE_NAME[] = "platforms.cpp";

namespace avm::platform {

#if defined(WIN32) || defined(_WIN32)

#include "windowsx.h"

    // оконная процедура для WIN32
    static LRESULT WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        CInput *in = GetInputClass();

        switch (Msg)
        {
            // закрытие главного окна приложения и завершение программы
            case WM_DESTROY:
            case WM_CLOSE: {
                in->KeyPressed(VK_ESCAPE);
                break;
            }

            // блокировка перерисовки области окна
            case WM_ERASEBKGND: {
                return 1;
            }

            // изменение размера окна
            case WM_SIZE: {
                Variant size;
                size.iData32[0] = LOWORD(lParam);
                size.iData32[1] = HIWORD(lParam);

                avm::event::SetEvent(avm::event::AppTypes::EVENT_APP_RESIZED, {nullptr}, size);

                break;
            }
            
            // TEMP: временно 
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN: {
                in->KeyPressed(wParam);
                break;
            }

            case WM_SYSKEYUP:
            case WM_KEYUP: {
                in->KeyReleased(wParam);
                break;
            }

            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN: {
                POINT pt{
                    .x = GET_X_LPARAM(lParam),
                    .y = GET_Y_LPARAM(lParam)
                };

                in->ButtonPressed(wParam, pt);

                break;
            }

            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP: {
                POINT pt{
                    .x = GET_X_LPARAM(lParam),
                    .y = GET_Y_LPARAM(lParam)
                };

                in->ButtonReleased(wParam, pt);

                break;
            }

            case WM_XBUTTONDOWN: {
                uint16_t keyState = GET_KEYSTATE_WPARAM(wParam);
                uint16_t buttonX = GET_XBUTTON_WPARAM(wParam);
                POINT pt{
                    .x = GET_X_LPARAM(lParam),
                    .y = GET_Y_LPARAM(lParam)};

                uint32_t code = (buttonX << 16) + keyState;
                in->ButtonPressed(code, pt);

                return true;
            }

            case WM_XBUTTONUP: {
                uint16_t keyState = GET_KEYSTATE_WPARAM(wParam);
                uint16_t buttonX = GET_XBUTTON_WPARAM(wParam);
                POINT pt{
                    .x = GET_X_LPARAM(lParam),
                    .y = GET_Y_LPARAM(lParam)};

                uint32_t code = (buttonX << 16) + keyState;
                in->ButtonReleased(code, pt);

                return true;
            }

            case WM_MOUSEMOVE: {
                POINT pt{
                    .x = GET_X_LPARAM(lParam),
                    .y = GET_Y_LPARAM(lParam)
                };

                in->MouseMove(wParam, pt);

                break;
            }

            case WM_MOUSEWHEEL: {
                uint16_t keyState = GET_KEYSTATE_WPARAM(wParam);
                int16_t delta = GET_WHEEL_DELTA_WPARAM(wParam);
                POINT pt{
                    .x = GET_X_LPARAM(lParam),
                    .y = GET_Y_LPARAM(lParam)};

                uint32_t code = (delta << 16) + keyState;
                in->MouseWheel(code, pt);

                break;
            }

            default:
                return DefWindowProc(hWnd, Msg, wParam, lParam);
        }

        return 0;
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
            .hbrBackground  = NULL,                  // Дескриптор фоновой кисти класса.
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

    void* OpenLibrary(const char *name)
    {
        return (void *)(LoadLibraryA(name));
    }

    void CloseLibrary(void *handle)
    {
        FreeLibrary((HMODULE)handle);
    }

    void* GetProcAddr(void *handle, const char *nameProc)
    {
        return (void *)(GetProcAddress((HMODULE)handle, nameProc));
    }

    float GetPerformanceFrequency()
    {
        LARGE_INTEGER li;
        if (!QueryPerformanceFrequency(&li))
            return 0.0;
        
        return static_cast<float>(li.QuadPart);
    }

    float GetPerformanceCounter()
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);

        return static_cast<float>(li.QuadPart);
    }

    void Sleep(uint32_t ms)
    {
        ::Sleep(ms);
    }

#else
    #error "Платформа пока не поддерживается"

    #include <unistd.h>

    void WindowCreate(WindowDesc& window, const char* name) 
    {
        THROW();
    }

    bool GetMessageWindow() 
    { 
        return false;
    }

    void *OpenLibrary(const char *name)
    {
        return nullptr;
    }

    void CloseLibrary(void *handle)
    {
    }

    void *GetProcAddr(void *handle, const char *nameProc)
    {
        return nullptr;
    }

    double GetPerformanceFrequency()
    {
        return 0.0;
    }

    double GetPerformanceCounter()
    {
        return 0.0;
    }

    void Sleep(uint32_t ms)
    {
        usleep(ms * 1000);
    }

#endif

}
