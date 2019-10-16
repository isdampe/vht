CXX = clang++
src = $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)

LDFLAGS = -O3
OSFLAGS = -lm

vht: $(obj)
	@mkdir -p bin
	$(CXX) -o bin/$@ $^ $(LDFLAGS) $(OSFLAGS)

install:
	@cp bin/vht /usr/local/bin/vht

uninstall:
	@rm /usr/local/bin/vht

clean:
	rm $(obj)
