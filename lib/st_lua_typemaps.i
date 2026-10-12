%{
#include <cstdio>
#include <memory>

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
        auto fn = std::make_shared<st_swig_lua_callback_ref>(L, ref); \
        temp = [fn]() { \
            lua_rawgeti(fn->L, LUA_REGISTRYINDEX, fn->ref); \
            if (lua_pcall(fn->L, 0, 0, 0) != LUA_OK) { \
                const char *error = lua_tostring(fn->L, -1); \
                fprintf(stderr, "Lua callback error: %s\n", \
                    error ? error : "unknown error"); \
                lua_pop(fn->L, 1); \
            } \
        }; \
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
