CC = /usr/bin/clang
CXX = /usr/bin/clang++

CXXFLAGS = -fPIC --std=c++23

OBJS = \
	./mosquitto-plugin-main.o \
	./backends/backend.o \
	./backends/mysql/be_mysql.o \
	./backends/sqlite/be_sqlite.o \
	./plugin.o \

all: auth-plugin.so

mosquitto-plugin-main.o: mosquitto-plugin-main.cpp
backends/backend.o: backends/backend.cpp
backends/mysql/be_mysql.o: backends/mysql/be_mysql.cpp
backends/sqlite/be_sqlite.o: backends/sqlite/be_sqlite.cpp
plugin.o: plugin.cpp

auth-plugin.so: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(OBJS) $(LDFLAGS)

clean :
	rm -f *.o *.so
