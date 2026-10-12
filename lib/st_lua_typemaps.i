%{
#include <initializer_list>
#include <memory>

#include "io/log.h"
#include "st_types.h"

// Owns a Lua registry reference for the lifetime of the callback.
struct st_swig_lua_callback_ref
{
    lua_State *L;
    int ref;

    st_swig_lua_callback_ref(lua_State *state, int reference) noexcept
        : L(state), ref(reference)
    {}

    ~st_swig_lua_callback_ref() noexcept
    {
        luaL_unref(L, LUA_REGISTRYINDEX, ref);
    }

    st_swig_lua_callback_ref(const st_swig_lua_callback_ref &) = delete;
    st_swig_lua_callback_ref &operator=(const st_swig_lua_callback_ref &) = delete;
};

// A coroutine can be collected while the engine still holds its callbacks, so references are made on the main
// thread, which lives as long as the state.
inline lua_State *st_swig_lua_main_thread(lua_State *L)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
    lua_State *main_thread = lua_tothread(L, -1);
    lua_pop(L, 1);
    return main_thread;
}

// Holds the Lua value at `index` in the registry until the returned token is released.
inline std::shared_ptr<void> st_swig_lua_pin(lua_State *L, int index)
{
    lua_pushvalue(L, index);
    const int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    return std::make_shared<st_swig_lua_callback_ref>(st_swig_lua_main_thread(L), ref);
}

inline int st_swig_lua_traceback(lua_State *L)
{
    const char *message = lua_tostring(L, 1);
    if (message == nullptr)
        message = "(error object is not a string)";
    luaL_traceback(L, L, message, 1);
    return 1;
}

// Errors are logged with a traceback and never propagate into the engine.
template <typename push_arguments>
inline void st_swig_lua_call(const st_swig_lua_callback_ref &callback,
                             int                             argument_count,
                             push_arguments                  push)
{
    lua_State *L = callback.L;
    const int base = lua_gettop(L);

    lua_pushcfunction(L, st_swig_lua_traceback);
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback.ref);
    push(L);
    if (lua_pcall(L, argument_count, 0, base + 1) != LUA_OK)
    {
        const char *error = lua_tostring(L, -1);
        st::io::log::error(error != nullptr ? error : "Unknown Lua callback error.");
    }
    lua_settop(L, base);
}

inline void st_swig_lua_invoke(const st_swig_lua_callback_ref &callback,
                               std::initializer_list<lua_Integer> arguments = {})
{
    st_swig_lua_call(callback, static_cast<int>(arguments.size()),
                     [&arguments](lua_State *L)
                     {
                         for (const lua_Integer argument : arguments)
                             lua_pushinteger(L, argument);
                     });
}

inline void st_swig_lua_invoke_bool(const st_swig_lua_callback_ref &callback, bool value)
{
    st_swig_lua_call(callback, 1, [value](lua_State *L) { lua_pushboolean(L, value ? 1 : 0); });
}
%}

#define ST_SWIG_IMPLICIT_CONVERSION_TO_STRING(type, temp_type, conversion) \
    %typemap(typecheck, precedence=SWIG_TYPECHECK_STRING) type { \
        $1 = lua_isstring(L, $input); \
    } \
    %typemap(in) type (temp_type temp) { \
        if (lua_type(L, $input) != LUA_TSTRING) { \
            SWIG_exception(SWIG_TypeError, "expected a string"); \
        } \
        temp = conversion; \
        $1 = &temp; \
    }

#define ST_SWIG_IMPLICIT_CONVERSION_TO_FUNCTION(type) \
    %typemap(typecheck) type & { \
        $1 = lua_isfunction(L, $input); \
    } \
    %typemap(in) type & (type temp) { \
        lua_pushvalue(L, $input); \
        const int ref = luaL_ref(L, LUA_REGISTRYINDEX); \
        auto fn = std::make_shared<st_swig_lua_callback_ref>(st_swig_lua_main_thread(L), ref); \
        temp = [fn]() { st_swig_lua_invoke(*fn); }; \
        $1 = &temp; \
    }

// Callbacks that receive an enumerator are called with its integer value.
#define ST_SWIG_IMPLICIT_CONVERSION_TO_ENUM_FUNCTION(type, enum_type) \
    %typemap(typecheck) type & { \
        $1 = lua_isfunction(L, $input); \
    } \
    %typemap(in) type & (type temp) { \
        lua_pushvalue(L, $input); \
        const int ref = luaL_ref(L, LUA_REGISTRYINDEX); \
        auto fn = std::make_shared<st_swig_lua_callback_ref>(st_swig_lua_main_thread(L), ref); \
        temp = [fn](enum_type value) { st_swig_lua_invoke(*fn, {static_cast<lua_Integer>(value)}); }; \
        $1 = &temp; \
    }

// Callbacks that receive a boolean are called with a Lua boolean.
#define ST_SWIG_IMPLICIT_CONVERSION_TO_BOOL_FUNCTION(type) \
    %typemap(typecheck) type & { \
        $1 = lua_isfunction(L, $input); \
    } \
    %typemap(in) type & (type temp) { \
        lua_pushvalue(L, $input); \
        const int ref = luaL_ref(L, LUA_REGISTRYINDEX); \
        auto fn = std::make_shared<st_swig_lua_callback_ref>(st_swig_lua_main_thread(L), ref); \
        temp = [fn](st::bool_t value) { st_swig_lua_invoke_bool(*fn, value); }; \
        $1 = &temp; \
    }

// convert all std::vector<std::string> to Lua tables
%typemap(out) std::vector<std::string>
{
    lua_createtable(L, static_cast<int>($1.size()), 0);

    for (std::size_t i = 0; i < $1.size(); ++i)
    {
        const std::string &value = $1[i];

        lua_pushlstring(L, value.data(), value.size());
        lua_rawseti(L, -2, static_cast<lua_Integer>(i + 1));
    }

    SWIG_arg++;
}

// allow implicit conversion of Lua strings to std::filesystem::path
ST_SWIG_IMPLICIT_CONVERSION_TO_STRING(
    const std::filesystem::path &,
    std::filesystem::path,
    std::filesystem::path(lua_tostring(L, $input))
);

// Lets a bound function receive the Lua state of its caller without taking an argument.
%typemap(in, numinputs=0) lua_State *state {
    $1 = L;
}
