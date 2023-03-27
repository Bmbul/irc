#if !defined(IMESSENGER_HPP)
#define IMESSENGER_HPP

#include <iostream>
#include <sys/socket.h>

class Client;

class IMessenger
{
	private:

	public:
		virtual ~IMessenger();
		void	SendMessageWithSocket(int clientSocket, const std::string &message) const;
		void	SendMessageToClient(const Client &client, const std::string &message) const;
};

#endif // IMESSENGER_HPP
