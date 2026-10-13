local unavailable = {
    "dofile", "loadfile", "load", "collectgarbage", "io", "os", "package", "debug", "swig_type", "swig_equals", "sticky",
}

for _, name in ipairs(unavailable) do
    assert(_G[name] == nil, name .. " must not be available")
end

assert(type(print) == "function")
assert(type(require) == "function")
assert(type(st) == "table")
assert(type(st.io.log.info) == "function")
assert(st.string == nil)
if st.audio then
    assert(st.audio.source().__disown == nil)
end

print("sandbox ok")
