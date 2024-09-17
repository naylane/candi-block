all:
	clear
	gcc -o main.c -lintelfpgaup
	sudo ./main
