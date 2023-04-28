deflate: deflate_cli.c
	gcc -ggdb deflate_cli.c -o deflate -DEFLATION_TEST -DEFLATION_VERBOSE

clean:
	rm deflate

check: deflate
	./deflate example example.def

install: deflate
	cp deflate /usr/bin/deflate

remove:
	rm -f /usr/bin/deflate

