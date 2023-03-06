//
// avmCTimer.cpp - системный таймер
//

#include "stdafx.hpp"
#include "platform/platforms.hpp"
#include "avmCTimer.hpp"

namespace avm {

    static float g_Frequency { (1.0f / platform::GetPerformanceFrequency()) }; // тиков в секунду

    inline float CTimer::Now()
    {
        m_currentTime = platform::GetPerformanceCounter() * g_Frequency;
        return m_currentTime;
    }

    inline float CTimer::Update()
    {
        float elapsed = m_currentTime - m_startTime;
        m_startTime = m_currentTime;
        return elapsed;
    }

    float CTimer::Start()
    {
        m_startTime = platform::GetPerformanceCounter() * g_Frequency;
        return m_startTime;
    }

    float CTimer::Elapsed()
    {
        Now();
        return Update();
    }
    
}