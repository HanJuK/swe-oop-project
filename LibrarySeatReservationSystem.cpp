#include "User.h"
#include "Admin.h"
#include "Seat.h"

#include <vector>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>

HANDLE gDoneEvent;

int DEF_RES_TIME = 60;

std::vector<User *> users;
std::vector<Admin *> admins;
std::vector<Seat *> seats;

VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired);
void initializeWithSampleData();
bool runMainMenu(User **currentUser, Admin **currentAdmin);
User *signIn();
Admin *signInAsAdmin();
User *createAccount();
bool runUserMenu(User *currentUser);
void showUserInfo(User *currentUser);
Seat *getSeatBySeatNo(int seatNo);
void showSeatStatus();
User *getUserByPhone(std::string phone);
void updateUserInfo(User *currentUser);
void reserveSeat(User *currentUser, int seatNo);
void cancelSeat(User *currentUser);
bool runAdminMenu(Admin *currentAdmin);
void showAdminInfo(Admin *currentAdmin);
void updateAdminInfo(Admin *currentAdmin);
void blockUser(Admin *currentAdmin);
void unblockUser(Admin *currentAdmin);
void makeSeatTemporarilyUnavailable(Admin *currentAdmin);
void makeSeatAvailable(Admin *currentAdmin);
void changeDefaultReservationTime(Admin *currentAdmin);
std::string truncateToLengthEleven(std::string str);
std::string addPaddingToLengthEleven(std::string str);
void deleteAllObjectsBeforeExit();

VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	if (lpParam == NULL)
	{
		printf("TimerRoutine lpParam is NULL\n");
	}
	else
	{
		// lpParam points to the argument; in this case it is an int

		//printf("Timer routine called. Parameter is %d.\n", *(int*)lpParam);
		if(TimerOrWaitFired)
		{
			printf("\n忙式式式式式式式式式式式式式式式式式式式式式式忖\n");
			printf("弛 A minute has passed! 弛\n");
			printf("戌式式式式式式式式式式式式式式式式式式式式式式戎\n");

			for (int i = 0; i < seats.size(); ++i)
			{
				if (seats[i]->getTimeRemainingInMinutes() != -1)
				{
					seats[i]->decrementSeatTimeRemaining();

					if (seats[i]->getTimeRemainingInMinutes() == -1)
					{
						cancelSeat(getUserByPhone(seats[i]->getCurrentSeatUserPhone()));
					}
				}
			}
		}
		else
		{
			printf("The wait event was signaled.\n");
		}
	}

	SetEvent(gDoneEvent);
}

void initializeWithSampleData()
{
	// initialize seats
	for (int i = 1; i <= 30; ++i)
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

	// sample seat reservation for users
	DEF_RES_TIME = 63;
	reserveSeat(users[0], 3);
	DEF_RES_TIME = 24;
	reserveSeat(users[1], 12);
	DEF_RES_TIME = 125;
	reserveSeat(users[2], 25);

	// reset DEF_RES_TIME;
	DEF_RES_TIME = 60;

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

		return true;
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
		else if ((*currentUser)->getIsBlocked() == true)
		{
			*currentUser = nullptr;

			system("cls");

			std::cout << "This account has been blocked!\n\n";
		}
		else // sign in successful
		{
			system("cls");
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
		else // sign in successful
		{
			system("cls");
		}
	}

	// create account
	else if (selection == 3)
	{
		system("cls");

		*currentUser = createAccount();
	}

	// exit
	else if (selection == 4)
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

bool runUserMenu(User *currentUser)
{
	showSeatStatus();

	showUserInfo(currentUser);

	std::cout << "Select an option.\n";
	std::cout << "1. Update user info\n";
	std::cout << "2. Reserve seat\n";
	std::cout << "3. Extend seat\n";
	std::cout << "4. Cancel seat\n";
	std::cout << "5. Sign out\n";
	std::cout << "6. Delete account\n";

	int selection;
	std::cout << ">> ";
	std::cin >> selection;

	if (!(1 <= selection && selection <= 6))
	{
		system("cls");

		std::cout << "Selection must be between 1 and 6!\n";

		return true;
	}

	// update user info
	if (selection == 1)
	{
		system("cls");

		updateUserInfo(currentUser);

		system("cls");

		std::cout << "User info updated!\n";
	}

	// reserve seat
	else if (selection == 2)
	{
		system("cls");

		if (currentUser->getSeatNo() != -1)
		{
			std::cout << "Seat already reserved for this user!\n";
		}
		else
		{
			reserveSeat(currentUser, -1);

			system("cls");

			std::cout << "Seat no." << currentUser->getSeatNo() << " reserved!\n";
		}
	}

	// extend seat
	else if (selection == 3)
	{
		system("cls");

		currentUser->extendSeat(getSeatBySeatNo(currentUser->getSeatNo()), DEF_RES_TIME);
	}

	// cancel seat
	else if (selection == 4)
	{
		system("cls");

		cancelSeat(currentUser);
	}

	// sign out
	else if (selection == 5)
	{
		system("cls");

		return false;
	}

	// delete account
	else if (selection == 6)
	{
		system("cls");

		if (currentUser->getSeatNo() != -1)
		{
			std::cout << "Please cancel your seat first.\n";
		}
		else
		{
			for (int i = 0; i < users.size(); ++i)
			{
				if (users[i] == currentUser)
				{
					users.erase(users.begin() + i);

					break;
				}
			}

			delete currentUser;

			return false;
		}
	}

	return true;
}

void showUserInfo(User *currentUser)
{
	Seat *currentSeat = nullptr;

	currentSeat = getSeatBySeatNo(currentUser->getSeatNo());

	std::cout << " ";
	for (int i = 0; i < 148; ++i)
	{
		std::cout << "式";
	}
	std::cout << "\n\n  ";

	std::cout << "ID: " << currentUser->getId() << "       ";
	std::cout << "Password: " << currentUser->getPw() << "       ";
	std::cout << "Name: " << currentUser->getName() << "       ";
	std::cout << "Phone: " << currentUser->getPhone() << "       ";
	if (currentSeat == nullptr)
	{
		std::cout << "Seat No.: " << "null       ";
	}
	else
	{
		std::cout << "Seat No.: " << currentUser->getSeatNo()
			<< " (" << currentSeat->getTimeRemainingInMinutes() << " min.)       ";
	}
	std::cout << "Is Blocked?: " << currentUser->getIsBlocked() << "\n";

	std::cout << "\n ";
	for (int i = 0; i < 148; ++i)
	{
		std::cout << "式";
	}
	std::cout << "\n";

	std::cout << "\n";
	for (int i = 0; i < 70; ++i)
	{
		std::cout << " ";
	}
	std::cout << "<User Info>\n\n\n";

	return;
}

Seat *getSeatBySeatNo(int seatNo)
{
	for (int i = 0; i < seats.size(); ++i)
	{
		if (seats[i]->getSeatNo() == seatNo)
		{
			return seats[i];
		}
	}
	
	return nullptr;
}

void showSeatStatus()
{
	std::cout << "\n";

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
			if (seats[i * 10 + j]->getIsTemporaryUnavailable() == true)
			{
				std::cout << "弛 "
					<< addPaddingToLengthEleven(std::to_string(seats[i * 10 + j]->getSeatNo())
												+ " (X)")
					<< " 弛";
			}
			else
			{
				std::cout << "弛 "
					<< addPaddingToLengthEleven(std::to_string(seats[i * 10 + j]->getSeatNo()))
					<< " 弛";
			}
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
			if (seats[(seats.size() / 10) * 10 + j]->getIsTemporaryUnavailable() == true)
			{
				std::cout << "弛 "
					<< addPaddingToLengthEleven(std::to_string(seats[(seats.size() / 10) * 10 + j]->getSeatNo())
												+ " (X)")
					<< " 弛";
			}
			else
			{
				std::cout << "弛 "
					<< addPaddingToLengthEleven(std::to_string(seats[(seats.size() / 10) * 10 + j]->getSeatNo()))
					<< " 弛";
			}
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

	std::cout << "\n";
	for (int i = 0; i < 69; ++i) // nice
	{
		std::cout << " ";
	}
	std::cout << "<Seat Status>\n\n\n";

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

void updateUserInfo(User *currentUser)
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

	currentUser->updateUserInfo(id, pw, name, phone);

	if (currentUser->getSeatNo() != -1)
	{
		getSeatBySeatNo(currentUser->getSeatNo())->setCurrentSeatUser(currentUser->getPhone());
	}

	return;
}

void reserveSeat(User *currentUser, int seatNo)
{
	while (true)
	{
		if (seatNo != -1)
		{
			break;
		}

		std::cout << "Enter seat no.: ";
		std::cin >> seatNo;

		if (getSeatBySeatNo(seatNo) == nullptr)
		{
			std::cout << "No such seat!\n";

			seatNo = -1;
		}

		else if (getSeatBySeatNo(seatNo)->getIsTemporaryUnavailable() == true)
		{
			std::cout << "This seat is temporary unavailable!\n";

			seatNo = -1;
		}

		else if (getSeatBySeatNo(seatNo)->getTimeRemainingInMinutes() != -1)
		{
			std::cout << "This seat is already taken!\n";

			seatNo = -1;
		}
	}

	currentUser->reserveSeat(seatNo);

	getSeatBySeatNo(currentUser->getSeatNo())->setCurrentSeatUser(currentUser->getPhone());
	getSeatBySeatNo(currentUser->getSeatNo())->extendSeatTimeRemaining(1 + DEF_RES_TIME);

	return;
}

void cancelSeat(User *currentUser)
{
	if (currentUser->getSeatNo() == -1)
	{
		std::cout << "No seat reserved for this user!\n";

		return;
	}

	getSeatBySeatNo(currentUser->getSeatNo())->clearCurrentSeatUser();

	currentUser->cancelSeat();

	return;
}

bool runAdminMenu(Admin *currentAdmin)
{
	showSeatStatus();

	showAdminInfo(currentAdmin);

	std::cout << "Select an option.\n";
	std::cout << "1. Update admin info\n";
	std::cout << "2. View all user info\n";
	std::cout << "3. Block user\n";
	std::cout << "4. Unblock user\n";
	std::cout << "5. Add seat\n";
	std::cout << "6. Remove seat\n";
	std::cout << "7. Make seat temporarily unavailable\n";
	std::cout << "8. Make seat available\n";
	std::cout << "9. Change default reservation time\n";
	std::cout << "10. Sign out\n";
	std::cout << "11. Delete account\n";

	int selection;
	std::cout << ">> ";
	std::cin >> selection;

	if (!(1 <= selection && selection <= 11))
	{
		system("cls");

		std::cout << "Selection must be between 1 and 11!\n\n";

		return true;
	}

	// update admin info
	if (selection == 1)
	{
		system("cls");

		updateAdminInfo(currentAdmin);

		system("cls");

		std::cout << "Admin info updated!\n";
	}

	// view all user info
	else if (selection == 2)
	{
		system("cls");

		currentAdmin->viewAllUserInfo(&users, showUserInfo);
	}

	// block user
	else if (selection == 3)
	{
		system("cls");

		blockUser(currentAdmin);
	}

	// unblock user
	else if (selection == 4)
	{
		system("cls");

		unblockUser(currentAdmin);
	}

	// add seat
	else if (selection == 5)
	{
		system("cls");

		currentAdmin->addSeat(&seats);
	}

	// remove seat
	else if (selection == 6)
	{
		system("cls");

		currentAdmin->removeSeat(&seats);
	}

	// make seat temporarily unavailable
	else if (selection == 7)
	{
		system("cls");

		makeSeatTemporarilyUnavailable(currentAdmin);
	}

	// make seat available
	else if (selection == 8)
	{
		system("cls");

		makeSeatAvailable(currentAdmin);
	}

	// change default reservation time
	else if (selection == 9)
	{
		system("cls");

		changeDefaultReservationTime(currentAdmin);
	}

	// sign out
	else if (selection == 10)
	{
		system("cls");

		return false;
	}

	// delete account
	else if (selection == 11)
	{
		system("cls");

		for (int i = 0; i < admins.size(); ++i)
		{
			if (admins[i] == currentAdmin)
			{
				admins.erase(admins.begin() + i);

				break;
			}
		}

		delete currentAdmin;

		return false;
	}

	return true;
}

void showAdminInfo(Admin *currentAdmin)
{
	std::cout << " ";
	for (int i = 0; i < 148; ++i)
	{
		std::cout << "式";
	}
	std::cout << "\n\n  ";

	std::cout << "ID: " << currentAdmin->getId() << "       ";
	std::cout << "Password: " << currentAdmin->getPw() << "       ";
	std::cout << "Name: " << currentAdmin->getName() << "       ";
	std::cout << "Phone: " << currentAdmin->getPhone() << "\n";

	std::cout << "\n ";
	for (int i = 0; i < 148; ++i)
	{
		std::cout << "式";
	}
	std::cout << "\n";

	std::cout << "\n";
	for (int i = 0; i < 69; ++i)
	{
		std::cout << " ";
	}
	std::cout << "<Admin Info>\n\n\n";

	return;
}

void updateAdminInfo(Admin *currentAdmin)
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

	currentAdmin->updateAdminInfo(id, pw, name, phone);

	return;
}

void blockUser(Admin *currentAdmin)
{
	std::string phone;

	std::cout << "Phone number of user to block: ";
	std::cin >> phone;

	currentAdmin->blockUser(phone, getUserByPhone, getSeatBySeatNo);

	return;
}

void unblockUser(Admin *currentAdmin)
{
	std::string phone;

	std::cout << "Phone number of user to unblock: ";
	std::cin >> phone;

	currentAdmin->unblockUser(phone, getUserByPhone);

	return;
}

void makeSeatTemporarilyUnavailable(Admin *currentAdmin)
{
	int seatNo;

	std::cout << "Enter seat no.: ";
	std::cin >> seatNo;

	if (getSeatBySeatNo(seatNo) == nullptr)
	{
		system("cls");

		std::cout << "No such seat!\n";
	}
	else if (getSeatBySeatNo(seatNo)->getTimeRemainingInMinutes() != -1)
	{
		system("cls");

		std::cout << "Cannot make seat temporarily unavailable! (currently occupied)!\n";
	}
	else
	{
		currentAdmin->makeSeatTemporaryUnavailable(getSeatBySeatNo(seatNo));

		system("cls");

		std::cout << "Made seat no." << seatNo << " temporary unavailable!\n";
	}

	return;
}

void makeSeatAvailable(Admin *currentAdmin)
{
	int seatNo;

	std::cout << "Enter seat no.: ";
	std::cin >> seatNo;

	if (getSeatBySeatNo(seatNo) == nullptr)
	{
		system("cls");

		std::cout << "No such seat!\n";
	}
	else
	{
		currentAdmin->makeSeatAvailable(getSeatBySeatNo(seatNo));

		system("cls");

		std::cout << "Made seat no." << seatNo << " available!\n";
	}

	return;
}

void changeDefaultReservationTime(Admin *currentAdmin)
{
	int minutes;

	std::cout << "Enter new default reservation time in minutes: ";
	std::cin >> minutes;

	currentAdmin->changeDefaultReservationTime(&DEF_RES_TIME, minutes);

	system("cls");

	std::cout << "Default reservation time changed to " << DEF_RES_TIME << " minutes!\n";

	return;
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
	HANDLE hTimer = NULL;
	HANDLE hTimerQueue = NULL;
	int arg = 123;

	// Use an event object to track the TimerRoutine execution
	gDoneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == gDoneEvent)
	{
		printf("CreateEvent failed (%d)\n", GetLastError());
		return 1;
	}

	// Create the timer queue.
	hTimerQueue = CreateTimerQueue();
	if (NULL == hTimerQueue)
	{
		printf("CreateTimerQueue failed (%d)\n", GetLastError());
		return 2;
	}

	// Set a timer to call the timer routine in 10 seconds.
	if (!CreateTimerQueueTimer( &hTimer, hTimerQueue, 
							   (WAITORTIMERCALLBACK)TimerRoutine, &arg , 10000, 10000, 0))
	{
		printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
		return 3;
	}

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
			while (runUserMenu(currentUser)) {}
		}
		
		// admin sign in
		else if (currentUser == nullptr && currentAdmin != nullptr)
		{
			while (runAdminMenu(currentAdmin)) {}
		}
	}

	deleteAllObjectsBeforeExit();

	// Wait for the timer-queue thread to complete using an event 
	// object. The thread will signal the event at that time.

	if (WaitForSingleObject(gDoneEvent, INFINITE) != WAIT_OBJECT_0)
		printf("WaitForSingleObject failed (%d)\n", GetLastError());

	CloseHandle(gDoneEvent);

	// Delete all timers in the timer queue.
	if (!DeleteTimerQueue(hTimerQueue))
		printf("DeleteTimerQueue failed (%d)\n", GetLastError());

	return 0;
}