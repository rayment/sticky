st.audio.sound {#lua_api_audio_sound}
===========

Class that manages audio sounds.

Overview
--------

This class provides a definition and access to sounds which may be played back during program runtime.

Sounds are loaded during the init phase via the `st.init.sounds` table.
Those that load successfully can be accessed at runtime by calling `st.audio.sound.get(...)`.

Static Functions
----------------

### st.audio.sound.get(name) {#lua_api_audio_sound_get}

This function will return a pointer to a loaded sound identified by the `name` parameter, or else return `nil` if the
sound is not found.

The `name` parameter must match the `name` key in the `st.init.sounds` table for lookup to succeed.

| Parameter | Type   | Description                                                |
|-----------|--------|------------------------------------------------------------|
| `name`    | string | Name of a sound previously loaded during the init phase. |

**Returns:** `st.audio.sound` - an instance of a sound, or `nil` if no sound can be found

```lua
local snd = st.audio.sound.get("foobar")
```

Methods
-------

### st.audio.sound:name() {#lua_api_audio_sound_name}

This method returns the name of the sound as defined during the init phase.

**Returns:** `string` - the name of a given sound

### st.audio.sound:path() {#lua_api_audio_sound_path}

This method returns the path of the sound as defined during the init phase.

Note that the path is relative to the `res/` folder located inside the mod root directory.

**Returns:** `string` - the path of a given sound

See also
--------

- @ref lua_api_init
