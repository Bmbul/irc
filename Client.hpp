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
		~Client();
		
		int	getSock();
		std::string getName();
		std::string getNick();
};



#endif // CLIENT_HPP
