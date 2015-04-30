default:
	@echo ""

	gcc -D_GNU_SOURCE -g3 -lm -pedantic -Werror -Wall \
		-lcurl \
		-lpthread \
		-Iusr/include \
		-Lusr/lib \
 		-o stockticker main.c

clean:
	rm stockticker

comment:

