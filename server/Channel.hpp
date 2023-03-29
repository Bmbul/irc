#if !defined(CHANNEL_HPP)
#define CHANNEL_HPP

#include "Client.hpp"
#include <vector>
#include <map>
#include "CommandResponse.hpp"


struct ModeType
{
	enum Mode
	{
		none = 0, 
		read = 1,
		write_ = 2,
		invite = 4,
		private_ = 8
	};
};

class CommandResponse;

class Channel : public  CommandResponse
{
	public:
		
		Channel();
		~Channel();
		Channel(std::string const &_name);
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

		void	Broadcast(const Client &sender, const std::string &message, const std::string &command) const;
		void	SendChannelReply(const std::string &message) const;
		void	SendJoinReply(const Client &client) const;
		void	SendWhoReply(const Client &client) const;
		int		getMemberCount();
		void	PrintData();

		void	SetPassword(const std::string &_password);
		bool	CheckPassword(const std::string &_checkingPass) const;

		int		HasMode(ModeType::Mode _mode)const;
		void	AddMode(ModeType::Mode mode);
		void	RemoveMode(ModeType::Mode mode);
		std::string	ModeInfo() const;
		std::string	GetAdmin();
		void ChannelWhoResponse(Client const &client);
		void ChannelJoinResponse(Client const &client);

	private:
		std::string	name;
		std::string	password;
		std::map<std::string, Client> members;
		int		mode;
		void	ValidateAdmin(const std::string &admin) const;
		void	ValidateAdminIsInChannel(const std::string &admin) const;

		void	ValidateClientIsInServer(const std::string &client) const;
		void	ValidateClientIsInChannel(const std::string &admin, const std::string &client) const;

		void	ValidateCanModifyAdmin(const std::string &admin, const std::string &newAdmin) const;


	private:
		void	SetAdmin(const std::string &newAdmin);
		void	DeleteAdmin(const std::string &removingAdmin);

		mutable std::vector<std::string> admins;
		std::vector<int> bannedClients;
};




#endif // CHANNEL_HPP
