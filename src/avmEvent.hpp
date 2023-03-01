//
// avmEvent.hpp - система событий
//

#pragma once

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

    // параметры или данные (16 байт)
    union Params
    {
        long long m_64[2]; // 64-битные данные
        int       m_32[4]; // 32-битные данные
        char      m_8[16];  // 8-битные данные
    };

    // данные обработчика событий
    struct HandlerEvents
    {
        void *object;                                 // объект регистриющий функцию события
        bool (*onEvent)(void *object, Params params); // функция обратного вызова
    };

    // API
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // установка события
    void SetEvent(AppTypes type, Params params);

    // обработка событий в текущем кадре
    void HandlingEvents();

    // регистрация обработчика события
    bool RegisterHandler(AppTypes typeEvent, HandlerEvents &he);

    // удаление регистрации обработчика события
    void UnRegisterHandler(AppTypes typeEvent, HandlerEvents &he);
}