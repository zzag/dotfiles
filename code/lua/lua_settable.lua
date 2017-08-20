test = {
   abc = 123,
   stuff = "ololo"
};

function main()
   print("--- main() ---\n");
   print("test.abc = " .. test.abc .. "\n");
   print("test.stuff = " .. test.stuff .. "\n");
end

main();