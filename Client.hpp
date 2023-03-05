#if !defined(CLIENT_HPP)
#define CLIENT_HPP

#include <iostream>
class Server;

class Client
{
	private:
		bool isAuthenticated;
		std::string name;
		std::string nick;

	public:
		Client();
		Client(std::string _name);
		virtual ~Client();
		
		int	getSock();
		std::string getName();
		std::string getNick();
		void	SentToServer(Server *server, std::string request);
};



#endif // CLIENT_HPP
