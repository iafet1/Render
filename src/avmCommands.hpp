//
// avmCommands.hpp - комманды приложения
//

#pragma once

#include "stdafx.hpp"

namespace avm::command {

// комманды приложения
enum class Commands {
    COMMAND_QUIT = 0, // выход из приложения

    COMMAND_MAX
};

// прототип функции обработки комманды
typedef Variant (*PFN_onCommand)(void* object, Variant param1, Variant param2);

// регистрация обработчика команды
bool RegisterHandler(Commands command, void* object, PFN_onCommand pfnOnCommand);

// отмена обработчика комманды
bool UnregisterHandler(Commands command, void *object);

// получить обработчик комманды
PFN_onCommand GetHandler(Commands command);

// выполнение комманды
Variant Execute(Commands command, Variant param1 = {0}, Variant param2 = {0});
}