#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include "Server.hpp"
#include "Client.hpp"
// class Server;

class Client;

class Channel : public Server
{
	public :
		//canocial form
		Channel();
		~Channel();
		Channel(Client *my_client);
		Channel(std::string num, std::string name);

		//setters et getters
		void setNameChannel(std::string nameToBe);
		void setTopicChannel(std::string topicToBe);
		void addMember(const std::string& member);
		bool isMember(std::string memberName);
		std::string getNameChannel();
		const std::string getNumChannel();
		const std::string getTopicChannel();
		std::vector<std::string> getMemberOfThisChan();
		std::vector<std::string> getInvitedMembers();
		void setMemberOfthisChan(std::vector<std::string> new_members);
		void addInviteMembers(std::string name);
		bool is_member_invited(std::string nick);
		bool is_member_operator(std::string nick);

	

		void setIsOperatorMode(bool isOperatorMode);
		bool getIsOperatorMode() const;

		void setHasPassword(bool isOperatorMode);
		bool getHasPassword() const;
		void setIsSuperOperMode(bool isSuperOperMode);
		bool getIsSuperOperMode() const;
		void setCanalInviteOnlyMode(bool canalInviteOnlyMode);
		bool getCanalInviteOnlyMode() const;
		void setTopicRestrictionMode(bool topicRestrictionMode);
		bool getTopicRestrictionMode() const;
		void setLimitUserMode(unsigned int limitUserMode);
		unsigned int getLimitUserMode() const;
		void setNewKeyMode(const std::string& newKeyMode);
		const std::string& getNewKeyMode() const;
		void setOperatorsMembers(std::vector<std::string>& operatorsMembers);
		void addOperatorsMembers(std::string name);
		const std::vector<std::string>& getOperatorsMembers() const;
		void setPassWord(std::string& passWord);
		const std::string& getPassWord() const;

	private :
	//channels relatable variables
		std::map<Client*, std::string> _clientListForChan;
		std::string nameChannel;
		std::string numChannel;
		std::string topicChannel;
		std::vector<std::string> memberOfThisChan;
	// mode channel variables
	bool						_isOperatorMode;
	bool						_isSuperOperMode;
	bool						_canalInviteOnlyMode;
	bool						_topicRestrictionMode;
	bool						_hasPassChannel;
	unsigned int				_limitUserMode;
	std::string					_newKeyMode;
	std::vector<std::string>	_operators_menbers;
	std::vector<std::string>	_invited_members;
	std::string					pass_word;

};

std::ostream& operator<<(std::ostream& o, const std::vector<std::string>& memberVector);
std::ostream& operator<<(std::ostream& o, const Channel *memberVector);

#endif