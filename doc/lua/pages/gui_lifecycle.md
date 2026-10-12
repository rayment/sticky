GUI Lifecycle {#lua_gui_lifecycle}
=============

How windows and widgets are rendered, how long they live, and how to let go of them.

Overview
--------

GUI components such as `st.gui.frame` and `st.gui.button` are created from Lua, but the engine draws them. There is no
render function to call and no per-frame GUI callback: once a window is shown it is drawn every frame until it is
closed.

Closing is the end of a component's life. After a window is closed the engine no longer references it, and Lua's
garbage collector reclaims it as soon as your script stops referencing it too. There is nothing to destroy by hand.

States
------

Every component is in exactly one of three states.

| State  | Meaning                                                                                       |
|--------|-----------------------------------------------------------------------------------------------|
| Hidden | The component was created but is neither shown nor given a parent. It is not drawn.           |
| Open   | The component was shown with `show()`, or it was given a parent that is open. It is drawn.    |
| Closed | The component was closed. It is never drawn again and cannot be shown or parented again.      |

Closing is final. To display a window again, create a new one.

Who keeps a component alive
---------------------------

An open component is kept alive by the engine, not by your variables.

- A component that was shown stays alive, and keeps rendering, until it is closed. You do not have to keep a reference
  to it, and the garbage collector will not take it.
- A component that has a parent stays alive for as long as the parent holds it.
- A component that was never shown and never given a parent is only kept alive by your own references. When your
  script drops them it is collected and nothing is ever drawn.
- A closed component is no longer held by the engine. Lua reclaims it when no reference remains.

The engine never holds on to a component after it closes, so a closed component and everything that only it referenced
can be collected.

Closing
-------

A component is closed by any of the following:

- the script calls `close()` on it,
- the user presses the titlebar close button of a closable frame (see @ref lua_api_gui_frame_set_closable),
- the user presses the button of a popup,
- its parent is closed, which closes all of its children.

Closing is idempotent. Calling `close()` on a component that is already closed does nothing, so it is always safe to
call from a callback.

When a component closes, the following happens once, in this order:

1. The `register_on_close` callbacks of a frame or popup are called.
2. The callbacks of the component are dropped. Click callbacks of buttons are dropped as well.
3. The children are closed.
4. The component is detached from its parent and from the screen.

Dropping the callbacks matters: a callback usually captures the window it belongs to, and releasing it is what lets
the window be collected after it closes.

Patterns
--------

### Fire and forget

For a window that needs no further interaction, build it, show it, and let go of it.

```lua
local function open_about()
    local win = st.gui.frame()
    win:set_title("About")
    win:set_auto_resize(true)

    local text = st.gui.label()
    text:set_text("Made with sticky.")
    text:set_parent(win)

    local ok = st.gui.button()
    ok:set_text("Close")
    ok:set_parent(win)
    ok:register_on_click(function() win:close() end)

    win:show()
end
```

Nothing keeps `win` after the function returns, and nothing needs to. The window stays on screen until the user closes
it with the button or the titlebar.

### Updating a window while it is open

Keep a reference only for as long as the window is open, and release it from the `on_close` callback.

```lua
local status = nil

function open_status()
    if status then
        return
    end

    local win = st.gui.frame()
    win:set_title("Status")

    local bar = st.gui.progress()
    bar:set_parent(win)

    win:register_on_close(function() status = nil end)
    win:show()

    status = { win = win, bar = bar }
end

function on_step(dt)
    if status then
        status.bar:set_value(compute_progress())
    end
end
```

Without the `on_close` callback `status` would keep pointing at a closed window. That is harmless, because calls on a
closed component do nothing, but the component would never be collected, and the `if status then` guard would no longer
tell you whether the window is open.

Alternatively, ask the component with @ref lua_api_gui_component_is_closed.

### Keeping your own state outside the widgets

Widgets are a view of your state, not the place to store it. Keep the data in your own tables, and copy it into the
widgets when it changes. When a window closes, only its widgets are released; your data stays.

```lua
local settings = { volume = 0.5 }

local function open_settings()
    local win = st.gui.frame()
    win:set_title("Settings")

    local bar = st.gui.progress()
    bar:set_value(settings.volume)
    bar:set_parent(win)

    win:show()
end
```

### One window at a time

Use the closed state to avoid opening duplicates.

```lua
local window = nil

function toggle_window()
    if window and not window:is_closed() then
        window:close()
        window = nil
        return
    end

    window = st.gui.frame()
    window:set_title("Inventory")
    window:show()
end
```

Things to know
--------------

- **Rendering is not tied to references.** Dropping your variable does not hide an open window. Call `close()` for
  that.
- **Calls on a closed component are harmless.** Setters have no visible effect. `show()` and `set_parent()` log a
  warning and are ignored, because a closed component cannot be reused.
- **Only top-level components are shown.** A component with a parent is drawn by its parent, and calling `show()` on it
  logs a warning.
- **Callbacks run inside the engine's frame.** Callbacks take no arguments. An error raised by a callback is logged
  with a stack traceback and does not stop the engine or the other callbacks.
- **Shutdown does not close windows.** When the engine quits, open components are destroyed without calling their
  `on_close` callbacks. Use `on_quit` to clean up state.
- **Each mod has its own windows.** Components belong to the mod that created them and cannot be passed between mods.

See also
--------

- @ref lua_api_gui_component
- @ref lua_api_gui_frame
- @ref lua_api_gui_button
- @ref lua_api_gui_label
- @ref lua_api_gui_progress
- @ref lua_api_gui_rect
- @ref lua_api_gui_hlayout
- @ref lua_api_gui_vlayout
- @ref lua_api_gui_popup
