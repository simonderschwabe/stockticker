default:
	@echo ""

	gcc -c -D_GNU_SOURCE -g3 -pedantic -Werror -Wall \
		-lcurl \
		-lpthread \
		-Iusr/include \
		-Lusr/lib \
 		-o stock_core.o stock_core.c
	gcc -c -D_GNU_SOURCE -g3 -pedantic -Werror -Wall \
		-lcurl \
		-lpthread \
		-Iusr/include \
		-Lusr/lib \
 		-o nn_file_api.o nn_file_api.c
	gcc -lpthread -o stockticker *.o

clean:
	rm *.o
	rm stockticker

comment:

