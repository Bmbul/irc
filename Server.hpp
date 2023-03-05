#if !defined(SERVER)
#define SERVER

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> //close


#include <map>
#include <iostream>
#include <cerrno>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Client.hpp"
#include "unistd.h"

class Server
{
	public:
		int	master_socket;
		int max_sd;
		int	port;
		std::string password;
		char buffer[1025];

	public:
		std::map<int, Client>	clients_map;
		std::map<int, Client>::iterator it;

		struct sockaddr_in address;
		socklen_t	addrlen;
		fd_set readfds;

	public:
		Server(int port, std::string password);
		~Server();

		void	Setup();
		void	ResetSockets();
		void	CreateServer();
		void	SetOptions();
		void	BindSocket();
		void	StartListening();
		int		AcceptNewSocket();
		void	AddNewSocket(int sockfd);
		void	ListenForClientInput();
		void	SentToClient(int sockfd, const char *message);
		void	WaitForActivity();
		void	HandleIncomingConnections();
		bool	TryToAuthenticate(std::string request);
		int		getMaster();

	private:
		Server();
};

#endif // SERVER



