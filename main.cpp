//
// main.cpp - точка входа
//

#include "src/stdafx.hpp"
#include "src/avmCLog.hpp"
#include "src/avmCApplication.hpp"

int main(int, char**) {

    uint32_t iRet = 0;

    #ifdef _WIN32
        // установка UTF-8 для консоли вывода
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
    #endif

    avm::CLog log("logger.log");
    uint32_t result = 0;

    try {
        avm::CApplication app("Render");

        app.Run();

        result = app.GetExitCode();
    }
    catch(const Exception& exc) {
        LOG_ERROR("File - %s, line - %d.", exc.str, exc.line);
        result = 0xFFFFFFFF;
    }
    
    
    return (int)result;
}
