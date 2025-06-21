/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:46:58 by vini              #+#    #+#             */
/*   Updated: 2025/06/21 12:39:04 by roberto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::kickUserFromChannel(std::string channelName, std::string user, std::string reason, int fd)
{
	Channel* channel = getChannel(channelName);
	if (!channel)
	{
		std::cout << "KICK error: Channel " << channelName << " does not exist." << std::endl;
		return;
	}

	int userSocket = -1;
	for (size_t i = 0; i < connectedClients.size(); i++)
	{
		if (connectedClients[i].getNickname() == user)
		{
			userSocket = connectedClients[i].getSocket();
			break;
		}
	}

	if (userSocket == -1)
	{
		std::cout << "KICK error: User " << user << " not found." << std::endl;
		return;
	}
	if (!channel->isMember(fd))
	{
		std::cout << "KICK error: You are not a member of " << channelName << std::endl;
		return;
	}
	if (!channel->isMember(userSocket))
	{
		std::cout << "KICK error: User " << user << " is not in channel " << channelName << std::endl;
		return;
	}

	// Check if kicker is an operator
	if (!channel->isOpMember(fd))
	{
		std::cout << "KICK error: You are not an operator in " << channelName << std::endl;
		return;
	}

	// Build and send KICK message to kicked user and broadcast to other users
	std::string prefix = getClient(fd)->getPrefix();
	std::string kickMsg = prefix + " KICK " + channelName + " " + user;
	if (!reason.empty())
		kickMsg += " :" + reason;
	kickMsg += "\r\n";

	const std::vector<int>& members = channel->getMembers();
	for (size_t i = 0; i < members.size(); i++)
	{
		int memberFd = members[i];
		send(getClient(memberFd)->getSocket(), kickMsg.c_str(), kickMsg.length(), 0);
	}

	channel->removeMember(userSocket);

	std::cout << "User " << user << " was kicked from " << channelName << " by " << getClient(fd)->getNickname() << std::endl;
}

void Server::kickCmd(std::vector<std::string>& params, int fd)
{
	if (!getClient(fd)->getAuthentication())
	{
		std::string authMsg = ":server 451 * :You have not registered\r\n";
		send(getClient(fd)->getSocket(), authMsg.c_str(), authMsg.length(), 0);
		return;
	}

	if (params.size() < 2)
	{
		std::cout << "KICK error: Not enough parameters provided." << std::endl;
		return;
	}
	std::vector<std::string> channelNames = splitComma(params[0]);
	std::vector<std::string> users = splitComma(params[1]);
	std::string reason = "";

	if (params.size() > 2)
	{
		reason = "";
		for (size_t i = 2; i < params.size(); i++)
		{
			if (i > 2)
				reason += " ";
			reason += params[i];
		}
	}

	for (size_t j = 0; j < channelNames.size(); ++j)
	{
		std::string channelName = channelNames[j];
		if (channelName[0] == '&')
			channelName[0] = '#';

		if (channelName[0] != '#')
		{
			std::cout << "KICK error: Invalid channel name " << channelName << std::endl;
			continue;
		}

		for (size_t k = 0; k < users.size(); ++k)
		{
			std::string user = users[k];
			kickUserFromChannel(channelName, user, reason, fd);
		}
	}
}
