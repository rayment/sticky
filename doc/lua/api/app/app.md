st.app {#lua_api_app}
===========

Module that allows interfacing to high-level engine functionality.

Overview
--------

This module exposes high-level engine functions to Lua for general use, such as closing the application.

Static Functions
----------------

### st.app.quit(code) {#lua_api_app_quit}

This function shuts down the engine and closes the application gracefully.

The `code` parameter is an integer that specifies the exit code to return to the operating system. Any non-zero value indicates an error.

The request is acted upon by the engine's main loop, so the application does not close immediately. The current iteration of the loop finishes, `on_quit` is then called for every mod, and the application exits with the given code. Code that follows the call in the same function keeps running.

Only the first call has any effect. Subsequent calls are ignored, even when made by a different mod.

Calling this function has no effect when the engine runs without the video module, as there is no main loop to stop.
