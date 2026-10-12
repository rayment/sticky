-- The host looks functions up in _G, which a script can make raise.
setmetatable(_G, {
    __index = function(_, key)
        error("global lookup of " .. key)
    end,
})

function on_after_metatable()
    print("function found despite metatable")
end
