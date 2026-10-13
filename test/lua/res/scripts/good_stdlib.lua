assert(string.format("%d-%s", 7, "x") == "7-x")
assert(table.concat({ 1, 2, 3 }, ",") == "1,2,3")
assert(math.floor(2.7) == 2)
assert(math.max(1, 5, 3) == 5)
assert(utf8.len("h\u{e9}llo") == 5)
assert(utf8.char(72, 105) == "Hi")

local co = coroutine.wrap(function(a)
    local b = coroutine.yield(a + 1)
    return b * 2
end)
assert(co(1) == 2)
assert(co(10) == 20)

assert(select("#", table.unpack({ 1, 2, 3 })) == 3)
assert(pcall(error, "x") == false)
assert(tostring(nil) == "nil")
assert(tonumber("0x10") == 16)
assert(setmetatable({}, { __index = function() return 1 end }).anything == 1)

print("stdlib ok")
