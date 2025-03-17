/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 01:03:48 by vini              #+#    #+#             */
/*   Updated: 2025/03/18 00:11:14 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <vector>

class Client
{
public:
	void	setSocket(int fd);
	void	setAddress(std::string address);

private:
	std::string	clientAddress;
	int			clientSocket;
};

#endif