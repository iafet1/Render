//
// avmCTimer.hpp - системный таймер
//

#pragma once

namespace avm {

    class CTimer final
    {
    public:
        // получить текущее значение, начальное значение не обновляется
        inline double Now();

        // обновить начало отсчета, возвращает истекший период
        inline double Update();

        // запуск таймера, задает начальное значение
        double Start();

        // остановка таймера, сбрасывает начальное значение
        inline void Stop() { m_startTime = 0.0; m_currentTime = 0.0; }

        // истекший период, обновляет начальное значение
        double Elapsed();

    private : 
        double m_startTime{0.0}; // начальное значение
        double m_currentTime{0.0}; // текущее время
    };
}