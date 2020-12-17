PROJECT	= mergetars
OBJ	= mergetars.o utilities.o tars.o archives.o files.o directories.o

C99	= cc -std=c99
CFLAGS	= -Wall -Werror -pedantic

$(PROJECT) : $(OBJ)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJ)


%.o : %.c mergetars.h
	$(C99) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) *.o *.tar *.tgz
