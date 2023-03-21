#if !defined(CHANNEL_HPP)
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>
#include <map>


struct ModeType
{
	enum Mode
	{
		read = 1,
		write_ = 2,
		invite = 4

	};
};

class Channel
{
	public:
		std::string name;
		
		Channel();
		~Channel();
		Channel(std::string const &name);
		void	AddMember(const std::string &newMember);
		void	KickMember(const std::string &admin, const std::string &removingMember);
		void	LeaveMember(const std::string &memberNick);
		void	LeaveIfMember(const std::string &memberNick);


		void	MakeAdmin(const std::string &admin, const std::string &newAdmin);
		void	RemoveFromAdmins(const std::string &admin, const std::string &oldAdmin);

		bool	IsAdmin(const std::string &memberNick) const;
		bool	HasMember(const std::string &memberName) const;
		void	Ban(const std::string &admin, const std::string &memberName);
		void	Unban(const std::string &admin, const std::string &memberName);

		void	Broadcast(const Client &sender, const std::string &message, const std::string &command);
		int		getMemberCount();
		void	PrintData();
		int GetMode()const;
		void AddMode(ModeType::Mode mode);
		void RemoveMode(ModeType::Mode mode);

	private:
		int		mode;
		void	ValidateAdmin(const std::string &admin) const;
		void	ValidateAdminIsInChannel(const std::string &admin) const;

		void	ValidateClientIsInServer(const std::string &client) const;
		void	ValidateClientIsInChannel(const std::string &admin, const std::string &client) const;

		void	ValidateCanModifyAdmin(const std::string &admin, const std::string &newAdmin) const;


	private:
		void	SetAdmin(const std::string &newAdmin);
		void	DeleteAdmin(const std::string &removingAdmin);

		std::map<std::string, Client> members;
		mutable std::vector<std::string> admins;
		std::vector<int> bannedClients;
};

#endif // CHANNEL_HPP
