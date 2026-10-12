st.gui.progress {#lua_api_gui_progress}
===============

Class that provides a progress bar.

Overview
--------

A progress bar is placed inside a frame with @ref lua_api_gui_component_set_parent. It displays a fraction between `0`
and `1` across the width of the frame.

A progress bar derives from `st.gui.component`, so it also has the `close`, `is_closed` and `set_parent` methods.

Methods
-------

### st.gui.progress() {#lua_api_gui_progress_ctor}

This method constructs a new progress bar with a value of `0`.

**Returns:** `st.gui.progress` - a new instance of the `st.gui.progress` class

```lua
local bar = st.gui.progress()
bar:set_value(0.25)
bar:set_parent(win)
```

### st.gui.progress:set_value(value) {#lua_api_gui_progress_set_value}

This method sets the fraction that is complete.

Values outside the range `0` to `1` are clamped to it.

| Parameter | Type   | Description                                     |
|-----------|--------|-------------------------------------------------|
| `value`   | number | The completed fraction, from `0` to `1`.        |

**Returns:** nothing

### st.gui.progress:value() {#lua_api_gui_progress_value}

This method returns the current fraction that is complete.

**Returns:** `number` - the value of the progress bar, from `0` to `1`

### st.gui.progress:set_text(text) {#lua_api_gui_progress_set_text}

This method sets the text drawn over the bar. An empty string, which is the default, shows the percentage complete.

| Parameter | Type   | Description                     |
|-----------|--------|---------------------------------|
| `text`    | string | The text drawn over the bar.    |

**Returns:** nothing

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_frame
