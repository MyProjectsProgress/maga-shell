executable = mega-shell
source_files = main.c input_parser.c
compiler = gcc

all:
	$(compiler) -o $(executable) $(source_files)

clean:
	rm -f *.o

fclean:
	rm -f $(executable)

re: fclean all