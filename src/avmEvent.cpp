//
// avmEvent.cpp - система событий
//

#include "stdafx.hpp"
#include "key_code.hpp"

#include "avmCLog.hpp"
#include "avmEvent.hpp"

namespace avm::event
{

// данные события приложения
struct EventApp
{
    long long tickApp; // номер тика фрейма состояния приложения
    AppTypes typeEvent;
    Params params;
};

// пул событий
struct Events
{
    // data
#define COUNT_QUEUE_EVENT 1024
#define INDEX_MASK_EVENT (COUNT_QUEUE_EVENT - 1)
#define HANDLERS_MAX 4

    // массив обработчиков событий
    HandlerEvents m_handlers[(uint64_t)AppTypes::EVENT_APP_MAX][HANDLERS_MAX]{};

    EventApp m_queueEvent[COUNT_QUEUE_EVENT]{}; // очередь событий

    uint64_t m_head{0};       // начало (голова) очереди событий
    uint64_t m_tail{0};       // конец (хвост) очереди событий
    uint64_t m_countEvent{0}; // количество событий в кадре

};

// пул событий
static Events g_events{};

void SetEvent(AppTypes type, Params params)
{
    assert(((g_events.m_head - g_events.m_tail) < INDEX_MASK_EVENT) &&
           "Нет места в очереди событий.");

    uint64_t index = _InterlockedIncrement64((LONG64 *)&g_events.m_head) - 1;
    index = index & INDEX_MASK_EVENT;
    //g_events.m_queueEvent[index].tickApp = tick;
    g_events.m_queueEvent[index].typeEvent = type;
    g_events.m_queueEvent[index].params = params;
}

void HandlingEvents()
{
    uint64_t countEvents = g_events.m_head - g_events.m_tail; // количество событий в очереди для обработки

    while (countEvents > 0)
    {
        int index = g_events.m_tail & INDEX_MASK_EVENT;
        EventApp& eventApp = g_events.m_queueEvent[index];
        int type = static_cast<int>(eventApp.typeEvent);

        for (int i = 0; i < HANDLERS_MAX; ++i)
        {
            HandlerEvents& handler = g_events.m_handlers[type][i];
            if (handler.object == nullptr ||
                handler.onEvent(handler.object, eventApp.params))
            {
                break;
            }
        }

        ++g_events.m_tail;
        --countEvents;
    }
}

bool RegisterHandler(AppTypes typeEvent, HandlerEvents &he)
{
    assert(AppTypes::EVENT_APP_MAX != typeEvent && 
           "Должно быть меньше максимального");

    uint32_t type = static_cast<uint32_t>(typeEvent);
    for (int i = 0; i < HANDLERS_MAX; ++i)
    {
        if (g_events.m_handlers[type][i].object == he.object) {
            LOG_ERROR("Событие %d уже зарегистрированно.", type);
            return false;
        }

        if (g_events.m_handlers[type][i].object == nullptr)
        {
            g_events.m_handlers[type][i] = he;
            return true;
        }
    }

    LOG_ERROR("Нет свободных слотов для обработчика %d типа.", type); assert(false);
    return false;
}

void UnRegisterHandler(AppTypes typeEvent, HandlerEvents &he)
{
    assert(AppTypes::EVENT_APP_MAX != typeEvent &&
           he.object != nullptr &&
           "Не правильный тип события или нулевой объект.");

    uint32_t type = static_cast<uint32_t>(typeEvent);

    int i;
    for (i = 0; i < HANDLERS_MAX; ++i)
    {
        if (g_events.m_handlers[type][i].object == he.object)
        {
            break;
        }
    }

    if (i >= HANDLERS_MAX)
    {
        LOG_WARNING("Обработчик события %d не найден.", type);
        return;
    }

    for (int n = HANDLERS_MAX - 1; n >= 0; --n)
    {
        if (g_events.m_handlers[type][n].object != nullptr)
        {
            g_events.m_handlers[type][i] = g_events.m_handlers[type][n];
            g_events.m_handlers[type][n] = {nullptr, nullptr};
        }
    }
}

}