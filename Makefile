CC=gcc
CFLAGS= -Wall -Wextra 

# Main client target
client: client.o helper.o requests.o functions.o
	$(CC) $^ -o $@ -lstdc++

# Source files
client.o: client.cpp
	$(CC) $(CFLAGS) $< -c

requests.o: requests.cpp
	$(CC) $(CFLAGS) $< -c

functions.o: functions.cpp
	$(CC) $(CFLAGS) $< -c

helper.o: helper.cpp
	$(CC) $(CFLAGS) $< -c

run: client
	./client

clean:
	rm -f *.o client