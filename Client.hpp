#if !defined(CLIENT_HPP)
#define CLIENT_HPP

#include <iostream>

class Client
{
	private:
		 bool isPassed;
		 bool isUsered;
		 bool isNicked;
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

		bool get_isPassed()const;
		bool get_isUsered()const;
		bool get_isNicked()const;

		void setIsPassed(bool is_passed);
		void setIsUsered(bool is_usered);
		void setIsNicked(bool is_Nicked);
		void setName(std::string const &name);
		void setNick(std::string const &nick);
		bool isDone();

};


#endif // CLIENT_HPP
