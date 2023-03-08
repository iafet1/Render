//
// avmCGxVulkan.hpp
//

#pragma once

#include "stdafx.hpp"
#include "platform/platforms.hpp"
#include "avmCGxDevice.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace avm::graphics {

    class CGxVulkan final : public CGxDevice
    {
    public:
        CGxVulkan();
        virtual ~CGxVulkan();

        CGxVulkan(const CGxVulkan &) = delete;
        CGxVulkan &operator=(const CGxVulkan &) = delete;

        // API
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // начало фрейма
        virtual bool BeginFrame(float dt);

        // конец фрейма
        virtual bool EndFrame(float dt);

        // обработчик события EVENT_APP_RESIZED (изменение размера главного окна приложения)
        static bool OnResized(void *object, union Variant param1, union Variant param2);


    private :
        // functions

        // создание экземпляра
        void createInstance();
        // уничтожение экземпляра
        void destroyInstance();

        // иницилизация функций экземпляра
        void initFunctionsInstance(VkInstance instance);
        // иницилизация функций устройства
        void initFunctionsDevice();
        // трансляция кода возврата функций Vulkan в строку
        const char *vkResultToString(VkResult result);

    private:
        // data

        // данные для графического устройства Vulkan

        #include "vulkanPrototipsFunctions.inc"

        #if defined(_LOG) || defined(_DEBUG)
            // VK_EXT_debug_utils ( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) — расширение экземпляра
            // объект мессенджера, который обрабатывает передачу сообщений отладки предоставленному обратному вызову отладки
            VkDebugUtilsMessengerEXT m_dbg_messenger{VK_NULL_HANDLE};
        #endif

        void* m_libVulkan{nullptr}; // дескриптор динамической библиотеки
        VkAllocationCallbacks *m_vkAllocator{nullptr}; // указатель на пользовательский менеджер памяти Vulkan
        VkInstance m_vkInstance {VK_NULL_HANDLE}; // дескриптор экземпляра Vulkan

    };

}
