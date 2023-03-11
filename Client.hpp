#if !defined(CLIENT_HPP)
#define CLIENT_HPP

#include <iostream>

class Client
{
	private:
		// bool isPassed;
		// bool isUsered;
		// bool isNicked;
		std::string name;
		std::string nick;
		int	fd;

	public:
		Client();
		Client(std::string _name, int _fd);
		~Client();
		
		std::string getName() const ;
		std::string getNick() const ;
		int getSocket() const ;

		void setNick(const std::string &newNick);

};



#endif // CLIENT_HPP
