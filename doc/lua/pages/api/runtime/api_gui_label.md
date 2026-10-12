st.gui.label {#lua_api_gui_label}
============

Class that provides a line of text.

Overview
--------

A label is placed inside a frame with @ref lua_api_gui_component_set_parent. Change the text at any time and the new text
is drawn on the next frame.

A label derives from `st.gui.component`, so it also has the `close`, `is_closed` and `set_parent` methods.

Methods
-------

### st.gui.label() {#lua_api_gui_label_ctor}

This method constructs a new label with no text.

**Returns:** `st.gui.label` - a new instance of the `st.gui.label` class

```lua
local text = st.gui.label()
text:set_text("Hello, world")
text:set_parent(win)
```

### st.gui.label:set_text(text) {#lua_api_gui_label_set_text}

This method sets the text shown by the label.

| Parameter | Type   | Description               |
|-----------|--------|---------------------------|
| `text`    | string | The text to display.      |

**Returns:** nothing

### st.gui.label:set_size(width, height) {#lua_api_gui_label_set_size}

This method sets a box, in pixels, that the text is centred in. This is useful for lining text up with other
components, such as the labels of a grid. A label with no text only takes up the box.

If either dimension is `0` the label fits its text, which is the default.

| Parameter | Type   | Description                 |
|-----------|--------|-----------------------------|
| `width`   | number | The width of the box.       |
| `height`  | number | The height of the box.      |

**Returns:** nothing

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_frame
