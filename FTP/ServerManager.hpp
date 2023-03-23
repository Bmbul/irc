#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include "../irc.hpp"
#include "./Server.hpp"


class ServerManager
{
    private:
        static ServerManager *intstance;
        std::map<int,Server>serverMap;
    public:
        ServerManager();
        ~ServerManager();
        int AddServerToReadFds(fd_set *readfds);
        static ServerManager *getManager();
        void	AddServer(const int socketFd);
        void ReciveAndTransfer();
    public:
        char buff[1024];

};

#endif