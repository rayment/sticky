%{
#include "io/keyboard.h"
%}

%nspace io::keyboard;

%ignore SDL_SCANCODE_COUNT;
%include <SDL3/SDL_scancode.h>

%luacode {
    -- Move all SDL_SCANCODE_* constants into sticky.io.keyboard as KEYCODE_*.
    local m = sticky
    for k, v in pairs(m) do
        local suffix = k:match("^SDL_SCANCODE_(.+)$")
        if suffix then
            m.io.keyboard["KEYCODE_" .. suffix] = v
            m[k] = nil
        end
    end
}

typedef st_int32 st_keycode;

%inline %{
namespace io {

class keyboard
{
public:
    static bool is_key_down(st_keycode code)
    {
        return st_keyboard_is_key_down(code);
    }

    static bool is_key_up(st_keycode code)
    {
        return st_keyboard_is_key_up(code);
    }

private:
    keyboard() {}
    ~keyboard() {}
};

}
%}
