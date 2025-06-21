/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:25:39 by vini              #+#    #+#             */
/*   Updated: 2025/06/20 13:33:54 by roberto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::modeCmd(std::vector<std::string>& params, int fd)
{
	std::cout << "estas entrando?" << std::endl;
	// Check if client is authenticated
	if (!getClient(fd)->getAuthentication())
	{
		std::string authMsg = ":server 451 * :You have not registered\r\n";
		send(getClient(fd)->getSocket(), authMsg.c_str(), authMsg.length(), 0);
		return;
	}

	if (params.size() < 2)
		return;

	std::string channelName = params[0];
	std::string modeStr = params[1];
	Channel* channel = getChannel(channelName);

	if (!channel)
	{
		std::string errMsg = ":server 403 " + getClient(fd)->getNickname() + " " + channelName + " :No such channel\r\n";
		send(fd, errMsg.c_str(), errMsg.length(), 0);
		return;
	}

	if (!channel->isOpMember(fd))
	{
		std::string errMsg = ":server 482 " + getClient(fd)->getNickname() + " " + channelName + " :You're not channel operator\r\n";
		send(fd, errMsg.c_str(), errMsg.length(), 0);
		return;
	}

	bool adding = true;
	size_t paramIndex = 2;
	std::string modeReply = ":" + getClient(fd)->getPrefix() + " MODE " + channelName + " ";
	std::string modeArgs = "";

	for (size_t i = 0; i < modeStr.length(); ++i)
	{
		char modeChar = modeStr[i];
		if (modeChar == '+')
		{
			adding = true;
			modeReply += "+";
		}
		else if (modeChar == '-')
		{
			adding = false;
			modeReply += "-";
		}
		else if (modeChar == 'i')
		{
			channel->setInviteOnly(adding);
			modeReply += "i";
		}
		else if (modeChar == 't')
		{
			channel->setTopicRestricted(adding);
			modeReply += "t";
		}
		else if (modeChar == 'k')
		{
			if (adding)
			{
				if (paramIndex >= params.size())
					break;
				channel->setKey(params[paramIndex]);
				modeArgs += params[paramIndex++] + " ";
				modeReply += "k";
			}
			else
			{
				channel->removeKey();
				modeReply += "k";
			}
		}
		else if (modeChar == 'l')
		{
			if (adding)
			{
				if (paramIndex >= params.size())
					break;
				int limit = std::atoi(params[paramIndex].c_str());
				channel->setUserLimit(limit);
				modeArgs += params[paramIndex++] + " ";
				modeReply += "l";
			}
			else
			{
				channel->removeUserLimit();
				modeReply += "l";
			}
		}
		else if (modeChar == 'o')
		{
			if (paramIndex >= params.size())
				break;
			std::string targetNick = params[paramIndex++];
			int targetFd = getClientFd(targetNick);
			if (targetFd == -1)
				continue;
			if (!channel->isMember(targetFd))
			{
				std::string errMsg = ":server 441 " + targetNick + " " + channelName + " :They aren't on that channel\r\n";
				send(fd, errMsg.c_str(), errMsg.length(), 0);
				continue;
			}
			if (adding)
				channel->addOpUser(targetFd);
			else
				channel->removeOpUser(targetFd);

			modeArgs += targetNick + " ";
			modeReply += "o";
		}
	}

	modeReply += " " + modeArgs + "\r\n";

	// Broadcast to all clients in the channel
	std::vector<int> members = channel->getMembers();
		for (size_t i = 0; i < members.size(); ++i)
		{
			send(members[i], modeReply.c_str(), modeReply.length(), 0);
		}
}
