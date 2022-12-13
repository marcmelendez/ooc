all: tangle test

tangle:
	txt2tangle ooc.litc
	txt2tangle matrix.litc
	txt2tangle set.litc
	txt2tangle iterator.litc

test:
	$(info ***** Compiling and running tests... *****)
	gcc -Wall examples/abstract_object_example.c -o examples/abstract_object_example
	./examples/abstract_object_example
	gcc -Wall examples/vector_example.c -o examples/vector_example -lm
	./examples/vector_example
	gcc -Wall examples/matrix_example.c -o examples/matrix_example -lm
	./examples/matrix_example
	gcc -Wall examples/set_example.c -o examples/set_example -lm
	./examples/set_example
	gcc -Wall examples/iterator_example.c -o examples/iterator_example -lm
	./examples/iterator_example
