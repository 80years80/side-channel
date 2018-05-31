all: flush-reload main

main: main.c flush-reload
	gcc -o $@ -I../common $< -L. -lflush-reload -Wl,-rpath=.

flush-reload: flush-reload.c
	gcc -o libflush-reload.so -shared -fPIC -I../common $<

clean:
	rm -rf libflush-reload.so main
