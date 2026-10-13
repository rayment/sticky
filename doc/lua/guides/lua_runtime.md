Built-in Lua runtime {#lua_runtime}
=============

Changes have been made to the built-in Lua runtime.

The following libraries are made available to both the init and runtime phases in their entirety:
- base library functions (`assert`, `error`, `getmetatable`, `ipairs`, `next`, `pairs`, `pcall`, `print`, `rawequal`,
  `rawget`, `rawlen`, `rawset`, `select`, `setmetatable`, `tonumber`, `tostring`, `type`, `warn`, `xpcall`, and the
  `_G` and `_VERSION` values), except for those listed as disabled below
- `coroutine`
- `math`
- `string`
- `table`
- `utf8`

The following libraries and functions are disabled and cannot be accessed:
- `collectgarbage`
- `debug`
- `dofile`
- `io`
- `os`
- `loadfile`
- `load`
- `package` (see exception below regarding `require`)

The following functions have been modified and do not behave as documented in the Lua reference manual:

| Function  | Behaviour                                                                                                                                                                                                                                                                                                                              |
|-----------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `print`   | Overridden to log through `st.io.log.info`, joining its arguments with tabs. Arguments are converted with `tostring`.                                                                                                                                                                                                                   |
| `require` | Modules are looked up as `<name>.lua`, where each `.` in the name is a folder separator, relative to the `lua/` folder within the mod root directory. Files cannot be included above this path, including through symlinks. Only text chunks may be loaded, names may only contain letters, digits, `_` and `.`, and results are cached. |
| `warn`    | Overridden to log through `st.io.log.warn`. The arguments (strings or numbers) are concatenated without separators. Control messages such as `"@on"` and `"@off"` are ignored, as warnings are always enabled. |

Errors are not routed through a function. An error that is not caught with `pcall` or `xpcall`, whether raised by
`error`, `assert` or the engine, aborts the running script or callback and is logged through `st.io.log.error`
together with a stack traceback. The engine itself keeps running and calls later callbacks as normal. Errors that are
caught are not logged.

The `require` function does not use `package.path`, `package.loaded` or `package.preload`, as the `package` library is
not available.

The following globals are added by the engine:

| Global   | Description                                                                          |
|----------|--------------------------------------------------------------------------------------|
| `st`     | The root table of the sticky API documented in the pages below.                       |

Precompiled Lua bytecode can never be loaded, so while `string.dump` is available its output is not usable.

Source files may start with a UTF-8 byte order mark, and a first line starting with `#` (such as `#!/usr/bin/env lua`)
is ignored, as in standard Lua. Error messages and tracebacks name files relative to the `lua/` folder of the mod, for
example `board.lua:12:`.
