st.gui.hlayout {#lua_api_gui_hlayout}
==============

Class that places its children side by side, from left to right.

Overview
--------

An hlayout is a container that is placed inside a frame or inside another layout with
@ref lua_api_gui_component_set_parent. Its children are drawn in the order that they were added, in a single row, and
the whole row is treated as one item by its parent.

Layouts can be nested, so an hlayout of vlayouts builds columns and a vlayout of hlayouts builds rows. See
@ref lua_api_gui_vlayout.

An hlayout derives from `st.gui.component`, so it also has the `close`, `is_closed` and `set_parent` methods. It is
closed when its parent is closed, and closing it closes its children.

Methods
-------

### st.gui.hlayout() {#lua_api_gui_hlayout_ctor}

This method constructs a new, empty hlayout that uses the default spacing of the theme.

**Returns:** `st.gui.hlayout` - a new instance of the `st.gui.hlayout` class

```lua
local row = st.gui.hlayout()
row:set_parent(win)

local left = st.gui.button()
left:set_text("Left")
left:set_parent(row)

local right = st.gui.button()
right:set_text("Right")
right:set_parent(row)
```

### st.gui.hlayout:set_spacing(spacing) {#lua_api_gui_hlayout_set_spacing}

This method sets the gap between children, in pixels. Use `0` for children that touch, such as the cells of a grid.

| Parameter | Type   | Description                                                                    |
|-----------|--------|--------------------------------------------------------------------------------|
| `spacing` | number | The gap in pixels. A negative value restores the default spacing of the theme. |

**Returns:** nothing

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_component
- @ref lua_api_gui_frame
- @ref lua_api_gui_vlayout
