/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 01:03:51 by vini              #+#    #+#             */
/*   Updated: 2025/03/23 22:33:01 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::setSocket(int fd)
{
	this->clientSocket = fd;
}

void	Client::setAddress(std::string address)
{
	this->clientAddress = address;
}

std::string	Client::getAddress()
{
	return this->clientAddress;
}

int	Client::getSocket()
{
	return this->clientSocket;
}