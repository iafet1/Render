//
// avmCGxVulkan.hpp
//

#pragma once

#include "../../stdafx.hpp"
#include "../../platform/platforms.hpp"
#include "../avmCGxDevice.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace avm::graphics
{

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

    private:
        // functions

        // создание экземпляра
        void createInstance();
        // уничтожение экземпляра
        void destroyInstance();

        // создание устройства Vulkan
        void createDevice();
        // уничтожение устройства Vulkan
        void destroyDevice();

        // иницилизация функций экземпляра
        void initFunctionsInstance(VkInstance instance);
        // иницилизация функций устройства
        void initFunctionsDevice(VkDevice device);
        // поиск необходимого GPU
        VkPhysicalDevice choosePhysicalDevice();
        // проверка поддержки представления для поверхности (зависит от платформы)
        bool checkPresentationSupport(VkPhysicalDevice gpu, uint32_t queueFamilyIndex);
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

        void *m_libVulkan{nullptr};                    // дескриптор динамической библиотеки
        VkAllocationCallbacks *m_vkAllocator{nullptr}; // указатель на пользовательский менеджер памяти Vulkan
        VkInstance m_vkInstance{VK_NULL_HANDLE};       // дескриптор экземпляра Vulkan

        VkPhysicalDevice m_vkPhysicalDevice{VK_NULL_HANDLE}; // физическое графическое устройство (GPU)
        VkDevice m_vkDevice{VK_NULL_HANDLE};                 // логическое устройство Vulkan

        // индексы семейств очередей и очереди
        std::vector<uint32_t> m_vkFamilies;                         // индексы семейств очередей
        std::vector<VkQueueFamilyProperties> m_vkQueueFamiliesProp; // массив структур, предоставляющих информацию о семействах очередей

        uint32_t m_vkGraphicsFamily{VK_QUEUE_FAMILY_IGNORED}; // индекс семейства очередей, поддерживающих обработку графики
        uint32_t m_vkComputeFamily{VK_QUEUE_FAMILY_IGNORED};  // индекс семейства очередей, поддерживающих вычисления
        uint32_t m_vkTransferFamily{VK_QUEUE_FAMILY_IGNORED}; // индекс семейства очередей, поддерживающих, передачу
        uint32_t m_vkPresentFamily{VK_QUEUE_FAMILY_IGNORED};  // индекс семейства очередей, поддерживающих, представление

        VkQueue m_vkGraphicsQueue{VK_NULL_HANDLE}; // очередь обработки графики
        VkQueue m_vkComputeQueue{VK_NULL_HANDLE};  // очередь вычислений
        VkQueue m_vkTransferQueue{VK_NULL_HANDLE}; // очередь передачи
        VkQueue m_vkPresentQueue{VK_NULL_HANDLE};  // очередь представления
        
    };
}
