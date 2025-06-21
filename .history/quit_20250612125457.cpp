/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:47:15 by vini              #+#    #+#             */
/*   Updated: 2025/06/04 20:32:54 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::quitServer(std::string reason, int fd)
{
	Client* quittingClient = getClient(fd);
	if (!quittingClient)
		return;

	std::string quitMsg = quittingClient->getPrefix() + " QUIT :" + reason + "\r\n";

	// For each channel, notify members and remove client
	for (size_t i = 0; i < channels.size(); ++i)
	{
		Channel& channel = channels[i];
		std::vector<int> members = channel.getMembers();

		// Only proceed if client is in this channel
		bool isMember = false;
		for (size_t j = 0; j < members.size(); ++j)
		{
			if (members[j] == fd)
			{
				isMember = true;
				break;
			}
		}
		if (!isMember)
			continue;

		// Broadcast QUIT to all other members
		for (size_t j = 0; j < members.size(); ++j)
		{
			int memberFd = members[j];
			if (memberFd != fd)
			{
				send(getClient(memberFd)->getSocket(), quitMsg.c_str(), quitMsg.length(), 0);
			}
		}

		// Remove client from channel
		channel.removeMember(fd);

		// If channel empty, remove it
		if (channel.getMembers().empty())
		{
			removeChannel(channel.getName());
			std::cout << "\033[31mChannel \033[0m" << channel.getName() << "\033[31m removed\033[0m" << std::endl;
			--i;
		}
	}

	// Finally send QUIT to the quitting client itself
	send(quittingClient->getSocket(), quitMsg.c_str(), quitMsg.length(), 0);
}

void	Server::quitCmd(std::vector<std::string>& params, int fd)
{
	std::string reason = "";
	for (size_t i = 0; i < params.size(); ++i)
    {
        if (i > 0)
            reason += " ";
        reason += params[i];
    }
	quitServer(reason, fd);
}
