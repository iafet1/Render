//
// avmCEvent.h - система событий
//

#pragma once

#include "stdafx.hpp"

namespace avm {

    // Типы событий приложения
    enum class EventAppTypes
    {
        EVENT_APP_KEYBOARD = 0, // событие клавиатуры
        EVENT_APP_MOUSE,        // событие мыши
        EVENT_APP_QUIT,         // завершение программы
        EVENT_APP_RESIZE,       // изменение размера главного окна приложения
        EVENT_APP_MAX           // максимальное количество типов событий приложения
    };

    // параметры или данные (16 байт)
    union EventParams
    {
        int64_t m_64[2]; // 64-битные данные
        int32_t m_32[4]; // 32-битные данные
        int8_t m_8[16];  // 8-битные данные
    };

    // данные обработчика событий
    struct HandlerEvents
    {
        void *object; // объект регистриющий функцию события
        bool (*onEvent)(void* object, EventParams params); // функция обратного вызова
    };


    // данные события приложения
    struct EventApp
    {
        uint64_t tickApp; // номер тика фрейма состояния приложения
        EventAppTypes typeEvent;
        EventParams params;
    };

    class avmCEvent final
    {
    public:
        avmCEvent();
        ~avmCEvent();

        // API
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // установка события
        void SetEvent(EventApp &eventApp);

        // обработка событий в текущем кадре
        void HandlingEvents();

        // регистрация обработчика события
        bool RegisterHandler(EventAppTypes typeEvent, HandlerEvents &he);

        // удаление регистрации обработчика события
        void UnRegisterHandler(EventAppTypes typeEvent, HandlerEvents &he);

    private:
        // functions

    private:
        // data
        #define COUNT_QUEUE_EVENT 1024
        #define INDEX_MASK_EVENT (COUNT_QUEUE_EVENT - 1)
        #define HANDLERS_MAX 4

        // массив обработчиков событий
        HandlerEvents m_handlers[(uint64_t)EventAppTypes::EVENT_APP_MAX][HANDLERS_MAX]{};

        EventApp m_queueEvent[COUNT_QUEUE_EVENT]{}; // очередь событий

        uint64_t m_head{0}; // начало (голова) очереди событий
        uint64_t m_tail{0}; // конец (хвост) очереди событий
        uint64_t m_countEvent{0}; // количество событий в кадре

    };
        
}
