local co = coroutine.wrap(function()
    error("inside coroutine")
end)

co()
