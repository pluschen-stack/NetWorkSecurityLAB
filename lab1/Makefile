all-debug:client-debug server-debug

all:client server

client-debug:client.c util.c NetworkFileTool.c
	gcc -Wall -Werror -D DEBUGON -g client.c util.c NetworkFileTool.c -o client

client:client.c util.c NetworkFileTool.c
	gcc client.c util.c NetworkFileTool.c -o client

server:server.c util.c NetworkFileTool.c
	gcc server.c util.c NetworkFileTool.c -o server

server-debug:server.c util.c NetworkFileTool.c
	gcc -Wall -Werror -D DEBUGON -g server.c util.c NetworkFileTool.c -o server

clean:
	rm -f client-debug server-debug client server