#include <iostream>
#include <fstream>
#include "dbcommands.h"
using namespace std;

int main() {
	Menu newMenu;
	User user;
	bool running = true;
	while (running) {
		newMenu.clear();
		int option = newMenu.showMenu();
		if (checkLogged()) {
			if (option == 1)user.insert();
			if (option == 2)user.withdraw();
			if (option == 3)user.makeTransfer();
			if (option == 4)user.showAccBil();
			if (option == 5)user.showSpendingH();
			if (option == 6)user.accInf(false);
			if (option == 7)setLogged(0);
			if (option == 8)break;
		}
		else {
			if (option == 1)user.login();
			if (option == 2)user.reg();
			if (option == 3)break;
		}
	}
	newMenu.clear();
	cout << "\n\nThanks for using my ATM System!!\nAuthor: Michal Wesolowski\n\n";
	system("pause");
}
