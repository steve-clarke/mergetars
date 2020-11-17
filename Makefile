PROJECT =  mergetars
HEADERS =  $(PROJECT).h
OBJ     =  mergetars.o dir_utilities.o sort.o print.o populators.o

C99     =  cc -std=c99
CFLAGS  =  -Wall -pedantic -Werror

$(PROJECT) : $(OBJ) 
	$(C99) $(CFLAGS) -o $(PROJECT) \
		$(OBJ)

%.o : %.c $(HEADERS)
	$(C99) $(CFLAGS) -c $<

clean: 
	rm -f $(PROJECT) $(OBJ)
