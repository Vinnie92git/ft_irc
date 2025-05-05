/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:35:20 by vini              #+#    #+#             */
/*   Updated: 2025/05/05 20:36:49 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
		newChannel.addMember(fd);
		channels.push_back(newChannel);
		std::cout << "\033[32mClient \033[0m" << getClient(fd)->getSocket() << "\033[32m created the channel \033[0m" << channelName << std::endl;
	}

	std::string joinMsg = getClient(fd)->getPrefix() + " JOIN :" + channelName + "\r\n";
	std::string	topicMsg = ":server 332 " + getClient(fd)->getNickname() + " " + channelName + " :No topic is set\r\n";
	std::string	namesMsg = ":server 353 " + getClient(fd)->getNickname() + " = " + channelName + " :";
	for (size_t i = 0; i < getChannel(channelName)->getMembers().size(); i++)
	{
		int	memberFd = getChannel(channelName)->getMembers()[i];
		namesMsg += getClient(memberFd)->getNickname() + " ";
	}
	namesMsg += "\r\n";
	std::string	endMsg = ":server 366 " + getClient(fd)->getNickname() + " " + channelName + " :End of /NAMES list\r\n";

	send(getClient(fd)->getSocket(), joinMsg.c_str(), joinMsg.length(), 0);
	send(getClient(fd)->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
	send(getClient(fd)->getSocket(), namesMsg.c_str(), namesMsg.length(), 0);
	send(getClient(fd)->getSocket(), endMsg.c_str(), endMsg.length(), 0);
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
