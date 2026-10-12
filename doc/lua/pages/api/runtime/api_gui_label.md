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

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_frame
