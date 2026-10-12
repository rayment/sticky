st.gui.vlayout {#lua_api_gui_vlayout}
==============

Class that places its children on top of each other, from top to bottom.

Overview
--------

A vlayout is a container that is placed inside a frame or inside another layout with
@ref lua_api_gui_component_set_parent. Its children are drawn in the order that they were added, in a single column,
and the whole column is treated as one item by its parent.

A frame already stacks its children vertically, so a vlayout is mostly useful for nesting: place vlayouts inside an
@ref lua_api_gui_hlayout to build columns, or use it to give a group of children their own spacing.

A vlayout derives from `st.gui.component`, so it also has the `close`, `is_closed` and `set_parent` methods. It is
closed when its parent is closed, and closing it closes its children.

Methods
-------

### st.gui.vlayout() {#lua_api_gui_vlayout_ctor}

This method constructs a new, empty vlayout that uses the default spacing of the theme.

**Returns:** `st.gui.vlayout` - a new instance of the `st.gui.vlayout` class

```lua
local column = st.gui.vlayout()
column:set_spacing(0)
column:set_parent(win)
```

### st.gui.vlayout:set_spacing(spacing) {#lua_api_gui_vlayout_set_spacing}

This method sets the gap between children, in pixels.

| Parameter | Type   | Description                                                                    |
|-----------|--------|--------------------------------------------------------------------------------|
| `spacing` | number | The gap in pixels. A negative value restores the default spacing of the theme. |

**Returns:** nothing

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_frame
- @ref lua_api_gui_hlayout
