//
// avmCApplication.cpp
//

#include "stdafx.hpp"
#include "avmCLog.hpp"
#include "avmCApplication.hpp"

constexpr char FILE_NAME[] = "avmCApplication.cpp";

using namespace avm;

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

    windowCreate(m_width, m_heght);
    m_initialized = true;

}

void avm::CApplication::shutdown()
{
    assert(m_initialized);

    #if defined(_WIN32)
        m_hInst = nullptr;
        m_hWnd = nullptr;
    #else
        #error "Not support"   
    #endif //     

    m_initialized = false;
}

#if defined(_WIN32)

// оконная процедура
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

void avm::CApplication::windowCreate(uint32_t width, uint32_t height)
{
    assert(m_hWnd == nullptr);

    m_hInst = GetModuleHandle(NULL);   // получить дескриптор приложения

    WNDCLASSEX  wcex = {   // Содержит информацию о классе окна.
        .cbSize         = sizeof(WNDCLASSEX),    // Размер в байтах этой структуры.
        .style          = CS_DBLCLKS,            // Стиль класса.
        .lpfnWndProc    = WndProc,               // Указатель на оконную процедуру.
        .cbClsExtra     = 0,                     // Количество дополнительных байтов, выделяемых в соответствии со структурой оконного класса.
        .cbWndExtra     = sizeof(void*),         // Количество дополнительных байтов, выделяемых после экземпляра окна.
        .hInstance      = m_hInst,               // Дескриптор экземпляра, содержащего оконную процедуру для класса.
        .hIcon          = NULL,                  // Дескриптор значка класса.
        .hCursor        = NULL,                  // Дескриптор курсора класса.
        .hbrBackground  = NULL,                  // (HBRUSH)COLOR_BACKGROUND;  // Дескриптор фоновой кисти класса.
        .lpszMenuName   = NULL,                  // Указатель на символьную строку с завершающим нулем, которая определяет имя ресурса меню класса.
        .lpszClassName  = m_name,                // Указатель на строку с завершающим нулем или является атомом.
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
        .right  = ((LONG)width),
        .bottom = ((LONG)height)
    };

    AdjustWindowRectEx(&rc, style, FALSE, exStyle);

    m_width = rc.right - rc.left;
    m_heght = rc.bottom - rc.top;

    // создание главного окна приложения
    m_hWnd = CreateWindowEx(exStyle, m_name, m_name, style, CW_USEDEFAULT, CW_USEDEFAULT,
                                   m_width, m_heght, NULL, NULL, m_hInst, NULL);

    if (NULL == m_hWnd) {
        LOG_ERROR("Не могу создать главное окно приложения.");
        THROW();;
    }
    
    GetClientRect(m_hWnd, &rc);
    LOG_DEBUG("Ширина - %d, Высота - %d.", rc.right, rc.bottom);

    ShowWindow(m_hWnd, SW_SHOW);

    return;
}

void avm::CApplication::getMessageWindow()
{
    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        switch (msg.message)
        {
            case WM_QUIT:
                SetExitCode(msg.wParam);
                break;

            default:
                break;;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

#else
    #error "Not support"   
#endif // 