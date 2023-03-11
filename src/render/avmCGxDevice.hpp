//
// avmCGxDevice.hpp
//

#pragma once

#include "../stdafx.hpp"

namespace avm::graphics {

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



    private:
        // functions


    protected:
        // data
        uint64_t m_indexFrame{0}; // индекс тикущего фрейма изображения


    };

}
