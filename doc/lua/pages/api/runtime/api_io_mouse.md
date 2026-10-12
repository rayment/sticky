st.io.mouse {#lua_api_io_mouse}
===========

Table of mouse button constants.

Overview
--------

The constants identify a mouse button. They are passed to callbacks that are called by a mouse interaction, such as
@ref lua_api_gui_rect_register_on_click.

Constants
---------

| Constant             | Description                                  |
|----------------------|----------------------------------------------|
| `st.io.mouse.Left`   | The left mouse button.                       |
| `st.io.mouse.Right`  | The right mouse button.                      |
| `st.io.mouse.Middle` | The middle mouse button, usually the wheel.  |
| `st.io.mouse.X1`     | The first extra button, usually "back".      |
| `st.io.mouse.X2`     | The second extra button, usually "forward".  |

```lua
rect:register_on_click(function(button)
    if button == st.io.mouse.Right then
        st.io.log.info("right clicked")
    end
end)
```

See also
--------

- @ref lua_api_gui_rect
