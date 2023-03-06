//
// avmCTimer.cpp - системный таймер
//

#include "stdafx.hpp"
#include "platform/platforms.hpp"
#include "avmCTimer.hpp"

namespace avm {

    static double g_Frequency { (1.0 / platform::GetPerformanceFrequency()) }; // тиков в секунду

    inline double CTimer::Now()
    {
        m_currentTime = platform::GetPerformanceCounter() * g_Frequency;
        return m_currentTime;
    }

    inline double CTimer::Update()
    {
        double elapsed = m_currentTime - m_startTime;
        m_startTime = m_currentTime;
        return elapsed;
    }

    double CTimer::Start()
    {
        m_startTime = platform::GetPerformanceCounter() * g_Frequency;
        return m_startTime;
    }

    double CTimer::Elapsed()
    {
        Now();
        return Update();
    }
    
}