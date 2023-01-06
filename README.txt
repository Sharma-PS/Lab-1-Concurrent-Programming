
******************* Commands to compile C files **************************
1.Serial Linkedlist
	gcc -g -Wall -o serial serial.c

2.Linked list with Mutex
	gcc -g -Wall -o mutex mutex.c -lm -lpthread

3.Linked list with read write lock
	gcc -g -Wall -o readWrite readWrite.c -lm -lpthread

***************************************************************************

****************** Commands to run the compiled files *********************
1.Serial Linkedlist
        ./serial <fraction of member> <fraction of insert> <fraction of delete>

2.Linked list with Mutex
        ./mutex <fraction of member> <fraction of insert> <fraction of delete> <no of threads>
 
3.Linked list with read write lock
        ./readWrite <fraction of member> <fraction of insert> <fraction of delete> <no of threads>

******************************************************************************

***Run the python file to find average, standard deviation and no of samples**
    python lab1.py

*******************************************************************************
