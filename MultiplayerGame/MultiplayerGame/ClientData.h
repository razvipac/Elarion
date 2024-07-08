#pragma once
#include <iostream>
class ClientData {
private:
	int id;
	std::string username;
public:
	ClientData(int id);
	void setUsername(std::string username);
	int getId();
	std::string getUsername();
};