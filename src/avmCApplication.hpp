//
// avmCApplication.hpp - определение основного класса приложения
//

#pragma once

#include "stdafx.hpp"
#include "platform/platforms.hpp"
#include "avmCGxDevice.hpp"

namespace avm {

    class CInput;
    class CRender;

    class CApplication
    {
    public:
        CApplication(const char* name);
        ~CApplication();

        // удаление
        CApplication(const CApplication &) = delete;
        CApplication &operator=(const CApplication &) = delete;


        // Интерфейс API
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // Запускает основной цикл программы
        void Run();

        // Запрос кода завершения
        inline uint32_t GetExitCode() { return m_exitCode;}

        //
        void Initialize();

        //
        void Update(float dt);

        //
        void FixedUpdate();

        //
        void Render();

        //
        void Compose(/*wi::graphics::CommandList cmd*/);



    private:
        /* functions */

        // иницилизация
        void initialization();
        // завершение
        void shutdown();

        // установка кода и флага завершения
        inline void setExitCode(uint32_t exitCode) { m_exitCode = exitCode; m_exit = true;}

        // создание основного окна приложения
        void windowCreate(uint32_t width, uint32_t height);

        // чтение сообщений окна приложения
        void getMessageWindow();

        // обработчик комманды QUIT
        static Variant onQuit(void *object, Variant param1, Variant param2);
        
    
    private :
        /* data */

        // система ввода
        CInput* m_input{nullptr};

        // рендер
        CRender *m_render{nullptr};

        // графическое устройство
        graphics::CGxDevice *m_gxDevice{nullptr};

        // приложение
        const char *m_name{nullptr}; // имя программы
        uint32_t m_exitCode{0};      // код завершения приложения

        float m_targetFrameRate{60.0f};   // частота кадров для цикла FixedUpdate() (по умолчанию = 60)
        float m_deltaTime{0.0f};            // разность времени между обновлениями
        float m_deltaTimeAccumulator{0.0f}; // аккумулированная разность времени между обновлениями для FixedUpdate()

        bool m_initialized{false};    // флаг иницилизации
        bool m_isWindowActive{true};  // окно активно?
        bool m_exit{false};           // флаг завершения приложения
        bool m_frameSkip{true};       // желаемое поведение цикла FixedUpdate() (по умолчанию = true)
        bool m_frameRateLock{false}; //

        // окно
        platform::WindowDesc m_window; // свойства окна


    };
}

// extern avm::CApplication* g_App;