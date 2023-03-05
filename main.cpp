#include "Server.hpp"

void	ValidateInput(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Wrong number of arguments!!" << std::endl
			<< "Try like: ./irc <port> <password>" << std::endl;
		exit(1);
	}
	int port = atoi(argv[1]);
	if (port < 1024 || port > 65535)
	{
		std::cout << "Wrong PORT specified!!! Please input a valid PORT" << std::endl
		<< "It should be a number between 1024 and 65535" << std::endl;
		exit(1);
	}
}

int main(int argc, char **argv)
{
	ValidateInput(argc, argv);
	Server	server(atoi(argv[1]), argv[2]);

	server.Setup();

	while(true)
	{
		std::cout << "start of the loop" << std::endl;
		server.ResetSockets();
		server.WaitForActivity();
		server.HandleIncomingConnections();
		server.ListenForClientInput();
		std::cout << "end of the loop" << std::endl;
	}
		
	return 0;
}

