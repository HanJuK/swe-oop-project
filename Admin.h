#pragma once

#include <string>
#include <iostream>

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

	void viewAllUserInfo()
	{
		// TODO: logic

		return;
	}

	void blockUser(std::string phone)
	{
		// TODO: logic

		return;
	}

	void addSeat(int seatNo)
	{
		// TODO: logic

		return;
	}

	void removeSeat(int seatNo)
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

	~Admin()
	{
		std::cout << "Admin " << this->name << " (" << this->phone << ") deleted!\n";
	}
};