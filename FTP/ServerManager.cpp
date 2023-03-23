#include "./ServerManager.hpp"

ServerManager *ServerManager::intstance = NULL;
ServerManager::ServerManager()
{
    if (!intstance)
		intstance = this;
    else
        delete this;
}

ServerManager::~ServerManager()
{
}

ServerManager *ServerManager::getManager()
{
    if(!intstance)
        new ServerManager();
    return(intstance);
}

int ServerManager::AddServerToReadFds(fd_set *readfds)
{
    std::map<int,Server>::iterator it = serverMap.begin();
    for ( ; it != serverMap.end(); it++)
        FD_SET(it->first,readfds);
    if(serverMap.size() > 0)
        return(serverMap.rbegin()->first);
    return(0);
    
}

void	ServerManager::AddServer(const int socketFd)
{
	std::pair<int, Server> newserver(socketFd, Server(socketFd));
	serverMap.insert(newserver);
	std::cout << "Adding to list of sockets as "
		<< serverMap.size() << std::endl;
}



void ServerManager::ReciveAndTransfer()
{
    int socket_,rd;
    for (std::map<int,Server>::iterator it = serverMap.begin(); it != serverMap.end();)
    {
        socket_ = it->first;
        if((rd = recv(socket_,buff,1024,MSG_DONTWAIT)) == 0)
        {
            ++it;
            close(socket_);
            std::map<int, Server>::iterator it = serverMap.find(socket_);

	        if (it != serverMap.end())
		        serverMap.erase(it);
        }
        else
        {
            buff[rd] = 0;
            if(buff[0] && !(buff[0] == '\n'))
                if (send(socket_, buff, strlen(buff) + 1, 0) < 0)
		            perror("send");

            if(serverMap.size() == 0)
                return ;
            it++;
        }
    }
    
}