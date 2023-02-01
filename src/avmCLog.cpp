//
// avmCLog.cpp - система логирования в общий файл и на терминал
//
// 30.10.2022 11:17
//


#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "stdafx.hpp"
#include "avmCLog.hpp"

// максимальный размер буфера сообщения
#define BUFFER_MSG_SIZE 4000

namespace avm {
    static CLog* g_Log = nullptr;   // глобальный логгер

    CLog::CLog(const char* name)
    {
        if (name == nullptr) {
            return;
        }

        if (g_Log == nullptr) {
            g_Log = this;
        }

        m_fileName = name;

        char buf[BUFFER_MSG_SIZE];
        time_t t;
        tm  st;

        _time64(&t);
        _gmtime64_s(&st, &t);
        uint32_t len = sprintf_s(buf, BUFFER_MSG_SIZE, "Начало логирования: %d.%d.%d %d:%d:%d.\n%s\n\n",
            st.tm_mday, st.tm_mon+1, st.tm_year+1900, st.tm_hour+3, st.tm_min, st.tm_sec,
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        
        printf_s(buf);  // вывод на консоль

        FILE* file = NULL;
        errno_t err = fopen_s(&file, m_fileName, "wb");
        if (file != NULL && !err) {
            char bom[] = {'\xEF', '\xBB', '\xBF'};
            fwrite(bom, sizeof(char), sizeof(bom), file);
            fwrite(buf, sizeof(char), len, file);
            fflush(file);
            fclose(file);
        }
    }

    CLog::~CLog()
    {
        if (m_fileName == nullptr) {
            return;
        }

        char buf[BUFFER_MSG_SIZE];
        uint32_t len = sprintf_s(buf, BUFFER_MSG_SIZE, "\n%s\nКонец логирования.\n", "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

        printf_s(buf);

        FILE* file = NULL;
        errno_t err = fopen_s(&file, m_fileName, "ab");
        if(file != NULL && !err) {
            fwrite(buf, sizeof(char), len, file);
            fflush(file);
            fclose(file);
        }
    }

    void CLog::Message(LogMessageType type, const char* format, va_list args)
    {
        if (m_fileName == nullptr) {
            return;
        }

        char typeMsg[][10] = {
            "[ERROR]  ",
            "[WARNING]",
            "[INFO]   ",
            "[DEBUG]  "
        };

        clock_t time = clock(); // текущий таймер
        uint32_t sec = time / CLOCKS_PER_SEC;
        uint32_t msec = time - (sec * CLOCKS_PER_SEC);

        static char msg[BUFFER_MSG_SIZE];
        uint32_t count;

        {
            char buf[BUFFER_MSG_SIZE]; // временный буфер
            vsprintf_s(buf, BUFFER_MSG_SIZE, format, args);
            
            count = sprintf_s(msg, BUFFER_MSG_SIZE, "%d.%d\t %s\t: %s\n", sec, msec,
                typeMsg[static_cast<int>(type)], buf);
        }

        {
            const static char color[4][6] = {
                "1;91", "22;33", "1;32", "1;34"
            };
            printf_s("\033[%sm%s\033[0m", color[static_cast<int>(type)], msg);
            fflush(NULL);
        }

        {
            FILE* file = NULL;
            errno_t err = fopen_s(&file, m_fileName, "ab");
            if (file != NULL && !err) {
                fwrite(msg, sizeof(char), count, file);
                fflush(file);
                fclose(file);
            }
        }
    }

    void Message(avm::LogMessageType type, const char* format, ...)
    {
        if (g_Log != nullptr) {
            va_list args;
            va_start(args, format);
            g_Log->Message(type, format, args);
            va_end(args);
        }
    }

}
