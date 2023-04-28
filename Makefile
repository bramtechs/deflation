deflation: deflation.h
	gcc deflation_cli.c -o deflation -DEFLATION_TEST -DEFLATION_VERBOSE -DEFLATION_IMPLEMENTATION

clean:
	rm deflation

run: deflation
	./deflation
