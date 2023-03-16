//
// avmVulkanCanvas.cpp
//

#include "../../stdafx.hpp"
#include "../../avmCLog.hpp"
#include "../../platform/platforms.hpp"

#include <algorithm>

#include "avmCGxVulkan.hpp"

constexpr char FILE_NAME[] = "avmVulkanCanvas.cpp";

namespace avm::graphics
{

    void CGxVulkan::CreateCanvas(platform::WindowDesc *window)
    {
        LOG_DEBUG("CGxVulkan::CreateCanvas(%x)", window);
        assert(window != nullptr);

        destroySurface();
        destroySwapChain();

        createSurface(window);
        createSwapChain();

        return;
    }

    void CGxVulkan::createSurface(platform::WindowDesc *window)
    {
#if defined(VK_USE_PLATFORM_WIN32_KHR)
        VkWin32SurfaceCreateInfoKHR surfaceCreateinfo = {
            .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, // является типом этой структуры
            .pNext = nullptr,                                         // указатель на структуру, расширяющую эту структуру. должен быть NULL
            .flags = 0,                                               // флаги, зарезервировано для использования в будущем. должен быть 0
            .hinstance = window->hInstance,                           // HINSTANCE для окна Win32, с которым ассоциируется поверхность
            .hwnd = window->hWnd                                      // HWND для окна Win32, с которым ассоциируется поверхность
        };

        VkResult result = vkCreateWin32SurfaceKHR(m_vkInstance, &surfaceCreateinfo, m_vkAllocator, &m_vkSurface);
        if (result != VK_SUCCESS)
        {
            LOG_ERROR("Немогу создать поверхность Vulkan. %s", vkResultToString(result));
            THROW();
        }

#else
#error "Not support"
#endif

        m_vkSurfaceResolution.width = window->width;
        m_vkSurfaceResolution.height = window->height;

        LOG_INFO("Поверхность Vulkan создана.");
    }

    void CGxVulkan::destroySurface()
    {
        // уничтожить поверхность Vulkan
        if (m_vkSurface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, m_vkAllocator);
            m_vkSurface = VK_NULL_HANDLE;
            LOG_INFO("Поверхность Vulkan уничтожена.");
        }
    }

    void CGxVulkan::getSurfaceFormat()
    {
        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_vkPhysicalDevice, m_vkSurface, &formatCount, NULL);

        std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
        VkResult res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_vkPhysicalDevice, m_vkSurface, &formatCount, surfaceFormats.data());
        if (VK_SUCCESS != res)
        {
            LOG_DEBUG("ERROR: vkGetPhysicalDeviceSurfaceFormatsKHR().");
            THROW();
        }

        if (formatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
        {
            m_vkColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
        }
        else
        {
            m_vkColorFormat = surfaceFormats[0].format;
        }

        m_vkColorSpace = surfaceFormats[0].colorSpace;
    }

    void CGxVulkan::getSurfaceCapabilities()
    {
        VkSurfaceCapabilitiesKHR surfaceCapabilities = {0};
        VkResult res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_vkPhysicalDevice, m_vkSurface, &surfaceCapabilities);
        if (VK_SUCCESS != res)
        {
            LOG_DEBUG("ERROR: vkGetPhysicalDeviceSurfaceCapabilitiesKHR().");
            THROW();
        }

        VkExtent2D extent = surfaceCapabilities.currentExtent;
        if (extent.width == -1 || extent.height == -1) {
            extent = m_vkSurfaceResolution;
        }

        extent.width = std::clamp(extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
        extent.height = std::clamp(extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);

        m_vkSurfaceResolution = extent;

        m_vkPreTransform = surfaceCapabilities.currentTransform;
        if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        {
            m_vkPreTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        }
    }

    void CGxVulkan::getSurfacePresentMode()
    {
        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_vkPhysicalDevice, m_vkSurface, &presentModeCount, NULL);
        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        VkResult res = vkGetPhysicalDeviceSurfacePresentModesKHR(m_vkPhysicalDevice, m_vkSurface, &presentModeCount, presentModes.data());
        if (VK_SUCCESS != res)
        {
            LOG_DEBUG("ERROR: vkGetPhysicalDeviceSurfacePresentModesKHR().");
        }

        m_vkPresentationMode = VK_PRESENT_MODE_FIFO_KHR;
        for (uint32_t i = 0; i < presentModeCount; ++i)
        {
            if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                m_vkPresentationMode = VK_PRESENT_MODE_MAILBOX_KHR;
                break;
            }
            else if (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
            {
                m_vkPresentationMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            }
        }
    }

    void CGxVulkan::createSwapChain()
    {
        VkResult res;

        // запросы параметров поверхности для цепочки обмена
        getSurfaceFormat();
        getSurfacePresentMode();
        getSurfaceCapabilities();

        // создание цепочки обмена
        VkSwapchainCreateInfoKHR scci = {                            // Структура, определяющая параметры вновь созданного объекта цепочки обмена
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, // тип этой структуры
            .pNext = nullptr,                                        // NULL или указатель на структуру, расширяющую эту структуру
            .flags = 0,                                           // битовая маска VkSwapchainCreateFlagBitsKHR, указывающая параметры создания цепочки обмена
            .surface = m_vkSurface,                               // поверхность, на которой свопчейн будет отображать изображения
            .minImageCount = m_swapchainImageCount,               // минимальное количество презентабельных изображений, которое необходимо приложению
            .imageFormat = m_vkColorFormat,                       // значение VkFormat , указывающее формат, в котором будут созданы изображения цепи обмена
            .imageColorSpace = m_vkColorSpace,                    // значение VkColorSpaceKHR , указывающее, как цепочка обмена интерпретирует данные изображения
            .imageExtent = m_vkSurfaceResolution,                 // размер (в пикселях) изображений цепи обмена
            .imageArrayLayers = 1,                                // количество просмотров на многоэкранной/стереоповерхности. Для нестереоскопических 3D-приложений это значение равно 1
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |   // битовая маска VkImageUsageFlagBits, описывающая предполагаемое использование (приобретенных) образов цепи обмена
                            VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,       // режим совместного использования, используемый для изображений цепочки обмена
            .queueFamilyIndexCount = 0,                          // количество семейств очередей, имеющих доступ к образу(ам) цепочки обмена, когда imageSharingMode равно VK_SHARING_MODE_CONCURRENT
            .pQueueFamilyIndices = nullptr,                      // указатель на массив индексов семейства очередей, имеющих доступ к изображениям цепочки обмена
            .preTransform = m_vkPreTransform,                    // значение VkSurfaceTransformFlagBitsKHR , описывающие преобразование относительно естественной ориентации механизма представления, примененное к содержимому изображения до представления
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, // значение VkCompositeAlphaFlagBitsKHR, указывающим режим альфа-компоновки, который следует использовать
            .presentMode = m_vkPresentationMode,                 // режим представления, который будет использовать цепочка обмена
            .clipped = VK_TRUE,                                  // указывает, разрешено ли реализации Vulkan отбрасывать операции рендеринга, затрагивающие невидимые области поверхности
            .oldSwapchain = VK_NULL_HANDLE                      //  VK_NULL_HANDLE или существующая незавершенная цепочка подкачки, которая в настоящее время связана с поверхностью
        };

        res = vkCreateSwapchainKHR(m_vkDevice, &scci, NULL, &m_vkSwapChain);
        if (VK_SUCCESS != res)
        {
            LOG_DEBUG("Ошибка создания цепочкм обмена.");
            THROW();
        }
        LOG_INFO("Цепочка обмена создана.");

        // создание и присоединение массива изображений
        vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapChain, &m_swapchainImageCount, NULL);
        m_vkSwapchainImages = new VkImage[m_swapchainImageCount];
        if (!m_vkSwapchainImages)
        {
            LOG_DEBUG("Не могу выделить память для массива изображений.");
        }
        vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapChain, &m_swapchainImageCount, m_vkSwapchainImages);

        // создание и присодинение представлений изображений цепочки обмена
        m_vkSwapchainImagesView = new VkImageView[m_swapchainImageCount];
        if (!m_vkSwapchainImagesView)
        {
            LOG_DEBUG("Не могу выделить память для представлений изображений.");
            THROW();
        }

        VkImageViewCreateInfo ivci = {                         // Структура, определяющая параметры только что созданного изображения
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, // тип этой структуры
            .pNext = nullptr,                                  // NULL или указатель на структуру, расширяющую эту структуру
            .flags = 0,                                        // битовая маска VkImageViewCreateFlagBits, описывающая дополнительные параметры просмотра изображения
            .viewType = VK_IMAGE_VIEW_TYPE_2D,                 // является значением VkImageViewType , указывающим тип представления изображения
            .format = m_vkColorFormat,                         // VkFormat , описывающий формат и тип, используемые для интерпретации блоков текселей в изображении
            .components.r = VK_COMPONENT_SWIZZLE_R,            // представляет собой структуру VkComponentMapping, определяющую переназначение компонентов цвета (или компонентов глубины или трафарета после их преобразования в компоненты цвета)
            .components.g = VK_COMPONENT_SWIZZLE_G,
            .components.b = VK_COMPONENT_SWIZZLE_B,
            .components.a = VK_COMPONENT_SWIZZLE_A,
            .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, // представляет собой структуру VkImageSubresourceRange , выбирающую набор уровней MIP-карты и слоев массива, которые будут доступны для представления
            .subresourceRange.baseMipLevel = 0,
            .subresourceRange.levelCount = 1,
            .subresourceRange.baseArrayLayer = 0,
            .subresourceRange.layerCount = 1
        };

        for (uint32_t i = 0; i < m_swapchainImageCount; ++i)
        {

            ivci.image = m_vkSwapchainImages[i]; // VkImage, для которого будет создано представление

            res = vkCreateImageView(m_vkDevice, &ivci, NULL, &m_vkSwapchainImagesView[i]);
            if (VK_SUCCESS != res)
            {
                LOG_DEBUG("ERROR: vkCreateImageView().");
                THROW();
            }
        }
    }

    void CGxVulkan::destroySwapChain()
    {
        if (m_vkSwapChain != VK_NULL_HANDLE)
        {

            if (m_vkSwapchainImagesView != nullptr)
            {
                for (uint32_t i = 0; i < m_swapchainImageCount; ++i)
                {
                    vkDestroyImageView(m_vkDevice, m_vkSwapchainImagesView[i], NULL);
                    LOG_DEBUG("Уничтожить Представление Изображения[%d].", i);
                }

                delete[] m_vkSwapchainImagesView;
                m_vkSwapchainImagesView = nullptr;
                LOG_DEBUG("Удалить массив представлений изображений.");
            }

            if (m_vkSwapchainImages != nullptr)
            {
                delete[] m_vkSwapchainImages;
                m_vkSwapchainImages = nullptr;
                LOG_DEBUG("Удалить массив изображений.");
            }

            vkDestroySwapchainKHR(m_vkDevice, m_vkSwapChain, NULL);
            m_vkSwapChain = VK_NULL_HANDLE;
            LOG_DEBUG("Удалить цепочку обмена.");
        }
    }
}
