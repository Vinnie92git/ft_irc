/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:30:55 by vini              #+#    #+#             */
/*   Updated: 2025/04/23 21:34:44 by vini             ###   ########.fr       */
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
#include <algorithm>
#include <iterator>

class Channel
{
	public:
		Channel();
		Channel(std::string name);
		Channel(const Channel& toCopy);
		Channel& operator=(const Channel& toAssign);
		~Channel();

		void	setName(std::string channelName);
		void	addMember(int memberFd);
		bool	removeMember(int memberFd);
		void	addOpUser(int memberFd);
		void	removeOpUser(int memberFd);

		std::vector<int>	getMembers();
		std::string			getName();
		std::vector<int>	getOpUsers();

	private:
		std::vector<int>	members;
		std::string			name;
		std::vector<int>	opUsers;
};

#endif
