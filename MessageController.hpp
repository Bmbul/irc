#if !defined(MESSAGE_CONTROLLER_HPP)
#define MESSAGE_CONTROLLER_HPP

#include "CommandData.hpp"

class MessageController
{
	public:
		MessageController();
		~MessageController();
		bool StringStartsWithFromSet(std::string str, std::string set);
	
	public:
		CommandData	Parse(const std::string &input) const;
		void	PrintData(const CommandData &data) const;
		bool	IsValidChannelName(std::string channelName);
		static	MessageController	*getController();
	private:
		static MessageController *instance;
};
     

#endif // MESSAGE_CONTROLLER_HPP
