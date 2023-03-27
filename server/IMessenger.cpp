#include "IMessenger.hpp"
#include "Client.hpp"
#include <stdio.h>

IMessenger::~IMessenger() { }

void	IMessenger::SendMessageWithSocket(int clientSocket,
	const std::string &message) const
{
	if (send(clientSocket, (message + "\n").c_str(), message.length() + 2, 0) < 0)
		perror("send");
}

void	IMessenger::SendMessageToClient(const Client &client,
	const std::string &message) const
{
	SendMessageWithSocket(client.getSocket(), message);
}