template <CommandType::Type type>
void	Command<type>::validate(Client &sender, const std::vector<std::string> &arguments)
{
	(void) sender;
	(void) arguments;

	throw std::exception();
}

template<>
void	Command<CommandType::pass>::validate(Client &sender, const std::vector<std::string> &arguments)
{
	if(sender.getIsPassed())
		throw AlreadyRegistered(sender.getNick());
	if(arguments.size() == 0)
		throw NeedMoreParams(sender.getNick(),"PASS");
	if (Server::getServer()->getPass() != arguments[0])
		throw PasswordMissmatch(sender.getNick());
}

template<>
void	Command<CommandType::user>::validate(Client &sender,const std::vector<std::string> &arguments)
{
	if(arguments.size() < 4)
		throw NeedMoreParams(sender.getNick(),"USER");
	if (sender.getIsPassed() == false)
		throw NOTAUTHORIZED(sender.getNick(),sender.getName());
	if(sender.isDone())
		throw AlreadyRegistered(sender.getNick());
}
