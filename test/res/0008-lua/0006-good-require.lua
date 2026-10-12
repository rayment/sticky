local good = require("mod_good")
assert(good.value == 42)
assert(require("mod_good") == good, "second require must return the cached module")

local nested = require("sub.mod_nested")
assert(nested.name == "nested")

assert(require("mod_returns_nil") == true, "a module returning nothing is cached as true")
assert(returns_nil_loaded == true)

assert(require("mod_false") == false)
assert(require("mod_false") == false)

require("mod_counter")
require("mod_counter")
assert(counter_loads == 1, "a module must only be executed once")

print("require ok")
