all: drk main

main: main.c drk
	gcc -o $@ -I../common $< -L. -ldrk -Wl,-rpath=.

drk: drk.c
	gcc -o libdrk.so -shared -fPIC -I../common $<

clean:
	rm -rf main libdrk.so
