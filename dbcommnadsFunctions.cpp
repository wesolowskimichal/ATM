#include<fstream>
#include<iostream>
#include<Windows.h>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>
#include "dbcommands.h"

void setLogged(bool o) {
	std::ofstream l("logged.txt");
	l << (int)o;
	l.close();
}

bool checkLogged() {
	std::string LBD = "logged.txt";		//name of file which conatins result if user is logged in
	std::ifstream plik(LBD);
	int n;
	if (plik.good()) {
		plik >> n;
		if (n == 1)return true;
		else return false;
	}
	else std::cout << "\n\n\t\t!FATAL ERROR!\n";
	plik.close();
	return false;
}

bool verify_date(std::string date) {
	if (date.size() != 10)return false;
	if (date[2] != '/')return false;
	if (date[5] != '/')return false;
	int n{};
	n = date[1] - '0';
	n += (date[0] - '0') * 10;
	if (n > 31 || n <= 0) return false;
	n = date[4] - '0';
	n += (date[3] - '0') * 10;
	if (n > 12 || n <= 0)return false;
	return true;
}

bool verify_email(std::string mail) {
	int x{};
	int h{}, id;
	for (auto c : mail) {
		if (c == '@') {
			x++; id = h;
		}
		h++;
	}
	if (x != 1) return false;
	x = 0;
	std::string::iterator it;
	for (it = mail.begin() + id; it != mail.end(); it++) {
		if (*it == '.')x++;
	}
	if (x == 0)return false;
	return true;
}

bool verify_password(std::string pass) {
	int special{}, sm{}, capital{}, number{};
	for (auto c : pass) {
		if (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&' || c == '*' || c == '(' || c == ')' || c == '+' || c == '_' || c == '[' || c == ']' || c == '{' || c == '}' || c == ';' || c == ':' || c == '\'' || c == '\"' || c == '.' || c == ',')special++;
		if ((int)c >= 65 && (int)c <= 90)capital++;
		if ((int)c >= 97 && (int)c <= 122)sm++;
		if ((int)c >= 48 && (int)c <= 57)number++;
	}
	if (pass.length() < 12)return false;
	if (capital == 0)return false;
	if (special == 0)return false;
	if (sm == 0)return false;
	if (number == 0)return false;
}

bool verify_pin(std::string pin) {
	if (pin.length() != 4)return false;
	for (auto c : pin)
		if (isdigit(c))
			return false;
	return true;
}

std::string genAccNum() {
	std::string cN, cSn, cP, cE, cpin, cdata, accNum;
	double currNum{};
	DBmanager lastNum("users_DB.txt");
	lastNum.openDatabase('n');
	while (lastNum.in >> cN >> cSn >> cP >> cE >> cpin >> cdata >> accNum) {}
	for (int i = 10; i >= 0; i--) {
		currNum += (accNum[i] - '0') * pow(10, std::abs(i - 10));
	}
	lastNum.closeDatabase();
	currNum += 1;
	std::string s = "";
	for (int i = 0; i < 11; i++) {
		s += (std::fmod(currNum, 10)) + '0';
		currNum /= 10;
	}
	std::reverse(s.begin(), s.end());
	return s;
}

void DBmanager::openDatabase(char open_operation) {
	in.open(name);
	if (open_operation == 'n') out.open(name, std::ios::app);
	else out.open(name);
	
	if (!in.good()) {
		std::cout << "\n\n\t\t!CANNOT OPEN DATABASE!\n\t\tSugested solution: CHECK NAME AND PATH\nCurrent path: " << name << '\n';
		in.close();
	}
}

void DBmanager::createDatabase() {
	out.open(name);
}


bool DBmanager::isOpened() {
	if (in.is_open() || out.is_open())return true;
	return false;
}

void DBmanager::closeDatabase() {
	if (isOpened()) {
		in.close();
		out.close();
	}
	else {
		std::cout << "\n\n\tDatabase is not openned\n";
	}
}



void Menu::clear() {
	system("cls");
}

int Menu::nMenu() {
	std::cout << "1) Log In\n2) Register\n3) Turn off\n";
	char c;
	std::cin >> c;
	if ((int)c >= 49 && (int)c <= 51)return c - '0';
	return -1;
}

int Menu::lMenu() {
	std::cout << "1) Insert money\n2) Withdraw money\n3) Make transfer\n4) Show account bilance\n5) Show transfer history\n6) Show account informations\n7) Log Out\n8) Turn off\n";
	char c;
	std::cin >> c;
	if ((int)c >= 48 && (int)c <= 57)return c - '0';
	return -1;
}

void Menu::header(){
	for (int i = 0; i < 45; i++)std::cout << '-';
	std::cout << "ATM SYSTEM by Michal Wesolowski";
	for (int i = 0; i < 45; i++)std::cout << '-';
	std::cout << '\n';
}

void Menu::lHeader() {
	for (int i = 0; i < 45; i++)std::cout << '-';
	std::cout << "ATM SYSTEM by Michal Wesolowski";
	for (int i = 0; i < 45; i++)std::cout << '-';
	DBmanager getSession("logged.txt");
	getSession.openDatabase('n');
	std::string temp;
	for (int i = 0; i < 2; i++) {
		getSession.in >> temp;
	}
	getSession.closeDatabase();
	std::cout << "\n\t\tAccount: "<<temp<< '\n';
}

int Menu::showMenu() {
	if (checkLogged()) {
		lHeader();
		return lMenu();
	}
	else {
		header();
		return nMenu();
	}
}

void User::login() {
	Menu newMenu;
	newMenu.clear();
	newMenu.header();
	bool wlogin = false;
	std::cout << "Log In: \n";
	DBmanager login_db("users_DB.txt");
	login_db.openDatabase('n');
	std::string fname, sname;
	std::string cN, cSn, cP, cE, pin, data, cACN;
	std::cout << "Username: ";
	std::cin >> fname >> sname;
	std::cout << "Email: ";
	std::cin >> email;
	std::cout << "Password: ";
	std::cin >> password;
	if (login_db.isOpened()) {

		while (login_db.in >> cN >> cSn >> cE >> cP >> pin >> data >> cACN) {
			if (fname == cN && sname == cSn && email == cE && password == cP) {
				setLogged(true);
			}
		}
		if (checkLogged() == false)
			wlogin = true;

		if (wlogin) {
			login_db.closeDatabase();
			while (wlogin){
				newMenu.clear();
				newMenu.header();
				std::cout << "Wrong username or password!\n";
				std::cout << "Log In: \n";
				DBmanager login_db("users_DB.txt");
				login_db.openDatabase('n');
				std::cout << "Full name: ";
				std::cin >> fname >> sname;
				std::cout << "Email: ";
				std::cin >> email;
				std::cout << "Password: ";
				std::cin >> password;
				while (login_db.in >> cN >> cSn >> cE >> cP >> pin >> data >> cACN) {
					if (fname == cN && sname == cSn && email == cE && password == cP) {
						setLogged(true);
						wlogin = false;
						break;
					}
				}
			}
		}
		if (!wlogin) {
			login_db.closeDatabase();
			DBmanager newSession("logged.txt");
			newSession.openDatabase('n');
			newSession.out << '\n' << email;
			newSession.closeDatabase();
			setUser(std::make_pair(fname, sname), email, password, pin, data, cACN);
		}
	}
	login_db.closeDatabase();
}

//void User::w_login() {
//	Menu newMenu;
//	newMenu.clear();
//	newMenu.header();
//	std::cout << "Wrong username or password!\n";
//	std::cout << "Log In: \n";
//	DBmanager login_db("users_DB.txt");
//	login_db.openDatabase('n');
//	std::string fname, sname;
//	std::string cN, cSn, cP, cE, pin, data, cACN;
//	std::cout << "Username: ";
//	std::cin >> fname >> sname;
//	name.first = fname;
//	name.second = sname;
//	std::cout << "Email: ";
//	std::cin >> email;
//	std::cout << "Password: ";
//	std::cin >> password;
//	bool good = false;
//	if (login_db.isOpened()) {
//
//		while (login_db.in >> cN >> cSn >> cE >> cP >> pin >> data >> cACN) {
//			if (name.first == cN && name.second == cSn && email == cE && password == cP) {
//				setLogged(true);
//				good = true;
//			}
//		}
//		
//		if (!good) {
//			while (!good) {
//
//			}
//		}
//		if (checkLogged() == false) {
//			login_db.closeDatabase();
//			w_login();
//		}
//	}
//	if (good) {
//		login_db.closeDatabase();
//		DBmanager newSession("logged.txt");
//		newSession.openDatabase('n');
//		newSession.out << '\n' << email;
//		newSession.closeDatabase();
//		setUser(std::make_pair(fname, sname), email, password, pin, data, cACN);
//	}
//	
//}

void User::reg() {
	Menu newMenu;
	newMenu.clear();
	newMenu.header();
	std::string password, fname, sname, email, pin, data, accNum;
	std::cout << "Register:\n";
	std::cout << "Name (eg. John Doe): ";
	std::cin >> fname >> sname;
	std::cout << "Birth date (dd/mm/yyyy): ";
	std::cin >> data;
	if (!verify_date(data)) {
		while (!verify_date(data)) {
			std::cout << "Insert date in proper form!\n";
			std::cout << "Birth date (dd/mm/yyyy): ";
			std::cin >> data;
		}
	}
	std::cout << "Email: ";
	std::cin >> email;
	if (!verify_email(email)) {
		while (!verify_email(email)) {
			std::cout << "Insert proper email!\n";
			std::cout << "Email: ";
			std::cin >> email;
		}
	}
	std::cout << "Password: ";
	std::cin >> password;
	if (!verify_password(password)) {
		while (!verify_password(password)) {
			std::cout << "Insert valid password! (min. length: 12, at least: 1 digit, 1 capital letter, 1 small letter, 1 special character)\n";
			std::cout << "Password: ";
			std::cin >> password;
		}
	}
	std::cout << "PIN (eg. 1234): ";
	std::cin >> pin;
	if (!verify_pin(pin)) {
		while (!verify_pin(pin)) {
			std::cout << "PIN (eg. 1234): ";
			std::cin >> pin;
		}
	}
	//dodanie
	DBmanager login_db("users_DB.txt");
	login_db.openDatabase('n');
	std::string cN, cSn, cP, cE, cpin, cdata, cACN;
	bool flag=true;
	if (login_db.isOpened()) {

		while (login_db.in >> cN >> cSn >> cE >> cP >> cpin >> cdata >> cACN) {
			if (email==cE) {
				std::cout << "Account already exists!\n";
				flag = false;
				system("pause");
				break;
			}
		}
		if (flag) {
			accNum = genAccNum();
			login_db.out << '\n' << fname << '\t' << sname << '\t' << email << '\t' << password << '\t' << pin << '\t' << data << '\t' << accNum;
			std::string bilans = "bilanse/" + email + ".txt";
			DBmanager newBilans(bilans);
			newBilans.createDatabase();
			newBilans.out << 0;
			newBilans.closeDatabase();
			std::cout << "Account creaeted successfully!\nLogin to your account!\n";
			system("pause");
		}
		
	}
	login_db.closeDatabase();
}

void User::setUser(std::pair<std::string, std::string> name, std::string email, std::string password, std::string PIN, std::string bday, std::string accNum) {
	this->name = name;
	this->email = email;
	this->password = password;
	this->PIN = PIN;
	this->bday = bday;
	this->accNum = accNum;
}

bool User::checkSession() {
	if (password.empty())return false;
	return true;
}

struct ACIflag {
	int p = -1;
	int pw = -1;

	void deconstruct() {
		p = -1;
		pw = -1;
	}
}accInfFlag;

void User::accInf(bool flag) {
	Menu newMenu;
	newMenu.clear();
	newMenu.lHeader();
	if (checkSession()) {
		if (!flag)accInfFlag.deconstruct();
		std::string money;
		std::cout << "Money on account: ";
		std::string path("bilanse/" + email + ".txt");
		DBmanager qDB(path);
		qDB.openDatabase('n');
		qDB.in >> money;
		qDB.closeDatabase();
		std::cout << money << '\n';
		std::cout << "Name: " << name.first << ' ' << name.second << '\n';
		std::cout << "Email: " << email << '\n';
		std::cout << "Brith date:" << bday<< '\n';
		std::cout << "Password: " << email << '\n';
		if (accInfFlag.p==-1)std::cout << "PIN: " << "****" << '\n';
		else std::cout <<"PIN: "<< PIN << '\n';
		if (accInfFlag.pw==-1) {
			std::cout << "Password: ";
			for (char c : password)std::cout << '*';
		}
		else std::cout <<"Password: "<< password;
		std::cout << "\n\n";
		if (accInfFlag.pw==-1)std::cout << "1) Show password\n";
		else std::cout<<"1) Hide password\n";
		if (accInfFlag.p==-1)std::cout << "2) Show PIN\n";
		else std::cout << "2) Hide PIN\n";
		std::cout << "3) Return\n";
		char c;
		int o{};
		std::cin >> c;
		if ((int)c >= 49 && (int)c <= 51)o = c - '0';
		switch (o){
		default:
			accInf(true);
			break;
		case 1:
			accInfFlag.pw *= -1;
			accInf(true);
			break;
		case 2:
			accInfFlag.p *= -1;
			accInf(true);
			break;
		case 3:
			system("pause");
			break;
		}
	}
	else {
		std::string email;
		DBmanager get("logged.txt");
		get.openDatabase('n');
		for (int i = 0; i < 2; i++)get.in >> email;
		get.closeDatabase();
		setUser(findUser(email).name, findUser(email).em, findUser(email).password, findUser(email).PIN, findUser(email).bday, findUser(email).accNum);		
		//system("pause");
		accInf(false);
	}
}



void User::showSpendingH() {
	Menu newMenu;
	newMenu.clear();
	newMenu.lHeader();
	if (checkSession()) {
		DBmanager bil("bilanse/" + email+".txt");
		bil.openDatabase('n');
		std::cout << "\n\tTRANSFER HISTORY:";
		std::cout << "\n\n|";

		std::string S{};
		for (int i = 0; i < 12; i++)S += ' ';
		S += "From / To";
		for (int i = 0; i < 12; i++)S += ' ';
		S += '|';
		for (int i = 0; i < 12; i++)S += ' ';
		S += "Amount";
		for (int i = 0; i < 12; i++)S += ' ';
		S += '|';
		for (int i = 0; i < 12; i++)S += ' ';
		S += "Description";
		for (int i = 0; i < 12; i++)S += ' ';
		S += '|';

		std::cout << S << '\n';
		for (int i = 0; i < S.length(); i++)std::cout << '-';
		std::cout << '\n';

		std::string temp{};
		bil.in >> temp;
		int t{}, X{}, copyX{};

		while (bil.in >> temp) {
			int LEN;
			copyX = X;

			if (t == 0)
				std::cout << '|';

			while (S[X] != '|')
				X++;

			LEN = X - copyX - temp.length();
			X++;

			for (int j = 0; j < LEN / 2; j++) 
				std::cout << ' ';
			
			//std::cout << temp;
			for (auto ch : temp) {
				if (ch == '`')
					std::cout << ' ';
				else
					std::cout << ch;
			}

			for (int j = 0; j < LEN / 2; j++) 
				std::cout << ' ';
			

			if (LEN % 2 == 0)
				std::cout << '|';
			else
				std::cout << " |";

			t++;

			if (t == 3) {
				std::cout << '\n';
				for (int i = 0; i < S.length(); i++)std::cout << '-';
				std::cout << '\n';
				t = 0;
				X = 0;
			}
		}
		bil.closeDatabase();
		system("pause");
	}
	else {
		std::string email;
		DBmanager get("logged.txt");
		get.openDatabase('n');
		for (int i = 0; i < 2; i++)get.in >> email;
		get.closeDatabase();
		DBmanager temp("users_DB.txt");
		setUser(findUser(email).name, findUser(email).em, findUser(email).password, findUser(email).PIN, findUser(email).bday, findUser(email).accNum);
		showSpendingH();
	}
}

void User::makeTransfer() {
	Menu newMenu;
	newMenu.clear();
	newMenu.lHeader();
	if (checkSession()) {
		std::cout << "MAKE A TRANSFER\n\n";
		std::string odbiorca;
		bool running = true;
		int x{};
		std::cout << "Insert person's email or account number (q -> exit): ";
		std::cin >> odbiorca;
		if (odbiorca == "q")
			running = false;
		for (char c : odbiorca)
			if (c == '@')
				x++;
		if (x != 0 && running) {
			if (!verify_email(odbiorca)) {
				while (!verify_email(odbiorca)) {
					std::cout << "Email is not valid!\n";
					std::cout << "\nInsert proper email (q -> exit): ";
					std::cin >> odbiorca;
					if (odbiorca == "q") {
						running = false;
						break;
					}
				}
			}
			if (findUser(odbiorca).PIN == "no_user" && running) {
				while (findUser(odbiorca).PIN == "no_user") {
					std::cout << "User with this email is not in this bank!\n";
					std::cout << "\nInsert proper email (q -> exit): ";
					std::cin >> odbiorca;
					if (odbiorca == "q") {
						running = false;
						break;
					}
				}
			}
			if (running) {

				DBmanager bilO("bilanse/" + odbiorca + ".txt");
				DBmanager bil("bilanse/" + email + ".txt");
				bilO.openDatabase('n');
				bil.openDatabase('n');
				double money, moneyO, amount;
				bil.in >> money;
				bilO.in >> moneyO;
				std::cout << "Insert amount of money: ";
				std::cin >> amount;
				if (money < amount || amount < 0) {
					while (money < amount || amount < 0)
					{
						if (money < amount)
							std::cout << "You do not have that amount of money on your account!\n";
						if (amount < 0)
							std::cout << "You cannot make a minus transfer!\n";
						std::cout << "Insert other amount (0 -> exit): ";
						std::cin >> amount;
						if (amount == 0) {
							running = false;
							break;
						}

					}
				}
				if (running) {
					money -= amount;
					moneyO += amount;
					std::vector<std::string>V;
					std::string line;
					char c;
					int t{};
					std::string desc{};
					std::cout << "Insert transfer's desription (optional) (max. 35 characters): ";
					std::cin.ignore();
					do
					{
						c = std::getchar();
						if (c == ' ')
							c = '`';
						desc += c;
						t++;
					} while (c != '\n' && c != EOF && t < 35);
					desc.erase(desc.end() - 1);
					if (desc.size() == 0)desc = "------";
					while (std::getline(bil.in, line))
						V.push_back(line);

					bil.closeDatabase();
					DBmanager bil("bilanse/" + email + ".txt");
					bil.openDatabase('f');
					bil.out << std::fixed << std::setprecision(2) << money << '\n';
					bil.out << odbiorca << "\t-" << amount << '\t' << desc << '\n';
					for (auto s : V)
						bil.out << s << '\n';
					bil.closeDatabase();
					V.clear();
					while (std::getline(bilO.in, line))
						V.push_back(line);
					bilO.closeDatabase();
					DBmanager bilO("bilanse/" + odbiorca + ".txt");
					bilO.openDatabase('f');
					bilO.out << std::fixed << std::setprecision(2) << moneyO << '\n';
					bilO.out << email << "\t+" << amount << '\t' << desc << '\n';
					for (auto s : V)
						bilO.out << s << '\n';
					bilO.closeDatabase();
					std::cout << "\n\nTransfer made positively!\n";
				}
			}

		}
		if (x == 0 && running) {
			int digits{};
			for (char c : odbiorca)
				if (std::isdigit(c))
					digits++;
			if (odbiorca.length() != 11 && digits != 11) {
				while (odbiorca.length() != 11 && digits != 11) {
					std::cout << "Wrong account number!\n";
					std::cout << "Insert proper account number (q -> quit): ";
					std::cin >> odbiorca;
					if (odbiorca == "q") {
						running = false;
						break;
					}
					else {
						digits = 0;
						for (char c : odbiorca)
							if (std::isdigit(c))
								digits++;
					}
				}
			}
			if (running) {
				DBmanager usr("users_DB.txt");
				usr.openDatabase('n');
				bool found = false;
				std::string nO, snO, emailO, pwO, pinO, bdO, anO;
				while (usr.in >> nO >> snO >> emailO >> pwO >> pinO >> bdO >> anO) {
					if (anO == odbiorca) {
						found = true;
						break;
					}
				}
				usr.closeDatabase();
				if (!found) {
					while (!found) {
						std::cout << "Could not find user with this account number!\n";
						std::cout << "Insert proper account number (q -> exit): ";
						std::cin >> odbiorca;
						if (odbiorca == "q") {
							running = false;
							break;
						}
						else {
							usr.openDatabase('n');
							while (usr.in >> nO >> snO >> emailO >> pwO >> pinO >> bdO >> anO) {
								if (anO == odbiorca) {
									found = true;
									break;
								}
							}
							usr.closeDatabase();
						}
					}
					//przelew
				}
				if (running) {
					odbiorca = emailO;
					DBmanager bilO("bilanse/" + odbiorca + ".txt");
					DBmanager bil("bilanse/" + email + ".txt");
					bilO.openDatabase('n');
					bil.openDatabase('n');
					double money, moneyO, amount;
					bil.in >> money;
					bilO.in >> moneyO;
					std::cout << "Insert amount of money: ";
					std::cin >> amount;
					if (amount < 0.0) {
						while (amount < 0.0) {
							std::cout << "You cannot make minus transfer!\n";
							std::cout << "Insert proper amount (0 -> exit): ";
							std::cin >> amount;
							if (amount == 0) {
								running = false;
								break;
							}
						}
					}
					if (money < amount && amount>0) {
						while (money < amount || amount < 0)
						{
							if (amount < 0) {
								std::cout << "You cannot make minus transfer!\n";
								std::cout << "Insert proper amount (0 -> exit): ";
							}
							else {
								std::cout << "You do not have that amount of money on your account!\n";
								std::cout << "Insert other amount (0 -> exit): ";
							}
							std::cin >> amount;
							if (amount == 0) {
								running = false;
								break;
							}
						}
					}
					if (running) {
						money -= amount;
						moneyO += amount;
						std::vector<std::string>V;
						std::string line;
						char c;
						int t{};
						std::string desc{};
						std::cout << "Insert transfer's desription (optional) (max. 35 characters): ";
						std::cin.ignore();
						do
						{
							c = std::getchar();
							if (c == ' ')
								c = '`';
							desc += c;
							t++;
						} while (c != '\n' && c != EOF && t < 35);
						desc.erase(desc.end() - 1);
						if (desc.size() == 0)desc = "------";
						while (std::getline(bil.in, line))
							V.push_back(line);

						bil.closeDatabase();
						DBmanager bil("bilanse/" + email + ".txt");
						bil.openDatabase('f');
						bil.out << std::fixed << std::setprecision(2) << money << '\n';
						bil.out << odbiorca << "\t-" << amount << '\t' << desc << '\n';
						for (auto s : V)
							bil.out << s << '\n';
						bil.closeDatabase();
						V.clear();
						while (std::getline(bilO.in, line))
							V.push_back(line);
						bilO.closeDatabase();
						DBmanager bilO("bilanse/" + odbiorca + ".txt");
						bilO.openDatabase('f');
						bilO.out << std::fixed << std::setprecision(2) << moneyO << '\n';
						bilO.out << email << "\t+" << amount << '\t' << desc << '\n';
						for (auto s : V)
							bilO.out << s << '\n';
						bilO.closeDatabase();
						std::cout << "\n\nTransfer made positively!\n";
					}
				}
			}

		}
		system("pause");
	}
	else {
		std::string email;
		DBmanager get("logged.txt");
		get.openDatabase('n');
		for (int i = 0; i < 2; i++)get.in >> email;
		get.closeDatabase();
		DBmanager temp("users_DB.txt");
		setUser(findUser(email).name, findUser(email).em, findUser(email).password, findUser(email).PIN, findUser(email).bday, findUser(email).accNum);
		makeTransfer();
	}
}


void User::showAccBil() {
	Menu newMenu;
	newMenu.clear();
	newMenu.lHeader();
	if (checkSession()) {
		DBmanager bil("bilanse/" + email + ".txt");
		bil.openDatabase('n');
		std::string B;
		bil.in >> B;
		bil.closeDatabase();
		std::cout << "Current bilance of account: " << B << "\n\n";
		system("pause");
	}
	else {
		std::string email;
		DBmanager get("logged.txt");
		get.openDatabase('n');
		for (int i = 0; i < 2; i++)get.in >> email;
		get.closeDatabase();
		DBmanager temp("users_DB.txt");
		setUser(findUser(email).name, findUser(email).em, findUser(email).password, findUser(email).PIN, findUser(email).bday, findUser(email).accNum);
		showAccBil();
	}
}

void User::withdraw() {
	Menu newMenu;
	newMenu.clear();
	newMenu.lHeader();
	if (checkSession()) {
		std::cout << "WITHDRAW MONEY\n\n";
		DBmanager bil("bilanse/" + email + ".txt");
		bil.openDatabase('n');
		double money;
		bil.in >> money;
		bil.closeDatabase();
		bool running = true;
		double amount;
		std::cout << "Insert amount of money: ";
		std::cin >> amount;
		if (money < amount || amount < 0) {
			while (money < amount || amount < 0)
			{
				if (money < amount)
					std::cout << "You do not have that amount of money on your account!\n";
				if (amount < 0)
					std::cout << "You cannot withdraw less than 0!\n";
				std::cout << "Insert other amount (0 -> exit): ";
				std::cin >> amount;
				if (amount == 0) {
					running = false;
					break;
				}

			}
		}
		if (running) {
			money -= amount;
			std::vector<std::string>V;
			std::string line;
			char c;
			int t{};
			std::string desc{};
			std::cout << "Insert withdraw's desription (optional) (max. 35 characters): ";
			std::cin.ignore();
			do
			{
				c = std::getchar();
				if (c == ' ')
					c = '`';
				desc += c;
				t++;
			} while (c != '\n' && c != EOF && t < 35);
			desc.erase(desc.end() - 1);
			if (desc.size() == 0)desc = "------";
			bil.openDatabase('n');
			std::getline(bil.in, line);
			while (std::getline(bil.in, line))
				V.push_back(line);
			bil.closeDatabase();
			bil.openDatabase('f');
			bil.out << std::fixed << std::setprecision(2) << money << '\n';
			bil.out << "withdraw" << "\t-" << amount << '\t' << desc << '\n';
			for (auto s : V)
				bil.out << s << '\n';
			bil.closeDatabase();
			V.clear();
			std::cout << "\n\nWithdraw made positively!\n";
		}
		system("pause");
	}
	else {
		std::string email;
		DBmanager get("logged.txt");
		get.openDatabase('n');
		for (int i = 0; i < 2; i++)get.in >> email;
		get.closeDatabase();
		setUser(findUser(email).name, findUser(email).em, findUser(email).password, findUser(email).PIN, findUser(email).bday, findUser(email).accNum);
		withdraw();
	}
}

void User::insert() {
	Menu newMenu;
	newMenu.clear();
	newMenu.lHeader();
	if (checkSession()) {
		std::cout << "INSERT MONEY\n\n";
		DBmanager bil("bilanse/" + email + ".txt");
		bil.openDatabase('n');
		double money;
		bil.in >> money;
		bil.closeDatabase();
		bool running = true;
		double amount;
		std::cout << "Insert amount of money: ";
		std::cin >> amount;
		if (amount < 0) {
			while (amount < 0)
			{
				if (amount < 0)
					std::cout << "You cannot insert less than 0!\n";
				std::cout << "Insert other amount (0 -> exit): ";
				std::cin >> amount;
				if (amount == 0) {
					running = false;
					break;
				}

			}
		}
		if (running) {
			money += amount;
			std::vector<std::string>V;
			std::string line;
			char c;
			int t{};
			std::string desc{};
			std::cout << "Insert insert's desription (optional) (max. 35 characters): ";
			std::cin.ignore();
			do
			{
				c = std::getchar();
				if (c == ' ')
					c = '`';
				desc += c;
				t++;
			} while (c != '\n' && c != EOF && t < 35);
			desc.erase(desc.end() - 1);
			if (desc.size() == 0)desc = "------";
			bil.openDatabase('n');
			std::getline(bil.in, line);
			while (std::getline(bil.in, line))
				V.push_back(line);
			bil.closeDatabase();
			bil.openDatabase('f');
			bil.out << std::fixed << std::setprecision(2) << money << '\n';
			bil.out << "insert" << "\t+" << amount << '\t' << desc << '\n';
			for (auto s : V)
				bil.out << s << '\n';
			bil.closeDatabase();
			V.clear();
			std::cout << "\n\nInsert made positively!\n";
		}
		system("pause");
	}
	else {
		std::string email;
		DBmanager get("logged.txt");
		get.openDatabase('n');
		for (int i = 0; i < 2; i++)get.in >> email;
		get.closeDatabase();
		setUser(findUser(email).name, findUser(email).em, findUser(email).password, findUser(email).PIN, findUser(email).bday, findUser(email).accNum);
		insert();
	}
}