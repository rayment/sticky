Adding Documentation Pages {#lua_doc_howto}
==========================

1. Copy `doc/lua/page_template.md.txt` to `doc/lua/pages/<module>.md`.
2. Replace `module_name` and the page id (`lua_api_<module>`) in the first line.
3. Link the new page from its parent with `@subpage lua_api_<module>`
   (usually in `pages/api_index.md`).
4. Build with `cmake --build --preset Debug --target doc-lua`; output is
   written to `build/Debug/doc/lua/html/index.html`.

Notes
-----

- Page ids must be unique across the whole manual.
- Reference other pages with `@ref lua_api_<module>`.
- Reference a section within a page with `@ref lua_api_<module>_<section>`
  after giving that section an id: `Section {#lua_api_<module>_<section>}`.
- Fenced code blocks with the `lua` language tag are rendered as code.
- Snippets stored in `doc/lua/examples/*.lua` can be embedded with
  `\include filename.lua`.
