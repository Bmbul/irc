#if !defined(MESSAGE_CONTROLLER_HPP)
#define MESSAGE_CONTROLLER_HPP

#include "CommandData.hpp"
#include "Client.hpp"
#include <cstring>
#include <sstream>

class MessageController
{
	public:
		MessageController();
		~MessageController();
		bool StringStartsWithFromSet(const std::string &str, const std::string &set) const;
	
	public:
		CommandData	Parse(const std::string &input) const;
		void	PrintData(const CommandData &data) const;
		bool	IsValidChannelName(std::string channelName) const;
		const std::string GetCommandDoesNotExistMessage(const std::string &command);

		void	SendMessageWithSocket(int clientSocket, const std::string &message) const;
		void	SendMessageToClient(const Client &client, const std::string &message) const;
		void	SendHelloMessage(const Client &client) const;
		void	SendHelloMessage(int clientSocket) const;
		static	MessageController	*getController();
	private:
		static MessageController *instance;
};
     

#endif // MESSAGE_CONTROLLER_HPP
