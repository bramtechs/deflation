deflate: deflate_cli.c deflation.h
	gcc -ggdb deflate_cli.c -o deflate -DEFLATION_TEST -DEFLATION_VERBOSE

clean:
	rm deflate

check: deflate
	./deflate example example.def

check2: deflate
	./deflate ~/dev/basalt/assets example.def

install: deflate
	cp deflate /usr/bin/deflate

remove:
	rm -f /usr/bin/deflate

