st.gui.rect {#lua_api_gui_rect}
===========

Class that provides a solid or textured rectangle drawn inside a frame.

Overview
--------

A rect is placed inside a frame with @ref lua_api_gui_component_set_parent. It draws either a solid colour, a texture, or
a texture tinted by a colour. Textures are loaded during the init phase and are looked up with
`st.video.texture.get(...)`.

A rect has an opaque white colour by default, so a textured rect shows the texture unchanged. A rect with no texture is
filled with its colour and needs a size set with @ref lua_api_gui_rect_set_size, otherwise it draws nothing.

A rect derives from `st.gui.component`, so it also has the `close`, `is_closed` and `set_parent` methods.

Methods
-------

### st.gui.rect() {#lua_api_gui_rect_ctor}

This method constructs a new rect with no texture and an opaque white colour.

**Returns:** `st.gui.rect` - a new instance of the `st.gui.rect` class

```lua
local solid = st.gui.rect()
solid:set_colour(1, 0, 0, 1)
solid:set_size(64, 16)
solid:set_parent(win)

local img = st.gui.rect()
img:set_texture(st.video.texture.get("foobar"))
img:set_parent(win)
```

### st.gui.rect:set_texture(texture) {#lua_api_gui_rect_set_texture}

This method sets the texture that is drawn.

| Parameter | Type               | Description                                  |
|-----------|--------------------|----------------------------------------------|
| `texture` | `st.video.texture` | A texture loaded during the init phase.      |

**Returns:** nothing

### st.gui.rect:set_colour(r, g, b, a) {#lua_api_gui_rect_set_colour}

This method sets the colour of the rect. If the rect has a texture, the colour tints the texture.

| Parameter | Type   | Description                                |
|-----------|--------|--------------------------------------------|
| `r`       | number | The red component, from `0` to `1`.        |
| `g`       | number | The green component, from `0` to `1`.     |
| `b`       | number | The blue component, from `0` to `1`.      |
| `a`       | number | The alpha component, from `0` to `1`.     |

**Returns:** nothing

### st.gui.rect:set_background(r, g, b, a) {#lua_api_gui_rect_set_background}

This method sets the colour drawn behind the texture, which shows through the transparent parts of the texture. The
default is fully transparent. It is not tinted by @ref lua_api_gui_rect_set_colour.

| Parameter | Type   | Description                                |
|-----------|--------|--------------------------------------------|
| `r`       | number | The red component, from `0` to `1`.        |
| `g`       | number | The green component, from `0` to `1`.      |
| `b`       | number | The blue component, from `0` to `1`.       |
| `a`       | number | The alpha component, from `0` to `1`.      |

**Returns:** nothing

### st.gui.rect:set_size(width, height) {#lua_api_gui_rect_set_size}

This method sets the size, in pixels, that the rect is drawn at.

With a texture, a dimension of `0` uses the natural size of the texture for that axis, which is also the default.
Without a texture, both dimensions must be greater than `0` for the rect to be drawn.

| Parameter | Type   | Description                 |
|-----------|--------|-----------------------------|
| `width`   | number | The width of the rect.      |
| `height`  | number | The height of the rect.     |

**Returns:** nothing

### st.gui.rect:register_on_click(callback) {#lua_api_gui_rect_register_on_click}

This method registers a function that is called every time the rect is clicked. Several callbacks may be registered and
they are called in the order they were added. A rect that is not drawn cannot be clicked.

The callback is called with the mouse button that was used, which is one of the constants of @ref lua_api_io_mouse.

| Parameter  | Type     | Description                                                          |
|------------|----------|----------------------------------------------------------------------|
| `callback` | function | The function to call when the rect is clicked: `function(button)`.   |

**Returns:** nothing

```lua
img:register_on_click(function(button)
    if button == st.io.mouse.Left then
        st.io.log.info("left clicked")
    end
end)
```

### st.gui.rect:clear_on_click() {#lua_api_gui_rect_clear_on_click}

This method removes every callback that was registered with @ref lua_api_gui_rect_register_on_click.

**Returns:** nothing

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_frame
- @ref lua_api_video_texture
- @ref lua_api_io_mouse
