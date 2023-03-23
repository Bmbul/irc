#include "./FTP.hpp"

FTP *FTP::instance = NULL;

FTP::FTP(){}

FTP::FTP(int port)
{
    this->port = port;
    if(!instance)
        instance = this;
    else
        this->~FTP();
}

FTP::~FTP()
{
}

FTP *FTP::getFtp()
{
    if (!instance)
		instance = new FTP();
	return (instance);
}


void FTP::startFTP()
{
    addrlen = sizeof(address);
    int option = -1;
    if((ftp_socket = socket(AF_INET,SOCK_STREAM,0)) == 0)
    {
        perror("ftp socket failed");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(ftp_socket,SOL_SOCKET,SO_REUSEADDR,(char *)&option,sizeof(option)) < 0)
    {
        perror("ftp set socket option failure");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);
    if(bind(ftp_socket,(struct sockaddr *)&address,addrlen) < 0)
    {
        perror(" ftp binding failure!!");
        exit(EXIT_FAILURE);
    }

    if(listen(ftp_socket,3) < 0)
    {
        perror("ftp listening failure!!!");
        exit(EXIT_FAILURE);
    }
	std::cout << "FTP server is ready for connections ..." << std::endl;

}



void FTP::HandleConnection()
{
    int new_socket;
    if(FD_ISSET(ftp_socket,&readfds))
    {
        new_socket = accept(ftp_socket,(struct sockaddr *)&address,&addrlen);
        if(new_socket < 0)
        {
            perror(" accept failure!!");
            exit(EXIT_FAILURE);
        }
        ServerManager::getManager()->AddServer(new_socket);
        ServerManager::getManager()->ReciveAndTransfer();
    }
}

void FTP::ftpLoop()
{
    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(ftp_socket,&readfds);
        int max_fd_in_clients =  ServerManager::getManager()->AddServerToReadFds(&readfds);
        max_socket = std::max(ftp_socket,max_fd_in_clients);
        int select_ = select(max_socket + 1,&readfds,NULL,NULL,NULL);
        if((select_ < 0) && (errno != EINTR))
        {
                std:: cerr << "SELECT FAILURE " << std::endl;
                exit(EXIT_FAILURE);
        }

    }
    
}