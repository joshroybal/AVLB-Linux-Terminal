cc = g++
cflags = -O2
lflags =
program = avlbase
objects = obj/bst.o \
          obj/avl.o \
			 obj/csv.o \
			 obj/fileio.o \
			 obj/screenio.o \
			 obj/main.o

bin/$(program): $(objects)
	$(cc) $(lflags) $(objects) -o bin/$(program)

obj/bst.o: src/bst.cpp src/bst.hpp
	$(cc) $(cflags) -c $< -o $@ 

obj/avl.o: src/avl.cpp src/avl.hpp src/bst.hpp
	$(cc) $(cflags) -c $< -o $@

obj/csv.o: src/csv.cpp src/csv.hpp
	$(cc) $(cflags) -c $< -o $@

obj/fileio.o: src/fileio.cpp src/fileio.hpp src/csv.hpp
	$(cc) $(cflags) -c $< -o $@

obj/screenio.o: src/screenio.cpp src/screenio.hpp
	$(cc) $(cflags) -c $< -o $@

obj/main.o: src/main.cpp src/bst.hpp src/avl.hpp src/csv.hpp src/screenio.hpp \
src/csv.hpp
	$(cc) $(cflags) -c $< -o $@

.PHONY : clean
clean:
	rm bin/$(program) obj/*.o
