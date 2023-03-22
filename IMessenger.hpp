#if !defined(IMESSENGER_HPP)
#define IMESSENGER_HPP

#include <iostream>
#include <sys/socket.h>

class Client;

class IMessenger
{
	private:
		void	SendMessageWithSocket(int clientSocket, const std::string &message) const;

	public:
		virtual ~IMessenger();
		void	SendMessageToClient(const Client &client, const std::string &message) const;
};

#endif // IMESSENGER_HPP
