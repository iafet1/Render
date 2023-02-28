//
// avmCEvent.cpp - система событий
//

#include "stdafx.hpp"
#include "avmCLog.hpp"

#include "avmCEvent.h"


namespace avm {

    static avmCEvent* g_Events{nullptr};

    avmCEvent::avmCEvent()
    {
        assert(g_Events == nullptr && "Класс событий уже создан");
        memset(m_handlers, 0, sizeof(m_handlers));
        g_Events = this;
    }

    avmCEvent::~avmCEvent()
    {
        g_Events = nullptr;
    }

    void avmCEvent::SetEvent(EventApp &eventApp)
    {
        uint64_t index = _InterlockedIncrement64((LONG64 *)&m_head) - 1;
        m_queueEvent[index & INDEX_MASK_EVENT] = eventApp;
    }

    void avmCEvent::HandlingEvents()
    {
        m_countEvent = m_head - m_tail; // количество событий в очереди для обработки

        while (m_countEvent > 0)
        {
            int index = m_tail &INDEX_MASK_EVENT;
            int type = static_cast<int>(m_queueEvent[index].typeEvent);
            EventApp eventApp = m_queueEvent[index];
            for (int i = 0; i < HANDLERS_MAX; ++i) {
                if (m_handlers[type][i].object == nullptr ||
                    m_handlers[type][i].onEvent(m_handlers[type][i].object, eventApp.params)) 
                    {
                        break;
                    }
            }

            ++m_tail;
            --m_countEvent;
        }
    }

    bool avmCEvent::RegisterHandler(EventAppTypes typeEvent, HandlerEvents & he)
    {
        assert(EventAppTypes::EVENT_APP_MAX != typeEvent && "Должно быть меньше максимального");

        uint32_t type = static_cast<uint32_t>(typeEvent);
        for (int i = 0; i < HANDLERS_MAX; ++i)
        {
            if (m_handlers[type][i].object == nullptr) {
                m_handlers[type][i] = he;
            }
        }

        LOG_ERROR("Нет свободных слотов для обработчика %d типа.", type);
        return false;
    }

    void avmCEvent::UnRegisterHandler(EventAppTypes typeEvent, HandlerEvents &he)
    {
        assert(EventAppTypes::EVENT_APP_MAX != typeEvent &&
               he.object != nullptr && 
               "Не правильный тип события или нулевой объект.");

        uint32_t type = static_cast<uint32_t>(typeEvent);

        int i;
        for (i = 0; i < HANDLERS_MAX; ++i) {
            if (m_handlers[type][i].object == he.object) {
                break;
            }
        }

        if (i >= HANDLERS_MAX) {
            LOG_WARNING("Обработчик события %d не найден.", type);
            return;
        }

        for (int n = HANDLERS_MAX - 1; n >= 0; --n)
        {
            if (m_handlers[type][n].object != nullptr)
            {
                m_handlers[type][i] = m_handlers[type][n];
                m_handlers[type][n] = {nullptr, nullptr};
            }
        }
    }
}