#include "Server.h"
#include "global.h"

int main(int argc, char* argv[])
{
	SERVER server;
	server.Init();
	while (1) {
		server.Update();
	}
	return 0;
}