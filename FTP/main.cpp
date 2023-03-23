#include "./FTP.hpp"

int main(int ac,char **av)
{
    FTP ftp(atoi(av[1]));
    ftp.startFTP();
    
}
