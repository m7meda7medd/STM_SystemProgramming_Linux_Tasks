# Readme for the First Task "Femto Shell"
## Sample of the Output

`$ ./myfemtoshell.exe`
`I can't hear you Say something > Something`
`you Said: Something? `
`I can't hear you Say something > Hi my Shell` 
`you Said: Hi my Shell ? `
`I can't hear you Say something > what is that ??? `
`you Said: what is that ??? ? `
`I can't hear you Say something > sorry i have to go `
`you Said: sorry i have to go ? `
`I can't hear you Say something > exit`
`I got it, Bye Dear`

## compilation of main.c file 
`gcc -E main.c -o main.i // take the intermediate file after preprocessing`
`gcc -S main.c -o main.s  // take the assembly file `
`gcc -c main.c -o main.o // take the relocatable object file`


## compilation of femtoshell.c file 
`cd FemtoShell`
`gcc -E femtoshell.c -o femtoshell.i // take the intermediate file after preprocessing`
`gcc -S femtoshell.c -o femtoshell.s  // take the assembly file`
`gcc -c femtoshell.c -o femtoshell.o // take the relocatable object file`


## linking the main.o , femtoshell.o and standard c library and some runtime c files 
`cd ..  // to return back  from the femtoshell directory `
`ld -dynamic-linker "/lib64/ld-linux-x86-64.so.2" -o main.exe "/usr/lib/x86_64-linux-gnu/crt1.o" "/usr/lib/x86_64-linux-gnu/crti.o" main.o "./FemtoShell/femtoshell.o" -lc "/usr/lib/x86_64-linux-gnu/crtn.o"`
## you  can do all the compilation process with just one cmd */
### from main directory 
`gcc main.c "FemtoShell/femtoshell.c" -o main.exe`
## used commands
`vim file` : used to edit c files and write the code 
`mkdir dir` : used to make directory in current directory 
`touch file`  : used to make file in current directory  

