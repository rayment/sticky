st.gui.popup {#lua_api_gui_popup}
============

Class that provides a small message window with a single button.

Overview
--------

A popup is a ready-made window that shows a message and a button. It resizes itself to fit its text and closes when the
user presses its button or the titlebar close button.

A popup is a standalone component and is displayed with @ref lua_api_gui_component_show. It can never be given a
parent, and it cannot contain other components. Calling `set_parent` on it logs a warning and is ignored.

A popup derives from `st.gui.component`, so it also has the `show`, `close` and `is_closed` methods. See
@ref lua_gui_lifecycle for how long a popup lives.

Methods
-------

### st.gui.popup() {#lua_api_gui_popup_ctor}

This method constructs a new, hidden popup with no text and the button text `OK`.

**Returns:** `st.gui.popup` - a new instance of the `st.gui.popup` class

```lua
local popup = st.gui.popup()
popup:set_title("Saved")
popup:set_text("Your game was saved.")
popup:show()
```

### st.gui.popup:set_title(title) {#lua_api_gui_popup_set_title}

This method sets the text shown in the titlebar.

| Parameter | Type   | Description               |
|-----------|--------|---------------------------|
| `title`   | string | The title of the popup.   |

**Returns:** nothing

### st.gui.popup:set_text(text) {#lua_api_gui_popup_set_text}

This method sets the message displayed in the popup.

| Parameter | Type   | Description               |
|-----------|--------|---------------------------|
| `text`    | string | The message to display.   |

**Returns:** nothing

### st.gui.popup:set_button_text(text) {#lua_api_gui_popup_set_button_text}

This method sets the text shown on the button. The default is `OK`.

| Parameter | Type   | Description                      |
|-----------|--------|----------------------------------|
| `text`    | string | The text shown on the button.    |

**Returns:** nothing

### st.gui.popup:register_on_close(callback) {#lua_api_gui_popup_register_on_close}

This method registers a function that is called when the popup is closed. Several callbacks may be registered and they
are called in the order they were added.

The callback is called once, takes no arguments, and is released afterwards.

| Parameter  | Type     | Description                                     |
|------------|----------|-------------------------------------------------|
| `callback` | function | The function to call when the popup is closed.  |

**Returns:** nothing

### st.gui.popup:clear_on_close() {#lua_api_gui_popup_clear_on_close}

This method removes every callback that was registered with @ref lua_api_gui_popup_register_on_close.

**Returns:** nothing

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
