#ifndef FTP_HPP
#define FTP_HPP

#include "../irc.hpp"
#include "./ServerManager.hpp"
class ServerManager;
class FTP
{
private:
    static FTP *instance;
    int ftp_socket;
    int port;
    struct sockaddr_in address;
    fd_set readfds;
    socklen_t addrlen;
    int max_socket;

   
public:
    FTP();
    ~FTP();
    FTP(int port);
    static FTP *getFtp();
    void startFTP();
    void ftpLoop();
    void FTP::HandleConnection();
};



#endif