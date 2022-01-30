# ATM
System bankowy z funkcjami bankomatu. System opiera się na bazie danych, która opiera się na plikach tekstowych. Program ma swego rodzaju zaminne sesyjne, dzięki którym nie trzeba się znowu logować po wyłączeniu programu.
Program napisany jest obiektowo. Każda klasa odpowiedzialna jest za obsługe czego innego:
  - user - zajmuje się opreracjami użytkownika, takimi jak robienie przelewów, wpłacanie pieniędzy, wypłacanie pieniędzy itp.
  - DBManager - posiada komendy operujące na bazie danych np. otwiera daną bazę danych
  - menu - tworzy header programu
