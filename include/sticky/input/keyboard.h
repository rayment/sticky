/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * keyboard.h
 * Keyboard input handler header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 09/05/2021
 */

#ifndef FR_RAYMENT_STICKY_KEYBOARD_H
#define FR_RAYMENT_STICKY_KEYBOARD_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/includes.h"
#include "sticky/common/types.h"

/**
 * @addtogroup keyboard
 * @{
 */

#define S_KEY_0               SDLK_0            /* 0 */
#define S_KEY_1               SDLK_1            /* 1 */
#define S_KEY_2               SDLK_2            /* 2 */
#define S_KEY_3               SDLK_3            /* 3 */
#define S_KEY_4               SDLK_4            /* 4 */
#define S_KEY_5               SDLK_5            /* 5 */
#define S_KEY_6               SDLK_6            /* 6 */
#define S_KEY_7               SDLK_7            /* 7 */
#define S_KEY_8               SDLK_8            /* 8 */
#define S_KEY_9               SDLK_9            /* 9 */

#define S_KEY_EXCLAMATIONMARK SDLK_EXCLAIM      /* ! */
#define S_KEY_AT              SDLK_AT           /* @ */
#define S_KEY_HASH            SDLK_HASH         /* # */
#define S_KEY_DOLLAR          SDLK_DOLLAR       /* $ */
#define S_KEY_PERCENT         SDLK_PERCENT      /* % */
#define S_KEY_CARET           SDLK_CARET        /* ^ */
#define S_KEY_AMPERSAND       SDLK_AMPERSAND    /* &*/
#define S_KEY_ASTERISK        SDLK_ASTERISK     /* * */
#define S_KEY_LEFTPAREN       SDLK_LEFTPAREN    /* ( */
#define S_KEY_RIGHTPAREN      SDLK_RIGHTPAREN   /* ) */

#define S_KEY_BACKQUOTE       SDLK_BACKQUOTE    /* ` */
#define S_KEY_BACKTICK        S_KEY_BACKQUOTE   /* ` alt */

#define S_KEY_MINUS           SDLK_MINUS        /* - */
#define S_KEY_UNDERSCORE      SDLK_UNDERSCORE   /* _ */
#define S_KEY_PLUS            SDLK_PLUS         /* + */
#define S_KEY_EQUALS          SDLK_EQUALS       /* = */

#define S_KEY_BACKSPACE       SDLK_BACKSPACE    /* backspace */
#define S_KEY_TAB             SDLK_TAB          /* tab */
#define S_KEY_RETURN          SDLK_RETURN       /* return */
#define S_KEY_ENTER           S_KEY_RETURN      /* return alt */
#define S_KEY_ESCAPE          SDLK_ESCAPE       /* escape */
#define S_KEY_SPACE           SDLK_SPACE        /* space */
#define S_KEY_CAPSLOCK        SDLK_CAPSLOCK     /* caps lock */

#define S_KEY_LEFTBRACKET     SDLK_LEFTBRACKET  /* [ */
#define S_KEY_RIGHTBRACKET    SDLK_RIGHTBRACKET /* ] */
#define S_KEY_BACKSLASH       SDLK_BACKSLASH    /* \ */
#define S_KEY_LEFTARROW       SDLK_LESS         /* < */
#define S_KEY_RIGHTARROW      SDLK_GREATER      /* > */
#define S_KEY_COMMA           SDLK_COMMA        /* , */
#define S_KEY_PERIOD          SDLK_PERIOD       /* . */
#define S_KEY_FULLSTOP        S_KEY_PERIOD      /* . alt */
#define S_KEY_QUESTIONMARK    SDLK_QUESTION     /* ? */
#define S_KEY_SLASH           SDLK_SLASH        /* / */
#define S_KEY_FORWARDSLASH    S_KEY_SLASH       /* / alt */
#define S_KEY_COLON           SDLK_COLON        /* : */
#define S_KEY_SEMICOLON       SDLK_SEMICOLON    /* ; */
#define S_KEY_QUOTEDBL        SDLK_QUOTEDBL     /* " */
#define S_KEY_QUOTE           SDLK_QUOTE        /* ' */

#define S_KEY_A               SDLK_a            /* a */
#define S_KEY_B               SDLK_b            /* b */
#define S_KEY_C               SDLK_c            /* c */
#define S_KEY_D               SDLK_d            /* d */
#define S_KEY_E               SDLK_e            /* e */
#define S_KEY_F               SDLK_f            /* f */
#define S_KEY_G               SDLK_g            /* g */
#define S_KEY_H               SDLK_h            /* h */
#define S_KEY_I               SDLK_i            /* i */
#define S_KEY_J               SDLK_j            /* j */
#define S_KEY_K               SDLK_k            /* k */
#define S_KEY_L               SDLK_l            /* l */
#define S_KEY_M               SDLK_m            /* m */
#define S_KEY_N               SDLK_n            /* n */
#define S_KEY_O               SDLK_o            /* o */
#define S_KEY_P               SDLK_p            /* p */
#define S_KEY_Q               SDLK_q            /* q */
#define S_KEY_R               SDLK_r            /* r */
#define S_KEY_S               SDLK_s            /* s */
#define S_KEY_T               SDLK_t            /* t */
#define S_KEY_U               SDLK_u            /* u */
#define S_KEY_V               SDLK_v            /* v */
#define S_KEY_W               SDLK_w            /* w */
#define S_KEY_X               SDLK_x            /* x */
#define S_KEY_Y               SDLK_y            /* y */
#define S_KEY_Z               SDLK_z            /* z */

#define S_KEY_F1              SDLK_F1           /* F1 */
#define S_KEY_F2              SDLK_F2           /* F2 */
#define S_KEY_F3              SDLK_F3           /* F3 */
#define S_KEY_F4              SDLK_F4           /* F4 */
#define S_KEY_F5              SDLK_F5           /* F5 */
#define S_KEY_F6              SDLK_F6           /* F6 */
#define S_KEY_F7              SDLK_F7           /* F7 */
#define S_KEY_F8              SDLK_F8           /* F8 */
#define S_KEY_F9              SDLK_F9           /* F9 */
#define S_KEY_F10             SDLK_F10          /* F10 */
#define S_KEY_F11             SDLK_F11          /* F11 */
#define S_KEY_F12             SDLK_F12          /* F12 */
#define S_KEY_F13             SDLK_F13          /* F13 */
#define S_KEY_F14             SDLK_F14          /* F14 */
#define S_KEY_F15             SDLK_F15          /* F15 */
#define S_KEY_F16             SDLK_F16          /* F16 */
#define S_KEY_F17             SDLK_F17          /* F17 */
#define S_KEY_F18             SDLK_F18          /* F18 */
#define S_KEY_F19             SDLK_F19          /* F19 */
#define S_KEY_F20             SDLK_F20          /* F20 */
#define S_KEY_F21             SDLK_F21          /* F21 */
#define S_KEY_F22             SDLK_F22          /* F22 */
#define S_KEY_F23             SDLK_F23          /* F23 */
#define S_KEY_F24             SDLK_F24          /* F24 */

#define S_KEY_LCTRL           SDLK_LCTRL        /* left control */
#define S_KEY_LSHIFT          SDLK_LSHIFT       /* left shift */
#define S_KEY_LALT            SDLK_LALT         /* left alt */
#define S_KEY_LGUI            SDLK_LGUI         /* left gui */
#define S_KEY_RCTRL           SDLK_RCTRL        /* right control */
#define S_KEY_RSHIFT          SDLK_RSHIFT       /* right shift */
#define S_KEY_RALT            SDLK_RALT         /* right alt */
#define S_KEY_RGUI            SDLK_RGUI         /* right gui */

#define S_KEY_NUMLOCK         SDLK_NUMLOCKCLEAR /* numlock */
#define S_KEY_KP_DIVIDE       SDLK_KP_DIVIDE    /* keypad / */
#define S_KEY_KP_MULTIPLY     SDLK_KP_MULTIPLY  /* keypad * */
#define S_KEY_KP_MINUS        SDLK_KP_MINUS     /* keypad - */
#define S_KEY_KP_PLUS         SDLK_KP_PLUS      /* keypad + */
#define S_KEY_KP_RETURN       SDLK_KP_ENTER     /* keypad return */
#define S_KEY_KP_ENTER        S_KEY_KP_RETURN   /* keypad return alt */
#define S_KEY_KP_0            SDLK_KP_0         /* keypad 0 */
#define S_KEY_KP_1            SDLK_KP_1         /* keypad 1 */
#define S_KEY_KP_2            SDLK_KP_2         /* keypad 2 */
#define S_KEY_KP_3            SDLK_KP_3         /* keypad 3 */
#define S_KEY_KP_4            SDLK_KP_4         /* keypad 4 */
#define S_KEY_KP_5            SDLK_KP_5         /* keypad 5 */
#define S_KEY_KP_6            SDLK_KP_6         /* keypad 6 */
#define S_KEY_KP_7            SDLK_KP_7         /* keypad 7 */
#define S_KEY_KP_8            SDLK_KP_8         /* keypad 8 */
#define S_KEY_KP_9            SDLK_KP_9         /* keypad 9 */
#define S_KEY_KP_PERIOD       SDLK_KP_PERIOD    /* keypad . */

#define S_KEY_INSERT          SDLK_INSERT       /* insert */
#define S_KEY_DELETE          SDLK_DELETE       /* delete */
#define S_KEY_HOME            SDLK_HOME         /* home */
#define S_KEY_END             SDLK_END          /* end */
#define S_KEY_PAGEUP          SDLK_PAGEUP       /* page up */
#define S_KEY_PAGEDOWN        SDLK_PAGEDOWN     /* page down */
#define S_KEY_PRINTSCREEN     SDLK_PRINTSCREEN  /* print screen */
#define S_KEY_SCROLLLOCK      SDLK_SCROLLLOCK   /* scroll lock */
#define S_KEY_PAUSE           SDLK_PAUSE        /* pause */

#define S_KEY_UP              SDLK_UP           /* up */
#define S_KEY_DOWN            SDLK_DOWN         /* down */
#define S_KEY_LEFT            SDLK_LEFT         /* left */
#define S_KEY_RIGHT           SDLK_RIGHT        /* right */

#define S_KEY_ANY             0                 /* any key */

/**
 * @brief Keycode type.
 *
 * A simple typedef for SDL keycodes. This is the type that is passed to the
 * functions {@link S_input_is_key_down(Skey)} and
 * {@link S_input_is_key_pressed(Skey)}.
 *
 * @since 1.0.0
 */
typedef SDL_Keycode Skey;

/**
 * @brief Check if a given key is currently being pressed down.
 *
 * Each time {@link S_window_poll(Swindow *)} is called, input is polled for.
 * This function is typically called once per frame. During the period of one
 * frame, if a key is detected as being depressed, then this function will
 * return {@link S_TRUE} for that given key. When the polling function is called
 * a second time, typically on the next frame, this function will return
 * {@link S_FALSE} if the key is released or kept down. The only way for it to
 * return {@link S_TRUE} again is to release the key and depress it again.
 *
 * @return {@link S_TRUE} if the given key is being pressed down on the current
 * poll period. If {@link S_KEY_ANY} is specified and the condition holds true
 * for <i>any</i> key, then {@link S_TRUE} will also be returned. Otherwise,
 * {@link S_FALSE}.
 * @since 1.0.0
 */
Sbool  S_input_is_key_down(Skey);

/**
 * @brief Check if a given key is currently being held down.
 *
 * @return {@link S_TRUE} if the given key is being held down.
 * If {@link S_KEY_ANY} is specified and <i>any</i> key is down, then
 * {@link S_TRUE} will also be returned. Otherwise, {@link S_FALSE}.
 * @since 1.0.0
 */
Sbool  S_input_is_key_pressed(Skey);

void  _S_input_keyboard_init(void);
void  _S_input_keyboard_reset(void);
void  _S_input_keyboard_event(SDL_Event);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_KEYBOARD_H */

