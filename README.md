# ATM
System bankowy z funkcjami bankomatu. System opiera się na bazie danych, która opiera się na plikach tekstowych. Program ma swego rodzaju zaminne sesyjne, dzięki którym nie trzeba się znowu logować po wyłączeniu programu.
Program napisany jest obiektowo. Każda klasa odpowiedzialna jest za obsługe czego innego:
  - User - zajmuje się opreracjami użytkownika, takimi jak robienie przelewów, wpłacanie pieniędzy, wypłacanie pieniędzy itp.
  - DBmanager - posiada komendy operujące na bazie danych np. otwiera daną bazę danych
  - Menu - tworzy header programu

Klasa User:
  - openDatabase(char open_operation) - otwiera bazę danych, argumenty:
     - n - dołacza elementy do bazy dancyh
     - f - czyści bazę danych i następnie dodaje elementy
  - isOpened() - sprawdza czy baza danych jest otwarta
  - closeDatabase() - zamyka bazę danych 
  - createDatabase() - tworzy bazę danych

Klasa Menu:
  - clear() - czyści konsole
  - lMenu() - wyświetla menu dla zalogowanego użytkownika
  - nMenu() - wyświetla menu dla niezalogowanego użytkownika
  - header() - wyświetla nagłówek dla niezalogowanego użytkownika
  - lHeader() -  wyświetla nagłówek dla zalogowanego użytkownika
  - showMenu() - sprawdza czy użytkownik jest zalogowany i wybiera odpowiednie menu

Klasa User:
  - findUser(string email) - znajduje użytkownika po emailu i zwraca o nim informacje w formacie struktury
  - setUser(std::pair<std::string, std::string> name, std::string email, std::string password, std::string PIN, std::string bday, std::string accNum) - ustawia w klasie danego użytkownika, jest konstruktorem
  - login() - obługuje proces logowania
  - reg() - obsługuje proces rejestracji
  - accInf(bool flag) - pokazuje informacje o użytkowniku, flag - falga pokazująca pin i hasło użytkownika
  - showSpendingH() - pokazuje historie transakcji użytkownika
  - makeTransfer() - obsługuje proces wysyłania przelewu do osiby
  - showAccBil() - pokazuje aktualny stan konta
  - withdraw() - obsługuje proces wypłacenia pieniędzy z konta
  - insert() - obsługuje proces wpłacanie pieniędzy na konto
  - checkSession() - sprawdza zmienną sesyjną, czy użytkownik się wylogował czy nie
 
 Inne komendy:
  - setLogged(bool o) - ustawia w pliku informacji o logowaniu status logowania, true - zalogowano, false - niezalogowano
  - checkLogged() - sprawdza czy użytkownik jest zalogowany
  - verify_date(std::string date) - sprawdza czy format podanej daty "date" jest poprawny
  - verify_email(std::string mail) - sprawdza czy format podanego mailu "mail" jest poprawny
  - verify_password(std::string pass) - sprawdza dane hasło "pass" spełnia wymogi bezpiecznego hasła
  - verify_pin(std::string pin) - sprawdza czy format danego pinu "pin" jest poprawny 
  - genAccNum() - genereuje numer konta, wiekskzy o 1 od poprzedniego
