#if !defined(CLIENT_MANAGER_HPP)
#define CLIENT_MANAGER_HPP


#include "Client.hpp"
#include "CommandHandler.hpp"
#include "irc.hpp"

class MessageController;

class ClientManager
{
	private:
		std::map<int, Client>	clientMap;
		mutable std::map<int, Client>::const_iterator it;
		char buffer[1025];

	private:
		static ClientManager *instance;
		MessageController	*messageController;

	public:
		bool	HasClient(const std::string &clientName) const;
		int		GetClientSocket(const std::string &clientName) const;
		void	AddClient(const int socketFd);
		void	RemoveClient(const int socketFd);
		void	RemoveClient(std::map<int, Client>::iterator iter);

		int		AddClientstToReadFds(fd_set *readfds);
		void	HandleInput(fd_set *readfds);
		void	HandleMessage(Client &client);

		ClientManager();
		~ClientManager();
		static ClientManager *getManager();
		Client *getClient(std::string const &NickName);

};


#endif // CLIENT_MANAGER_HPP
