//
// avmEvent.hpp - система событий
//

#pragma once

#include "stdafx.hpp"

namespace avm::event {

    // Типы событий приложения
    enum class AppTypes
    {
        EVENT_APP_KEYBOARD = 0, // событие клавиатуры
        EVENT_APP_MOUSE,        // событие мыши
        EVENT_APP_QUIT,         // завершение программы
        EVENT_APP_RESIZE,       // изменение размера главного окна приложения
        
        EVENT_APP_MAX           // максимальное количество типов событий приложения
    };

    // функция обратного вызова события
    typedef bool (*PFN_OnEvent)(void *object, Variant param1, Variant param2);
    
    // API
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // установка события
    void SetEvent(AppTypes type, Variant param1, Variant param2);

    // обработка событий в текущем кадре
    void HandlingEvents();

    // регистрация обработчика события
    bool RegisterHandler(AppTypes typeEvent, void *object, PFN_OnEvent pFunction);

    // удаление регистрации обработчика события
    void UnRegisterHandler(AppTypes typeEvent, void *object);
}