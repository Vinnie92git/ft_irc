/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:35:20 by vini              #+#    #+#             */
/*   Updated: 2025/06/21 12:38:53 by roberto          ###   ########.fr       */
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
		newChannel.addOpUser(fd);
		channels.push_back(newChannel);
		std::cout << "\033[32mClient \033[0m" << getClient(fd)->getSocket() << "\033[32m created the channel \033[0m" << channelName << std::endl;
	}

	std::string joinMsg = getClient(fd)->getPrefix() + " JOIN :" + channelName + "\r\n";
	std::string	topicMsg = ":server 332 " + getClient(fd)->getNickname() + " " + channelName + " :" + getChannel(channelName)->getTopic() + "\r\n";
	std::string	namesMsg = ":server 353 " + getClient(fd)->getNickname() + " = " + channelName + " :";
	for (size_t i = 0; i < getChannel(channelName)->getMembers().size(); i++)
	{
		int	memberFd = getChannel(channelName)->getMembers()[i];
		if (getChannel(channelName)->isOpMember(memberFd))
        	namesMsg += "@";
		namesMsg += getClient(memberFd)->getNickname() + " ";
	}
	namesMsg += "\r\n";
	std::string	endMsg = ":server 366 " + getClient(fd)->getNickname() + " " + channelName + " :End of /NAMES list\r\n";

	for (size_t i = 0; i < getChannel(channelName)->getMembers().size(); i++)
	{
		int	memberFd = getChannel(channelName)->getMembers()[i];
		send(getClient(memberFd)->getSocket(), joinMsg.c_str(), joinMsg.length(), 0);
	}
	send(getClient(fd)->getSocket(), topicMsg.c_str(), topicMsg.length(), 0);
	send(getClient(fd)->getSocket(), namesMsg.c_str(), namesMsg.length(), 0);
	send(getClient(fd)->getSocket(), endMsg.c_str(), endMsg.length(), 0);
}

bool Server::canJoinChannel(Client& client, Channel& channel, const std::string& providedKey, std::string& errorMsg)
{
	int fd = client.getSocket();

	if (channel.getInviteOnly() && !channel.isInvited(fd))
	{
		errorMsg = ":server 473 " + client.getNickname() + " " + channel.getName() + " :Cannot join channel (+i)\r\n";
		return false;
	}

	if (channel.hasKey() && channel.getKey() != providedKey)
	{
		errorMsg = ":server 475 " + client.getNickname() + " " + channel.getName() + " :Cannot join channel (+k)\r\n";
		return false;
	}

	if (channel.hasUserLimit() && channel.getMembers().size() >= channel.getUserLimit())
	{
		errorMsg = ":server 471 " + client.getNickname() + " " + channel.getName() + " :Cannot join channel (+l)\r\n";
		return false;
	}

	return true;
}


void	Server::joinCmd(std::vector<std::string>& params, int fd)
{
	if (!getClient(fd)->getAuthentication())
	{
		std::string authMsg = ":server 451 * :You have not registered\r\n";
		send(getClient(fd)->getSocket(), authMsg.c_str(), authMsg.length(), 0);
		return;
	}

	if (params.empty())
	{
		std::string errMsg = ":server 461 JOIN :Not enough parameters\r\n";
		send(getClient(fd)->getSocket(), errMsg.c_str(), errMsg.length(), 0);
		return ;
	}
	std::vector<std::string> channelNames = splitComma(params[0]);
	std::vector<std::string> channelKeys;

	if (params.size() > 1)
		channelKeys = splitComma(params[1]);
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		std::string channelName = channelNames[i];
		std::string key;

		if (i < channelKeys.size())
			key = channelKeys[i];
		else
			key = "";
		if (channelName.empty() || channelName[0] != '#')
		{
			std::string errMsg = ":server 476 " + channelName + " :Bad Channel Mask\r\n";
			send(getClient(fd)->getSocket(), errMsg.c_str(), errMsg.length(), 0);
			continue;
		}

		Channel* chan = getChannel(channelName);
		Client* client = getClient(fd);

		if (chan != NULL)
		{
			std::string errorMsg;
			if (!canJoinChannel(*client, *chan, key, errorMsg))
			{
				send(client->getSocket(), errorMsg.c_str(), errorMsg.length(), 0);
				continue;
			}
		}

		joinChannel(channelName, fd);
	}
}
