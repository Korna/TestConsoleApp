#include "stdafx.h"
#include <regex>
#include "functions.hpp"

int main()
{
	setlocale(LC_ALL, "Russian");
	std::cout << "Чтобы выйти, наберите 'q'\nВведите запрос:";
	smatch matchedStrings;
	regex parseRule("^[ ]*test[ ]+(-f[ ]+([^ ]+)[ ]+-m[ ]+(words[ ]+-v[ ]+([^ ]+)|checksum)|-h)$");//правило для проверки команды на правильность и обработку информации
	std::string input;

	do {
		getline(cin, input);
		if (!regex_match(input, matchedStrings, parseRule))//если запрос был введен неверно
			std::cout << "Неверный запрос" << endl;
		else {
			//условия для проверки выбранного режима
			if (matchedStrings[4] != "")
				std::cout << "Число найденных слов в файле:" << command::substrFind(matchedStrings[2].str(), matchedStrings[4].str()) << endl;
			else {
				if (matchedStrings[3] == "")
					command::help();
				else
					std::cout << "Чексумма файла:" << command::checkSum(matchedStrings[2].str()) << endl;
			}

		}

	} while (input != "q");

	return 1;
}

