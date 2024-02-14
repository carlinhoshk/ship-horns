CXX = cc
CFLAGS = -Wall
INCLUDE = -I/usr/local/include -I/usr/include -Iinclude
LDFLAGS = -L/usr/local/lib -I/usr/lib
LDLIBS = -lcurl

SOURCES = src/horns.c src/twilio.c
OUT = bin/horns

all: build

build: $(SOURCES)
	$(CXX) -o $(OUT) $(INCLUDE) $(LDFLAGS) $(CFLAGS) $(SOURCES) $(LDLIBS)

clean:
	rm -f bin/horns