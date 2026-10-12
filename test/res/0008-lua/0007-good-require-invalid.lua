local invalid = {
    "",
    ".",
    "..",
    ".mod_good",
    "mod_good.",
    "mod..good",
    "../0002-good-print",
    "sub/mod_nested",
    "sub\\mod_nested",
    "/mod_good",
    "mod good",
    "mod-good",
    "mod_good\0evil",
}

for _, name in ipairs(invalid) do
    local ok, err = pcall(require, name)
    assert(not ok, "require must reject '" .. name .. "'")
    assert(string.find(err, "invalid module name", 1, true), err)
end

local unresolved = { "does_not_exist", "sub", "mod_good.lua", "sub.does_not_exist", 42 }

for _, name in ipairs(unresolved) do
    local ok, err = pcall(require, name)
    assert(not ok, "require must fail for '" .. tostring(name) .. "'")
    assert(string.find(err, "could not be resolved", 1, true), err)
end

assert(not pcall(require))
assert(not pcall(require, nil))
assert(not pcall(require, {}))
assert(not pcall(require, true))

assert(not pcall(require, "mod_syntax_error"))
assert(not pcall(require, "mod_syntax_error"), "a failed module must not be cached")
assert(not pcall(require, "mod_runtime_error"))
assert(not pcall(require, "mod_runtime_error"), "a failed module must not be cached")

-- Failing requires must not leak or corrupt state, whatever the name length.
for i = 1, 1000 do
    local name = "a_module_name_long_enough_to_need_a_heap_allocation_" .. i
    assert(not pcall(require, name))
end
assert(require("mod_good").value == 42)

print("require invalid ok")
