CC = /usr/bin/clang
CXX = /usr/bin/clang++

CXXFLAGS = -fPIC --std=c++23
LDFLAGS = -static-libstdc++ -static-libgcc -lcurl -largon2

OBJS = \
	./backends/backend.o \
	./backends/file/be_file.o \
	./backends/http/be_http.o \
	./backends/mysql/be_mysql.o \
	./backends/sqlite/be_sqlite.o \
	./mosquitto-plugin-main.o \
        ./plugin.o \

all: auth-plugin.so

backends/backend.o: backends/backend.cpp
backends/http/be_http.o: backends/http/be_http.cpp
backends/mysql/be_mysql.o: backends/mysql/be_mysql.cpp
backends/file/be_file.o: backends/file/be_file.cpp
backends/sqlite/be_sqlite.o: backends/sqlite/be_sqlite.cpp
mosquitto-plugin-main.o: mosquitto-plugin-main.cpp
plugin.o: plugin.cpp

auth-plugin.so: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(OBJS) $(LDFLAGS)

clean :
	find . -type f \( -name '*.o' -o -name '*.so' \) -delete
