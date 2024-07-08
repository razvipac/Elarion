#include "ClientData.h"

ClientData::ClientData(int id) : id(id) {}
void ClientData::setUsername(std::string username) {
	this->username = username;
}
int ClientData::getId() {
	return id;
}
std::string ClientData::getUsername() {
	return username;
}