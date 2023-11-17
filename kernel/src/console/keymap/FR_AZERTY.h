/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include "Keymap.h"

namespace kernel::console::keymap {

keymap AZERTY_FR = {
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_RESERVED
    {.base = '\e', .maj = '\e', .ctrl_alt = ''}, // KEY_ESC
    {.base = '&', .maj = '1', .ctrl_alt = ''},   // KEY_1
    {.base = 'é', .maj = '2', .ctrl_alt = ''},   // KEY_2
    {.base = '"', .maj = '3', .ctrl_alt = ''},   // KEY_3
    {.base = '\'', .maj = '4', .ctrl_alt = ''},  // KEY_4
    {.base = '(', .maj = '5', .ctrl_alt = ''},   // KEY_5
    {.base = '-', .maj = '6', .ctrl_alt = ''},   // KEY_6
    {.base = 'è', .maj = '7', .ctrl_alt = ''},   // KEY_7
    {.base = '_', .maj = '8', .ctrl_alt = ''},   // KEY_8
    {.base = 'ç', .maj = '9', .ctrl_alt = ''},   // KEY_9
    {.base = 'à', .maj = '0', .ctrl_alt = ''},   // KEY_0
    {.base = ')', .maj = '°', .ctrl_alt = ''},   // KEY_MINUS
    {.base = '=', .maj = '+', .ctrl_alt = ''},   // KEY_EQUAL
    {.base = '\b', .maj = '\b', .ctrl_alt = ''}, // KEY_BACKSPACE
    {.base = '\t', .maj = '\t', .ctrl_alt = ''}, // KEY_TAB
    {.base = 'a', .maj = 'A', .ctrl_alt = ''},   // KEY_Q
    {.base = 'z', .maj = 'Z', .ctrl_alt = ''},   // KEY_W
    {.base = 'e', .maj = 'E', .ctrl_alt = '€'},  // KEY_E
    {.base = 'r', .maj = 'R', .ctrl_alt = ''},   // KEY_R
    {.base = 't', .maj = 'T', .ctrl_alt = ''},   // KEY_T
    {.base = 'y', .maj = 'Y', .ctrl_alt = ''},   // KEY_Y
    {.base = 'u', .maj = 'U', .ctrl_alt = ''},   // KEY_U
    {.base = 'i', .maj = 'I', .ctrl_alt = ''},   // KEY_I
    {.base = 'o', .maj = 'O', .ctrl_alt = ''},   // KEY_O
    {.base = 'p', .maj = 'P', .ctrl_alt = ''},   // KEY_P
    {.base = '(', .maj = '¨', .ctrl_alt = ''},   // KEY_LEFTBRACE
    {.base = ')', .maj = '£', .ctrl_alt = ''},   // KEY_RIGHTBRACE
    {.base = '\n', .maj = '\n', .ctrl_alt = ''}, // KEY_ENTER
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_LEFTCTRL
    {.base = 'q', .maj = 'Q', .ctrl_alt = ''},   // KEY_A
    {.base = 's', .maj = 'S', .ctrl_alt = ''},   // KEY_S
    {.base = 'd', .maj = 'D', .ctrl_alt = ''},   // KEY_D
    {.base = 'f', .maj = 'F', .ctrl_alt = ''},   // KEY_F
    {.base = 'g', .maj = 'G', .ctrl_alt = ''},   // KEY_G
    {.base = 'h', .maj = 'H', .ctrl_alt = ''},   // KEY_H
    {.base = 'j', .maj = 'J', .ctrl_alt = ''},   // KEY_J
    {.base = 'k', .maj = 'K', .ctrl_alt = ''},   // KEY_K
    {.base = 'l', .maj = 'L', .ctrl_alt = ''},   // KEY_L
    {.base = 'm', .maj = 'M', .ctrl_alt = ''},   // KEY_SEMICOLON
    {.base = 'ù', .maj = '%', .ctrl_alt = ''},   // KEY_APOSTROPHE
    {.base = '²', .maj = '', .ctrl_alt = ''},    // KEY_GRAVE
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_LEFTSHIFT
    {.base = '*', .maj = 'µ', .ctrl_alt = ''},   // KEY_BACKSLASH
    {.base = 'w', .maj = 'W', .ctrl_alt = ''},   // KEY_Z
    {.base = 'x', .maj = 'X', .ctrl_alt = ''},   // KEY_X
    {.base = 'c', .maj = 'C', .ctrl_alt = ''},   // KEY_C
    {.base = 'v', .maj = 'V', .ctrl_alt = ''},   // KEY_V
    {.base = 'b', .maj = 'B', .ctrl_alt = ''},   // KEY_B
    {.base = 'n', .maj = 'N', .ctrl_alt = ''},   // KEY_N
    {.base = ',', .maj = '?', .ctrl_alt = ''},   // KEY_M
    {.base = ';', .maj = '.', .ctrl_alt = ''},   // KEY_COMMA
    {.base = ':', .maj = '/', .ctrl_alt = ''},   // KEY_DOT
    {.base = '!', .maj = '§', .ctrl_alt = ''},   // KEY_SLASH
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_RIGHTSHIFT
    {.base = '*', .maj = '*', .ctrl_alt = ''},   // KEY_KPASTERISK
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_LEFTALT
    {.base = ' ', .maj = ' ', .ctrl_alt = ''},   // KEY_SPACE
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_CAPSLOCK
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_F1
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_F2
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_F3
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_F4
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_F5
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_F6
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_F7
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_F8
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_F9
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_F10
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_NUMLOCK
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_SCROLLLOCK
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_KP7
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_KP8
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_KP9
    {.base = '-', .maj = '-', .ctrl_alt = ''},   // KEY_KPMINUS
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_KP4
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_KP5
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_KP6
    {.base = '+', .maj = '+', .ctrl_alt = ''},   // KEY_KPPLUS
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_KP1
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_KP2
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_KP3
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_KP0
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_KPDOT
    {.base = '', .maj = '', .ctrl_alt = ''},     // KEY_ZENKAKUHANKAKU
    {.base = '<', .maj = '>', .ctrl_alt = ''},   // KEY_102ND
};

}
