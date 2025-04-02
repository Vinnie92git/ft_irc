/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:53:20 by vini              #+#    #+#             */
/*   Updated: 2025/04/02 13:10:49 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::setClientPassword(std::string& command, int fd)
{
	(void)command;
	(void)fd;
	std::cout << "Setting password..." << std::endl;
}

void	Server::setClientNickname(std::string& command, int fd)
{
	(void)command;
	(void)fd;
	std::cout << "Assigning nickname..." << std::endl;
}

void	Server::setClientUsername(std::string& command, int fd)
{
	(void)command;
	(void)fd;
	std::cout << "Assigning username..." << std::endl;
}

void	Server::joinCmd(std::string& command, int fd)
{
	(void)command;
	(void)fd;
	std::cout << "Joining channel/s..." << std::endl;
}