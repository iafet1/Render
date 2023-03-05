//
// avmCTimer.cpp - системный таймер
//

#include "stdafx.hpp"
#include "platform/platforms.hpp"
#include "avmCTimer.hpp"

namespace avm {

    double CTimer::Start()
    {
        m_Frequency = 1.0 / platform::GetPerformanceFrequency();
        assert(m_Frequency != 0.0 && "Ошибка запроса частоты.");

        m_startTime = platform::GetPerformanceCounter() * m_Frequency;
        return m_startTime;
    }

    inline double CTimer::Now()
    {
        m_currentTime = platform::GetPerformanceCounter() * m_Frequency;
        return m_currentTime;
    }

    double CTimer::Elapsed()
    {
        Now();
        return Update();
    }
    
    inline double CTimer::Update()
    {
        double elapsed = m_currentTime - m_startTime;
        m_startTime = m_currentTime;
        return elapsed;
    }

}