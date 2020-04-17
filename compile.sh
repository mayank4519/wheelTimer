rm -rf *.o
gcc -g -c LinkList.c -o LinkList.o
gcc -g -c WheelTimer.c -o WheelTimer.o
gcc -g -c main.c -o main.o
gcc -g main.o WheelTimer.o LinkList.o -o exe -lpthread
