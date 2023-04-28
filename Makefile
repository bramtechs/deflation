deflate: deflate_cli.c
	gcc deflate_cli.c -o deflate -DEFLATION_TEST -DEFLATION_VERBOSE -DEFLATION_IMPLEMENTATION

clean:
	rm deflate

test: deflate
	./deflate

install: deflate
	cp deflate /usr/bin/deflate

remove:
	rm -f /usr/bin/deflate

