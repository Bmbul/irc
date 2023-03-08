#if !defined(CHANNEL_HPP)
#define CHANNEL_HPP

#include "Client.hpp"
#include <map>

class Channel
{
	public:
		std::string name;
		Channel();
		~Channel();
		
	private:
		std::map<std::string, Client> members;
};





#endif // CHANNEL_HPP
