/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 01:03:51 by vini              #+#    #+#             */
/*   Updated: 2025/03/18 00:12:12 by vini             ###   ########.fr       */
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