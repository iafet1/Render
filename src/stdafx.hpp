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

// общий тип переменной
union Variant
{
    void* ptr; // указатель

    double f64; // двойной точности
    float f32; // с плавающей точкой одинарной точности

    int64_t i64; // целое со знаком 64 бит
    int32_t i32; // целое со знаком 32 бит
    int16_t i16; // целое со знаком 16 бит
    int8_t i8; // целое со знаком 8 бит

    uint64_t u64; // целое без знаком 64 бит
    uint32_t u32; // целое без знаком 32 бит
    uint16_t u16; // целое без знаком 16 бит
    uint8_t u8;   // целое без знаком 8 бит

    uint32_t a32[2]; // массив (2) 32 бит
    uint16_t a16[4]; // массив (4) 16 бит
    uint8_t a8[8]; // массив (8) 16 бит
};
