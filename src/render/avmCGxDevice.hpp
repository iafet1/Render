//
// avmCGxDevice.hpp
//

#pragma once

#include "../stdafx.hpp"
#include "../platform/platforms.hpp"

namespace avm::graphics
{
    // максимальное количество буферов подкачки
    constexpr int MAX_FRAMES_IN_FLIGHT{3};

    // Интерфейс графического драйвера
    class CGxDevice
    {
    public:
        CGxDevice();
        virtual ~CGxDevice();

        CGxDevice(const CGxDevice &) = delete;
        CGxDevice &operator=(const CGxDevice &) = delete;

        // API
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // начало фрейма
        virtual bool BeginFrame(float dt) = 0;

        // конец фрейма
        virtual bool EndFrame(float dt) = 0;

        // создание поверхности (холста) для вывода изображения
        virtual void CreateCanvas(platform::WindowDesc *window) = 0;

    private:
        // functions

    protected:
        // data
        uint64_t m_indexFrame{0}; // индекс тикущего фрейма изображения

        bool m_resize{false};
    };

}
