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

		void	AddMember(const std::string &admin, const std::string &newMember);
		void	RemoveMember(const std::string &admin, const std::string &removingMember);

		void	MakeAdmin(const std::string &maker, const std::string &newAdmin);
		bool	IsAdmin(const std::string &memberNick) const;
		bool	HasMember(const std::string &memberName) const;
		void	Ban(const std::string &memberName);
		void	Unban(const std::string &memberName);

		void	Broadcast(const Client &sender, const std::string &message);

	private:
		void	ValidateAdmin(const std::string &admin) const;
		void	ValidateClientIsInServer(const std::string &client) const;
		void	ValidateCanAdd(const std::string &admin, const std::string &newMember) const;
		void	ValidateCanRemove(const std::string &admin, const std::string &removingMember) const;



	private:
		std::map<std::string, Client> members;
		mutable std::map<std::string, bool> adminsMap;
		std::vector<int> bannedClients;
};


#endif // CHANNEL_HPP
