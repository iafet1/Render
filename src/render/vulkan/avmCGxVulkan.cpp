//
// avmCGxVulkan.cpp
//

#include "../../stdafx.hpp"
#include "../../avmCLog.hpp"
#include "../../platform/platforms.hpp"
#include "../avmCGxDevice.hpp"

#include "avmCGxVulkan.hpp"

constexpr char FILE_NAME[] = "avmCGxVulkan.cpp";

#if defined(_WIN32) || defined(WIN32)
#define VULKAN_LIB_NAME "vulkan-1.dll"
#else
#error "Платформа пока не поддерживается"
#define VULKAN_LIB_NAME
#endif

// запрос функций экземпляра Vulkan'a
#define GetInstanceProcAddr(inst, name)                            \
    name = (PFN_##name)vkGetInstanceProcAddr(inst, #name);         \
    if (!name)                                                     \
    {                                                              \
        LOG_ERROR("Не могу получить адресс функции " #name "()."); \
        THROW();                                                   \
    }

// запрос функций устройства Vulkan'a
#define GetDeviceProcAddr(dev, name)                               \
    name = (PFN_##name)vkGetDeviceProcAddr(dev, #name);            \
    if (!name)                                                     \
    {                                                              \
        LOG_ERROR("Не могу получить адресс функции " #name "()."); \
        THROW();                                                   \
    }

namespace avm::graphics
{

    // список требуемых расширений экземпляра Vulkan
    const char *g_InstanceExtensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME, // расширения поверхности Vulkan

#if defined(VK_USE_PLATFORM_WIN32_KHR)
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME, // расширения поверхности WIN32 в Vulkan
#else
#error "Not support"
#endif // platform extenstion

#if defined(_LOG) || defined(_DEBUG)
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME, // расширения отладки
#endif                                     // _LOG || _DEBUG
    };

#if defined(_LOG) || defined(_DEBUG)
    // список требуемых слоев экземпляра Vulkan
    const char *g_InstanceLayers[] = {
        "VK_LAYER_KHRONOS_validation"};
#endif // _LOG || _DEBUG

    // список необходимых расширений устройства Vulkan
    const char *g_DeviceExtension[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME, // представляет объекты VkSwapchainKHR, которые позволяют отображать результаты рендеринга на поверхности Vulkan.
    };

#if defined(_LOG) || defined(_DEBUG)
    // VK_EXT_debug_utils ( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) — расширение экземпляра

    VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {
        switch (messageSeverity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            LOG_ERROR("%s - %s.", pCallbackData->pMessageIdName, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            LOG_WARNING("%s - %s.", pCallbackData->pMessageIdName, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            LOG_INFO("%s - %s.", pCallbackData->pMessageIdName, pCallbackData->pMessage);
            break;
        default:
            LOG_DEBUG("%s - %s.", pCallbackData->pMessageIdName, pCallbackData->pMessage);
            break;
        }

        // Продолжаем
        return VK_FALSE;
    }
#endif

    CGxVulkan::CGxVulkan()
    {
        LOG_INFO("CGxVulkan::CGxVulkan().");

        createInstance();
        createDevice();
    }

    CGxVulkan::~CGxVulkan()
    {
        destroyDevice();
        destroyInstance();

        LOG_INFO("CGxVulkan::~CGxVulkan().");
    }

    bool CGxVulkan::BeginFrame(float dt)
    {
        return true;
    }

    bool CGxVulkan::EndFrame(float dt)
    {
        ++m_indexFrame;
        return true;
    }

    bool CGxVulkan::OnResized(void *object, Variant param1, Variant param2)
    {
        return false;
    }

    void CGxVulkan::createInstance()
    {
        // загрузка библиотеки Vulkan
        m_libVulkan = platform::OpenLibrary(VULKAN_LIB_NAME);
        if (nullptr == m_libVulkan)
        {
            LOG_ERROR("Не могу загрузить библиотеку Vulkan'a.");
            THROW();
        }
        // запрос функций определения адресов функций для данного Vulkan'a
        vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)platform::GetProcAddr(m_libVulkan, "vkGetInstanceProcAddr");
        vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)platform::GetProcAddr(m_libVulkan, "vkGetDeviceProcAddr");
        if (!vkGetInstanceProcAddr || !vkGetDeviceProcAddr)
        {
            LOG_ERROR("Не могу получить адресс функции vkGetInstanceProcAddr или vkGetDeviceProcAddr.");
            THROW();
        }

        // получение адресов функций, не требующих действительного экземпляра Vulkan'a
        GetInstanceProcAddr(nullptr, vkEnumerateInstanceVersion);             // запрашивает версию уровня экземпляра
        GetInstanceProcAddr(nullptr, vkEnumerateInstanceExtensionProperties); // возвращает запрошенное количество глобальных свойств расширения
        GetInstanceProcAddr(nullptr, vkEnumerateInstanceLayerProperties);     // возвращает запрошенное количество свойств глобального слоя
        GetInstanceProcAddr(nullptr, vkCreateInstance);                       // создает новый экземпляр Vulkan

        VkResult result; // код результата выполнения функций Vulkan

        { // запрос версии экземпляра vulkan'a
            uint32_t version = 0;
            vkEnumerateInstanceVersion(&version);
            if (version < (VK_MAKE_VERSION(1, 2, 0)))
            {
                LOG_DEBUG("Версия Vulkan'a меньше небходимой 1.2.");
                THROW();
            }

            LOG_INFO("Версия Vulkan - %d.%d.%d.", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
        }

        // создание экземпляра Vulkan'a
        ///////////////////////////////////////////////////////////////////////

        VkApplicationInfo appInfo = {
            // структура, определяющая информацию о приложении
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,    // является типом этой структуры
            .pNext = nullptr,                               // NULL или указатель на структуру, расширяющую эту структуру.
            .pApplicationName = "Render",                   // NULL или указатель на строку UTF-8 с нулевым окончанием, содержащую имя приложения
            .applicationVersion = VK_MAKE_VERSION(0, 0, 1), // номер версии приложения, предоставленный разработчиком
            .pEngineName = "avmVulkanEngine",               // имя движка (если таковой имеется), используемого для создания приложения
            .engineVersion = VK_MAKE_VERSION(0, 0, 1),      // предоставленный разработчиком номер версии движка, используемого для создания приложения
            .apiVersion = VK_MAKE_VERSION(1, 2, 0)          // самая высокая версия Vulkan, для которой предназначено приложение
        };

        VkInstanceCreateInfo instanceCreateInfo = {
            // Структура, определяющая параметры вновь созданного экземпляра
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,          // тип этой структуры
            .pNext = nullptr,                                         // NULL или указатель на структуру , расширяющую эту структуру
            .flags = 0,                                               // 0, зарезервировано для использования в будущем
            .pApplicationInfo = &appInfo,                             // NULL или указатель на VkApplicationInfo структуру. Если не NULL, эта информация помогает реализациям распознавать поведение, свойственное классам приложений
            .enabledLayerCount = 0,                                   // количество глобальных слоев для включения
            .ppEnabledLayerNames = nullptr,                           // это указатель на массив строк UTF-8 с завершающим нулем, содержащих имена слоев, которые необходимо включить для созданного экземпляра
            .enabledExtensionCount = ARRAYSIZE(g_InstanceExtensions), // количество глобальных расширений для включения
            .ppEnabledExtensionNames = g_InstanceExtensions           // это указатель на массив строк UTF-8 с завершающим нулем, содержащих имена подключаемых расширений
        };

#if defined(_LOG) || defined(_DEBUG)
        // подключение слоев валидации и расширения отладки
        //////////////////////////////////////////////////////////
        // VK_EXT_debug_utils
        VkDebugUtilsMessengerCreateInfoEXT dbgMessengerCreateInfo = {
            // Структура, определяющая параметры созданного отладочного мессенджера
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT, // тип этой структуры
            .pNext = nullptr,                                                 // NULL или указатель на структуру, расширяющую эту структуру
            .flags = 0,                                                       // 0, зарезервировано для использования в будущем
            .messageSeverity =                                                // битовая маска VkDebugUtilsMessageSeverityFlagBitsEXT, указывающая, какая серьезность событий приведет к вызову этого обратного вызова
            // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = // битовая маска  VkDebugUtilsMessageTypeFlagBitsEXT, указывающая, какой тип событий вызовет этот обратный вызов
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = debugUtilsMessengerCallback, // функция обратного вызова приложения для сообщения
            .pUserData = nullptr                            // данные пользователя, которые должны быть переданы для обратного вызова
        };

        // назначение расширения экземпляра
        instanceCreateInfo.pNext = &dbgMessengerCreateInfo;
        instanceCreateInfo.enabledLayerCount = ARRAYSIZE(g_InstanceLayers);
        instanceCreateInfo.ppEnabledLayerNames = g_InstanceLayers;
#endif

        // создание экземпляра Vulkan
        result = vkCreateInstance(&instanceCreateInfo, m_vkAllocator, &m_vkInstance);
        if (VK_SUCCESS != result)
        {
            LOG_ERROR("Экземпляр Vulkan'a не создан.");
            LOG_ERROR("%s", vkResultToString(result));
            THROW();
        }
        LOG_INFO("Создан экземпляр Vulkan.");

        // иницилизация функций Vulkan'a, требующих действительного дескриптора экземпляра
        initFunctionsInstance(m_vkInstance);

#if defined(_LOG) || defined(_DEBUG)
        // создание мессенджера отладки
        result = vkCreateDebugUtilsMessengerEXT(m_vkInstance, &dbgMessengerCreateInfo, m_vkAllocator, &m_dbg_messenger);
        if (VK_SUCCESS != result)
        {
            LOG_ERROR("Мессенджер отладки не создан.");
            THROW();
        }
#endif // _LOG || _DEBUG
    }

    void CGxVulkan::destroyInstance()
    {

#if defined(_LOG) || defined(_DEBUG)
        // уничтожение мессенджера отладки
        if (m_dbg_messenger != VK_NULL_HANDLE && vkDestroyDebugUtilsMessengerEXT)
        {
            vkDestroyDebugUtilsMessengerEXT(m_vkInstance, m_dbg_messenger, m_vkAllocator);
            m_dbg_messenger = nullptr;
            LOG_INFO("Мессенджер отладки уничтожен.");
        }
#endif // _DEBUG

        // уничтожить экземпляр Vulkan'a
        if (m_vkInstance != VK_NULL_HANDLE && vkDestroyInstance)
        {
            vkDestroyInstance(m_vkInstance, m_vkAllocator);
            m_vkInstance = VK_NULL_HANDLE;
            LOG_INFO("Экземпляр Vulkan'а уничтожен.");
        }

        // освобождаем библиотеку Vulkan
        if (m_libVulkan != nullptr)
        {
            platform::CloseLibrary(m_libVulkan);
            m_libVulkan = nullptr;
        }
    }

    void CGxVulkan::createDevice()
    {
        // поиск подходящего GPU
        m_vkPhysicalDevice = choosePhysicalDevice();

        // создаем и записываем массив информации о очередях
        uint32_t countQueueInFamily[QueueType::QT_MAX]{};
        buildQueueInfos(countQueueInFamily);

        // параметры создаваемых очередей
        const float queuePriority[QueueType::QT_MAX]{1.0f, 1.0f, 1.0f, 1.0f}; // приоритеты очередей
        VkDeviceQueueCreateInfo queueCreateInfo = {
            // структура, определяющая параметры вновь созданной очереди устройств
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, // является типом этой структуры
            .pNext = nullptr,                                    // должен быть NULL или указателем на действительный экземпляр VkDeviceQueueGlobalPriorityCreateInfoKHR
            .flags = 0,                                          // битовая маска, указывающая поведение очередей. должен быть допустимой комбинацией значений VkDeviceQueueCreateFlagBits
            .pQueuePriorities = queuePriority                    // указатель на массив нормализованных значений с плавающей запятой, указывающий приоритеты работы
        };

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos; // массив информации создания очередей
        for (int i = 0; i < QueueType::QT_MAX; ++i)
        {
            if (countQueueInFamily[i] > 0)
            {
                queueCreateInfo.queueFamilyIndex = m_vkQueueFamily[i]; // индекс семейства очередей
                queueCreateInfo.queueCount = countQueueInFamily[i];    // количество очередей для данного семейства

                queueCreateInfos.push_back(queueCreateInfo); // запись в массив информации создания очереди
            }
        }

        // TODO: перенести в настройки и поиск GPU
        // детализированные особенности, поддерживаемые устройством
        VkPhysicalDeviceFeatures features = {}; // структура, описывающая детализированные функции, которые могут поддерживаться реализацией
        features.shaderClipDistance = VK_TRUE;  // указывает, поддерживаются ли расстояния отсечения в коде шейдера

        // создаем логическое устройство
        VkDeviceCreateInfo createInfo = {
            // структура, определяющая параметры только что созданного устройства
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,             // является типом этой структуры
            .pNext = nullptr,                                          // указатель на структуру, расширяющую эту структуру или nullptr
            .flags = 0,                                                // зарезервировано для использования в будущем, должен быть 0
            .queueCreateInfoCount = (uint32_t)queueCreateInfos.size(), // размер массива pQueueCreateInfos
            .pQueueCreateInfos = queueCreateInfos.data(),              // является указателем на массив структур VkDeviceQueueCreateInfo , описывающих очереди, которые требуется создать вместе с логическим устройством
            .enabledLayerCount = 0,                                    // устарело и игнорируется, должно быть 0
            .ppEnabledLayerNames = nullptr,                            // устарело и игнорируется, должно быть nullptr
            .enabledExtensionCount = ARRAYSIZE(g_DeviceExtension),     // количество расширений устройств, которые необходимо включить
            .ppEnabledExtensionNames = g_DeviceExtension,              // массив строк UTF-8 с завершающим нулем, содержащих имена расширений, которые необходимо включить для созданного устройства
            .pEnabledFeatures = &features,                             // указатель на структуру VkPhysicalDeviceFeatures, содержащую логические индикаторы всех функций, которые необходимо включить
        };

        VkResult result = vkCreateDevice(m_vkPhysicalDevice, &createInfo, m_vkAllocator, &m_vkDevice);
        if (result != VK_SUCCESS)
        {
            LOG_ERROR("Не могу создать логическое устройство Vulkan.");
            LOG_ERROR("%s", vkResultToString(result));
            THROW();
        }
        LOG_INFO("Логическое устройство Vulkan создано.");

        // иницилизация функций Vulkan'a, требующих действительного дескриптора логического устройства
        initFunctionsDevice(m_vkDevice);

        // запрос очередей из логического устройства
        for (int i = 0; i < QueueType::QT_MAX; ++i) {
            vkGetDeviceQueue(m_vkDevice, m_vkQueueFamily[i], m_vkQueueIndex[i], &m_vkQueue[i]);
        }

    }

    void CGxVulkan::destroyDevice()
    {
        // уничтожаем логическое устройство Vulkan
        if (m_vkDevice != VK_NULL_HANDLE)
        {
            vkDeviceWaitIdle(m_vkDevice);

            vkDestroyDevice(m_vkDevice, m_vkAllocator);
            m_vkDevice = VK_NULL_HANDLE;
            m_vkPhysicalDevice = VK_NULL_HANDLE;
            for (int i = 0; i < QueueType::QT_MAX; ++i) {
                m_vkQueueFamily[i] = VK_QUEUE_FAMILY_IGNORED;
                m_vkQueueIndex[i] = 0;
                m_vkQueue[i] = VK_NULL_HANDLE;
            }
            LOG_INFO("Логическое устройство Vulkan уничтожено.");
        }
    }

    void CGxVulkan::initFunctionsInstance(VkInstance instance)
    {
#if defined(_LOG) || defined(_DEBUG)
        // VK_EXT_debug_utils ( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) — расширение экземпляра
        GetInstanceProcAddr(instance, vkCreateDebugUtilsMessengerEXT);
        GetInstanceProcAddr(instance, vkDestroyDebugUtilsMessengerEXT);
        GetInstanceProcAddr(instance, vkSetDebugUtilsObjectNameEXT);
        GetInstanceProcAddr(instance, vkSetDebugUtilsObjectTagEXT);
        GetInstanceProcAddr(instance, vkSubmitDebugUtilsMessageEXT);
        GetInstanceProcAddr(instance, vkQueueBeginDebugUtilsLabelEXT);
        GetInstanceProcAddr(instance, vkQueueEndDebugUtilsLabelEXT);
        GetInstanceProcAddr(instance, vkQueueInsertDebugUtilsLabelEXT);
        GetInstanceProcAddr(instance, vkCmdBeginDebugUtilsLabelEXT);
        GetInstanceProcAddr(instance, vkCmdEndDebugUtilsLabelEXT);
        GetInstanceProcAddr(instance, vkCmdInsertDebugUtilsLabelEXT);
#endif // _LOG || _DEBUG

        GetInstanceProcAddr(instance, vkDestroyInstance);
        GetInstanceProcAddr(instance, vkEnumeratePhysicalDevices);
        GetInstanceProcAddr(instance, vkEnumerateDeviceExtensionProperties);

        GetInstanceProcAddr(instance, vkGetPhysicalDeviceFeatures);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceFormatProperties);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceImageFormatProperties);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceMemoryProperties);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceProperties);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceQueueFamilyProperties);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceSparseImageFormatProperties);

        GetInstanceProcAddr(instance, vkGetPhysicalDeviceExternalBufferProperties);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceExternalFenceProperties);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceExternalSemaphoreProperties);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceFeatures2);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceFormatProperties2);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceImageFormatProperties2);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceMemoryProperties2);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceProperties2);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceQueueFamilyProperties2);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceSparseImageFormatProperties2);

        GetInstanceProcAddr(instance, vkCreateDevice);

        // VK_KHR_surface ( VK_KHR_SURFACE_EXTENSION_NAME ) - расширение экземпляра
        GetInstanceProcAddr(instance, vkDestroySurfaceKHR);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceSurfaceFormatsKHR);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceSurfacePresentModesKHR);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceSurfaceSupportKHR);

#if defined(VK_USE_PLATFORM_WIN32_KHR)
        // VK_KHR_win32_surface ( VK_KHR_WIN32_SURFACE_EXTENSION_NAME ) - расширение экземпляра
        GetInstanceProcAddr(instance, vkCreateWin32SurfaceKHR);
        GetInstanceProcAddr(instance, vkGetPhysicalDeviceWin32PresentationSupportKHR);
#else
#error "Not support"
#endif // platform extenstion
    }

    void CGxVulkan::initFunctionsDevice(VkDevice device)
    {
        GetDeviceProcAddr(device, vkDestroyDevice);
        GetDeviceProcAddr(device, vkDeviceWaitIdle);
        GetDeviceProcAddr(device, vkGetDeviceQueue);

        GetDeviceProcAddr(device, vkCreateImageView);
        GetDeviceProcAddr(device, vkDestroyImageView);
        GetDeviceProcAddr(device, vkCreateImage);
        GetDeviceProcAddr(device, vkDestroyImage);

        GetDeviceProcAddr(device, vkGetImageMemoryRequirements);
        GetDeviceProcAddr(device, vkAllocateMemory);
        GetDeviceProcAddr(device, vkFreeMemory);
        GetDeviceProcAddr(device, vkBindImageMemory);

        // VK_KHR_swapchain - расширение устройства
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        GetDeviceProcAddr(device, vkCreateSwapchainKHR);
        GetDeviceProcAddr(device, vkDestroySwapchainKHR);
        GetDeviceProcAddr(device, vkGetSwapchainImagesKHR);
        GetDeviceProcAddr(device, vkAcquireNextImageKHR);
        GetDeviceProcAddr(device, vkQueuePresentKHR);
        GetDeviceProcAddr(device, vkAcquireNextImage2KHR);
    }

    VkPhysicalDevice CGxVulkan::choosePhysicalDevice()
    {
        VkPhysicalDevice gpu{VK_NULL_HANDLE};
        VkResult result;

        uint32_t deviceCount{0}; // количество физических устройств
        result = vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);
        if (deviceCount == 0)
        {
            LOG_ERROR("Нет физических устройств GPU, поддерживающих Vulkan.");
            THROW();
        }

        VkPhysicalDevice devices[deviceCount]; // массив физических устройств (GPU) Vulkan
        result = vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices);
        if (result != VK_SUCCESS)
        {
            LOG_ERROR("Ошибка запроса перечня физических устройств. %s.", vkResultToString(result));
            THROW();
        }

        VkPhysicalDeviceProperties2 gpuProperties2 = {
            //  структура, определяющая свойства физического устройства (GPU)
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        };

        // перебор физических устройств и проверка необходимых свойств
        for (const auto &dev : devices)
        {
            uint32_t extensionCount; // количество расширений устройства
            result = vkEnumerateDeviceExtensionProperties(dev, nullptr, &extensionCount, nullptr);
            VkExtensionProperties available_deviceExtensions[extensionCount]; // массив свойств расширений
            result = vkEnumerateDeviceExtensionProperties(dev, nullptr, &extensionCount, available_deviceExtensions);
            if (result != VK_SUCCESS)
            {
                LOG_DEBUG("Ошибка запроса свойств расширений. %s.", vkResultToString(result));
                THROW();
            }

            bool suitable = true; // флаг того, что нужные расширения поддерживаются физическим устройством
            for (auto &checkExtension : g_DeviceExtension)
            {
                bool check = false;
                for (const auto &ext : available_deviceExtensions)
                {
                    if (strcmp(ext.extensionName, checkExtension) == 0)
                    {
                        check = true;
                        break;
                    }
                }

                if (!check)
                {
                    suitable = false;
                    break;
                }
            }

            if (!suitable)
                continue;

            // проверка наличия семейств очередей с поддержкой графики и представления
            uint32_t queueFamilyCount{0};
            vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueFamilyCount, nullptr);
            VkQueueFamilyProperties queueFamiliesProp[queueFamilyCount];
            vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueFamilyCount, queueFamiliesProp);

            bool supportGraphic{false};
            bool supportPresent{false};
            for (uint32_t index = 0; index < queueFamilyCount; ++index)
            {
                if (queueFamiliesProp[index].queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
                {
                    supportGraphic = true;
                }

                supportPresent = checkPresentationSupport(dev, index);
                if (supportGraphic && supportPresent)
                {
                    break;
                }
            }

            if (!supportGraphic && !supportPresent)
                continue;

            vkGetPhysicalDeviceProperties2(dev, &gpuProperties2); // запрос свойств GPU

            bool discrete = (gpuProperties2.properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
            if (discrete || gpu == VK_NULL_HANDLE)
            {
                gpu = dev;
                if (discrete)
                {
                    LOG_DEBUG("Найден дискретный GPU.");
                    break; // если это дискретный GPU, отдайем ему предпочтение и дальше не ищем
                }
            }
        }

        if (gpu == VK_NULL_HANDLE)
        {
            LOG_ERROR("Не могу найти подходящее физическое устройство GPU.");
            THROW();
        }

        LOG_INFO("Необходимое физическое устройство GPU найдено.");
        return gpu;
    }

    bool CGxVulkan::checkPresentationSupport(VkPhysicalDevice gpu, uint32_t queueFamilyIndex)
    {
        VkBool32 ret{VK_FALSE};

        #if defined(VK_USE_PLATFORM_WIN32_KHR)
            ret = vkGetPhysicalDeviceWin32PresentationSupportKHR(gpu, queueFamilyIndex);
        #else
            #error "Пока другие платформы не поддерживаются"
        #endif

        if (ret)
        {
            m_vkQueueFamily[QueueType::QT_PRESENT] = queueFamilyIndex;
//            m_vkPresentFamily = queueFamilyIndex;
            return true;
        }

        return false;
    }

    void CGxVulkan::buildQueueInfos(uint32_t* countQueueInFamily)
    {
        // получение, поддерживаемых семейств очередей
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, nullptr);
        m_vkQueueFamiliesProp.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, m_vkQueueFamiliesProp.data());

        // запрос индексов семейств очередей
        uint32_t familyIndex = 0;
        for (const auto &queueFamily : m_vkQueueFamiliesProp)
        {
            if (m_vkQueueFamily[QT_GRAPHICS] == VK_QUEUE_FAMILY_IGNORED &&
                queueFamily.queueCount > 0 &&
                queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_vkQueueFamily[QueueType::QT_GRAPHICS] = familyIndex;
//                m_vkGraphicsFamily = familyIndex;
            }

            if (m_vkQueueFamily[QT_COMPUTE] == VK_QUEUE_FAMILY_IGNORED &&
                queueFamily.queueCount > 0 &&
                queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                m_vkQueueFamily[QueueType::QT_COMPUTE] = familyIndex;
//                m_vkComputeFamily = familyIndex;
            }

            if (m_vkQueueFamily[QT_TRANSFER] == VK_QUEUE_FAMILY_IGNORED &&
                queueFamily.queueCount > 0 &&
                queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                m_vkQueueFamily[QueueType::QT_TRANSFER] = familyIndex;
//                m_vkTransferFamily = familyIndex;
            }

            ++familyIndex;
        }

        // теперь пробуем найти выделенные семейства очередей вычислений и передачи
        familyIndex = 0;
        for (const auto &queueFamily : m_vkQueueFamiliesProp)
        {
            if (queueFamily.queueCount > 0 &&
                queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT &&
                !(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
            {
                m_vkQueueFamily[QueueType::QT_COMPUTE] = familyIndex;
//                m_vkComputeFamily = familyIndex;
            }

            if (m_vkQueueFamily[QT_TRANSFER] == m_vkQueueFamily[QT_COMPUTE] &&
                queueFamily.queueCount > 0 &&
                queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT &&
                !(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                !(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT))
            {
                m_vkQueueFamily[QueueType::QT_TRANSFER] = familyIndex;
//                m_vkTransferFamily = familyIndex;
            }

            ++familyIndex;
        }


        // индексы очередей для получения из запроса у логического устройства
        {
            ++countQueueInFamily[QT_GRAPHICS];

            if (m_vkQueueFamily[QT_COMPUTE] == m_vkQueueFamily[QT_GRAPHICS] &&
                m_vkQueueFamiliesProp[m_vkQueueFamily[QT_GRAPHICS]].queueCount > countQueueInFamily[QT_GRAPHICS])
            {
                m_vkQueueIndex[QT_COMPUTE] = countQueueInFamily[QT_GRAPHICS];
                ++countQueueInFamily[QT_GRAPHICS];
            }
            else
            {
                ++countQueueInFamily[QT_COMPUTE];
            }

            if (m_vkQueueFamily[QT_TRANSFER] == m_vkQueueFamily[QT_GRAPHICS] &&
                m_vkQueueFamiliesProp[m_vkQueueFamily[QT_GRAPHICS]].queueCount > countQueueInFamily[QT_GRAPHICS])
            {
                m_vkQueueIndex[QT_TRANSFER] = countQueueInFamily[QT_GRAPHICS];
                ++countQueueInFamily[QT_GRAPHICS];
            }
            else if (m_vkQueueFamily[QT_TRANSFER] == m_vkQueueFamily[QT_COMPUTE] &&
                     m_vkQueueFamiliesProp[m_vkQueueFamily[QT_COMPUTE]].queueCount > countQueueInFamily[QT_COMPUTE])
            {
                m_vkQueueIndex[QT_TRANSFER] = countQueueInFamily[QT_COMPUTE];
                ++countQueueInFamily[QT_COMPUTE];
            }
            else
            {
                ++countQueueInFamily[QT_TRANSFER];
            }

            if (m_vkQueueFamily[QT_PRESENT] == m_vkQueueFamily[QT_GRAPHICS] &&
                m_vkQueueFamiliesProp[m_vkQueueFamily[QT_GRAPHICS]].queueCount > countQueueInFamily[QT_GRAPHICS])
            {
                m_vkQueueIndex[QT_PRESENT] = countQueueInFamily[QT_GRAPHICS];
                ++countQueueInFamily[QT_GRAPHICS];
            }
            else if (m_vkQueueFamily[QT_PRESENT] == m_vkQueueFamily[QT_COMPUTE] &&
                     m_vkQueueFamiliesProp[m_vkQueueFamily[QT_COMPUTE]].queueCount > countQueueInFamily[QT_COMPUTE])
            {
                m_vkQueueIndex[QT_PRESENT] = countQueueInFamily[QT_COMPUTE];
                ++countQueueInFamily[QT_COMPUTE];
            }
            else if (m_vkQueueFamily[QT_PRESENT] == m_vkQueueFamily[QT_TRANSFER] &&
                     m_vkQueueFamiliesProp[m_vkQueueFamily[QT_TRANSFER]].queueCount > countQueueInFamily[QT_TRANSFER])
            {
                m_vkQueueIndex[QT_PRESENT] = countQueueInFamily[QT_TRANSFER];
                ++countQueueInFamily[QT_TRANSFER];
            }
            else
            {
                ++countQueueInFamily[QT_PRESENT];
            }
        }

    }

    const char *CGxVulkan::vkResultToString(VkResult result)
    {
        switch (result)
        {
        case VK_NOT_READY:
            return ("Fence или Query еще не завершен.");
        case VK_TIMEOUT:
            return ("Операция ожидания не завершена в указанное время.");
        case VK_EVENT_SET:
            return ("Событие находится в сигнальном состоянии.");
        case VK_EVENT_RESET:
            return ("Событие сброшено.");
        case VK_INCOMPLETE:
            return ("Массив возврата был слишком мал для результата.");
        case VK_SUBOPTIMAL_KHR:
            return ("Swapchain больше не точно соответствует свойствам поверхности.");
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return ("Не удалось выделить память хоста.");
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return ("Не удалось выделить память устройства.");
        case VK_ERROR_INITIALIZATION_FAILED:
            return ("Инициализация объекта не может быть завершена по причинам, "
                    "связанным с реализацией.");
        case VK_ERROR_DEVICE_LOST:
            return ("Логическое или физическое устройство было потеряно.");
        case VK_ERROR_MEMORY_MAP_FAILED:
            return ("Отображение объекта памяти не удалось.");
        case VK_ERROR_LAYER_NOT_PRESENT:
            return ("Запрошенный слой отсутствует или не может быть загружен.");
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return ("Запрошенное расширение не поддерживается.");
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return ("Запрошенная функция не поддерживается.");
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return ("Запрошенная версия Vulkan не поддерживается драйвером или "
                    "несовместима по другим причинам, связанным с реализацией.");
        case VK_ERROR_TOO_MANY_OBJECTS:
            return ("Уже создано слишком много объектов этого типа.");
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return ("Запрошенный формат не поддерживается на этом устройстве.");
        case VK_ERROR_FRAGMENTED_POOL:
            return ("Не удалось выделить пул из-за фрагментации памяти пула.");
        case VK_ERROR_SURFACE_LOST_KHR:
            return ("Поверхность больше не доступна.");
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return ("Запрошенное окно уже используется Vulkan или другим API "
                    "таким образом, что это предотвращает его повторное использование.");
        case VK_ERROR_OUT_OF_DATE_KHR:
            return ("Поверхность изменилась таким образом, что больше не совместима с "
                    "цепочкой обмена, и дальнейшие запросы на представление с использованием цепочки обмена не будут выполнены.");
        default:
            return ("");
        }

        return ("");
    }

}
