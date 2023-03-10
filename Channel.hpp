#if !defined(CHANNEL_HPP)
#define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"
#include <map>

class Channel
{
	public:
		std::string name;
		Channel();
		~Channel();
		void	AddMember(std::string memberNick);
		void	RemoveMember(std::string memberNick, std::string reason = NULL);
		void	MakeAdmin(std::string maker, std::string newAdmin);
		
	private:
		std::map<std::string, Client> members;
		std::map<std::string, bool> isAdmin;
};


#endif // CHANNEL_HPP
