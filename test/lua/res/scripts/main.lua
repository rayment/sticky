local mod = require("mod_good")

function on_start()
    print("main start " .. mod.value)
end

function on_fixed_step(dt)
    print("main fixed_step")
end

function on_step(dt)
    print("main step")
end

function on_quit()
    print("main quit")
end
