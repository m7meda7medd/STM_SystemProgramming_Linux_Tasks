<<<<<<< HEAD:UnixUtilites/Readme.md
# Sample of the Output
 
=======
# Readme for the Second Task "Unix utilites"
### note : you have to add their pathes to the env path variable to run from all dir as shell cmd
## Sample of the Output of FemtoShell

'code' $ ./myfemtoshell.exe
'code' abokhalil@abokhalil-linux:~/Desktop/Repos/STM_Tasks/Unix_Utilities/Extended_Shell$ ./myfemtoshell
###start
'code'abokhalil@stm-linux:$ echo Here's current pwd 
'code' Here's current pwd 
'code' abokhalil@stm-linux:$ pwd
'code'/home/abokhalil/Desktop/Repos/STM_Tasks/Unix_Utilities/Extended_Shell
'code'abokhalil@stm-linux:$ cd ..
'code'abokhalil@stm-linux:$ echo directory changed 
'code'directory changed 
'code'abokhalil@stm-linux:$ pwd
'code'/home/abokhalil/Desktop/Repos/STM_Tasks/Unix_Utilities
'code'abokhalil@stm-linux:$ cd ./Extended_Shell
'code' abokhalil@stm-linux:$ pwd
'code' /home/abokhalil/Desktop/Repos/STM_Tasks/Unix_Utilities/Extended_Shell
'code' abokhalil@stm-linux:$ aycmd 
'code' aycmd: command not found
'code' abokhalil@stm-linux:$ exit
'code' abokhalil@abokhalil-linux:~/Desktop/Repos/STM_Tasks/Unix_Utilities/Extended_Shell$ 
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

