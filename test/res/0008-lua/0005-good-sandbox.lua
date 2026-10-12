local unavailable = { "dofile", "loadfile", "load", "collectgarbage", "io", "os", "package", "debug" }

for _, name in ipairs(unavailable) do
    assert(_G[name] == nil, name .. " must not be available")
end

assert(type(print) == "function")
assert(type(require) == "function")

print("sandbox ok")
