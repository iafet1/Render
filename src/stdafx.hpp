//
// stdafx.hpp
//

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

// указатели на функции Vulkan необходимо запрашивать (нет прототипов функций)
#define VK_NO_PROTOTYPES

#if defined(_WIN32) || defined(WIN32)
     // Файлы заголовков Windows:
    #define WIN32_LEAN_AND_MEAN             // Исключаем редко используемые компоненты из заголовков Windows
    #include <windows.h>

    #define VK_USE_PLATFORM_WIN32_KHR		// платформа WIN32 для Vulkan

#else
    #error "Другие платформы пока не поддерживаются"
#endif

// данные для исключения
struct Exception {
    const char* str;  // имя файла
    uint32_t    line; // номер строки
};

// вызов исключения
#define THROW() throw((Exception){ FILE_NAME, __LINE__ })


// размер массива
#ifndef ARRAYSIZE
    #define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
#endif // ARRAYSIZE