INCLUDEPATH=/usr/local/include
LIBS=/usr/local/lib
#FLAGS=-lopencv_features2d -lopencv_imgproc -lopencv_highgui -lopencv_core -lopencv_imgcodecs -lz
FLAGS=-std=c++0x -lopencv_videoio -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lpthread
CC=g++
SRCS=$(wildcard src/*.cc)
OBJS = $(patsubst src/%.cc,%.o,%(SRCS))
PROGS = $(patsubst src/%.cc,%,$(SRCS))
MADE_OBJ = false

all:	$(PROGS)

%.o:	src/%.cc src/measure_latencies.hh src/udp.hh
	$(CC) -c $< -I$(INCLUDEPATH) -L$(LIBS) $(FLAGS)

%:	src/%.cc %.o
	@ if [ "$@" = "streamserver" ] || [ "$@" = "streamclient" ] || [ "$@" = "test_udpserv" ] || [ "$@" = "test_udpclient" ]; then \
		echo "Building udp objects..." ;\
		echo $(CC) -c src/udp.cc -I$(INCLUDEPATH) -L$(LIBS) $(FLAGS) ;\
		$(CC) -c src/udp.cc -I$(INCLUDEPATH) -L$(LIBS) $(FLAGS) ;\
		echo $(CC) -c src/udpserver.cc -I$(INCLUDEPATH) -L$(LIBS) $(FLAGS) ;\
        $(CC) -c src/udpserver.cc -I$(INCLUDEPATH) -L$(LIBS) $(FLAGS) ;\
		echo $(CC) -c src/udpclient.cc -I$(INCLUDEPATH) -L$(LIBS) $(FLAGS) ;\
        $(CC) -c src/udpclient.cc -I$(INCLUDEPATH) -L$(LIBS) $(FLAGS) ;\
		echo $(CC) -o $@ udp.o udpserver.o udpclient.o $< -I$(INCLUDEPATH) -L$(LIBS) $(FLAGS) ;\
        $(CC) -o $@ udp.o udpserver.o udpclient.o $< -I$(INCLUDEPATH) -L$(LIBS) $(FLAGS) ; \
		$(eval MADE_OBJ = true) \
	elif [ "$@" != "udp" ] &&  [ "$@" != "udpserver" ] && [ "$@" != "udpclient" ]; then \
		echo $(CC) -o $@ $< -I$(INCLUDEPATH) -L$(LIBS) $(FLAGS) ; \
		$(CC) -o $@ $< -I$(INCLUDEPATH) -L$(LIBS) $(FLAGS) ;\
	fi;

clean:
	rm -rf $(PROGS)

