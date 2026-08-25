%{
#include "io/keyboard.h"
%}

%nspace io::keyboard;

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
