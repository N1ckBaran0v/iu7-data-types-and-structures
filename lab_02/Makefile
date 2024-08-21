# compiler
CC := gcc

# keys to compile
CKEYS := -std=c99 -Wall -Werror -pedantic

# keys to linking
LKEYS := -lm

# remove
RM := rm

# build debug
debug : CKEYS += -DDEBUG -g3 --coverage
debug : LKEYS += --coverage
debug : app.exe

# build release
release : CKEYS += -DNDEBUG -g0
release : app.exe

# object files
OBJS :=  ./out/my_create.o ./out/my_input.o ./out/my_out.o ./out/my_resize.o ./out/my_sorts.o ./out/my_time.o

# build app
app.exe : out/main.o $(OBJS)
	$(CC) -o app.exe ./out/main.o $(OBJS) $(LKEYS)

# compile ./out/main.o
./out/main.o: src/main.c inc/my_record.h inc/my_resize.h inc/my_create.h inc/my_errors.h inc/my_input.h inc/my_time.h inc/my_out.h inc/my_sorts.h
	$(CC) $(CKEYS) -c ./src/main.c -o ./out/main.o

# compile ./out/my_create.o
./out/my_create.o: src/my_create.c inc/my_create.h inc/my_record.h inc/my_errors.h inc/my_input.h
	$(CC) $(CKEYS) -c ./src/my_create.c -o ./out/my_create.o

# compile ./out/my_input.o
./out/my_input.o: src/my_input.c inc/my_input.h inc/my_record.h inc/my_errors.h
	$(CC) $(CKEYS) -c ./src/my_input.c -o ./out/my_input.o

# compile ./out/my_out.o
./out/my_out.o: src/my_out.c inc/my_out.h inc/my_record.h inc/my_sorts.h inc/my_errors.h inc/my_input.h
	$(CC) $(CKEYS) -c ./src/my_out.c -o ./out/my_out.o

# compile ./out/my_resize.o
./out/my_resize.o: src/my_resize.c inc/my_resize.h inc/my_record.h inc/my_create.h inc/my_input.h inc/my_errors.h inc/my_sorts.h
	$(CC) $(CKEYS) -c ./src/my_resize.c -o ./out/my_resize.o

# compile ./out/my_sorts.o
./out/my_sorts.o: src/my_sorts.c inc/my_sorts.h inc/my_record.h inc/my_errors.h
	$(CC) $(CKEYS) -c ./src/my_sorts.c -o ./out/my_sorts.o

# compile ./out/my_time.o
./out/my_time.o: src/my_time.c inc/my_time.h inc/my_record.h inc/my_errors.h inc/my_sorts.h inc/my_create.h
	$(CC) $(CKEYS) -c ./src/my_time.c -o ./out/my_time.o

# clean
.PHONY : clean debug release
clean :
	$(RM) -f ./out/* ./*.exe