
#include "User.h"
#include "Admin.h"
#include "Seat.h"

#include <vector>

std::vector<User *> users;
std::vector<Admin *> admins;
std::vector<Seat *> seats;

// TODO: pts

void initializeWithSampleData()
{
	// initialize seats
	for (int i = 1; i <= 100; ++i)
	{
		seats.push_back(new Seat(i));
	}

	// initialize one admin
	admins.push_back(new Admin("admin", "1234", "HanJu Kim", "01089446650"));

	// initialize users
	users.push_back(new User("id1", "pw1", "Lee", "01011111111"));
	users.push_back(new User("id2", "pw2", "Park", "01022222222"));
	users.push_back(new User("id3", "pw3", "Kim", "01033333333"));
	users.push_back(new User("id4", "pw4", "Choi", "01044444444"));
	users.push_back(new User("id5", "pw5", "Jang", "01055555555"));

	return;
}

// TODO:

void deleteAllObjectsBeforeExit()
{
	// delete users
	for (int i = 0; i < users.size(); ++i)
	{
		delete users[i];
	}

	// delete admins
	for (int i = 0; i < admins.size(); ++i)
	{
		delete admins[i];
	}

	// delete seats
	for (int i = 0; i < seats.size(); ++i)
	{
		delete seats[i];
	}

	return;
}

int main()
{
	// TODO: signal setup for alarm

	initializeWithSampleData();

	for (int i = 0; i < 5; ++i)
	{
		users[i]->printUser();
	}

	while (true)
	{
		std::cout << "options..."; // TODO: update this


	}

	deleteAllObjectsBeforeExit();

	return 0;
}