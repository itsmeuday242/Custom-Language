lang_name = mylang


exec = $(lang_name).out
sources = $(wildcard src/*.cpp)
objects = $(sources:.cpp=.o)
flags = -g


$(exec): $(objects)
	g++ $(objects) $(flags) -o $(exec)

%.o: %.cpp include/%.h
	g++ -c $(flags) $< -o $@

install:
	make
	cp ./$(exec) /usr/local/bin/$(lang_name)

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
