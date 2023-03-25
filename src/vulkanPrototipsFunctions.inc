//
// vulkanPrototipsFunctions.inc - прототипы функций Vulkan'a
//

// декларация указателей на функции Vulkan'a
#define DECLARE_VK(func) \
    PFN_##func func      \
    {                    \
        nullptr          \
    }

// указатели на функции экземпляра Vulkan
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DECLARE_VK(vkGetInstanceProcAddr); // функция библиотеки Vulkan для получения указателей на функции для конкретного экземпляра
DECLARE_VK(vkGetDeviceProcAddr);   // функция библиотеки Vulkan для получения указателей функций для конкретного логического устройства

// функции не требующие действительный экземпляр Vulkan'a
DECLARE_VK(vkEnumerateInstanceVersion);             // запрашивает версию уровня экземпляра
DECLARE_VK(vkEnumerateInstanceExtensionProperties); // возвращает запрошенное количество глобальных свойств расширения
DECLARE_VK(vkEnumerateInstanceLayerProperties);     // возвращает запрошенное количество свойств глобального слоя
DECLARE_VK(vkCreateInstance);                       // создает новый экземпляр Vulkan

// необходим действительный экземпляр Vulkan'a
DECLARE_VK(vkDestroyInstance);                    // уничтожить экземпляр Vulkan
DECLARE_VK(vkEnumeratePhysicalDevices);           // перечисляет физические устройства, доступные экземпляру Vulkan
DECLARE_VK(vkEnumerateDeviceExtensionProperties); // перечисляет расширения физические устройства

DECLARE_VK(vkGetPhysicalDeviceFeatures);
DECLARE_VK(vkGetPhysicalDeviceFormatProperties);
DECLARE_VK(vkGetPhysicalDeviceImageFormatProperties);
DECLARE_VK(vkGetPhysicalDeviceMemoryProperties);
DECLARE_VK(vkGetPhysicalDeviceProperties);
DECLARE_VK(vkGetPhysicalDeviceQueueFamilyProperties);
DECLARE_VK(vkGetPhysicalDeviceSparseImageFormatProperties);

DECLARE_VK(vkGetPhysicalDeviceExternalBufferProperties);
DECLARE_VK(vkGetPhysicalDeviceExternalFenceProperties);
DECLARE_VK(vkGetPhysicalDeviceExternalSemaphoreProperties);
DECLARE_VK(vkGetPhysicalDeviceFeatures2);
DECLARE_VK(vkGetPhysicalDeviceFormatProperties2);
DECLARE_VK(vkGetPhysicalDeviceImageFormatProperties2);
DECLARE_VK(vkGetPhysicalDeviceMemoryProperties2);
DECLARE_VK(vkGetPhysicalDeviceProperties2);
DECLARE_VK(vkGetPhysicalDeviceQueueFamilyProperties2);
DECLARE_VK(vkGetPhysicalDeviceSparseImageFormatProperties2);

DECLARE_VK(vkCreateDevice); // создает логическое устройство Vulkan

// VK_KHR_surface ( VK_KHR_SURFACE_EXTENSION_NAME ) - расширение экземпляра
DECLARE_VK(vkDestroySurfaceKHR);                       // уничтожить объект VkSurfaceKHR
DECLARE_VK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR); // запрос возможности поверхности
DECLARE_VK(vkGetPhysicalDeviceSurfaceFormatsKHR);      // запрос цветовых форматов, поддерживаемых поверхностью
DECLARE_VK(vkGetPhysicalDeviceSurfacePresentModesKHR); // запрос поддерживаемых режимов представления
DECLARE_VK(vkGetPhysicalDeviceSurfaceSupportKHR);      // запрос, поддерживает ли поверхность представление

// VK_KHR_win32_surface ( VK_KHR_WIN32_SURFACE_EXTENSION_NAME ) - расширение экземпляра
DECLARE_VK(vkCreateWin32SurfaceKHR);                        // создает объект VkSurfaceKHR для родного окна Win32
DECLARE_VK(vkGetPhysicalDeviceWin32PresentationSupportKHR); // поддержка семейства очередей запросов для презентации на дисплее Win32

#if defined(_LOG) || defined(_DEBUG)
                                                            // VK_EXT_debug_utils ( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) — расширение экземпляра
DECLARE_VK(vkCreateDebugUtilsMessengerEXT);  // создать объект отладочного мессенджера
DECLARE_VK(vkDestroyDebugUtilsMessengerEXT); // уничтожить объект отладочного мессенджера
DECLARE_VK(vkSetDebugUtilsObjectNameEXT);    // дает понятное имя объекту
DECLARE_VK(vkSetDebugUtilsObjectTagEXT);     // прикрепить произвольные данные к объекту
DECLARE_VK(vkSubmitDebugUtilsMessageEXT);    // вставить сообщение в поток отладки
DECLARE_VK(vkQueueBeginDebugUtilsLabelEXT);  // открыть область метки отладки очереди
DECLARE_VK(vkQueueEndDebugUtilsLabelEXT);    // закрывает область метки отладки очереди
DECLARE_VK(vkQueueInsertDebugUtilsLabelEXT); // вставить метку в очередь
DECLARE_VK(vkCmdBeginDebugUtilsLabelEXT);    // открывает область метки отладки командного буфера
DECLARE_VK(vkCmdEndDebugUtilsLabelEXT);      // закрывает область метки командного буфера
DECLARE_VK(vkCmdInsertDebugUtilsLabelEXT);   // вставить метку в командный буфер
#endif

// необходим действительный дескриптор логического устройства Vulkan
DECLARE_VK(vkDestroyDevice);  // уничтожить логическое устройство Vulkan
DECLARE_VK(vkDeviceWaitIdle); // подождать, пока устройство станет бездействующим
DECLARE_VK(vkGetDeviceQueue); // получить дескриптор очереди с логического устройства

DECLARE_VK(vkCreateImageView);  // Создать представление изображения из существующего изображения
DECLARE_VK(vkDestroyImageView); // Уничтожить представления изображения
DECLARE_VK(vkCreateImage);      // Создать новый объект изображение
DECLARE_VK(vkDestroyImage);     // Уничтожить объект изображения

DECLARE_VK(vkGetImageMemoryRequirements); // возвращает требования к памяти для указанного объекта Vulkan
DECLARE_VK(vkAllocateMemory);             // Выделить память устройства Vulkan
DECLARE_VK(vkFreeMemory);                 // Освободить память устройства Vulkan
DECLARE_VK(vkBindImageMemory);            // привязать память устройства к объекту изображения

// VK_KHR_swapchain
DECLARE_VK(vkAcquireNextImageKHR);   // Получить индекс следующего доступного изображения
DECLARE_VK(vkCreateSwapchainKHR);    // создать цепочку обмена
DECLARE_VK(vkDestroySwapchainKHR);   // уничтожить цепочку обмена
DECLARE_VK(vkGetSwapchainImagesKHR); // получение массива изображений, связанных с цепочкой обмена
DECLARE_VK(vkQueuePresentKHR);       // поставить изображение в очередь для презентации
DECLARE_VK(vkAcquireNextImage2KHR);  // Получить индекс следующего доступного изображения

