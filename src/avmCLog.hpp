//
// avmCLog.hpp - система логирования в общий файл и на терминал
//

#pragma once

namespace avm {
    class CLog; // класс для логирования

    // тип сообщения лога
    enum class LogMessageType {
        LMT_ERROR,
        LMT_WARNING,
        LMT_INFO,
        LMT_DEBUG
    };

    class CLog final
    {
    public:
        CLog(const char* name = nullptr); // конструктор
        ~CLog(); // деструктор

        // отправка сообщения для лога
        void Message(LogMessageType type, const char* format, va_list args);

    private:
        const char* m_fileName{nullptr}; // имя файла для лога
    };

    // отправка сообщения в систему логирования
    void Message(avm::LogMessageType type, const char* format, ...);
}

#if defined(_LOG) || defined(_DEBUG) || defined(DEBUG)
    // макросы для глобального логирования
    #define LOG_ERROR(...)   avm::Message(avm::LogMessageType::LMT_ERROR, __VA_ARGS__)
    #define LOG_WARNING(...) avm::Message(avm::LogMessageType::LMT_WARNING, __VA_ARGS__)
    #define LOG_INFO(...)    avm::Message(avm::LogMessageType::LMT_INFO, __VA_ARGS__)
    #define LOG_DEBUG(...)   avm::Message(avm::LogMessageType::LMT_DEBUG, __VA_ARGS__)

#else
    #define LOG_WARNING(...)
    #define LOG_INFO(...) 
    #define LOG_DEBUG(...)
    #define LOG_ERROR(...)

#endif // _LOG