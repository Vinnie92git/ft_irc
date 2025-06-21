/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:47:04 by vini              #+#    #+#             */
/*   Updated: 2025/06/04 19:33:12 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::partChannel(std::string channelName, std::string reason, int fd)
{
	if (getChannel(channelName))
	{
		if (getChannel(channelName)->removeMember(fd))
		{
			std::cout << "\033[31mClient \033[0m" << getClient(fd)->getSocket() << "\033[31m left the channel \033[0m" << channelName << std::endl;
			std::string partMsg;
			if (reason != "")
				partMsg = getClient(fd)->getPrefix() + " PART :" + channelName + " :" + reason + "\r\n";
			else
				partMsg = getClient(fd)->getPrefix() + " PART :" + channelName + "\r\n";

			for (size_t i = 0; i < getChannel(channelName)->getMembers().size(); i++)
			{
				int	memberFd = getChannel(channelName)->getMembers()[i];
				send(getClient(memberFd)->getSocket(), partMsg.c_str(), partMsg.length(), 0);
			}
			send(getClient(fd)->getSocket(), partMsg.c_str(), partMsg.length(), 0);
			if (getChannel(channelName)->getMembers().empty())
			{
				removeChannel(channelName);
				std::cout << "\033[31mChannel \033[0m" << channelName << "\033[31m removed\033[0m" << std::endl;
			}
		}
	}
	else
		std::cout << "\033[31mClient \033[0m" << getClient(fd)->getSocket() << "\033[31m, you are not in \033[0m" << channelName << std::endl;

}

void Server::partCmd(std::vector<std::string>& params, int fd) // part <channel> <reason>
{
	if (!getClient(fd)->getAuthentication())
	{
		std::string authMsg = ":server 451 * :You have not registered\r\n";
		send(getClient(fd)->getSocket(), authMsg.c_str(), authMsg.length(), 0);
		return;
	}

	if (params.empty())
	{
		std::cout << "PART error: No channel provided." << std::endl;
		return;
	}

	std::string channelNames = params[0];
	std::string reason = "";

	if (params.size() > 1)
		reason = params[1];

	std::vector<std::string> channels = splitComma(channelNames);

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string channelName = channels[i];
		if (channelName[0] == '&')
			channelName[0] = '#';
		if (channelName[0] != '#')
		{
			std::cout << "PART error: Invalid channel name." << std::endl;
			return;
		}
		partChannel(channelName, reason, fd);
	}
}
