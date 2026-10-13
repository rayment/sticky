st.init {#lua_api_init}
===========

Module that defines global configuration to load on mod init.

Overview
--------

This module contains a set of Lua tables that can be modified by the executing Lua mod environment to define
instructions on how the mod should be initialised.

NOTE: This module is only accessible during the init phase. Attempts to access the module during the runtime phase will
result in an error.

Tables
------

### st.init.config {#lua_api_init_config}

This table defines a list of configurable settings that are specific to the mod.

Settings are stored in a JSON file in the root directory of the game executable, and are loaded every time the
application is started. If any settings are removed from the file or are unavailable, they are automatically replaced
with a default value set in this table.

| Key           | Type   | Description                                                                                                                              |
|---------------|--------|------------------------------------------------------------------------------------------------------------------------------------------|
| `name`        | string | Name of the setting. This will be prefixed by the name of the mod, followed by a fullstop '.' unless the mod name already ends with one. |
| `type`        | string | Data type that the setting expects. Valid values are `"string"`, `"number"` or `"boolean"`.                                              |
| `description` | string | Human-readable description of what the setting does.                                                                                     |
| `default`     | any    | Default value of the setting. The type must match that defined by the `type` key.                                                        |

```lua
st.init.config = {
    {
        name        = "fullscreen",
        description = "Set to true to start the application in fullscreen rather than windowed.",
        type        = "boolean",
        default     = false
    },
    {
        name        = "name",
        description = "Name of the person playing the game."
        type        = "string",
        default     = "John Smith"
    },
    {
        name        = "random_seed",
        description = "Seed for the random number generator.",
        type        = "number",
        default     = 12345
    },
}
```

### st.init.sounds {#lua_api_init_sounds}

This table defines a list of sounds that should be loaded and made available to the mod.

Sound paths must be defined as relative paths only, and are relative to the `res/` folder located inside the mod root
directory only.

| Key    | Type   | Description                                                             |
|--------|--------|-------------------------------------------------------------------------|
| `name` | string | Name to identify the sound file as during runtime. This must be unique. |
| `path` | string | Relative path to the sound file, including file extension.              |

```lua
st.init.sounds = {
    { name = "test", path = "test.wav" },
    { name = "foobar", path = "foobar.mod" }
}
```

#### Supported filetypes:
- WAV (8-bit and 16-bit, mono or stereo)
- Tracker modules (MOD, S3M, XM, IT)

Mono files are kept mono and everything else is mixed down to stereo. Use mono files for sounds that are positioned in
the world, as they pan and fade most clearly with distance.

### st.init.textures {#lua_api_init_textures}

This table defines a list of textures that should be loaded and made available to the mod.

Texture paths must be defined as relative paths only, and are relative to the `res/` folder located inside the mod root
directory only.

| Key    | Type   | Description                                                               |
|--------|--------|---------------------------------------------------------------------------|
| `name` | string | Name to identify the texture file as during runtime. This must be unique. |
| `path` | string | Relative path to the texture file, including file extension.              |

```lua
st.init.textures = {
    { name = "test", path = "test.png" },
    { name = "foobar", path = "foobar.png" }
}
```

#### Supported filetypes:
- PNG (8-bit and 16-bit, RGB or RGBA)

See also
--------

- @ref lua_api_util_config
- @ref lua_api_audio_sound
- @ref lua_api_video_texture
