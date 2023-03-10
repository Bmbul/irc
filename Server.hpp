#if !defined(SERVER)
#define SERVER

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> //close

#include <map>
#include <cerrno>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include "unistd.h"

#include "Channel.hpp"
#include "ClientManager.hpp"
#include "MessageController.hpp"

class Channel;
class ClientManager;

class Server
{
	private:
		int	master_socket;
		int max_sd;
		int	port;
		std::string password;
		static Server *instance;

		std::map<std::string, Channel> channels;
		struct sockaddr_in address;
		socklen_t	addrlen;
		fd_set readfds;
		ClientManager	*clientManager;

	public:
		Server(int port, std::string password);
		~Server();

		static Server	*getServer();
		void	Setup();
		void	ResetSockets();
		void	CreateServer();
		void	SetOptions();
		void	BindSocket();
		void	StartListening();
		int		AcceptNewSocket();
		void	ListenForClientInput();
		void	SentToClient(int sockfd, const char *message);
		void	WaitForActivity();
		void	HandleIncomingConnections();
		bool	TryToAuthenticate(std::string request);
		int		getaddrlen();
		struct	sockaddr_in	*GetAddress();

	private:
		Server();
};

#endif // SERVER



