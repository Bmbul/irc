#if !defined(MESSAGE_CONTROLLER_HPP)
#define MESSAGE_CONTROLLER_HPP

#include "CommandData.hpp"
#include "Client.hpp"
#include <cstring>
#include <sstream>
#include <map>
#include <vector>

class MessageController
{
	public:
		MessageController();
		~MessageController();

	public:
		std::vector<CommandData>	Parse(std::string &input) const;
		CommandData	ParseSingleCommand(const std::string &singleCommand) const;

		void	PrintData(std::vector<CommandData> &data) const;

		bool	IsValidChannelName(const std::string &channelName) const;
		bool	StringStartsWithFromSet(const std::string &str, const std::string &set) const;
		bool	GotEndOfMessage(const char *messageChunk) const;

		void	SendMessage(const Client &sender, const Client &reciever
			, const std::string &command, const std::string message) const;

		std::string	GetClientFormatedName(const Client &client) const;
		std::vector<std::string> Split(std::string str, std::string delimiter);

		void	SendMessageWithSocket(int clientSocket, const std::string &message) const;
		void	SendMessageToClient(const Client &client, const std::string &message) const;
		void	SendHelloMessage(const Client &client) const;
		void	SendHelloMessage(int clientSocket) const;

		bool	ContainsChunk(int clientSocket) const;
		void	AddChunk(int clientSocket, const std::string &messageChunk);
		void	ClearChunk(int clientSocket);
		std::string	ConstructFullMessage(int clientSocket);

		static	MessageController	*getController();

	private:
		std::map<int, std::vector<std::string> > chunksMap;
		static MessageController *instance;
};
     

#endif // MESSAGE_CONTROLLER_HPP
