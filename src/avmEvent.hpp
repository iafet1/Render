//
// avmEvent.hpp - система событий
//

#pragma once

namespace avm::event {

    // Типы событий приложения
    enum class AppTypes
    {
        EVENT_APP_COMMAND = 0,     // событие комманда
        EVENT_APP_KEY_PRESSED,     // событие нажатие клавиши клавиатуры
        EVENT_APP_KEY_RELEASED,    // событие отпускания клавиши клавиатуры
        EVENT_APP_BUTTON_PRESSED,  // событие нажатие кнопки мыши
        EVENT_APP_BUTTON_RELEASED, // событие отпускания кнопки мыши
        EVENT_APP_MOUSE_MOVED,     // событие перемещение мыши
        EVENT_APP_MOUSE_WHEEL,     // событие прокрутка колесика мыши
        EVENT_APP_RESIZED,         // изменение размера главного окна приложения
        EVENT_APP_QUIT,            // завершение программы

        EVENT_APP_MAX              // максимальное количество типов событий приложения
    };

    // функция обратного вызова события. если возвращает 'true', то следующий обработчик не вызывается
    typedef bool (*PFN_OnEvent)(void *object, union Variant param1, union Variant param2);
    
    // API
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // установка события
    void SetEvent(AppTypes type, union Variant param1, union Variant param2);

    // обработка событий в текущем кадре
    void HandlingEvents();

    // регистрация обработчика события
    bool RegisterHandler(AppTypes typeEvent, void *object, PFN_OnEvent pFunction);

    // удаление регистрации обработчика события
    void UnRegisterHandler(AppTypes typeEvent, void *object);
}