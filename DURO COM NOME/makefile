serverFile=cobrinhaServer
clientFile=cobrinhaClient
serverName=SERVER
clientName=CLIENT

all: compServer compClient

server: compServer runServer

client: compClient runClient

compClient:
	gcc -std=c99 -o $(clientName) $(clientFile).c libAllegro/AllegroCore.c libSocket/client.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lm -lallegro_dialog -lallegro_audio -lallegro_acodec

compServer:
	gcc -std=c99 -o $(serverName) $(serverFile).c libAllegro/AllegroCore.c libSocket/server.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lm -lallegro_dialog -lallegro_audio -lallegro_acodec

runClient:
	./$(clientName)

runServer:
	./$(serverName)
