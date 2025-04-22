/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:53:20 by vini              #+#    #+#             */
/*   Updated: 2025/04/22 13:25:33 by roberto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::setClientPassword(std::vector<std::string>& params, int fd)
{
	getClient(fd)->setPassword(params[0]);
	std::cout << "Client " << fd << " password: " << getClient(fd)->getPassword() << std::endl;
}

void	Server::setClientNickname(std::vector<std::string>& params, int fd)
{
	getClient(fd)->setNickname(params[0]);
	getClient(fd)->setPrefix(":" + getClient(fd)->getNickname() + "!user@localhost");
	std::cout << "Client " << fd << " nickname: " << getClient(fd)->getNickname() << std::endl;
	std::cout << "Client " << fd << " prefix: " << getClient(fd)->getPrefix() << std::endl;
}

void	Server::setClientUsername(std::vector<std::string>& params, int fd)
{
	getClient(fd)->setUsername(params[0]);
	std::cout << "Client " << fd << " username: " << getClient(fd)->getUsername() << std::endl;
}

void	Server::joinCmd(std::vector<std::string>& params, int fd)
{
	if (params.empty())
	{
		std::cout << "JOIN error: No channel provided." << std::endl;
		return ;
	}

	std::vector<std::string> channelNames = splitComma(params[0]);

	for (size_t i = 0; i < channelNames.size(); i++)
	{
		std::string channelName = channelNames[i];
		if (channelName[0] != '#')
		{
			std::cout << "JOIN error: Invalid channel name." << std::endl;
			return ;
		}
		joinChannel(channelName, fd);
	}
}

void Server::partCmd(std::vector<std::string>& params, int fd)
{
	if (params.empty())
	{
		std::cout << "PART error: No channel provided." << std::endl;
		return ;
	}

	std::vector<std::string> channelNames = splitComma(params[0]);

	for (size_t i = 0; i < channelNames.size(); i++)
	{
		std::string channelName = channelNames[i];
		if (channelName[0] != '#')
		{
			std::cout << "PART error: Invalid channel name." << std::endl;
			return ;
		}
		partChannel(channelName, fd);}
}


/*

Command: MODE	target			+-
  Parameters: <canal/user> [<modestring> [<mode arguments>...]]
	MODE #foobar +mb *@127.0.0.1

*/
/* void Server::modeCmd(std::vector<std::string>& params, int fd)
{
	if (params.empty())
	{
		std::cout << "MODE error: No target provided." << std::endl;
		return ;
	}
	if (params.size() == 1)
	{
		std::cout << "0 entras" << std::endl;
		std::vector<std::string> ChannelNameTofind = splitComma(params[0]);
		std::cout << "ChannelNameTofind: " << ChannelNameTofind[0] << std::endl;
		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i].getName() == ChannelNameTofind[0])
			{
				std::cout << "linea 79 Channel found: " << channels[i].getName() << std::endl;
				std::string channelName = ChannelNameTofind[0];
				if (channelName[0] != '#')
				{
					std::cout << "MODE error: Invalid channel name." << std::endl;
					return;
				}
				modeTarget(channels[i].getName(), params,fd, 1);
			}
		}
	}
	std::cout << "entrando1" << std::endl;
	std::cout << "params: " << params[0] << " " << params[1] << std::endl;


	std::vector<std::string> modestring = splitComma(params[1]);
	std::string mode = modestring[0];
	std::cout << "entrando2 printea modestring y mode"<< modestring[0] << " " << mode << std::endl;

	if (mode[0] != '+' && mode[0] != '-')
	{
		std::cout << "MODE error: Invalid modestring." << std::endl;
		return;
	}


	//std::cout << "entrando3" << std::endl;
	//	si es un canal entra aquí
	//recorrer el vector de canales y si lo encuentra entrar

	std::vector<std::string> ChannelNameTofind = splitComma(params[0]);
	std::cout << "ChannelNameTofind: " << ChannelNameTofind[0] << std::endl;

	std::cout << "entrando4" << std::endl;

	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getName() == ChannelNameTofind[0])
		{
			std::cout << "Channel found: " << channels[i].getName() << std::endl;
			std::string channelName = ChannelNameTofind[0];
			if (channelName[0] != '#')
			{
				std::cout << "MODE error: Invalid channel name." << std::endl;
				return;
			}
			modeTarget(channels[i].getName(), params,fd, 1);
		}
	}

	std::cout << "entrando5" << std::endl;
	// si es un usuario entra aquí
	std::vector<std::string> UserNameTofind = splitComma(params[0]);
	std::cout << "UserNameTofind: " << ChannelNameTofind[0] << std::endl;

	for (size_t i = 0; i < connectedClients.size(); i++)
	{
		if (connectedClients[i].getNickname() == UserNameTofind[0])
		{
			std::cout << "User found: " << connectedClients[i].getNickname() << std::endl;
			modeTarget(connectedClients[i].getNickname(), fd, 0);
		}
	}
	std::cout << "entrando6" << std::endl;

	std::cout << "MODE error: Target not found." << std::endl;
}
 */
std::vector<std::string>	Server::splitComma(std::string param)
{
	std::vector<std::string>	tokenVector;
	std::istringstream			iss(param);
	std::string					token;

	while(std::getline(iss, token, ','))
	{
		if (!token.empty())
			tokenVector.push_back(token);
	}

	return tokenVector;
}

void	Server::joinChannel(std::string channelName, int fd)
{
	if (getChannel(channelName))
	{
		getChannel(channelName)->addMember(fd);
		std::cout << "\033[32mClient \033[0m" << getClient(fd)->getSocket() << "\033[32m joined the channel \033[0m" << channelName << std::endl;
	}
	else
	{
		Channel	newChannel(channelName);
		channels.push_back(newChannel);
		newChannel.addMember(fd);
		std::cout << "\033[32mClient \033[0m" << getClient(fd)->getSocket() << "\033[32m created the channel \033[0m" << channelName << std::endl;
	}

	std::string joinMsg = getClient(fd)->getPrefix() + " JOIN :" + channelName + "\r\n";
	std::string	topicMsg = ":server 332 " + getClient(fd)->getNickname() + " " + channelName + " :No topic is set\r\n";
	std::string	namesMsg = ":server 353 " + getClient(fd)->getNickname() + " = " + channelName + " :";
	for (size_t i = 0; i < getChannel(channelName)->getMembers().size(); i++)
		namesMsg += getChannel(channelName)->getMembers()[i];
	namesMsg += "\r\n";
	std::string	endMsg = ":server 366 " + getClient(fd)->getNickname() + " " + channelName + " :End of /NAMES list\r\n";

	send(getClient(fd)->getSocket(), joinMsg.c_str(), joinMsg.length(), 0);
	send(getClient(fd)->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
	send(getClient(fd)->getSocket(), namesMsg.c_str(), namesMsg.length(), 0);
	send(getClient(fd)->getSocket(), endMsg.c_str(), endMsg.length(), 0);
}

/* void	Server::modeTarget(std::string channelName, std::vector<std::string>& params, int fd, int mode)
{
	(void)channelName;
	(void)fd;
	std::cout << "entrando7" << std::endl;
		MODE <target> [<modestring> [<mode arguments>...]]

	//0 es un usuario


	//1 es un canal
	if (mode == 1)
	{
		if(params.size() == 1)
		{
			std::cout << "requesting modes for channel: " << channelName << std::endl;
		}
		else if(params.size() >= 2)
		{
			std::cout << "setting mode for channel: " << channelName << " with mode: " << params[1] << " with args: " << params[2] << std::endl;
		}
	}
}
	*/

void	Server::partChannel(std::string channelName, int fd)
{
	if (getChannel(channelName))
	{
		getChannel(channelName)->removeMember(fd);
		std::cout << "\033[31mClient \033[0m" << getClient(fd)->getSocket() << "\033[31m left the channel \033[0m" << channelName << std::endl;
		if (getChannel(channelName)->getMembers().empty())
		{
			std::cout << "estoy entrando en empty channels de PART" << std::endl;
			removeChannel(channelName);
			std::cout << "\033[31mChannel \033[0m" << channelName << "\033[31m removed\033[0m" << std::endl;
		}
	}
	else
		std::cout << "\033[31mClient \033[0m" << getClient(fd)->getSocket() << "\033[31m, you are no longer in \033[0m" << channelName << std::endl;

	std::string partMsg = getClient(fd)->getPrefix() + " PART :" + channelName + "\r\n";


	send(getClient(fd)->getSocket(), partMsg.c_str(), partMsg.length(), 0);
}
