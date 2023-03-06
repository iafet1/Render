//
// avmCTimer.hpp - системный таймер
//

#pragma once

namespace avm {

    class CTimer final
    {
    public:
        // получить текущее значение в секундах, начальное значение не обновляется
        inline float Now();

        // обновить начало отсчета, возвращает истекший период в секундах
        inline float Update();

        // запуск таймера, задает начальное значение в секундах
        float Start();

        // остановка таймера, сбрасывает начальное значение
        inline void Stop() { m_startTime = 0.0; m_currentTime = 0.0; }

        // истекший период в секундах, обновляет начальное значение
        float Elapsed();

    private :
        float m_startTime{0.0};   // начальное значение в секундах
        float m_currentTime{0.0}; // текущее время в секундах
    };
}