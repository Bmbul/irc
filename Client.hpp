#if !defined(CLIENT_HPP)
#define CLIENT_HPP

#include <iostream>

class Client
{
	private:
		std::string name;
		std::string nick;
		Client();

	public:
		Client(std::string _name);
		virtual ~Client();
		
		int	getSock();
		std::string getName();
		std::string getNick();
};



#endif // CLIENT_HPP
