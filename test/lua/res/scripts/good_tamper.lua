-- A script that destroys the globals it was handed must not take down the
-- runtime: print and the error path keep working.
tostring = nil
select = nil
table = nil
string = nil
st = nil
require = nil

print("print survives tampering")

function on_tampered()
    print("function survives tampering")
end
