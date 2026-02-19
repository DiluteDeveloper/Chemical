cd "$(dirname "$0")"
bear -- premake5 gmake  
bear -- make -C ../ 
mv compile_commands.json ../
cd "../modules/chemical"
../../bin/Chemical/linux-Debug/Chemical
