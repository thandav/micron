CC=g++
CFLAGS=-c -Wall
SOURCES=main.cpp \
	connection_manager.cpp \
	thread_manager.cpp \
	queue_manager.cpp \
	message.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=project

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o: 
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
	rm -rf project

