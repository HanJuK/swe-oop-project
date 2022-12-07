#pragma once

#include "User.h"

#include <string>
#include <iostream>
#include <vector>

class Admin
{
	std::string id;
	std::string pw;
	std::string name;
	std::string phone;

public:
	Admin(std::string id, std::string pw, std::string name, std::string phone)
	{
		this->id = id;
		this->pw = pw;
		this->name = name;
		this->phone = phone;
	}

	void updateAdminInfo(std::string id, std::string pw, std::string name, std::string phone)
	{
		this->id = id;
		this->pw = pw;
		this->name = name;
		this->phone = phone;

		return;
	}
	//void showUserInfo(User *currentUser)
	void viewAllUserInfo(std::vector<User *> *users, void (*showUserInfo)(User *user))
	{
		for (int i = 0; i < (*users).size(); ++i)
		{
			showUserInfo((*users)[i]);
		}

		return;
	}

	void blockUser(std::string phone)
	{
		// TODO: logic

		return;
	}

	void addSeat()
	{
		// TODO: logic

		return;
	}

	void removeSeat()
	{
		// TODO: logic

		return;
	}

	void makeSeatTemporaryUnavailable(int seatNo)
	{
		// TODO: logic

		return;
	}

	void changeDefaultReservationTime(int minutes)
	{
		// TODO: logic

		return;
	}

	std::string getId()
	{
		return this->id;
	}

	std::string getPw()
	{
		return this->pw;
	}

	std::string getName()
	{
		return this->name;
	}

	std::string getPhone()
	{
		return this->phone;
	}

	~Admin()
	{
		std::cout << "Admin " << this->name << " (" << this->phone << ") deleted!\n";
	}
};