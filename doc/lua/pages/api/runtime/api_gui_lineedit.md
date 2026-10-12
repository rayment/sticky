st.gui.lineedit {#lua_api_gui_lineedit}
===============

Class that provides a single-line text editor.

Overview
--------

A line editor is placed inside a frame with @ref lua_api_gui_component_set_parent and fills the width of the frame. The
user edits the text in place, and @ref lua_api_gui_lineedit_text returns what is currently typed. For several lines of
text see @ref lua_api_gui_textedit.

A line editor can be in one of three states, set with @ref lua_api_gui_lineedit_set_enabled and
@ref lua_api_gui_lineedit_set_read_only.

| Enabled | Read-only | Behaviour                                                      |
|---------|-----------|----------------------------------------------------------------|
| `true`  | `false`   | The text can be focused, selected and edited. This is the default. |
| `true`  | `true`    | The text can be focused, selected and copied, but not edited.  |
| `false` | any       | The editor is greyed out and cannot be focused.                |

A line editor derives from `st.gui.component`, so it also has the `close`, `is_closed` and `set_parent` methods.

Methods
-------

### st.gui.lineedit() {#lua_api_gui_lineedit_ctor}

This method constructs a new, enabled, editable and unmasked line editor with no text.

**Returns:** `st.gui.lineedit` - a new instance of the `st.gui.lineedit` class

```lua
local password = st.gui.lineedit()
password:set_hint("Password")
password:set_masked(true)
password:set_parent(win)
```

### st.gui.lineedit:text() {#lua_api_gui_lineedit_text}

This method returns the text in the editor, including any changes made by the user.

**Returns:** `string` - the text of the editor

### st.gui.lineedit:set_text(text) {#lua_api_gui_lineedit_set_text}

This method replaces the text in the editor.

| Parameter | Type   | Description                      |
|-----------|--------|----------------------------------|
| `text`    | string | The new text of the editor.      |

**Returns:** nothing

### st.gui.lineedit:set_hint(hint) {#lua_api_gui_lineedit_set_hint}

This method sets the text shown in grey while the editor is empty. An empty string, which is the default, shows nothing.

| Parameter | Type   | Description                          |
|-----------|--------|--------------------------------------|
| `hint`    | string | The text shown while the editor is empty. |

**Returns:** nothing

### st.gui.lineedit:set_enabled(enabled) {#lua_api_gui_lineedit_set_enabled}

This method sets whether the editor reacts to the user. A disabled editor is greyed out and cannot be focused. The
default is `true`.

| Parameter | Type    | Description                                          |
|-----------|---------|------------------------------------------------------|
| `enabled` | boolean | `true` to enable the editor, `false` to disable it.  |

**Returns:** nothing

### st.gui.lineedit:is_enabled() {#lua_api_gui_lineedit_is_enabled}

This method returns whether the editor is enabled.

**Returns:** `boolean` - `true` if the editor is enabled

### st.gui.lineedit:set_read_only(read_only) {#lua_api_gui_lineedit_set_read_only}

This method sets whether the text can be changed by the user. A read-only editor can still be focused and its text
selected and copied. The default is `false`.

| Parameter   | Type    | Description                                            |
|-------------|---------|--------------------------------------------------------|
| `read_only` | boolean | `true` to prevent the user editing the text.           |

**Returns:** nothing

### st.gui.lineedit:is_read_only() {#lua_api_gui_lineedit_is_read_only}

This method returns whether the editor is read-only.

**Returns:** `boolean` - `true` if the editor is read-only

### st.gui.lineedit:set_masked(masked) {#lua_api_gui_lineedit_set_masked}

This method sets whether every character is drawn as `*`, as for a password. A masked editor cannot be copied from. The
text returned by @ref lua_api_gui_lineedit_text is not masked. The default is `false`.

| Parameter | Type    | Description                                       |
|-----------|---------|---------------------------------------------------|
| `masked`  | boolean | `true` to hide the characters of the text.        |

**Returns:** nothing

### st.gui.lineedit:is_masked() {#lua_api_gui_lineedit_is_masked}

This method returns whether the editor is masked.

**Returns:** `boolean` - `true` if the editor is masked

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_textedit
- @ref lua_api_gui_frame
