TestClass = {};

function TestClass:new()

   mt = {};
   self.__index = self;
   return setmetatable(mt, self);

end

function TestClass:test()

   print("Hello world!");

end

tester = TestClass:new();

function main()
end

main();