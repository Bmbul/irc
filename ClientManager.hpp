#if !defined(CLIENT_MANAGER_HPP)
#define CLIENT_MANAGER_HPP


#include <iostream>
#include <map>
#include "unistd.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> //clo

#include "Client.hpp"
#include "Server.hpp"
#include "MessageController.hpp"

class Server;

class ClientManager
{
	private:
		std::map<int, Client>	clientMap;
		mutable std::map<int, Client>::iterator it;
		char buffer[1025];
	private:
		static ClientManager *instance;

	public:
		bool	HasClient(const std::string &clientName);
		void	AddClient(const int socketFd);
		void	RemoveClient(const int socketFd);
		void	RemoveClient(std::map<int, Client>::iterator iter);

		int		AddClientstToReadFds(fd_set *readfds);
		void	HandleInput(fd_set *readfds);
		ClientManager();
		~ClientManager();
		static ClientManager *getManager();

};


#endif // CLIENT_MANAGER_HPP
