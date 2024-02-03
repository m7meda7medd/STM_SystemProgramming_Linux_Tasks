<<<<<<< HEAD:UnixUtilites/Readme.md
# Sample of the Output
 
=======
# Readme for the Second Task "Unix utilites"
### note : you have to add their pathes to the env path variable to run from all dir as shell cmd
## Sample of the Output of FemtoShell
'code' 

$ ./myfemtoshell.exe
'code'
abokhalil@abokhalil-linux:~/Desktop/Repos/STM_Tasks/Unix_Utilities/Extended_Shell$ ./myfemtoshell
###start
abokhalil@stm-linux:$ echo Here's current pwd 
Here's current pwd 
abokhalil@stm-linux:$ pwd
/home/abokhalil/Desktop/Repos/STM_Tasks/Unix_Utilities/Extended_Shell
abokhalil@stm-linux:$ cd ..
abokhalil@stm-linux:$ echo directory changed 
directory changed 
abokhalil@stm-linux:$ pwd
/home/abokhalil/Desktop/Repos/STM_Tasks/Unix_Utilities
abokhalil@stm-linux:$ cd ./Extended_Shell
abokhalil@stm-linux:$ pwd
/home/abokhalil/Desktop/Repos/STM_Tasks/Unix_Utilities/Extended_Shell
abokhalil@stm-linux:$ aycmd 
aycmd: command not found
abokhalil@stm-linux:$ exit
abokhalil@abokhalil-linux:~/Desktop/Repos/STM_Tasks/Unix_Utilities/Extended_Shell$ 
'code'
#end

## Sample of  the output of external utilities 
###echo 
'code'./echo Mohamed Ahmed "AboKhalil" 'code'
'code' Mohamed Ahmed AboKhalil 'code'
### mv 
./mv path1 path2 
### pwd
'code' abokhalil@abokhalil-linux:~/Desktop/Repos/STM_Tasks/Unix_Utilities/Utils$ ./pwd 'code'
'code' /home/abokhalil/Desktop/Repos/STM_Tasks/Unix_Utilities/Utils 'code'

## Comilation from main.c dir 
'code' gcc main.c ./FemtoShell/femtoshell.c -o myfemtoshell 'code' 
'
## Compilation of extrnal Utilities
gcc Util.c -o Util for every Util :D

