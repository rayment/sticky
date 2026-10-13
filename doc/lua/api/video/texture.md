st.video.texture {#lua_api_video_texture}
===========

Class that manages video textures.

Overview
--------

This class provides a definition and access to textures which may be rendered on the screen.

Textures are loaded during the init phase via the `st.init.textures` table.
Those that load successfully can be accessed at runtime by calling `st.video.texture.get(...)`.

Static Functions
----------------

### st.video.texture.get(name) {#lua_api_video_texture_get}

This function will return a pointer to a loaded texture identified by the `name` parameter, or else return `nil` if the
texture is not found.

The `name` parameter must match the `name` key in the `st.init.textures` table for lookup to succeed.

| Parameter | Type   | Description                                                |
|-----------|--------|------------------------------------------------------------|
| `name`    | string | Name of a texture previously loaded during the init phase. |

**Returns:** `st.video.texture` - an instance of a texture, or `nil` if no texture can be found

```lua
local tex = st.video.texture.get("foobar")
```

Methods
-------

### st.video.texture:name() {#lua_api_video_texture_name}

This method returns the name of the texture as defined during the init phase.

**Returns:** `string` - the name of a given texture

### st.video.texture:path() {#lua_api_video_texture_path}

This method returns the path of the texture as defined during the init phase.

Note that the path is relative to the `res/` folder located inside the mod root directory.

**Returns:** `string` - the path of a given texture

### st.video.texture:width() {#lua_api_video_texture_width}

This method returns the width of the texture in pixels.

**Returns:** `number` - the width of a given texture

### st.video.texture:height() {#lua_api_video_texture_height}

This method returns the height of the texture in pixels.

**Returns:** `number` - the height of a given texture

See also
--------

- @ref lua_api_init
