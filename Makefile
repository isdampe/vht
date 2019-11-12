CXX = gcc
src = $(wildcard src/*.c)
obj = $(src:.c=.o)

LDFLAGS = -O3
OSFLAGS = -std=c11

sc:
	sc -i "./src/*.sc"
	$(vht)

vht: $(obj)
	@mkdir -p bin
	$(CXX) -o bin/$@ $^ $(LDFLAGS) $(OSFLAGS)

install:
	@cp bin/vht /usr/local/bin/vht

uninstall:
	@rm /usr/local/bin/vht

clean:
	rm src/*.c
	rm src/*.h
	@rm $(obj)
