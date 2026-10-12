st.gui.frame {#lua_api_gui_frame}
============

Class that provides a window which contains other components.

Overview
--------

A frame is a window. Add components to it with @ref lua_api_gui_component_set_parent and display it with
@ref lua_api_gui_component_show. The engine draws it every frame until it is closed, either by the script or by the user
with the titlebar close button.

A frame is a root component: it can contain other components but can never be given a parent itself. It derives from
`st.gui.component`, so it also has the `show`, `close` and `is_closed` methods. See @ref lua_gui_lifecycle for how long
a frame lives.

Methods
-------

### st.gui.frame() {#lua_api_gui_frame_ctor}

This method constructs a new, hidden frame. A frame is closable, movable, resizable and collapsable by default, shows a
titlebar, and does not resize itself to fit its contents.

**Returns:** `st.gui.frame` - a new instance of the `st.gui.frame` class

```lua
local win = st.gui.frame()
```

### st.gui.frame:set_title(title) {#lua_api_gui_frame_set_title}

This method sets the text shown in the titlebar.

| Parameter | Type   | Description              |
|-----------|--------|--------------------------|
| `title`   | string | The title of the frame.  |

**Returns:** nothing

### st.gui.frame:set_position(x, y) {#lua_api_gui_frame_set_position}

This method moves the frame to a position, in pixels, relative to the top left corner of the window.

The frame is moved once, on the next frame that it is drawn, and the user may then move it freely unless
@ref lua_api_gui_frame_set_movable was disabled.

| Parameter | Type   | Description                    |
|-----------|--------|--------------------------------|
| `x`       | number | The horizontal position.       |
| `y`       | number | The vertical position.         |

**Returns:** nothing

### st.gui.frame:set_size(width, height) {#lua_api_gui_frame_set_size}

This method resizes the frame, in pixels.

The frame is resized once, on the next frame that it is drawn.

| Parameter | Type   | Description         |
|-----------|--------|---------------------|
| `width`   | number | The frame width.    |
| `height`  | number | The frame height.   |

**Returns:** nothing

### st.gui.frame:set_auto_resize(auto_resize) {#lua_api_gui_frame_set_auto_resize}

This method sets whether the frame always resizes itself to fit its contents. The default is `false`.

| Parameter     | Type    | Description                                      |
|---------------|---------|--------------------------------------------------|
| `auto_resize` | boolean | `true` to fit the frame to its contents.         |

**Returns:** nothing

### st.gui.frame:set_closable(closable) {#lua_api_gui_frame_set_closable}

This method sets whether the titlebar shows a close button. The default is `true`.

A frame that is not closable can still be closed from the script with @ref lua_api_gui_component_close.

| Parameter  | Type    | Description                                   |
|------------|---------|-----------------------------------------------|
| `closable` | boolean | `true` to let the user close the frame.       |

**Returns:** nothing

### st.gui.frame:set_collapsable(collapsable) {#lua_api_gui_frame_set_collapsable}

This method sets whether the user can collapse the frame to its titlebar. The default is `true`.

| Parameter     | Type    | Description                                |
|---------------|---------|--------------------------------------------|
| `collapsable` | boolean | `true` to let the user collapse the frame. |

**Returns:** nothing

### st.gui.frame:set_movable(movable) {#lua_api_gui_frame_set_movable}

This method sets whether the user can drag the frame. The default is `true`.

| Parameter | Type    | Description                              |
|-----------|---------|------------------------------------------|
| `movable` | boolean | `true` to let the user move the frame.   |

**Returns:** nothing

### st.gui.frame:set_resizable(resizable) {#lua_api_gui_frame_set_resizable}

This method sets whether the user can resize the frame. The default is `true`.

| Parameter   | Type    | Description                                |
|-------------|---------|--------------------------------------------|
| `resizable` | boolean | `true` to let the user resize the frame.   |

**Returns:** nothing

### st.gui.frame:set_show_titlebar(show_titlebar) {#lua_api_gui_frame_set_show_titlebar}

This method sets whether the titlebar is drawn. The default is `true`.

Without a titlebar the user has no close button, so close the frame from the script.

| Parameter       | Type    | Description                            |
|-----------------|---------|----------------------------------------|
| `show_titlebar` | boolean | `true` to draw the titlebar.           |

**Returns:** nothing

### st.gui.frame:register_on_close(callback) {#lua_api_gui_frame_register_on_close}

This method registers a function that is called when the frame is closed. Several callbacks may be registered and they
are called in the order they were added.

The callback is called once, takes no arguments, and is released afterwards. It is the place to forget references to
the frame. A frame that is closed by its parent also calls its callbacks.

| Parameter  | Type     | Description                                    |
|------------|----------|------------------------------------------------|
| `callback` | function | The function to call when the frame is closed. |

**Returns:** nothing

```lua
win:register_on_close(function()
    print("window closed")
end)
```

### st.gui.frame:clear_on_close() {#lua_api_gui_frame_clear_on_close}

This method removes every callback that was registered with @ref lua_api_gui_frame_register_on_close.

**Returns:** nothing

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_button
- @ref lua_api_gui_label
- @ref lua_api_gui_progress
- @ref lua_api_gui_rect
- @ref lua_api_gui_hlayout
- @ref lua_api_gui_vlayout
