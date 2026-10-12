st.gui.component {#lua_api_gui_component}
================

Base class of every GUI component.

Overview
--------

Every class in `st.gui` derives from `st.gui.component`, so the methods on this page are available on frames, buttons,
labels, progress bars, images and popups. `st.gui.nestable_component` is the base of components that can hold children,
which is only `st.gui.frame`.

These classes cannot be constructed directly. Create one of the concrete classes instead.

Components are drawn by the engine; see @ref lua_gui_lifecycle for how long they live and how to release them.

Methods
-------

### st.gui.component:show() {#lua_api_gui_component_show}

This method shows a top-level component so that the engine draws it on every frame until it is closed.

The engine keeps the component alive while it is shown, so it is not necessary to keep a reference to it. Showing a
component that is already shown does nothing.

A warning is logged and the call is ignored if the component has a parent, because the parent draws it, or if it has
already been closed.

**Returns:** nothing

```lua
local win = st.gui.frame()
win:set_title("Hello")
win:show()
```

### st.gui.component:close() {#lua_api_gui_component_close}

This method closes the component, which is final. It is safe to call more than once.

Closing runs the close callbacks of frames and popups, drops the callbacks of the component, closes all children and
removes the component from its parent and from the screen. See @ref lua_gui_lifecycle for details.

**Returns:** nothing

### st.gui.component:is_closed() {#lua_api_gui_component_is_closed}

This method queries whether the component has been closed.

**Returns:** `boolean` - `true` if the component was closed, or `false` otherwise

### st.gui.component:set_parent(parent) {#lua_api_gui_component_set_parent}

This method makes the component a child of a frame, which then draws it and closes it when the frame is closed.

Children are drawn in the order that they were added. Setting the same parent again moves the component to the end.
Setting a different parent moves the component away from its previous one. A component that was shown stops being drawn
on its own once it has a parent.

A warning is logged and the call is ignored if `parent` is `nil` or closed, if the component is closed, or if `parent`
is the component itself or one of its descendants. Frames and popups are root components and can never be given a
parent, so the call is ignored for them as well.

| Parameter | Type           | Description                           |
|-----------|----------------|---------------------------------------|
| `parent`  | `st.gui.frame` | The frame that should contain the component. |

**Returns:** nothing

```lua
local ok = st.gui.button()
ok:set_parent(win)
```

See also
--------

- @ref lua_gui_lifecycle
- @ref lua_api_gui_frame
