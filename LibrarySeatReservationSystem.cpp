#include "User.h"
#include "Admin.h"
#include "Seat.h"

#include <vector>
#include <stdlib.h>

std::vector<User *> users;
std::vector<Admin *> admins;
std::vector<Seat *> seats;

void initializeWithSampleData();
bool runMainMenu(User **currentUser, Admin **currentAdmin);
User *signIn();
Admin *signInAsAdmin();
User *createAccount();
bool runUserMenu();
void showSeatStatus();
User *getUserByPhone(std::string phone);
std::string truncateToLengthEleven(std::string str);
std::string addPaddingToLengthEleven(std::string str);
void deleteAllObjectsBeforeExit();
// TODO: pts

void initializeWithSampleData()
{
	// TODO: set default seat reservation time

	// initialize seats
	for (int i = 1; i <= 98; ++i) // TODO: change to 100 seats
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

	// TODO: sample seat reservation for users
	// temp
	seats[3]->setCurrentSeatUser("01011111111");
	seats[3]->extendSeatTimeRemaining(60 + 1);
	seats[46]->setCurrentSeatUser("01022222222");
	seats[46]->extendSeatTimeRemaining(60 + 1);
	seats[93]->setCurrentSeatUser("01033333333");
	seats[93]->extendSeatTimeRemaining(60 + 1);
	// temp

	return;
}

bool runMainMenu(User **currentUser, Admin **currentAdmin)
{
	std::cout << "Select an option.\n";
	std::cout << "1. Sign in\n";
	std::cout << "2. Sign in (Administrator)\n";
	std::cout << "3. Create account\n";
	std::cout << "4. Exit\n";

	int selection;
	std::cout << ">> ";
	std::cin >> selection;

	if (!(1 <= selection && selection <= 4))
	{
		system("cls");

		std::cout << "Selection must be between 1 and 4!\n\n";
	}

	// sign in
	if (selection == 1)
	{
		system("cls");

		*currentUser = signIn();

		if (*currentUser == nullptr)
		{
			system("cls");

			std::cout << "Sign in failed!\n\n";
		}
	}

	// sign in (admin)
	else if (selection == 2)
	{
		system("cls");

		*currentAdmin = signInAsAdmin();

		if (*currentAdmin == nullptr)
		{
			system("cls");

			std::cout << "Sign in failed!\n\n";
		}
	}

	// create account
	else if (selection == 3)
	{
		system("cls");

		*currentUser = createAccount();
	}

	// exit
	else
	{
		return false;
	}

	return true;
}

User *signIn()
{
	std::string id, pw;

	std::cout << "ID: ";
	std::cin >> id;
	std::cout << "Password: ";
	std::cin >> pw;

	for (int i = 0; i < users.size(); ++i)
	{
		if (users[i]->getId() == id && users[i]->getPw() == pw)
		{
			return users[i];
		}
	}

	return nullptr;
}

Admin *signInAsAdmin()
{
	std::string id, pw;

	std::cout << "ID: ";
	std::cin >> id;
	std::cout << "Password: ";
	std::cin >> pw;

	for (int i = 0; i < admins.size(); ++i)
	{
		if (admins[i]->getId() == id && admins[i]->getPw() == pw)
		{
			return admins[i];
		}
	}

	return nullptr;
}

User *createAccount()
{
	std::string id, pw, name, phone;

	std::cout << "ID: ";
	std::cin >> id;
	std::cout << "Password: ";
	std::cin >> pw;
	std::cout << "Name: ";
	std::cin >> name;
	std::cout << "Phone: ";
	std::cin >> phone;

	// TODO: phone number duplicate check

	users.push_back(new User(id, pw, name, phone));

	return users[users.size() - 1];
}

bool runUserMenu()
{
	showSeatStatus();

	// TODO:

	return true;
}

void showSeatStatus()
{
	// for each row...
	for (int i = 0; i < seats.size() / 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			std::cout << "忙式式式式式式式式式式式式式忖";
		}

		std::cout << "\n";

		// seat no.
		for (int j = 0; j < 10; ++j)
		{
			std::cout << "弛 "
				<< addPaddingToLengthEleven(std::to_string(seats[i * 10 + j]->getSeatNo()))
				<< " 弛";
		}

		std::cout << "\n";

		// name
		for (int j = 0; j < 10; ++j)
		{
			User *currentUser = getUserByPhone(seats[i * 10 + j]->getCurrentSeatUserPhone());

			if (currentUser == nullptr)
			{
				std::cout << "弛             弛";
			} else
			{
				std::cout << "弛 "
					<< addPaddingToLengthEleven(truncateToLengthEleven(currentUser->getName()))
					<< " 弛";
			}
		}

		std::cout << "\n";

		// phone
		for (int j = 0; j < 10; ++j)
		{
			User *currentUser = getUserByPhone(seats[i * 10 + j]->getCurrentSeatUserPhone());

			if (currentUser == nullptr)
			{
				std::cout << "弛             弛";
			} else
			{
				std::cout << "弛 "
					<< addPaddingToLengthEleven(truncateToLengthEleven(currentUser->getPhone()))
					<< " 弛";
			}
		}

		std::cout << "\n";

		// time remaining
		for (int j = 0; j < 10; ++j)
		{
			if (seats[i * 10 + j]->getTimeRemainingInMinutes() == -1)
			{
				std::cout << "弛             弛";
			} else
			{
				std::cout << "弛 "
					<< addPaddingToLengthEleven(truncateToLengthEleven(
						std::to_string(seats[i * 10 + j]->getTimeRemainingInMinutes()) + " min."))
					<< " 弛";
			}
		}

		std::cout << "\n";

		for (int j = 0; j < 10; ++j)
		{
			std::cout << "戌式式式式式式式式式式式式式戎";
		}

		std::cout << "\n";
	}

	// for the last row
	if (seats.size() % 10 != 0)
	{
		for (int j = 0; j < seats.size() % 10; ++j)
		{
			std::cout << "忙式式式式式式式式式式式式式忖";
		}

		std::cout << "\n";

		// seat no.
		for (int j = 0; j < seats.size() % 10; ++j)
		{
			std::cout << "弛 "
				<< addPaddingToLengthEleven(std::to_string(seats[(seats.size() / 10) * 10 + j]->getSeatNo()))
				<< " 弛";
		}

		std::cout << "\n";

		// name
		for (int j = 0; j < seats.size() % 10; ++j)
		{
			User *currentUser = getUserByPhone(seats[(seats.size() / 10) * 10 + j]->getCurrentSeatUserPhone());

			if (currentUser == nullptr)
			{
				std::cout << "弛             弛";
			}
			else
			{
				std::cout << "弛 "
					<< addPaddingToLengthEleven(truncateToLengthEleven(currentUser->getName()))
					<< " 弛";
			}
		}

		std::cout << "\n";

		// phone
		for (int j = 0; j < seats.size() % 10; ++j)
		{
			User *currentUser = getUserByPhone(seats[(seats.size() / 10) * 10 + j]->getCurrentSeatUserPhone());

			if (currentUser == nullptr)
			{
				std::cout << "弛             弛";
			}
			else
			{
				std::cout << "弛 "
					<< addPaddingToLengthEleven(truncateToLengthEleven(currentUser->getPhone()))
					<< " 弛";
			}
		}

		std::cout << "\n";

		// time remaining
		for (int j = 0; j < seats.size() % 10; ++j)
		{
			if (seats[(seats.size() / 10) * 10 + j]->getTimeRemainingInMinutes() == -1)
			{
				std::cout << "弛             弛";
			}
			else
			{
				std::cout << "弛 "
					<< addPaddingToLengthEleven(truncateToLengthEleven(
						std::to_string(seats[(seats.size() / 10) * 10 + j]->getTimeRemainingInMinutes()) + " min."))
					<< " 弛";
			}
		}

		std::cout << "\n";

		for (int j = 0; j < seats.size() % 10; ++j)
		{
			std::cout << "戌式式式式式式式式式式式式式戎";
		}

		std::cout << "\n";
	}

	return;
}

User *getUserByPhone(std::string phone)
{
	for (int i = 0; i < users.size(); ++i)
	{
		if (users[i]->getPhone() == phone)
		{
			return users[i];
		}
	}

	return nullptr;
}

std::string truncateToLengthEleven(std::string str)
{
	if (str.length() <= 11)
	{
		return str;
	}

	return str.substr(0, 8) + "...";
}

std::string addPaddingToLengthEleven(std::string str)
{
	int initLength = str.length();

	for (int i = 0; i < 11 - initLength; ++i)
	{
		str += " ";
	}

	return str;
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

	while (true)
	{
		// reset sign in (pre - sign in)
		User *currentUser = nullptr;
		Admin *currentAdmin = nullptr;

		// main menu
		if (runMainMenu(&currentUser, &currentAdmin) == false)
		{
			break;
		}

		// sign in fail
		if (currentUser == nullptr && currentAdmin == nullptr)
		{
			continue;
		}

		// user sign in
		else if (currentUser != nullptr && currentAdmin == nullptr)
		{
			std::cout << "User sign in!\n"; // TODO: remove
			runUserMenu();
		}
		
		// admin sign in
		else if (currentUser == nullptr && currentAdmin != nullptr)
		{
			std::cout << "Admin sign in!\n"; // TODO: remove
		}
	}

	deleteAllObjectsBeforeExit();

	return 0;
}