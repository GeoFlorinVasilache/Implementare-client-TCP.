all: build

build: server subscriber

server: 
	gcc rezervaserverrezerva.c -o server -lm

subscriber:
	gcc subscriber.c -o subscriber

clean:
	rm -rf server subscriber