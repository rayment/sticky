st.gui.button {#lua_api_gui_button}
=============

Class that provides a clickable button.

Overview
--------

A button is placed inside a frame with @ref lua_api_gui_component_set_parent. Clicking it calls the functions registered
with @ref lua_api_gui_button_register_on_click.

A button derives from `st.gui.component`, so it also has the `close`, `is_closed` and `set_parent` methods. Closing a
button, or the frame that contains it, drops its click callbacks. See @ref lua_gui_lifecycle for details.

Methods
-------

### st.gui.button() {#lua_api_gui_button_ctor}

This method constructs a new, enabled button with no text.

**Returns:** `st.gui.button` - a new instance of the `st.gui.button` class

```lua
local ok = st.gui.button()
ok:set_text("OK")
ok:set_parent(win)
```

### st.gui.button:set_text(text) {#lua_api_gui_button_set_text}

This method sets the text shown on the button.

| Parameter | Type   | Description                  |
|-----------|--------|------------------------------|
| `text`    | string | The text shown on the button. |

**Returns:** nothing

### st.gui.button:set_enabled(enabled) {#lua_api_gui_button_set_enabled}

This method sets whether the button reacts to clicks. A disabled button is greyed out and does not call its callbacks.
The default is `true`.

| Parameter | Type    | Description                                   |
|-----------|---------|-----------------------------------------------|
| `enabled` | boolean | `true` to enable the button, `false` to disable it. |

**Returns:** nothing

### st.gui.button:register_on_click(callback) {#lua_api_gui_button_register_on_click}

This method registers a function that is called every time the button is clicked. Several callbacks may be registered
and they are called in the order they were added.

The callback takes no arguments.

| Parameter  | Type     | Description                                     |
|------------|----------|-------------------------------------------------|
| `callback` | function | The function to call when the button is clicked. |

**Returns:** nothing

```lua
ok:register_on_click(function()
    win:close()
end)
```

### st.gui.button:clear_on_click() {#lua_api_gui_button_clear_on_click}

This method removes every callback that was registered with @ref lua_api_gui_button_register_on_click.

**Returns:** nothing

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_frame
