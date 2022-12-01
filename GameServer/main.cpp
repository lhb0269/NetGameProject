#include "Server.h"
#include "global.h"

SERVER server;

int main(int argc, char* argv[])
{
	server.Init();
	while (1) {
		server.Update();
	}
	return 0;
}