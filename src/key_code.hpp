//
// key_code.h - коды клавиш клавиатуры, мыши и т.д и т.п.
//

#pragma once

enum class KeyCode : unsigned char
{
    KEY_CODE_LBUTTON = 0x01, // VK_LBUTTON - Left mouse button
    KEY_CODE_RBUTTON = 0x02, // VK_RBUTTON - Right mouse button

    KEY_CODE_CANCEL = 0x03, // VK_CANCEL - Control-break processing

    KEY_CODE_MBUTTON = 0x04,  // VK_MBUTTON - Middle mouse button (three-button mouse)
    KEY_CODE_XBUTTON1 = 0x05, // VK_XBUTTON1 - X1 mouse button
    KEY_CODE_XBUTTON2 = 0x06, // VK_XBUTTON2 - X2 mouse button

    KEY_CODE_BACK = 0x08, // VK_BACK - BACKSPACE key
    KEY_CODE_TAB = 0x09,  // VK_TAB - TAB key

    KEY_CODE_CLEAR = 0x0C,  // VK_CLEAR - CLEAR key
    KEY_CODE_RETURN = 0x0D, // VK_RETURN - ENTER key

    KEY_CODE_WIN = 0x0F, // Windows key (Мое определение)
    KEY_CODE_SHIFT = 0x10, // VK_SHIFT - SHIFT key
    KEY_CODE_CONTROL = 0x11, // VK_CONTROL - CTRL key
    KEY_CODE_MENU = 0x12, // VK_MENU - ALT key
    
    KEY_CODE_PAUSE = 0x13, // VK_PAUSE - PAUSE key
    KEY_CODE_CAPITAL = 0x14, // VK_CAPITAL - CAPS LOCK key

    KEY_CODE_KANA = 0x15,    // VK_KANA - IME Kana mode
    KEY_CODE_IME_ON = 0x16,  // VK_IME_ON - IME On
    KEY_CODE_JUNJA = 0x17,   // VK_JUNJA - IME Junja mode
    KEY_CODE_FINAL = 0x18,   // VK_FINAL - IME final mode
    KEY_CODE_KANJI = 0x19,   // VK_KANJI - IME Kanji mode
    KEY_CODE_IME_OFF = 0x1A, // VK_IME_OFF - IME Off

    KEY_CODE_ESCAPE = 0x1B, // VK_ESCAPE - ESC key

    KEY_CODE_CONVERT = 0x1C,    // VK_CONVERT - IME convert
    KEY_CODE_NONCONVERT = 0x1D, // VK_NONCONVERT - IME nonconvert
    KEY_CODE_ACCEPT = 0x1E,     // VK_ACCEPT - IME accept
    KEY_CODE_MODECHANGE = 0x1F, // VK_MODECHANGE - IME mode change request

    KEY_CODE_SPACE = 0x20,    // VK_SPACE - SPACEBAR
    KEY_CODE_PRIOR = 0x21,    // VK_PRIOR - PAGE UP key
    KEY_CODE_NEXT = 0x22,     // VK_NEXT - PAGE DOWN key
    KEY_CODE_END = 0x23,      // VK_END - END key
    KEY_CODE_HOME = 0x24,     // VK_HOME - HOME key
    KEY_CODE_LEFT = 0x25,     // VK_LEFT - LEFT ARROW key
    KEY_CODE_UP = 0x26,       // VK_UP - UP ARROW key
    KEY_CODE_RIGHT = 0x27,    // VK_RIGHT - RIGHT ARROW key
    KEY_CODE_DOWN = 0x28,     // VK_DOWN - DOWN ARROW key
    KEY_CODE_SELECT = 0x29,   // VK_SELECT - SELECT key
    KEY_CODE_PRINT = 0x2A,    // VK_PRINT - PRINT key
    KEY_CODE_EXECUTE = 0x2B,  // VK_EXECUTE - EXECUTE key
    KEY_CODE_SNAPSHOT = 0x2C, // VK_SNAPSHOT - PRINT SCREEN key
    KEY_CODE_INSERT = 0x2D,   // VK_INSERT - INS key
    KEY_CODE_DELETE = 0x2E,   // VK_DELETE - DEL key
    KEY_CODE_HELP = 0x2F,     // VK_HELP- HELP key

    KEY_CODE_0 = 0x30, // VK_0 - 0 key
    KEY_CODE_1 = 0x31, // VK_1 - 1 key
    KEY_CODE_2 = 0x32, // VK_2 - 2 key
    KEY_CODE_3 = 0x33, // VK_3 - 3 key
    KEY_CODE_4 = 0x34, // VK_4 - 4 key
    KEY_CODE_5 = 0x35, // VK_5 - 5 key
    KEY_CODE_6 = 0x36, // VK_6 - 6 key
    KEY_CODE_7 = 0x37, // VK_7 - 7 key
    KEY_CODE_8 = 0x38, // VK_8 - 8 key
    KEY_CODE_9 = 0x39, // VK_9 - 9 key

    KEY_CODE_A = 0x41, // VK_A - A key
    KEY_CODE_B = 0x42, // VK_B - B key
    KEY_CODE_C = 0x43, // VK_C - C key
    KEY_CODE_D = 0x44, // VK_D - D key
    KEY_CODE_E = 0x45, // VK_E - E key
    KEY_CODE_F = 0x46, // VK_F - F key
    KEY_CODE_G = 0x47, // VK_G - G key
    KEY_CODE_H = 0x48, // VK_H - H key
    KEY_CODE_I = 0x49, // VK_I - I key
    KEY_CODE_J = 0x4A, // VK_J - J key
    KEY_CODE_K = 0x4B, // VK_K - K key
    KEY_CODE_L = 0x4C, // VK_L - L key
    KEY_CODE_M = 0x4D, // VK_M - M key
    KEY_CODE_N = 0x4E, // VK_N - N key
    KEY_CODE_O = 0x4F, // VK_O - O key
    KEY_CODE_P = 0x50, // VK_P - P key
    KEY_CODE_Q = 0x51, // VK_Q - Q key
    KEY_CODE_R = 0x52, // VK_R - R key
    KEY_CODE_S = 0x53, // VK_S - S key
    KEY_CODE_T = 0x54, // VK_T - T key
    KEY_CODE_U = 0x55, // VK_U - U key
    KEY_CODE_V = 0x56, // VK_V - V key
    KEY_CODE_W = 0x57, // VK_W - W key
    KEY_CODE_X = 0x58, // VK_X - X key
    KEY_CODE_Y = 0x59, // VK_Y - Y key
    KEY_CODE_Z = 0x5A, // VK_Z - Z key

    KEY_CODE_LWIN = 0x5B, // VK_LWIN - Left Windows key (Natural keyboard)
    KEY_CODE_RWIN = 0x5C, // VK_RWIN - Right Windows key (Natural keyboard)
    KEY_CODE_APPS = 0x5D, // VK_APPS - Applications key (Natural keyboard)

    KEY_CODE_SLEEP = 0x5F, // VK_SLEEP - Computer Sleep key

    KEY_CODE_NUMPAD0 = 0x60,   // VK_NUMPAD0 - Numeric keypad 0 key
    KEY_CODE_NUMPAD1 = 0x61,   // VK_NUMPAD1 - Numeric keypad 1 key
    KEY_CODE_NUMPAD2 = 0x62,   // VK_NUMPAD2 - Numeric keypad 2 key
    KEY_CODE_NUMPAD3 = 0x63,   // VK_NUMPAD3 - Numeric keypad 3 key
    KEY_CODE_NUMPAD4 = 0x64,   // VK_NUMPAD4 - Numeric keypad 4 key
    KEY_CODE_NUMPAD5 = 0x65,   // VK_NUMPAD5 - Numeric keypad 5 key
    KEY_CODE_NUMPAD6 = 0x66,   // VK_NUMPAD6 - Numeric keypad 6 key
    KEY_CODE_NUMPAD7 = 0x67,   // VK_NUMPAD7 - Numeric keypad 7 key
    KEY_CODE_NUMPAD8 = 0x68,   // VK_NUMPAD8 - Numeric keypad 8 key
    KEY_CODE_NUMPAD9 = 0x69,   // VK_NUMPAD9 - Numeric keypad 9 key
    KEY_CODE_MULTIPLY = 0x6A,  // VK_MULTIPLY - Multiply key |*|
    KEY_CODE_ADD = 0x6B,       // VK_ADD - Add key |+|
    KEY_CODE_SEPARATOR = 0x6C, // VK_SEPARATOR - Separator key
    KEY_CODE_SUBTRACT = 0x6D,  // VK_SUBTRACT - Subtract key |-|
    KEY_CODE_DECIMAL = 0x6E,   // VK_DECIMAL - Decimal key |.|
    KEY_CODE_DIVIDE = 0x6F,    // VK_DIVIDE - Divide key |/|

    KEY_CODE_F1 = 0x70,  // VK_F1 - F1 key
    KEY_CODE_F2 = 0x71,  // VK_F2 - F2 key
    KEY_CODE_F3 = 0x72,  // VK_F3 - F3 key
    KEY_CODE_F4 = 0x73,  // VK_F4 - F4 key
    KEY_CODE_F5 = 0x74,  // VK_F5 - F5 key
    KEY_CODE_F6 = 0x75,  // VK_F6 - F6 key
    KEY_CODE_F7 = 0x76,  // VK_F7 - F7 key
    KEY_CODE_F8 = 0x77,  // VK_F8 - F8 key
    KEY_CODE_F9 = 0x78,  // VK_F9 - F9 key
    KEY_CODE_F10 = 0x79, // VK_F10 - F10 key
    KEY_CODE_F11 = 0x7A, // VK_F11 - F11 key
    KEY_CODE_F12 = 0x7B, // VK_F12 - F12 key
    KEY_CODE_F13 = 0x7C, // VK_F13 - F13 key
    KEY_CODE_F14 = 0x7D, // VK_F14 - F14 key
    KEY_CODE_F15 = 0x7E, // VK_F15 - F15 key
    KEY_CODE_F16 = 0x7F, // VK_F16 - F16 key
    KEY_CODE_F17 = 0x80, // VK_F17 - F17 key
    KEY_CODE_F18 = 0x81, // VK_F18 - F18 key
    KEY_CODE_F19 = 0x82, // VK_F19 - F19 key
    KEY_CODE_F20 = 0x83, // VK_F20 - F20 key
    KEY_CODE_F21 = 0x84, // VK_F21 - F21 key
    KEY_CODE_F22 = 0x85, // VK_F22 - F22 key
    KEY_CODE_F23 = 0x86, // VK_F23 - F23 key
    KEY_CODE_F24 = 0x87, // VK_F24 - F24 key

    KEY_CODE_NAVIGATION_VIEW = 0x88,   //
    KEY_CODE_NAVIGATION_MENU = 0x89,   //
    KEY_CODE_NAVIGATION_UP = 0x8A,     //
    KEY_CODE_NAVIGATION_DOWN = 0x8B,   //
    KEY_CODE_NAVIGATION_LEFT = 0x8C,   //
    KEY_CODE_NAVIGATION_RIGHT = 0x8D,  //
    KEY_CODE_NAVIGATION_ACCEPT = 0x8E, //
    KEY_CODE_NAVIGATION_CANCEL = 0x8F, //

    KEY_CODE_NUMLOCK = 0x90, // VK_NUMLOCK - NUM LOCK key
    KEY_CODE_SCROLL = 0x91,  // VK_SCROLL - SCROLL LOCK key

    KEY_CODE_OEM_NEC_EQUAL = 0x92, // '=' key on numpad

    KEY_CODE_OEM_FJ_MASSHOU = 0x93, // 'Unregister word' key
    KEY_CODE_OEM_FJ_TOUROKU = 0x94, // 'Register word' key
    KEY_CODE_OEM_FJ_LOYA = 0x95,    // 'Left OYAYUBI' key
    KEY_CODE_OEM_FJ_ROYA = 0x96,    // 'Right OYAYUBI' key

    KEY_CODE_LSHIFT = 0xA0,   // VK_LSHIFT - Left SHIFT key
    KEY_CODE_RSHIFT = 0xA1,   // VK_RSHIFT - Right SHIFT key
    KEY_CODE_LCONTROL = 0xA2, // VK_LCONTROL - Left CONTROL key
    KEY_CODE_RCONTROL = 0xA3, // VK_RCONTROL - Right CONTROL key
    KEY_CODE_LMENU = 0xA4,    // VK_LMENU - Left ALT key
    KEY_CODE_RMENU = 0xA5,    // VK_RMENU - Right ALT key

    KEY_CODE_BROWSER_BACK = 0xA6,      //
    KEY_CODE_BROWSER_FORWARD = 0xA7,   //
    KEY_CODE_BROWSER_REFRESH = 0xA8,   //
    KEY_CODE_BROWSER_STOP = 0xA9,      //
    KEY_CODE_BROWSER_SEARCH = 0xAA,    //
    KEY_CODE_BROWSER_FAVORITES = 0xAB, //
    KEY_CODE_BROWSER_HOME = 0xAC,      //

    KEY_CODE_VOLUME_MUTE = 0xAD,         //
    KEY_CODE_VOLUME_DOWN = 0xAE,         //
    KEY_CODE_VOLUME_UP = 0xAF,           //
    KEY_CODE_MEDIA_NEXT_TRACK = 0xB0,    //
    KEY_CODE_MEDIA_PREV_TRACK = 0xB1,    //
    KEY_CODE_MEDIA_STOP = 0xB2,          //
    KEY_CODE_MEDIA_PLAY_PAUSE = 0xB3,    //
    KEY_CODE_LAUNCH_MAIL = 0xB4,         //
    KEY_CODE_LAUNCH_MEDIA_SELECT = 0xB5, //
    KEY_CODE_LAUNCH_APP1 = 0xB6,         //
    KEY_CODE_LAUNCH_APP2 = 0xB7,         //

    KEY_CODE_OEM_1 = 0xBA,      // ';:' for US
    KEY_CODE_OEM_PLUS = 0xBB,   // '+' any country
    KEY_CODE_OEM_COMMA = 0xBC,  // ',' any country
    KEY_CODE_OEM_MINUS = 0xBD,  // '-' any country
    KEY_CODE_OEM_PERIOD = 0xBE, // '.' any country
    KEY_CODE_OEM_2 = 0xBF,      // '/?' for US
    KEY_CODE_OEM_3 = 0xC0,      // '`~' for US

    KEY_CODE_GAMEPAD_A = 0xC3,                       //
    KEY_CODE_GAMEPAD_B = 0xC4,                       //
    KEY_CODE_GAMEPAD_X = 0xC5,                       //
    KEY_CODE_GAMEPAD_Y = 0xC6,                       //
    KEY_CODE_GAMEPAD_RIGHT_SHOULDER = 0xC7,          //
    KEY_CODE_GAMEPAD_LEFT_SHOULDER = 0xC8,           //
    KEY_CODE_GAMEPAD_LEFT_TRIGGER = 0xC9,            //
    KEY_CODE_GAMEPAD_RIGHT_TRIGGER = 0xCA,           //
    KEY_CODE_GAMEPAD_DPAD_UP = 0xCB,                 //
    KEY_CODE_GAMEPAD_DPAD_DOWN = 0xCC,               //
    KEY_CODE_GAMEPAD_DPAD_LEFT = 0xCD,               //
    KEY_CODE_GAMEPAD_DPAD_RIGHT = 0xCE,              //
    KEY_CODE_GAMEPAD_MENU = 0xCF,                    //
    KEY_CODE_GAMEPAD_VIEW = 0xD0,                    //
    KEY_CODE_GAMEPAD_LEFT_THUMBSTICK_BUTTON = 0xD1,  //
    KEY_CODE_GAMEPAD_RIGHT_THUMBSTICK_BUTTON = 0xD2, //
    KEY_CODE_GAMEPAD_LEFT_THUMBSTICK_UP = 0xD3,      //
    KEY_CODE_GAMEPAD_LEFT_THUMBSTICK_DOWN = 0xD4,    //
    KEY_CODE_GAMEPAD_LEFT_THUMBSTICK_RIGHT = 0xD5,   //
    KEY_CODE_GAMEPAD_LEFT_THUMBSTICK_LEFT = 0xD6,    //
    KEY_CODE_GAMEPAD_RIGHT_THUMBSTICK_UP = 0xD7,     //
    KEY_CODE_GAMEPAD_RIGHT_THUMBSTICK_DOWN = 0xD8,   //
    KEY_CODE_GAMEPAD_RIGHT_THUMBSTICK_RIGHT = 0xD9,  //
    KEY_CODE_GAMEPAD_RIGHT_THUMBSTICK_LEFT = 0xDA,   //

    KEY_CODE_OEM_4 = 0xDB, //  '[{' for US
    KEY_CODE_OEM_5 = 0xDC, //  '\|' for US
    KEY_CODE_OEM_6 = 0xDD, //  ']}' for US
    KEY_CODE_OEM_7 = 0xDE, //  ''"' for US
    KEY_CODE_OEM_8 = 0xDF, //

    KEY_CODE_OEM_AX = 0xE1,   //  'AX' key on Japanese AX kbd
    KEY_CODE_OEM_102 = 0xE2,  //  "<>" or "\|" on RT 102-key kbd.
    KEY_CODE_ICO_HELP = 0xE3, //  Help key on ICO
    KEY_CODE_ICO_00 = 0xE4,   //  00 key on ICO

    KEY_CODE_PROCESSKEY = 0xE5, //

    KEY_CODE_ICO_CLEAR = 0xE6, //

    KEY_CODE_PACKET = 0xE7, //

    KEY_CODE_OEM_RESET = 0xE9,   //
    KEY_CODE_OEM_JUMP = 0xEA,    //
    KEY_CODE_OEM_PA1 = 0xEB,     //
    KEY_CODE_OEM_PA2 = 0xEC,     //
    KEY_CODE_OEM_PA3 = 0xED,     //
    KEY_CODE_OEM_WSCTRL = 0xEE,  //
    KEY_CODE_OEM_CUSEL = 0xEF,   //
    KEY_CODE_OEM_ATTN = 0xF0,    //
    KEY_CODE_OEM_FINISH = 0xF1,  //
    KEY_CODE_OEM_COPY = 0xF2,    //
    KEY_CODE_OEM_AUTO = 0xF3,    //
    KEY_CODE_OEM_ENLW = 0xF4,    //
    KEY_CODE_OEM_BACKTAB = 0xF5, //

    KEY_CODE_ATTN = 0xF6,     //
    KEY_CODE_CRSEL = 0xF7,    //
    KEY_CODE_EXSEL = 0xF8,    //
    KEY_CODE_EREOF = 0xF9,    //
    KEY_CODE_PLAY = 0xFA,     //
    KEY_CODE_ZOOM = 0xFB,     //
    KEY_CODE_NONAME = 0xFC,   //
    KEY_CODE_PA1 = 0xFD,      //
    KEY_CODE_OEM_CLEAR = 0xFE //
};

enum MouseCode : unsigned short
{
    // кнопки мыши
    MOUSE_CODE_LBUTTON = 0x01,  // левая кнопка мыши
    MOUSE_CODE_RBUTTON = 0x02,  // правая кнопка мыши
    MOUSE_CODE_MBUTTON = 0x04,  // средняя кнопка мыши
    MOUSE_CODE_4_BUTTON = 0x08, // 1-я дополнительная кнопка мыши
    MOUSE_CODE_5_BUTTON = 0x10, // 2-я дополнительная кнопка мыши
    MOUSE_CODE_6_BUTTON = 0x20, // 3-я дополнительная кнопка мыши
    MOUSE_CODE_7_BUTTON = 0x40, // 4-я дополнительная кнопка мыши
    MOUSE_CODE_8_BUTTON = 0x80, // 5-я дополнительная кнопка мыши

    // ключи модификаторы
    MOUSE_CODE_MODE_LSHIFT = 0x0100, // левый shift - нажата?
    MOUSE_CODE_MODE_RSHIFT = 0x0200, // правый shift - нажата?
    MOUSE_CODE_MODE_LCTRL = 0x0400, // левый Ctrl - нажата?
    MOUSE_CODE_MODE_RCTRL = 0x0800, // правый Ctrl - нажата?
    MOUSE_CODE_MODE_LALT = 0x1000, // левый Alt - нажата?
    MOUSE_CODE_MODE_RALT = 0x2000, // правый Alt - нажата?
    MOUSE_CODE_MODE_LWIN = 0x4000, // левый Win - нажата?
    MOUSE_CODE_MODE_RWIN = 0x8000, // правый Win - нажата?

    MOUSE_CODE_MAX
};
