Engine C++ Reference {#mainpage}
====================

Reference documentation for the sticky engine library.

Browse the code by:

- **Namespaces** — the `st::*` hierarchy (`st::io`, `st::gui`, `st::audio`, ...).
- **Classes** — class index and inheritance diagrams.
- **Files** — the `lib/` directory tree.

Documenting code
----------------

Use Javadoc-style comments directly above declarations in headers:

```cpp
/**
 * @brief Short one-line summary.
 *
 * Longer description if needed.
 *
 * @param name Description of the parameter.
 * @return Description of the return value.
 */
```

Build this documentation with `cmake --build --preset Debug --target doc-cpp`.
