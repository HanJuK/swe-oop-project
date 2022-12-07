#pragma once

#include "User.h"
#include "Seat.h"

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

	void viewAllUserInfo(std::vector<User *> *users, void (*showUserInfo)(User *user))
	{
		for (int i = 0; i < (*users).size(); ++i)
		{
			showUserInfo((*users)[i]);
		}

		return;
	}

	void blockUser(std::string phone, User *(*getUserByPhone)(std::string phone),
				   Seat *(*getSeatBySeatNo)(int seatNo))
	{
		User *userToBlock = nullptr;

		userToBlock = getUserByPhone(phone);

		if (userToBlock == nullptr)
		{
			std::cout << "No such user!\n";
		}
		else
		{
			// first, cancel their seat
			if (userToBlock->getSeatNo() != -1)
			{
				getSeatBySeatNo(userToBlock->getSeatNo())->clearCurrentSeatUser();

				userToBlock->cancelSeat();
			}

			// then, block them
			userToBlock->setIsBlocked(true);
		}

		return;
	}

	void unblockUser(std::string phone, User *(*getUserByPhone)(std::string phone))
	{
		User *userToUnblock = nullptr;

		userToUnblock = getUserByPhone(phone);

		if (userToUnblock == nullptr)
		{
			std::cout << "No such user!\n";
		}
		else
		{
			userToUnblock->setIsBlocked(false);
		}

		return;
	}

	void addSeat(std::vector<Seat *> *seats)
	{
		(*seats).push_back(new Seat((*seats).size() + 1));

		return;
	}

	void removeSeat(std::vector<Seat *> *seats)
	{
		if ((*((*seats).end() - 1))->getTimeRemainingInMinutes() != -1)
		{
			std::cout << "Cannot remove seat! (currently occupied)\n";
		}
		else
		{
			Seat *temp = *((*seats).end() - 1);

			(*seats).pop_back();

			delete temp;
		}

		return;
	}

	void makeSeatTemporaryUnavailable(Seat *seat)
	{
		seat->changeIsTemoprarilyUnavailable(true);

		return;
	}

	void makeSeatAvailable(Seat *seat)
	{
		seat->changeIsTemoprarilyUnavailable(false);

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