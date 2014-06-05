#include <utopia.h>
#include "keyboard.h"

static int scancodeTable[] =
{
    KEY_UNKNOWN,       // 0x00
    KEY_ESCAPE,        // 0x01
    KEY_1,             // 0x02
    KEY_2,             // 0x03
    KEY_3,             // 0x04
    KEY_4,             // 0x05
    KEY_5,             // 0x06
    KEY_6,             // 0x07
    KEY_7,             // 0x08
    KEY_8,             // 0x09
    KEY_9,             // 0x0A
    KEY_0,             // 0x0B
    KEY_MINUS,         // 0x0C
    KEY_EQUAL,         // 0x0D
    KEY_BACKSPACE,     // 0x0E
    KEY_TAB,           // 0x0F
    KEY_Q,             // 0x10
    KEY_W,             // 0x11
    KEY_E,             // 0x12
    KEY_R,             // 0x13
    KEY_T,             // 0x14
    KEY_Y,             // 0x15
    KEY_U,             // 0x16
    KEY_I,             // 0x17
    KEY_O,             // 0x18
    KEY_P,             // 0x19
    KEY_LEFTBRACKET,   // 0x1A
    KEY_RIGHTBRACKET,  // 0x1B
    KEY_RETURN,        // 0x1C
    KEY_LCTRL,         // 0x1D
    KEY_A,             // 0x1E
    KEY_S,             // 0x1F
    KEY_D,             // 0x20
    KEY_F,             // 0x21
    KEY_G,             // 0x22
    KEY_H,             // 0x23
    KEY_J,             // 0x24
    KEY_K,             // 0x25
    KEY_L,             // 0x26
    KEY_SEMICOLON,     // 0x27
    KEY_QUOTE,         // 0x28
    KEY_GRAVE,         // 0x29
    KEY_LSHIFT,        // 0x2A
    KEY_BACKSLASH,     // 0x2B
    KEY_Z,             // 0x2C
    KEY_X,             // 0x2D
    KEY_C,             // 0x2E
    KEY_V,             // 0x2F
    KEY_B,             // 0x30
    KEY_N,             // 0x31
    KEY_M,             // 0x32
    KEY_COMMA,         // 0x33
    KEY_DOT,           // 0x34
    KEY_SLASH,         // 0x35
    KEY_RSHIFT,        // 0x36
    KEY_KP_ASTERISK,   // 0x37
    KEY_RALT,          // 0x38
    KEY_SPACE,         // 0x39
    KEY_CAPSLOCK,      // 0x3A
    KEY_F1,            // 0x3B
    KEY_F2,            // 0x3C
    KEY_F3,            // 0x3D
    KEY_F4,            // 0x3E
    KEY_F5,            // 0x3F
    KEY_F6,            // 0x40
    KEY_F7,            // 0x41
    KEY_F8,            // 0x42
    KEY_F9,            // 0x43
    KEY_F10,           // 0x44
    KEY_KP_NUMLOCK,    // 0x45
    KEY_SCROLLLOCK,    // 0x46
    KEY_HOME,          // 0x47
    KEY_KP_8,          // 0x48
    KEY_PAGEUP,        // 0x49
    KEY_KP_2,          // 0x50
    KEY_KP_3,          // 0x51
    KEY_KP_0,          // 0x52
    KEY_KP_DECIMAL,    // 0x53
    KEY_UNKNOWN,       // 0x54
    KEY_UNKNOWN,       // 0x55
    KEY_UNKNOWN,       // 0x56
    KEY_F11,           // 0x57
    KEY_F12            // 0x58
};

int main(void)
{
    irq_subscribe(1);

    while(1)
    {
        irq_wait(1);

        if (inb(0x64) & 1)
        {
            int code = inb(0x60);
            if (code & 0x80) continue;

            int key = scancodeTable[code];
            put((char)key);
        }
    }

    return 0;
}
