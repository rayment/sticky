st.gui.menubar {#lua_api_gui_menubar}
==============

Classes that provide a menu bar across the top of the window, with menus, buttons, checkboxes and separators.

Overview
--------

A menu bar is built from five classes.

| Class                    | Purpose                                                                     |
|--------------------------|-----------------------------------------------------------------------------|
| `st.gui.menubar`         | The bar itself. Holds menus and entries.                                    |
| `st.gui.menu`            | A named menu that opens when hovered. Can be nested to make sub-menus.      |
| `st.gui.menu_button`     | An entry that calls its callbacks when clicked.                             |
| `st.gui.menu_checkbox`   | An entry with a check mark that the user toggles.                           |
| `st.gui.menu_separator`  | A line that groups the entries of a menu.                                   |

A menu bar belongs to a single frame: give it to a @ref lua_api_gui_frame with
@ref lua_api_gui_component_set_parent and it is drawn along the top of that frame, below the titlebar. The frame makes
room for the bar by itself. A menu bar must be a direct child of the frame, not of a layout, and a frame draws one menu
bar. There is no window-wide menu bar.

Menus, buttons, checkboxes and separators are added to the menu bar or to a menu with
@ref lua_api_gui_component_set_parent. A menu inside a menu is a sub-menu that opens when it is hovered.

All of them derive from `st.gui.component`, so they also have the `close`, `is_closed` and `set_parent` methods.
Closing a frame closes its menu bar and everything in it, and closing a button or checkbox drops its callbacks. See
@ref lua_gui_lifecycle for details.

```lua
local bar = st.gui.menubar()

local file = st.gui.menu()
file:set_text("File")
file:set_parent(bar)

local new = st.gui.menu_button()
new:set_text("New")
new:set_parent(file)
new:register_on_click(function() print("new") end)

local recent = st.gui.menu()
recent:set_text("Recent")
recent:set_parent(file)

local sep = st.gui.menu_separator()
sep:set_parent(file)

local grid = st.gui.menu_checkbox()
grid:set_text("Show grid")
grid:set_checked(true)
grid:set_parent(file)
grid:register_on_toggle(function(checked) print("grid", checked) end)

local win = st.gui.frame()
win:set_title("Editor")
bar:set_parent(win)
win:show()
```

Methods
-------

### st.gui.menubar() {#lua_api_gui_menubar_ctor}

This method constructs a new, empty menu bar. It is drawn once it is given to a frame.

**Returns:** `st.gui.menubar` - a new instance of the `st.gui.menubar` class

### st.gui.menu() {#lua_api_gui_menu_ctor}

This method constructs a new, enabled menu with no text.

**Returns:** `st.gui.menu` - a new instance of the `st.gui.menu` class

### st.gui.menu:set_text(text) {#lua_api_gui_menu_set_text}

This method sets the text shown for the menu.

| Parameter | Type   | Description               |
|-----------|--------|---------------------------|
| `text`    | string | The text shown on the menu. |

**Returns:** nothing

### st.gui.menu:set_enabled(enabled) {#lua_api_gui_menu_set_enabled}

This method sets whether the menu can be opened. A disabled menu is greyed out. The default is `true`.

| Parameter | Type    | Description                                |
|-----------|---------|--------------------------------------------|
| `enabled` | boolean | `true` to enable the menu, `false` to disable it. |

**Returns:** nothing

### st.gui.menu_button() {#lua_api_gui_menu_button_ctor}

This method constructs a new, enabled menu button with no text.

**Returns:** `st.gui.menu_button` - a new instance of the `st.gui.menu_button` class

### st.gui.menu_button:set_text(text) {#lua_api_gui_menu_button_set_text}

This method sets the text shown on the button.

| Parameter | Type   | Description                  |
|-----------|--------|------------------------------|
| `text`    | string | The text shown on the button. |

**Returns:** nothing

### st.gui.menu_button:set_enabled(enabled) {#lua_api_gui_menu_button_set_enabled}

This method sets whether the button reacts to clicks. A disabled button is greyed out and does not call its callbacks.
The default is `true`.

| Parameter | Type    | Description                                         |
|-----------|---------|-----------------------------------------------------|
| `enabled` | boolean | `true` to enable the button, `false` to disable it. |

**Returns:** nothing

### st.gui.menu_button:register_on_click(callback) {#lua_api_gui_menu_button_register_on_click}

This method registers a function that is called every time the button is clicked. Several callbacks may be registered
and they are called in the order they were added.

The callback takes no arguments.

| Parameter  | Type     | Description                                      |
|------------|----------|--------------------------------------------------|
| `callback` | function | The function to call when the button is clicked. |

**Returns:** nothing

### st.gui.menu_button:clear_on_click() {#lua_api_gui_menu_button_clear_on_click}

This method removes every callback that was registered with @ref lua_api_gui_menu_button_register_on_click.

**Returns:** nothing

### st.gui.menu_checkbox() {#lua_api_gui_menu_checkbox_ctor}

This method constructs a new, enabled, unchecked menu checkbox with no text.

**Returns:** `st.gui.menu_checkbox` - a new instance of the `st.gui.menu_checkbox` class

### st.gui.menu_checkbox:set_text(text) {#lua_api_gui_menu_checkbox_set_text}

This method sets the text shown on the checkbox.

| Parameter | Type   | Description                     |
|-----------|--------|---------------------------------|
| `text`    | string | The text shown on the checkbox. |

**Returns:** nothing

### st.gui.menu_checkbox:set_enabled(enabled) {#lua_api_gui_menu_checkbox_set_enabled}

This method sets whether the user can toggle the checkbox. The default is `true`.

| Parameter | Type    | Description                                           |
|-----------|---------|-------------------------------------------------------|
| `enabled` | boolean | `true` to enable the checkbox, `false` to disable it. |

**Returns:** nothing

### st.gui.menu_checkbox:set_checked(checked) {#lua_api_gui_menu_checkbox_set_checked}

This method sets whether the checkbox is checked. The default is `false`.

It does not call the callbacks registered with @ref lua_api_gui_menu_checkbox_register_on_toggle, which are only called
when the user toggles the checkbox.

| Parameter | Type    | Description                           |
|-----------|---------|---------------------------------------|
| `checked` | boolean | `true` to check the box.              |

**Returns:** nothing

### st.gui.menu_checkbox:is_checked() {#lua_api_gui_menu_checkbox_is_checked}

This method gets whether the checkbox is checked.

**Returns:** `boolean` - `true` if the checkbox is checked

### st.gui.menu_checkbox:register_on_toggle(callback) {#lua_api_gui_menu_checkbox_register_on_toggle}

This method registers a function that is called every time the user toggles the checkbox. Several callbacks may be
registered and they are called in the order they were added.

The callback receives the new state as a boolean.

| Parameter  | Type     | Description                                         |
|------------|----------|-----------------------------------------------------|
| `callback` | function | The function to call, as `callback(checked)`.       |

**Returns:** nothing

```lua
grid:register_on_toggle(function(checked)
    board:set_grid_visible(checked)
end)
```

### st.gui.menu_checkbox:clear_on_toggle() {#lua_api_gui_menu_checkbox_clear_on_toggle}

This method removes every callback that was registered with @ref lua_api_gui_menu_checkbox_register_on_toggle.

**Returns:** nothing

### st.gui.menu_separator() {#lua_api_gui_menu_separator_ctor}

This method constructs a new separator. It has no other methods.

**Returns:** `st.gui.menu_separator` - a new instance of the `st.gui.menu_separator` class

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_button
- @ref lua_api_gui_frame
