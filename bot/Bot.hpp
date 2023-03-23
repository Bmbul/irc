#if !defined(BOT_HPP)
#define BOT_HPP

#include "../server/irc.hpp"

class Bot
{
    private:
		socklen_t	addrlen;
		struct sockaddr_in address;
		
		int				port;
		int				socketfd;

		std::string		host;
		std::string		pass;
		std::string 	user;
		std::string 	nick;

		void	Setup();
		void	CreateServer();
		void	SetOptions();
		void	BindSocket();

	public:
		Bot(const std::string &host, int _port, const std::string &_pass,
			const std::string &_user = "havayi", const std::string &_nick = "butul");
        ~Bot();
		
		void	ConnectToServer();
		void	SetUser(const std::string &_user);
		void	SetNick(const std::string &_nick);
};


#endif // BOT_HPP
