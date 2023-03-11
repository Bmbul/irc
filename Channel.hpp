#if !defined(CHANNEL_HPP)
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>
#include <map>

class Channel
{
	public:
		std::string name;
		
		Channel();
		~Channel();

		void	AddMember(std::string memberNick);
		void	RemoveMember(const std::string &memberNick, const std::string &reason = NULL);
		void	MakeAdmin(const std::string &maker, const std::string &newAdmin);
		bool	IsAdmin(const std::string &memberNick);
		bool	HasMember(const std::string &memberName) const;
		void	Ban(const std::string &memberName);
	
	private:
		std::map<std::string, Client> members;
		std::map<std::string, bool> adminsMap;
		std::vector<int> bannedClients;
};


#endif // CHANNEL_HPP
