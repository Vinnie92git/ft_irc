/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:53:20 by vini              #+#    #+#             */
/*   Updated: 2025/04/17 18:07:29 by roberto          ###   ########.fr       */
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
/*

Command: MODE
  Parameters: <target> [<modestring> [<mode arguments>...]]


*/
void Server::modeCmd(std::vector<std::string>& params, int fd)
{
	if (params.empty())
	{
		std::cout << "MODE error: No target provided." << std::endl;
		return ;
	}

	std::vector<std::string> ChannelNameTofind = splitComma(params[0]);
	std::cout << "ChannelNameTofind: " << ChannelNameTofind[0] << std::endl;

	for (size_t i = 0; i < channels.size(); i++)



	//modeChannel(channelName, fd);
}



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

void	Server::modeChannel(std::string channelName, int fd)
{
	/*
		MODE <target> [<modestring> [<mode arguments>...]]

	*/
}
