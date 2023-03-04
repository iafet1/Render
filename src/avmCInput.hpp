//
// avmCInput.hpp - система ввода
//

#pragma once

#include "stdafx.hpp"

namespace avm {

    // состояние мыши
    struct StateMouse {
        int32_t xPos;
        int32_t yPos;
        int32_t buttonMode;
        int32_t wheel;
    };

    class CInput final
    {
    public:
        CInput();
        ~CInput();

        // удаление
        CInput(const CInput &) = delete;
        CInput &operator=(const CInput &) = delete;

        // API
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // нажатие клавиши клавиатуры
        void KeyPressed(uint8_t key_code);

        // отпускание клавиши клавиатуры
        void KeyReleased(uint8_t key_code);

        // нажатие кнопки мыши
        void ButtonPressed(uint32_t code, POINT pt);

        // отпускание кнопки мыши
        void ButtonReleased(uint32_t code, POINT pt);

        // премещение мыши
        void MouseMove(uint32_t code, POINT pt);

        // прокрутка колесика мыши
        void MouseWheel(uint32_t code, POINT pt);

        // получить состояние клавиши (true- нажата, false - отпущена)
        bool GetKeyState(uint8_t code);

        // получить состояние всех клавиш
        void GetStateKeyboard(uint8_t stateKeyboard[]);

    private:
        // function

        // установка состояния мыши
        void setStateMouse(uint32_t code, POINT pt);


    private:
        // data

        uint8_t m_stateKeyboard[256]{}; // состояние клавиатуры
        StateMouse m_stateMouse{}; // состояние мыши
    };
    
    // получить указатель на систему ввода
    CInput* GetInputClass();
}