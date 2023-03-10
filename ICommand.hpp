#if !defined(ICOMMAND_HPP)
#define ICOMMAND_HPP
 
#include <map>
#include <vector>
#include "Client.hpp"


struct ArgumentType
{
	enum Type
	{
		nickname,
		comment, 
		user,
		servername,
		channel, 
		password
	};
};
      
class ICommand
{
	public:
		virtual ~ICommand() {}
		virtual void	execute(Client &sender,const std::vector<std::string> &arguments) = 0;
};

// ICommand::ICommand() { }

// ICommand::~ICommand() { }


#endif // ICOMMAND_HPP
