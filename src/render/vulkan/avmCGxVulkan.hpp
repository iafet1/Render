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
    // типы очередей
    enum QueueType
    {
        QT_GRAPHICS = 0, // графическая
        QT_COMPUTE,      // вычисления
        QT_TRANSFER,     // передачи
        QT_PRESENT,      // представления

        QT_MAX
    };

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

        // создание поверхности (холста) для вывода изображения
        virtual void CreateCanvas(platform::WindowDesc *window);

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

        // создание поверхности Vulkan
        void createSurface(platform::WindowDesc *window);
        // уничтожение поверхности Vulkan
        void destroySurface();
        // запрос форматов поверхности
        void getSurfaceFormat();
        // Запрос размеров и преобразования поверхности
        void getSurfaceCapabilities();
        // Запрос и установка режима представления
        void getSurfacePresentMode();
        // создание цепочки подкачки
        void createSwapChain();
        // уничтожение цепочки подкачки
        void destroySwapChain();
        
        // иницилизация функций экземпляра
        void initFunctionsInstance(VkInstance instance);
        // иницилизация функций устройства
        void initFunctionsDevice(VkDevice device);

        // поиск необходимого GPU
        VkPhysicalDevice choosePhysicalDevice();
        // проверка поддержки представления для поверхности (зависит от платформы)
        bool checkPresentationSupport(VkPhysicalDevice gpu, uint32_t queueFamilyIndex);
        // сделать информацию очередей для логического устройства
        void buildQueueInfos(uint32_t *countQueueInFamily);

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

        uint32_t m_vkQueueFamily[QueueType::QT_MAX]{
            VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
            VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED}; // индекс семейства очередей для данной очереди
        uint32_t m_vkQueueIndex[QueueType::QT_MAX]{};          // индекс очереди, при запросе у логического устройства
        VkQueue m_vkQueue[QueueType::QT_MAX]{VK_NULL_HANDLE};  // дескриптор очереди
        

        // поверхность представления
        VkSurfaceKHR m_vkSurface{VK_NULL_HANDLE};       // дескриптор поверхности Vulkan
        
        // поддерживаемые свойства
        VkExtent2D m_vkSurfaceResolution;               // размеры поверхности
        VkColorSpaceKHR m_vkColorSpace;                 // цветовое пространство поверхности
        VkFormat m_vkColorFormat;                       // цветовой формат поверхности
        VkPresentModeKHR m_vkPresentationMode;          // режим представления
        VkSurfaceTransformFlagBitsKHR m_vkPreTransform; // преобразование относительно представления

        // цепочка обмена (буфферизация)
        VkSwapchainKHR m_vkSwapChain{VK_NULL_HANDLE};         // цепочка обмена(подкачки) изображения
        uint32_t m_swapchainImageCount{MAX_FRAMES_IN_FLIGHT}; // количество изображений в цепочке обмена
        VkImage *m_vkSwapchainImages{nullptr};         // массив изображений в цепочке обмена
        VkImageView *m_vkSwapchainImagesView{nullptr}; // массив представлений изображений цепочки обмена
    };
}
