#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include <algorithm>


Channel::Channel()
{
	// clientListForChan();
	_isOperatorMode = false;
	_topicRestrictionMode = false;
	_canalInviteOnlyMode = false;
	_isSuperOperMode = false;
	_hasPassChannel = false;
	_limitUserMode = 0;
}

Channel::~Channel()
{

}

Channel::Channel(std::string num, std::string name)
{
	numChannel = num;
	nameChannel = name;
	_isOperatorMode = false;
	_topicRestrictionMode = false;
	_canalInviteOnlyMode = false;
	_isSuperOperMode = false;
	_limitUserMode = 0;
}

void Channel::setNameChannel(std::string nameToBe)
{
	nameChannel = nameToBe;
}

void Channel::setTopicChannel(std::string topicToBe)
{
	topicChannel = topicToBe;
}

std::string Channel::getNameChannel()
{
	return nameChannel;
}

const std::string Channel::getNumChannel()
{
	return numChannel;
}

const std::string Channel::getTopicChannel()
{
	return topicChannel;
}

std::vector<std::string> Channel::getMemberOfThisChan()
{
	return memberOfThisChan;
}

std::vector<std::string>	Channel::getMemberOfActiveChan(Client *my_client, std::map<Channel *, bool> chanList, std::string chan)
{
	(void)my_client;
	std::vector<std::string> activeChan;
	std::vector<std::string> memberOfThisChan;
	for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		if (it->second == true && it->first->getNameChannel() == chan)
		{
			memberOfThisChan = it->first->getMemberOfThisChan();
			break ;
		}
	}
	return memberOfThisChan;
}

void Channel::setMemberOfthisChan(std::vector<std::string> new_members)
{
	memberOfThisChan = new_members;
}


void Channel::setIsOperatorMode(bool isOperatorMode) {
    _isOperatorMode = isOperatorMode;
}

bool Channel::getIsOperatorMode() const {
    return _isOperatorMode;
}

void Channel::setIsSuperOperMode(bool isSuperOperMode) {
    _isSuperOperMode = isSuperOperMode;
}

bool Channel::getIsSuperOperMode() const {
    return _isSuperOperMode;
}

void Channel::setCanalInviteOnlyMode(bool canalInviteOnlyMode) {
    _canalInviteOnlyMode = canalInviteOnlyMode;
}

bool Channel::getCanalInviteOnlyMode() const {
    return _canalInviteOnlyMode;
}

void Channel::setTopicRestrictionMode(bool topicRestrictionMode) {
    _topicRestrictionMode = topicRestrictionMode;
}

bool Channel::getTopicRestrictionMode() const {
    return _topicRestrictionMode;
}

void Channel::setLimitUserMode(unsigned int limitUserMode) {
    _limitUserMode = limitUserMode;
}

unsigned int Channel::getLimitUserMode() const {
    return _limitUserMode;
}

void Channel::setNewKeyMode(const std::string& newKeyMode) {
    _newKeyMode = newKeyMode;
}

const std::string& Channel::getNewKeyMode() const {
    return _newKeyMode;
}

void Channel::setOperatorsMembers(std::vector<std::string>& operatorsMembers) {
    _operators_menbers = operatorsMembers;
}

const std::vector<std::string>& Channel::getOperatorsMembers() const {
    return _operators_menbers;
}

void Channel::setPassWord(std::string& passWord) {
    pass_word = passWord;
}

const std::string& Channel::getPassWord() const {
    return pass_word;
}

void Channel::addOperatorsMembers(std::string name)
{
	_operators_menbers.push_back(name);
}


void Channel::setHasPassword(bool isOperatorMode)
{
	_hasPassChannel = isOperatorMode;
}

bool Channel::getHasPassword() const
{
	return _hasPassChannel;
}

std::vector<std::string> Channel::getInvitedMembers()
{
	return _invited_members;
}

void Channel::addInviteMembers(std::string name)
{
	_invited_members.push_back(name);
}

bool Channel::is_member_invited(std::string nick)
{
	for (size_t i = 0; i < _invited_members.size(); i++)
	{
		if (_invited_members[i] == nick)
			return (true);
	}
	return (false);
}

bool Channel::is_member_operator(std::string nick)
{
	for (size_t i = 0; i < _operators_menbers.size(); i++)
	{
		if (_operators_menbers[i] == nick)
			return (true);
	}
	return (false);
}


Channel::Channel(Client *my_client)
{
	_isOperatorMode = false;
	_topicRestrictionMode = false;
	_canalInviteOnlyMode = false;
	_isSuperOperMode = false;
	_limitUserMode = 0;

	Server *tmp = my_client->getMyServer();
	std::vector<Client *> tmp_connectedClients = tmp->GetRegisteredClients();
	for (std::vector<Client *>::iterator it = tmp_connectedClients.begin(); it != tmp_connectedClients.end(); it++)
	{
		Client *my_g = *it;
		std::string dataNickname = my_g->getNickname();
		_clientListForChan.insert(std::make_pair(my_g, dataNickname));
	}
}

bool Channel::isMember(std::string memberName)
{
	for(size_t i = 0; i != memberOfThisChan.size(); i++)
	{
		if (memberOfThisChan[i] == memberName)
			return true;
		else
			return false;
	}
	return false;
}

void Channel::addMember(const std::string& member)
{
	size_t i = 0;
	int count = 0;

	if (memberOfThisChan.empty())
		memberOfThisChan.push_back(member);
	else
	{
		while (i < memberOfThisChan.size())
		{
			if (member == memberOfThisChan[i])
				count += 1;
			i++;
		}
		if (count == 0)
			memberOfThisChan.push_back(member);
	}
}

std::ostream& operator<<(std::ostream& o, const std::vector<std::string>& memberVector)
{
	o << "[";
	for (size_t i(0); i < memberVector.size(); i++)
	{
		o << memberVector[i];
		if (i < memberVector.size() - 1)
			o << ", ";
	}
	o << "]";
	return o;
}

std::ostream& operator<<(std::ostream& o, const Channel *memberVector)
{
	o << "_inviteOnly    = " << memberVector->getCanalInviteOnlyMode() << std::endl;
	o << "_topic_Restric = " << memberVector->getTopicRestrictionMode() << std::endl;
	o << "_hasPassChann  =" << memberVector->getHasPassword() << std::endl;
	o << "_limitUserMode = " << memberVector->getLimitUserMode() << std::endl;
	o << "and operators members are :" << std::endl;
	for (size_t i(0); i < memberVector->getOperatorsMembers().size(); i++)
	{
		o << memberVector->getOperatorsMembers()[i];
		if (i <  memberVector->getOperatorsMembers().size() - 1)
			o << ", ";
	}
	o << std::endl;
	return o;
}