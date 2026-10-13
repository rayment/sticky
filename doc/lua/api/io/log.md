st.io.log {#lua_api_io_log}
===========

Module that allows logging messages to the console.

Overview
--------

This module provides helper functions to log messages to the console of various log levels/severities.

The console is provided in three different ways: stdout, a log file, and the window if libsticky is compiled with video
support.

Static Functions
----------------

### st.io.log.trace(msg) {#lua_api_io_log_trace}

This function prints a trace message to the console.

```lua
st.io.log.trace("Trace message, normally used for debug tracing.")
```

### st.io.log.debug(msg) {#lua_api_io_log_debug}

This function prints a debug message to the console.

```lua
st.io.log.debug("Debug message!")
```

### st.io.log.info(msg) {#lua_api_io_log_info}

This function prints an informational message to the console.

```lua
st.io.log.info("Hello, world!")
```

### st.io.log.warn(msg) {#lua_api_io_log_warn}

This function prints a warning message to the console.

```lua
st.io.log.warn("Warning: The player died!")
```

### st.io.log.error(msg) {#lua_api_io_log_error}

This function prints an error message to the console.

```lua
st.io.log.error("Error: Something went wrong!")
```
