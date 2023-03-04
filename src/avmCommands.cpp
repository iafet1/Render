//
// avmCommands.cpp - 
//

#include "avmCommands.hpp"
#include "avmCLog.hpp"

namespace avm::command {

// обработчик комманды
struct HandlerCommand {
    void* object; // объект, чей обработчик
    PFN_onCommand pfnHandler; // обработчик комманды
};

// массив обработчиков комманд (1 команда = 1 обработчик)
static HandlerCommand g_handlers[static_cast<int>(Commands::COMMAND_MAX)];

bool RegisterHandler(Commands command, void *object, PFN_onCommand pfnOnCommand)
{
    assert(command != Commands::COMMAND_MAX && "Не допустимый параметр.");

    HandlerCommand &hd = g_handlers[static_cast<int>(command)];
    if(pfnOnCommand == nullptr) {
        LOG_ERROR("Нулевой указатель на обработчик комманды.");
        return false;
    }

    g_handlers[static_cast<int>(command)].object = object;
    g_handlers[static_cast<int>(command)].pfnHandler = pfnOnCommand;
    return true;
}

bool UnregisterHandler(Commands command, void *object){
    assert(command != Commands::COMMAND_MAX &&
           g_handlers[static_cast<int>(command)].object == object &&
           "Не допустимый параметр.");

    g_handlers[static_cast<int>(command)].object = nullptr;
    g_handlers[static_cast<int>(command)].pfnHandler = nullptr;
    return true;
}

PFN_onCommand GetHandler(Commands command)
{
    assert(command != Commands::COMMAND_MAX && "Не допустимый параметр.");
    return g_handlers[static_cast<int>(command)].pfnHandler;
}

Variant Execute(Commands command, Variant param1, Variant param2)
{
    assert(command != Commands::COMMAND_MAX && "Не допустимый параметр.");
    HandlerCommand &hd = g_handlers[static_cast<int>(command)];
    if (hd.pfnHandler != nullptr) {
        return hd.pfnHandler(hd.object, param1, param2);
    } 
    
    return {0};
}

}