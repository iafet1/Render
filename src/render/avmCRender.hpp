//
//
//

#pragma once

#include "../stdafx.hpp"
#include "avmCGxDevice.hpp"


namespace avm {

    enum class GxDeviceType {
        VULKAN,
        DIRECTX_12,
        DIRECTX_11,
        OPENGL_4
    };
    
    class CRender final
    {
    public:
        CRender(GxDeviceType type = GxDeviceType::VULKAN);
        ~CRender();

        // удаление
        CRender() = delete;
        CRender(const CRender &) = delete;
        CRender &operator=(const CRender &) = delete;

        // API
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // запрос указателя графического устройства
        inline graphics::CGxDevice* GetGxDevice() { return m_device; }

        // начало фрейма
        bool BeginFrame(float dt);

        // конец фрейма
        bool EndFrame(float dt);

        // отрисовка
        bool Render(float dt);

        // вывод на экран
        bool Present(float dt);

        // установка поверхности (холста) для вывода изображения
        bool SetCanvas(struct platform::WindowDesc *window);

    private:
        // functions


    private:
        // data
        graphics::CGxDevice* m_device{nullptr}; // графическое устройство

    };

}