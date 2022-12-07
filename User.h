#pragma once

#include "Seat.h"

#include <string>
#include <iostream>

class User
{
	std::string id;
	std::string pw;
	std::string name;
	std::string phone;
	int seatNo;
	bool isBlocked;

public:
	User(std::string id, std::string pw, std::string name, std::string phone)
	{
		this->id = id;
		this->pw = pw;
		this->name = name;
		this->phone = phone;
		this->seatNo = -1;
		this->isBlocked = false;
	}

	void updateUserInfo(std::string id, std::string pw, std::string name, std::string phone)
	{
		this->id = id;
		this->pw = pw;
		this->name = name;
		this->phone = phone;

		return;
	}

	void reserveSeat(int seatNo)
	{
		if (this->seatNo == -1)
		{
			this->seatNo = seatNo;
		}
		else
		{
			std::cout << "Seat already reserved for this user!\n";
		}

		return;
	}

	void extendSeat(Seat *currentSeat, int minutes)
	{
		if (this->seatNo == -1)
		{
			std::cout << "No seat reserved for this user!\n";
		}
		else
		{
			currentSeat->extendSeatTimeRemaining(minutes);
			
			std::cout << "Seat time has been extended by " << minutes << " minutes.\n";
		}

		return;
	}

	void cancelSeat()
	{
		if (this->seatNo == -1)
		{
			std::cout << "No seat reserved for this user!\n";
		}
		else
		{
			this->seatNo = -1;
		}

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

	int getSeatNo()
	{
		return this->seatNo;
	}

	bool getIsBlocked()
	{
		return this->isBlocked;
	}

	~User()
	{
		std::cout << "User " << this->name << " (" << this->phone << ") deleted!\n";
	}
};