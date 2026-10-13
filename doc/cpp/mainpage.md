Engine C++ Reference {#mainpage}
====================

Reference documentation for the sticky engine library (`lib/`) and executable (`src/`).

Start with **Topics**, which lists the modules of the engine:

- @ref core — start-up, types and assertions
- @ref io — logging, files and input devices
- @ref os — native message boxes
- @ref util — persistent settings
- @ref resource — named resources declared by mods
- @ref audio — sounds, sources and the listener
- @ref video — the window and textures
- @ref gui — components drawn by the engine
- @ref lua — the Lua runtime and native bindings
- @ref app — the executable

Conventions
-----------

- Everything lives in the `st` namespace, which mirrors the folders of `lib/`.
- The engine is built without exceptions. Functions are `noexcept` and report failure through their return value
  (`false`, null or an empty `std::optional`), after logging the reason. #ST_ASSERT only checks for programming errors.
- Strings are UTF-8, including file paths converted to and from `std::string`.
- Classes hide their state behind a private implementation and cannot be copied or moved.

Documenting code
----------------

Document every public declaration in its header with a Javadoc-style comment, and add the header to its module with
`@addtogroup`:

```cpp
/** @addtogroup io
 *  @{
 */

/**
 * @brief Short one-line summary.
 *
 * Longer description if needed.
 *
 * @param name Description of the parameter.
 * @return Description of the return value.
 */

/** @} */
```

Build this documentation with `cmake --build build/Debug --target doc-cpp`. Undocumented public symbols are listed in
`build/Debug/doc/cpp/warnings.log`.
