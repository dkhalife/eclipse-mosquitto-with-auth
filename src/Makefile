CC = /usr/bin/clang
CXX = /usr/bin/clang++

OBJS = \
	./mosquitto-plugin-main.o \

all: auth-plugin.so

mosquitto-plugin-main.o: mosquitto-plugin-main.cpp

auth-plugin.so: $(OBJS)
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $(OBJS) $(LDFLAGS)

clean :
	rm -f *.o *.so
