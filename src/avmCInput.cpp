//
// avmCInput.cpp - система ввода
//

#include "stdafx.hpp"
#include "avmCLog.hpp"
#include "key_code.hpp"
#include "avmEvent.hpp"

#include "avmCInput.hpp"

namespace avm {

    CInput* g_input{nullptr};

    CInput::CInput()
    {
        assert(g_input == nullptr && "Нельзя создавать больше одного экземпляра.");
        g_input = this;
    }

    CInput::~CInput()
    {
        if (g_input != nullptr)
            g_input = nullptr;
    }

    void CInput::KeyPressed(uint8_t key_code)
    {
        m_stateKeyboard[key_code] = 0xFF;
        Variant code{.u8 = key_code};
        Variant mode{.i64 = 0};

        switch(key_code) {
            case static_cast<uint8_t>(KeyCode::KEY_CODE_LSHIFT):
            case static_cast<uint8_t>(KeyCode::KEY_CODE_RSHIFT): {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_SHIFT)] = 0xFF;
                break;
            }

            case static_cast<uint8_t>(KeyCode::KEY_CODE_LCONTROL):
            case static_cast<uint8_t>(KeyCode::KEY_CODE_RCONTROL):
            {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_CONTROL)] = 0xFF;
                break;
            }

            case static_cast<uint8_t>(KeyCode::KEY_CODE_LMENU):
            case static_cast<uint8_t>(KeyCode::KEY_CODE_RMENU):
            {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_MENU)] = 0xFF;
                break;
            }

            case static_cast<uint8_t>(KeyCode::KEY_CODE_LWIN):
            case static_cast<uint8_t>(KeyCode::KEY_CODE_RWIN):
            {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_WIN)] = 0xFF;
                break;
            }
        }

        //LOG_DEBUG("Клавиша [%x] НАЖАТА.", key_code);
        event::SetEvent(event::AppTypes::EVENT_APP_KEY_PRESSED, code, mode);
    }

    void CInput::KeyReleased(uint8_t key_code)
    {
        m_stateKeyboard[key_code] = 0x00;
        Variant code{.u8 = key_code};
        Variant mode{.i64 = 0};

        switch (key_code)
        {
        case static_cast<uint8_t>(KeyCode::KEY_CODE_LSHIFT):
        {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_SHIFT)] &= 0x0F;
                break;
        }
        case static_cast<uint8_t>(KeyCode::KEY_CODE_RSHIFT):
        {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_SHIFT)] &= 0xF0;
                break;
        }

        case static_cast<uint8_t>(KeyCode::KEY_CODE_LCONTROL):
        {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_CONTROL)] &= 0x0F;
                break;
        }
        case static_cast<uint8_t>(KeyCode::KEY_CODE_RCONTROL):
        {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_CONTROL)] &= 0xF0;
                break;
        }

        case static_cast<uint8_t>(KeyCode::KEY_CODE_LMENU):
        {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_MENU)] &= 0x0F;
                break;
        }
        case static_cast<uint8_t>(KeyCode::KEY_CODE_RMENU):
        {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_MENU)] = 0xF0;
                break;
        }

        case static_cast<uint8_t>(KeyCode::KEY_CODE_LWIN):
        {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_WIN)] &= 0x0F;
                break;
        }
        case static_cast<uint8_t>(KeyCode::KEY_CODE_RWIN):
        {
                m_stateKeyboard[static_cast<uint8_t>(KeyCode::KEY_CODE_WIN)] &= 0xF0;
                break;
        }
        }

        //LOG_DEBUG("Клавиша [%x] ОТПУЩЕНА.", key_code);
        event::SetEvent(event::AppTypes::EVENT_APP_KEY_RELEASED, code, mode);
    }

    void CInput::ButtonPressed(uint32_t code, POINT pt)
    {
        setStateMouse(code, pt);
        if ((code >> 16) & XBUTTON1) {
            m_stateMouse.buttonMode |= MouseCode::MOUSE_CODE_4_BUTTON;
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON1)] = 0xFF;
        }
        else {
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON1)] = 0x00;
        }

        if ((code >> 16) & XBUTTON2) {
            m_stateMouse.buttonMode |= MouseCode::MOUSE_CODE_4_BUTTON;
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON2)] = 0xFF;
        }
        else {
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON2)] = 0x00;
        }

        Variant mode, position;
        position.iData32[0] = pt.x;
        position.iData32[1] = pt.y;
        mode.i32 = m_stateMouse.buttonMode;

        event::SetEvent(event::AppTypes::EVENT_APP_BUTTON_PRESSED, mode, position);
    }

    void CInput::ButtonReleased(uint32_t code, POINT pt)
    {
        setStateMouse(code, pt);
        if ((code >> 16) & XBUTTON1)
        {
            m_stateMouse.buttonMode |= MouseCode::MOUSE_CODE_4_BUTTON;
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON1)] = 0xFF;
        }
        else
        {
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON1)] = 0x00;
        }

        if ((code >> 16) & XBUTTON2)
        {
            m_stateMouse.buttonMode |= MouseCode::MOUSE_CODE_4_BUTTON;
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON2)] = 0xFF;
        }
        else
        {
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON2)] = 0x00;
        }

        Variant mode, position;
        position.iData32[0] = pt.x;
        position.iData32[1] = pt.y;
        mode.i32 = m_stateMouse.buttonMode;

        event::SetEvent(event::AppTypes::EVENT_APP_BUTTON_RELEASED, mode, position);
    }

    void CInput::MouseMove(uint32_t code, POINT pt)
    {
        setStateMouse(code, pt);

        Variant mode, position;
        position.iData32[0] = pt.x;
        position.iData32[1] = pt.y;
        mode.i32 = m_stateMouse.buttonMode;

        event::SetEvent(event::AppTypes::EVENT_APP_MOUSE_MOVED, mode, position);
    }

    void CInput::MouseWheel(uint32_t code, POINT pt)
    {
        setStateMouse(code, pt);
        m_stateMouse.wheel = ((int16_t)(code >> 16)) > 0 ? 1 : -1;

        Variant mode, position;
        position.iData32[0] = pt.x;
        position.iData32[1] = pt.y;
        mode.iData32[0] = m_stateMouse.buttonMode;
        mode.iData32[1] = m_stateMouse.wheel;

        event::SetEvent(event::AppTypes::EVENT_APP_MOUSE_WHEEL, mode, position);
    }

    bool CInput::GetKeyState(uint8_t code)
    {
        return (m_stateKeyboard[code] > 0);
    }

    void CInput::GetStateKeyboard(uint8_t stateKeyboard[])
    {
        uint64_t* dst = (uint64_t*)stateKeyboard;
        uint64_t *src = (uint64_t*)m_stateKeyboard;
        for(int i = 0; i < 32; ++i) {
            *dst = *src;
            ++dst;
            ++src;
        }
    }

    void CInput::setStateMouse(uint32_t code, POINT pt)
    {
        uint32_t mode{0};

        if (code & MK_LBUTTON)
        {
            mode |= MouseCode::MOUSE_CODE_LBUTTON;
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_LBUTTON)] = 0xFF;
        }
        else {
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_LBUTTON)] = 0x00;
        }

        if (code & MK_RBUTTON)
        {
            mode |= MouseCode::MOUSE_CODE_RBUTTON;
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_RBUTTON)] = 0xFF;
        }
        else {
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_RBUTTON)] = 0x00;
        }

        if (code & MK_MBUTTON)
        {
            mode |= MouseCode::MOUSE_CODE_MBUTTON;
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_MBUTTON)] = 0xFF;
        }
        else {
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_MBUTTON)] = 0x00;
        }

        if (code & MK_XBUTTON1)
        {
            mode |= MouseCode::MOUSE_CODE_4_BUTTON;
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON1)] = 0xFF;
        }
        else {
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON1)] = 0x00;
        }

        if (code & MK_XBUTTON2)
        {
            mode |= MouseCode::MOUSE_CODE_5_BUTTON;
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON2)] = 0xFF;
        }
        else {
            m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_XBUTTON2)] = 0x00;
        }

        if (code & MK_SHIFT)
        {
            mode |= MouseCode::MOUSE_CODE_MODE_LSHIFT;
            mode |= MouseCode::MOUSE_CODE_MODE_RSHIFT;
        }
        if (code & MK_CONTROL)
        {
            mode |= MouseCode::MOUSE_CODE_MODE_LCTRL;
            mode |= MouseCode::MOUSE_CODE_MODE_RCTRL;
        }
        if (m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_MENU)] > 0)
        {
            mode |= MouseCode::MOUSE_CODE_MODE_LALT;
            mode |= MouseCode::MOUSE_CODE_MODE_RALT;
        }
        if (m_stateKeyboard[static_cast<int>(KeyCode::KEY_CODE_WIN)] > 0)
        {
            mode |= MouseCode::MOUSE_CODE_MODE_LWIN;
            mode |= MouseCode::MOUSE_CODE_MODE_RWIN;
        }

        m_stateMouse.xPos = pt.x;
        m_stateMouse.yPos = pt.y;
        m_stateMouse.buttonMode = mode;
    }

    CInput* GetInputClass()
    {
        return g_input;
    }

}