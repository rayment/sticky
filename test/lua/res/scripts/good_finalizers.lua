-- Finalizers run when the runtime is destroyed. A failing one must not stop
-- the others or take the host down.
kept = setmetatable({}, {
    __gc = function()
        print("finalizer ran")
    end,
})

failing = setmetatable({}, {
    __gc = function()
        error("finalizer failed")
    end,
})

closed = setmetatable({}, {
    __gc = function()
        print("second finalizer ran")
    end,
})
