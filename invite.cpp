/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:46:50 by vini              #+#    #+#             */
/*   Updated: 2025/06/04 18:31:42 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::inviteUserToChannel(std::string channelName, std::string user, int userSocket, int fd)
{
	// Envía un mensaje al usuario invitado para que se una al canal
	std::string inviteMsg = ": INVITE " + user + " " + "to" + " " + channelName + "\r\n";
	send(userSocket, inviteMsg.c_str(), inviteMsg.length(), 0);

	// Envía un mensaje al usuario que invitó para confirmar la invitación
	std::string confirmMsg =  getClient(fd)->getNickname() + " has invited " + user + " to the channel " + channelName + "\r\n";
	send(fd, confirmMsg.c_str(), confirmMsg.length(), 0);

	getChannel(channelName)->inviteClient(userSocket);
}

void	Server::inviteCmd(std::vector<std::string>& params, int fd)
{
	if (!getClient(fd)->getAuthentication())
	{
		std::string authMsg = ":server 451 * :You have not registered\r\n";
		send(getClient(fd)->getSocket(), authMsg.c_str(), authMsg.length(), 0);
		return;
	}

	if (params.size() != 2)
	{
		std::cout << "INVITE error: Not enough parameters provided." << std::endl;
		return;
	}
	std::string	user = params[0];
	std::string	channelName = params[1];
	int			userSocket = -1;
	size_t		i = 0;

	for (i = 0; i < connectedClients.size(); i++)
	{
		if (connectedClients[i].getNickname() == user)
		{
			std::cout << "El usuario existe" << std::endl;
			userSocket = connectedClients[i].getSocket();
			break;
		}
	}
	if (userSocket == -1)
	{
		std::cout << "El usuario no existe" << std::endl;
		return;
	}
	if (getChannel(channelName)->isMember(userSocket))
	{
		std::cout << "El usuario ya estaba en el canal" << std::endl;
		return;
	}
	if (!getChannel(channelName))
	{
		std::cout << "INVITE error: Channel does not exist." << std::endl;
		return;
	}
	if (!getChannel(channelName)->isMember(fd))
	{
		std::cout << "INVITE error: You are not in the channel." << std::endl;
		return;
	}
	std::cout << "entrasndo: " << channelName << " " << user << " " <<  std::endl;
	inviteUserToChannel(channelName, user, userSocket, fd);
}
