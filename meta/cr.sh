cd "$(dirname "$0")"
bear -- premake5 gmake  
bear -- make -C ../ 
mv compile_commands.json ../
../bin/Chemical/linux-Debug/Chemical
