//
// avmCGxDevice.hpp
//

#pragma once

#include "stdafx.hpp"
#include "platform/platforms.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace avm::graphics {

    class CGxDevice
    {
    public:
        CGxDevice();
        ~CGxDevice();

        CGxDevice(const CGxDevice &) = delete;
        CGxDevice &operator=(const CGxDevice &) = delete;

        // API

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
    
    // получить указатель на графическое устройство
    inline CGxDevice* GetGraphicsDevice();

}
