local function recurse(n)
    return 1 + recurse(n + 1)
end

recurse(1)
