st.util.config {#lua_api_util_config}
==============

Module that reads and writes persistent settings.

Overview
--------

Settings are stored in `config.json` next to the executable and loaded on every start. Every setting belongs to a single
owner, which is the name that prefixes its key (for example the `chess` in `chess.auto_flip`). Settings declared with
@ref lua_api_init_config belong to the owner that declared them.

A script can read every declared setting, whoever owns it, and can only write the settings that it owns. This lets a
choice made in a GUI be remembered the next time the application starts.

The same store is used by the engine itself, so a setting that was changed from Lua is seen by native code and the
other way around.

NOTE: This module is only usable during the runtime phase. Calls made during the init phase log an error and fail. All
declared settings exist before the first `runtime.lua` is executed, so reading from its top level is safe.

NOTE: Settings are stored in a plain text file and can be read by every script. Do not store secrets in them.

Static Functions
----------------

### st.util.config.get(name) {#lua_api_util_config_get}

Returns the current value of one of the caller's own settings, or `nil` if it has no setting called `name`.

`name` is the name as written in `st.init.config`, without the owner prefix. The returned type is the declared `type`:
a string, a number or a boolean.

```lua
local flip = st.util.config.get("auto_flip")
```

### st.util.config.get_from(owner, name) {#lua_api_util_config_get_from}

Returns the current value of a setting that belongs to another owner, or `nil` if the owner or the setting does not
exist.

```lua
local fullscreen = st.util.config.get_from("core", "fullscreen")
```

### st.util.config.set(name, value) {#lua_api_util_config_set}

Changes one of the caller's own settings and returns `true` on success.

The call logs an error and returns `false`, leaving the setting unchanged, when:

- the caller has no setting called `name` (settings cannot be created at runtime, and settings that belong to another
  owner cannot be changed);
- the type of `value` differs from the declared `type` (nothing is converted);
- `value` is not a string, number or boolean;
- `value` is `nan` or infinite.

```lua
function board:set_show_coordinates(enabled)
    self.show_coordinates = enabled
    st.util.config.set("show_coordinates", enabled)
end
```

The value is written to `config.json` when the application quits normally. A crash or a forced kill loses the changes
made during that session. If `config.json` could not be read at start-up, it is not overwritten.

See also
--------

- @ref lua_api_init_config
