# OS-Project3
ims190003 | Isabella Sobey

I am compliling this code via command line using:
gcc -Wall -Wextra -std=c11 -O2 main.c index.c btree.c -o project3

The structure definitions and node definitions were smooth sailing, mostly copy paste from my previous homework assignments for older classes that I tweaked to fit here. index.h contains all the declarations, btree.c defines all the functions, and index.c reads and writes nodes and headers. The only real issues I had were some confusion over data types and an issue due to including .c files at the top of main.c. 

I've tested the code with my own input.csv file and added some keys with the command line arguments then used extract to create the output.csv file. 
