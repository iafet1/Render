//
// avmCTimer.hpp - системный таймер
//

#pragma once

namespace avm {

    struct CTimer final
    {
    public:
        // запуск таймера, задает начальное значение
        double Start();

        // получить текущее значение, начальное значение не обновляется
        inline double Now();

        // истекший период, обновляет начальное значение
        double Elapsed();

        // обновить начало отсчета, возвращает истекший период
        inline double Update();

        // остановка таймера, сбрасывает начальное значение
        inline void Stop() { m_startTime = 0.0; m_currentTime = 0.0; }

    private : 
        double m_Frequency{0.0}; // тиков в секунду
        double m_startTime{0.0}; // начальное значение
        double m_currentTime{0.0}; // текущее время
    };
}