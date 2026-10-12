st.gui.image {#lua_api_gui_image}
============

Class that provides a texture drawn inside a frame.

Overview
--------

An image is placed inside a frame with @ref lua_api_gui_component_set_parent. It draws a texture that was loaded during
the init phase and is looked up with `st.video.texture.get(...)`.

An image without a texture draws nothing.

An image derives from `st.gui.component`, so it also has the `close`, `is_closed` and `set_parent` methods.

Methods
-------

### st.gui.image() {#lua_api_gui_image_ctor}

This method constructs a new image with no texture.

**Returns:** `st.gui.image` - a new instance of the `st.gui.image` class

```lua
local img = st.gui.image()
img:set_texture(st.video.texture.get("foobar"))
img:set_parent(win)
```

### st.gui.image:set_texture(texture) {#lua_api_gui_image_set_texture}

This method sets the texture that is drawn.

| Parameter | Type               | Description                                  |
|-----------|--------------------|----------------------------------------------|
| `texture` | `st.video.texture` | A texture loaded during the init phase.      |

**Returns:** nothing

### st.gui.image:set_size(width, height) {#lua_api_gui_image_set_size}

This method sets the size, in pixels, that the texture is drawn at.

A dimension of `0` uses the natural size of the texture for that axis, which is also the default.

| Parameter | Type   | Description                 |
|-----------|--------|-----------------------------|
| `width`   | number | The width of the image.     |
| `height`  | number | The height of the image.    |

**Returns:** nothing

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_frame
- @ref lua_api_video_texture
