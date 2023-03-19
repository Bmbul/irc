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

		void	AddMember(const std::string &newMember);
		void	RemoveMember(const std::string &admin, const std::string &removingMember);

		void	MakeAdmin(const std::string &admin, const std::string &newAdmin);
		void	RemoveFromAdmins(const std::string &admin, const std::string &oldAdmin);

		bool	IsAdmin(const std::string &memberNick) const;
		bool	HasMember(const std::string &memberName) const;
		void	Ban(const std::string &memberName);
		void	Unban(const std::string &memberName);

		void	Broadcast(const Client &sender, const std::string &message, const std::string &command);
		Client &getNextMember();
		int getMemberCount(std::string const &type);
		void setAdmin(std::string const &name);

	private:
		void	ValidateAdmin(const std::string &admin) const;
		void	ValidateClientIsInServer(const std::string &client) const;

		void	ValidateCanAdd(const std::string &newMember) const;
		void	ValidateCanRemove(const std::string &admin, const std::string &removingMember) const;
		void	ValidateCanModifyAdmin(const std::string &admin, const std::string &newAdmin) const;


	private:
		void	SetAdmin(const std::string &newAdmin);

		std::map<std::string, Client> members;
		mutable std::vector<std::string> admins;
		std::vector<int> bannedClients;
};




#endif // CHANNEL_HPP
