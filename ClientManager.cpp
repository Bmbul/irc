#include "ClientManager.hpp"
#include "Server.hpp"



ClientManager	*ClientManager::instance = NULL;


ClientManager	*ClientManager::getManager()
{
	std::cout << "trying to get client Manager" << std::endl;
	if (!instance)
		new ClientManager();
	return (instance);
}


ClientManager::ClientManager()
{
	if (!instance)
		instance = this;
	else
	{
		std::cout << "Creating second instance of Server!!!" << std::endl 
			<< "Bad idea, try new tricks!!!" << std::endl;
		delete this;
	}
}

ClientManager::~ClientManager()
{

}

void	ClientManager::AddClient(const int socketFd)
{
	std::pair<int, Client> newClient(socketFd, Client("Client Name", socketFd));
	clientMap.insert(newClient);
	std::cout << "Adding to list of sockets as "
		<< clientMap.size() << std::endl;
}

void	ClientManager::RemoveClient(const int socketFd)
{
	std::map<int, Client>::iterator it = clientMap.find(socketFd);

	if (it != clientMap.end())
		clientMap.erase(it);
	else
		std::cout << "No such Client!!" << std::endl;
}

void	ClientManager::RemoveClient(std::map<int, Client>::iterator iter)
{
	clientMap.erase(iter);
}



int		ClientManager::AddClientstToReadFds(fd_set *readfds)
{
	for (it = clientMap.begin(); it != clientMap.end(); it++)
		FD_SET(it->first, readfds);
	if (clientMap.size() > 0)
		return (clientMap.rbegin()->first);
	return (0);
}

bool	ClientManager::HasClient(const std::string &clientName)
{
	for (it = clientMap.begin(); it != clientMap.end(); it++)
	{
		if (it->second.getName() == clientName)
			return (true);
	}
	return (false);
}

void	ClientManager::HandleInput(fd_set *readfds)
{
	int sd, valread;
	struct sockaddr_in *address;
	socklen_t	addrlen;

	address = Server::getServer()->GetAddress();
	addrlen = Server::getServer()->getaddrlen();
	for (it = clientMap.begin(); it != clientMap.end();)
	{
		sd = it->first; 
		if (FD_ISSET(sd , readfds))
		{
			if ((valread = recv(sd, buffer, 1024, MSG_DONTWAIT)) == 0)
			{
				//Somebody disconnected , get his details and print
				getpeername(sd , (sockaddr *)address , \
					&addrlen);
				printf("Host disconnected , ip %s , port %d \n" ,
					inet_ntoa(address->sin_addr) , ntohs(address->sin_port));
					
				//Close the socket and mark as 0 in list for reuse
				close( sd );
				RemoveClient(it++);
			}
			else // in case if client inputed message
			{
				buffer[valread] = '\0';
				// clients_map[sd].SentToServer(this, buffer);
				CommandData data = MessageController::getController()->Parse(buffer);
				MessageController::getController()->PrintData(data);
				CommandHandler::getHandler()->ExecuteCommand(it->second, data);
				++it;
			}
		}
		else
			++it;
	}
}