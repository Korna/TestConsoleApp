#include "stdafx.h"
#include <regex>
#include "functions.hpp"

int main()
{
	setlocale(LC_ALL, "Russian");
	std::cout << "����� �����, �������� 'q'\n������� ������:";
	smatch matchedStrings;
	regex parseRule("^[ ]*test[ ]+(-f[ ]+([^ ]+)[ ]+-m[ ]+(words[ ]+-v[ ]+([^ ]+)|checksum)|-h)$");//������� ��� �������� ������� �� ������������ � ��������� ����������
	std::string input;

	do {
		getline(cin, input);
		if (!regex_match(input, matchedStrings, parseRule))//���� ������ ��� ������ �������
			std::cout << "�������� ������" << endl;
		else {
			//������� ��� �������� ���������� ������
			if (matchedStrings[4] != "")
				std::cout << "����� ��������� ���� � �����:" << command::substrFind(matchedStrings[2].str(), matchedStrings[4].str()) << endl;
			else {
				if (matchedStrings[3] == "")
					command::help();
				else
					std::cout << "�������� �����:" << command::checkSum(matchedStrings[2].str()) << endl;
			}

		}

	} while (input != "q");

	return 1;
}

