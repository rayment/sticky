error(setmetatable({}, {
    __tostring = function()
        error("tostring failed")
    end,
}))
