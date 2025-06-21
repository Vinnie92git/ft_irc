/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roberto <roberto@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:32:56 by vini              #+#    #+#             */
/*   Updated: 2025/06/21 12:21:19 by roberto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(){}
Channel::Channel(std::string name) : name(name)
{
	this->topic = "No topic is set";
	this->inviteOnly = false;
	this->topicRestricted = false;
	this->userLimit = 0;
	this->key = "";
}
Channel::Channel(const Channel& toCopy){*this = toCopy;}
Channel&	Channel::operator=(const Channel& toAssign)
{
	if (this != &toAssign)
	{
		this->members = toAssign.members;
		this->name = toAssign.name;
		this->opUsers = toAssign.opUsers;
		this->topic = toAssign.topic;
		this->inviteOnly = toAssign.inviteOnly;
		this->topicRestricted = toAssign.topicRestricted;
		this->userLimit = toAssign.userLimit;
		this->key = toAssign.key;
	}
	return *this;
}
Channel::~Channel(){}

void	Channel::setName(std::string channelName){this->name = channelName;}
void	Channel::addMember(int memberFd)
{
	if (!isMember(memberFd))
		members.push_back(memberFd);
}

void	Channel::addOpUser(int memberFd)
{
	if (!isOpMember(memberFd))
		opUsers.push_back(memberFd);
}

bool	Channel::removeMember(int memberFd)
{
	bool result = false;
	for (size_t i = 0; i < members.size(); i++)
		if (members[i] == memberFd)
		{
			members.erase(members.begin() + i);
			removeOpUser(memberFd);
			result = true;
		}
	return (result);
}

bool	Channel::removeOpUser(int memberFd)
{
	bool result = false;
	for (size_t i = 0; i < opUsers.size(); i++)
		if (opUsers[i] == memberFd)
		{
			opUsers.erase(opUsers.begin() + i);
			result = true;
		}
	return (result);
}
std::vector<int>	Channel::getMembers() {return this->members;}
std::string	Channel::getName(){return this->name;}

bool	Channel::isMember(int memberFd)
{
	bool result = false;
	for (size_t i = 0; i < members.size(); i++)
		if (members[i] == memberFd)
		{
			result = true;
			return (result);
		}

	return (result);
}

bool	Channel::isOpMember(int memberFd)
{
	bool result = false;
	for (size_t i = 0; i < opUsers.size(); i++)
		if (opUsers[i] == memberFd)
		{
			result = true;
			return (result);
		}

	return (result);
}

void		Channel::setTopic(std::string topic){this->topic = topic;}
std::string	Channel::getTopic(){return this->topic;}

void	Channel::setInviteOnly(bool value)
{
	inviteOnly = value;
}

bool	Channel::getInviteOnly() const
{
	return inviteOnly;
}

void Channel::setTopicRestricted(bool value)
{
	topicRestricted = value;
}

bool Channel::getTopicRestricted() const
{
	return topicRestricted;
}

void Channel::setKey(const std::string& newKey)
{
	key = newKey;
}

void Channel::removeKey()
{
	key = "";
}

std::string Channel::getKey() const
{
	return key;
}

bool Channel::hasKey() const
{
	return !key.empty();
}

void Channel::setUserLimit(size_t limit)
{
	userLimit = limit;
}

void Channel::removeUserLimit()
{
	userLimit = 0;
}

size_t Channel::getUserLimit() const
{
	return userLimit;
}

bool Channel::hasUserLimit() const
{
	return userLimit > 0;
}

bool Channel::isInvited(int fd) const
{
	for (size_t i = 0; i < invitedClients.size(); ++i)
		if (invitedClients[i] == fd)
			return true;
	return false;
}

void Channel::inviteClient(int fd)
{
	for (size_t i = 0; i < invitedClients.size(); ++i)
		if (invitedClients[i] == fd)
			return;
	invitedClients.push_back(fd);
}

void Channel::removeInvite(int fd)
{
	for (std::vector<int>::iterator it = invitedClients.begin(); it != invitedClients.end(); ++it)
	{
		if (*it == fd)
		{
			invitedClients.erase(it);
			break;
		}
	}
}
