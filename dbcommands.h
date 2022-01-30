#pragma once
#include<fstream>
#include<Windows.h>
bool checkLogged();
void setLogged(bool o);
class DBmanager{
public:
	std::string name;
	std::ifstream in;
	std::ofstream out;
	DBmanager(std::string name) {
		this->name = name;
	}
	void openDatabase(char open_operation);		//n - get elements and add alements to db, f - !CLEARS DB! get elements and save elements but first make db empty
	bool isOpened();
	void closeDatabase();
	void createDatabase();
};

class Menu{
public:
	void clear();
	int lMenu();
	int nMenu();
	void header();
	void lHeader();
	int showMenu();
};

class User {
public:
	std::pair<std::string, std::string> name;
private:
	std::string PIN;
	std::string bday;
	std::string email;
	std::string password;
	std::string accNum;
public:
	struct UserStruct
	{
		std::pair<std::string, std::string> name; std::string em, password, PIN, bday, accNum;
	} findUser(std::string email) {
			DBmanager newDB("users_DB.txt");
			newDB.openDatabase('n');
			std::string cN, cSn, cP, cE, cpin, cdata, cACN;
			if (newDB.isOpened()) {
				while (newDB.in >> cN >> cSn >> cE >> cP >> cpin >> cdata >> cACN) {
					if (email == cE) {
						newDB.closeDatabase();
						return { std::make_pair(cN, cSn), cE, cP, cpin, cdata, cACN };
					}
				}
			}
			newDB.closeDatabase();
			return{ {"no_user", "no_user"}, "no_user" , "no_user", "no_user", "no_user", "no_user" };
	}
	void setUser(std::pair<std::string, std::string> name, std::string email, std::string password, std::string PIN, std::string bday, std::string accNum);
	void login();
	//void w_login();
	void reg();
	void accInf(bool flag);
	void showSpendingH();
	void makeTransfer();
	void showAccBil();
	void withdraw();
	void insert();
	bool checkSession();
};