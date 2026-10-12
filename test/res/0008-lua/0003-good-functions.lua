counter = 0
not_a_function = 42
tbl = { fn = function() end }

function on_noop()
end

function on_start()
    print("on_start called")
end

function on_step(dt)
    assert(math.type(dt) == "float", "dt must be a float")
    print("on_step " .. dt)
end

function on_counter()
    counter = counter + 1
    print("counter " .. counter)
end

function on_args(i, f, s, b, n, extra)
    assert(math.type(i) == "integer", "i must be an integer")
    assert(math.type(f) == "float", "f must be a float")
    assert(type(s) == "string", "s must be a string")
    assert(type(b) == "boolean", "b must be a boolean")
    assert(n == nil, "n must be nil")
    assert(extra == nil, "extra must be nil")
    print("on_args", i, f, s, b)
end

function on_arg_count(...)
    print("arg_count " .. select("#", ...))
end

function on_string_length(s)
    print("string_length " .. #s)
end

function on_print_values()
    print("a", 1, 2.5, true, nil)
end

function on_print_empty()
    print()
end

function on_print_meta()
    print(setmetatable({}, { __tostring = function() return "custom tostring" end }))
end

function on_print_error()
    print(setmetatable({}, { __tostring = function() error("tostring failed") end }))
end

function on_print_bad_tostring()
    print(setmetatable({}, { __tostring = function() return {} end }))
end

function on_error()
    error("boom from on_error")
end

function on_error_table()
    error({ code = 7 })
end

function on_error_meta()
    error(setmetatable({}, { __tostring = function() return "custom error object" end }))
end

function on_error_nil()
    error(nil)
end

function on_overflow()
    local function recurse(n)
        return 1 + recurse(n + 1)
    end
    recurse(1)
end

function on_yield()
    coroutine.yield()
end

function on_bad_arith()
    return {} + 1
end
