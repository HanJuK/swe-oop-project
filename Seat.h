#pragma once

#include <string>
#include <iostream>

class Seat
{
	int seatNo;
	std::string currentSeatUserPhone;
	int timeRemainingInMinutes;
	bool isTemporaryUnavailable;

public:
	Seat(int seatNo)
	{
		this->seatNo = seatNo;
		this->currentSeatUserPhone = "-1";
		this->timeRemainingInMinutes = -1;
		this->isTemporaryUnavailable = false;
	}

	void setCurrentSeatUser(std::string phone)
	{
		this->currentSeatUserPhone = phone;
	}

	void extendSeatTimeRemaining(int minutes)
	{
		this->timeRemainingInMinutes += minutes;

		return;
	}

	void clearCurrentSeatUser()
	{
		this->currentSeatUserPhone = "-1";
		this->timeRemainingInMinutes = -1;

		return;
	}

	void changeIsTemoprarilyUnavailable(bool isTemporaryUnavailable)
	{
		this->isTemporaryUnavailable = isTemporaryUnavailable;

		return;
	}

	int getSeatNo()
	{
		return this->seatNo;
	}

	std::string getCurrentSeatUserPhone()
	{
		return this->currentSeatUserPhone;
	}

	int getTimeRemainingInMinutes()
	{
		return this->timeRemainingInMinutes;
	}

	~Seat()
	{
		std::cout << "Seat " << this->seatNo << " deleted!\n";
	}
};