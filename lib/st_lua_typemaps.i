%{
#include "st_types.h"
%}

%template(StringVector) std::vector<std::string>;

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
