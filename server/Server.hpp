#if !defined(SERVER_HPP)
#define SERVER_HPP

#include "ClientManager.hpp"
#include "Channel.hpp"

class Server : public IMessenger
{
	private: // for singleton
		static Server *instance;
	
	private:
		int	master_socket;
		int max_sd;
		int	port;
		std::string password;

		std::map<std::string, Channel> channels;
		struct sockaddr_in address;
		socklen_t	addrlen;
		fd_set readfds;

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
		void	SendToClient(int sockfd, const char *message);
		void	WaitForActivity();
		void	HandleIncomingConnections();
		void	ClearClientFromChannels(const Client &client);
		int		getaddrlen();
		struct	sockaddr_in	*GetAddress();
		std::string const &getPass()const;
		std::string	const getHost() const;
		void	SendHelloMessage(const Client &client) const;

		bool HasChannel(std::string const &name);
		//const Channel &getChannel(std::string const &name)const;
		Channel &getChannel(std::string const &name);
		//void AddChannel(std::string const &name);
		void removeChannel(std::string const &name);

	private:
		Server();
};


//void Server::AddChannel(std::string const &name);//der chem grel!!

#endif // SERVER_HPP


