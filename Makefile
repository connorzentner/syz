syz: syz.c
	gcc -o syz syz.c

clean:
	rm syz
