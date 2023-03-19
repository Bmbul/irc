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

		bool getIsPassed()const;
		bool getIsUsered()const;
		bool getIsNicked()const;

		void setIsPassed(bool is_passed);
		void setIsUsered(bool is_usered);
		void setIsNicked(bool is_Nicked);
		void setName(std::string const &name);
		void setNick(std::string const &nick);
		bool isDone();
		std::vector<std::string> split(std::string str, std::string delimiter);

};


std::vector<std::string> Client::split(std::string str, std::string delimiter)
{

 std::vector<std::string> values = std::vector<std::string>();

 size_t position;
 while ((position = str.find(delimiter)) != std::string::npos)
 {
  values.push_back(str.substr(0, position));
  str.erase(0, position + delimiter.length());
 }
 values.push_back(str);

 return values;
}


#endif // CLIENT_HPP
