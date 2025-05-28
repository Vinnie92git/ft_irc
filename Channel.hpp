/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vini <vini@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:30:55 by vini              #+#    #+#             */
/*   Updated: 2025/05/28 23:16:47 by vini             ###   ########.fr       */
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
		bool	removeOpUser(int memberFd);
		int		getFdFromNickname(std::string nickname);

		std::vector<int>	getMembers();
		std::string			getName();
		// std::vector<int>	getOpUsers();

		bool		isMember(int memberFd);
		bool		isOpMember(int memberFd);
		void		setTopic(std::string topic);
		std::string	getTopic();

		void		setInviteOnly(bool value);
		bool		getInviteOnly() const;

		void		setTopicRestricted(bool value);
		bool		getTopicRestricted() const;

		void		setKey(const std::string& newKey);
		void		removeKey();
		std::string	getKey() const;
		bool		hasKey() const;

		void		setUserLimit(size_t limit);
		void		removeUserLimit();
		size_t		getUserLimit() const;
		bool		hasUserLimit() const;


	private:
		std::string			name;
		std::string			topic;
		std::string			key;
		std::vector<int>	members;
		std::vector<int>	opUsers;
		bool				inviteOnly;
		bool				topicRestricted;
		size_t				userLimit;
};

#endif
