st.gui.textedit {#lua_api_gui_textedit}
===============

Class that provides a multi-line text editor.

Overview
--------

A text editor is placed inside a frame with @ref lua_api_gui_component_set_parent. It fills the width of the frame and is
eight lines tall unless a size is given with @ref lua_api_gui_textedit_set_size. The user edits the text in place, and
@ref lua_api_gui_textedit_text returns what is currently typed. For a single line of text see
@ref lua_api_gui_lineedit.

A text editor has the same enabled, read-only and masked states as @ref lua_api_gui_lineedit.

| Enabled | Read-only | Behaviour                                                      |
|---------|-----------|----------------------------------------------------------------|
| `true`  | `false`   | The text can be focused, selected and edited. This is the default. |
| `true`  | `true`    | The text can be focused, selected and copied, but not edited.  |
| `false` | any       | The editor is greyed out and cannot be focused.                |

### Scrolling

The vertical scrollbar appears when the text is taller than the editor and can be hidden with
@ref lua_api_gui_textedit_set_vertical_scrollbar. A hidden scrollbar does not stop the text from scrolling with the
mouse wheel or the caret.

Long lines wrap at the width of the editor by default. Enabling the horizontal scrollbar with
@ref lua_api_gui_textedit_set_horizontal_scrollbar stops the wrapping and lets the lines scroll sideways instead,
following the caret and the mouse wheel. The sideways scroll is not drawn as a bar.

A text editor derives from `st.gui.component`, so it also has the `close`, `is_closed` and `set_parent` methods.

Methods
-------

### st.gui.textedit() {#lua_api_gui_textedit_ctor}

This method constructs a new, enabled, editable and unmasked text editor with no text. It wraps long lines and has a
vertical scrollbar.

**Returns:** `st.gui.textedit` - a new instance of the `st.gui.textedit` class

```lua
local notes = st.gui.textedit()
notes:set_size(0, 200)
notes:set_text("First line\nSecond line")
notes:set_parent(win)
```

### st.gui.textedit:text() {#lua_api_gui_textedit_text}

This method returns the text in the editor, including any changes made by the user. Lines are separated by `\n`.

**Returns:** `string` - the text of the editor

### st.gui.textedit:set_text(text) {#lua_api_gui_textedit_set_text}

This method replaces the text in the editor.

| Parameter | Type   | Description                                  |
|-----------|--------|----------------------------------------------|
| `text`    | string | The new text of the editor. May contain `\n`. |

**Returns:** nothing

### st.gui.textedit:set_size(width, height) {#lua_api_gui_textedit_set_size}

This method sets the size of the editor in pixels. A `width` of `0` or below fills the width of the frame, and a
`height` of `0` or below gives the default height of eight lines.

| Parameter | Type   | Description                                          |
|-----------|--------|------------------------------------------------------|
| `width`   | number | The width of the editor, or `0` to fill the frame.   |
| `height`  | number | The height of the editor, or `0` for eight lines.    |

**Returns:** nothing

### st.gui.textedit:set_enabled(enabled) {#lua_api_gui_textedit_set_enabled}

This method sets whether the editor reacts to the user. A disabled editor is greyed out and cannot be focused. The
default is `true`.

| Parameter | Type    | Description                                          |
|-----------|---------|------------------------------------------------------|
| `enabled` | boolean | `true` to enable the editor, `false` to disable it.  |

**Returns:** nothing

### st.gui.textedit:is_enabled() {#lua_api_gui_textedit_is_enabled}

This method returns whether the editor is enabled.

**Returns:** `boolean` - `true` if the editor is enabled

### st.gui.textedit:set_read_only(read_only) {#lua_api_gui_textedit_set_read_only}

This method sets whether the text can be changed by the user. A read-only editor can still be focused and its text
selected and copied. The default is `false`.

| Parameter   | Type    | Description                                            |
|-------------|---------|--------------------------------------------------------|
| `read_only` | boolean | `true` to prevent the user editing the text.           |

**Returns:** nothing

### st.gui.textedit:is_read_only() {#lua_api_gui_textedit_is_read_only}

This method returns whether the editor is read-only.

**Returns:** `boolean` - `true` if the editor is read-only

### st.gui.textedit:set_masked(masked) {#lua_api_gui_textedit_set_masked}

This method sets whether every character is drawn as `*`. A masked editor cannot be copied from. The text returned by
@ref lua_api_gui_textedit_text is not masked. The default is `false`.

| Parameter | Type    | Description                                       |
|-----------|---------|---------------------------------------------------|
| `masked`  | boolean | `true` to hide the characters of the text.        |

**Returns:** nothing

### st.gui.textedit:is_masked() {#lua_api_gui_textedit_is_masked}

This method returns whether the editor is masked.

**Returns:** `boolean` - `true` if the editor is masked

### st.gui.textedit:set_vertical_scrollbar(visible) {#lua_api_gui_textedit_set_vertical_scrollbar}

This method sets whether the vertical scrollbar is drawn. The default is `true`.

| Parameter | Type    | Description                                       |
|-----------|---------|---------------------------------------------------|
| `visible` | boolean | `true` to draw the vertical scrollbar.            |

**Returns:** nothing

### st.gui.textedit:has_vertical_scrollbar() {#lua_api_gui_textedit_has_vertical_scrollbar}

This method returns whether the vertical scrollbar is drawn.

**Returns:** `boolean` - `true` if the vertical scrollbar is drawn

### st.gui.textedit:set_horizontal_scrollbar(visible) {#lua_api_gui_textedit_set_horizontal_scrollbar}

This method sets whether long lines scroll sideways instead of wrapping. The default is `false`, which wraps lines.

| Parameter | Type    | Description                                            |
|-----------|---------|--------------------------------------------------------|
| `visible` | boolean | `true` to scroll long lines sideways, `false` to wrap. |

**Returns:** nothing

### st.gui.textedit:has_horizontal_scrollbar() {#lua_api_gui_textedit_has_horizontal_scrollbar}

This method returns whether long lines scroll sideways instead of wrapping.

**Returns:** `boolean` - `true` if long lines scroll sideways

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_lineedit
- @ref lua_api_gui_frame
