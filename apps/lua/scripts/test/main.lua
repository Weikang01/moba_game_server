-- self

local a = {}

function a.test() -- dot
    print("a.test")
end

a.test()

-- in function defined with colon, there's a self object to represent the instance of object
local b = {}
function b:test(a, b) -- colon
    print("b:test", self)
    print(a, b)
end

b:test(1, 2)

function b:test2()
    self.test(self, 44, 3)
end

function b.test3()
    print("there's no self in b.test3", self) -- nil
end

b:test2()
b.test2(b)
b.test3()

print(("hello"):sub(1, 2))

a = {
    name = "name in a"
}
local meta_a = {
    __index = {
        name = "name in meta_a",
        age = 10,
        sex = 0
    }
}

-- add two tables, first argument is a table, second argument is a table as well
-- meta_a is the meta table of a
setmetatable(a, meta_a)                  -- set meta table
print("get meta table", getmetatable(a)) -- get meta table

-- there's a important key in meta table: __index
-- when lua traverse keys in a table, if lua cannot find key in table
-- it will search key in __index key of meta table
print(a.name) -- name in a
print(a.age)  -- 10


-- oop
-- lua does not have oop syntax, but we can simulate it

local base = {}

-- member function
function base:test()
    print("base:test", self)
end

function base:new(inst)
    if not inst then
        inst = {}
    end

    setmetatable(inst, self)
    print("self.__index:", self.__index)
    self.__index = self
    print("self.__index:", self.__index)

    return inst
end

-- we call base "table of class"
-- instatiate a class
b = base:new({
    name = "my name",
    age = 33
})
b:test()
-- end

-- inheritance
-- base table
local person = {}
function person:test()
    print("person:test")
end

function person:get_age()
    print("person:get_age")
end

function person:get_sex()
    print("person:get_sex")
end

function person:new(instance)
    instance = instance or {}
    setmetatable(instance, { __index = self });
    return instance
end

-- inherit class
local man = person:new()
function man:test_man()
    print("man:test_man")
end

function man:test_man2()
    print("man:test_man2")
end

function man:get_age() -- function overrided
    print("man:get_age")
end

local p = man:new() -- p's meta table in __index is man table

p:test_man2()
p:get_age()
p:get_sex() -- from person
man.get_age(p)

return man
