all:
	clear
	gcc main.c -o main -lintelfpgaup
	sudo ./main
