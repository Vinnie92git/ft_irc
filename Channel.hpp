/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:30:55 by vini              #+#    #+#             */
/*   Updated: 2025/04/02 14:33:43 by vini             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

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

class Channel
{
public:
	Channel();
	Channel(const Channel& toCopy);
	Channel& operator=(const Channel& toAssign);
	~Channel();

	void	setName(std::string channelName);

	std::string	getName();

private:
	std::string	name;
};

#endif