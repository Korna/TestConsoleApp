#ifndef COMMAND_FUNCTIONS
#define COMMAND_FUNCTIONS
#include <fstream>
#include <iostream>
using namespace std;

//�� �������� ������� �������� � ������� ����� ��������
size_t stringShift(size_t* sft, size_t length, char ch)
{
	for (size_t i = 0; i <= length; ++i)//������ �� ������� ��������
		if (sft[i] == ch)
			return i + 1;
	return length;
}

namespace command {
	//������� ���������� ��������� ����� � �����
	size_t substrFind(std::string name, std::string word)
	{
		const short BLOCK_SIZE = 4096;//����������������� ������ �������� � �������
		size_t wlength = word.length();

		//�������� � ������������� ������� ��������
		size_t *sft_2 = new size_t[wlength];
		for (size_t i = 0; i < wlength; ++i)
			sft_2[i] = word[wlength - 1];

		//���������� ������� ��������
		for (int i = wlength - 1; i >= 0; --i)
			if (sft_2[wlength - 2 - i] != word[i] && sft_2[wlength - 2 - i] == word[wlength - 1])
				sft_2[wlength - 2 - i] = word[i];

		std::ifstream file;
		file.open("File.txt", ios::binary);
		char *buf = new char[BLOCK_SIZE + wlength - 1];//��������� ����� � ����������� ����� ��� ����� �����
		size_t count = 0;//����� ���������� ����� � �����

		do {
			file.read(&buf[wlength - 1], BLOCK_SIZE);//���������� ���������� �����, ������� � �������, ���������� ����� ������� ����������� �����
			buf[file.gcount() + wlength - 1] = '\0';
			size_t i = 0;//����� ������� �� ������
			int j;//������ ��������� �� ������
			bool flg;//���� ���������� ����� �� �������

			while (i < file.gcount()) {//������ �� ������������ ������
				flg = true;
				for (j = wlength - 1; j >= 0; --j) {//��������� ������� � ����� ����� �� �������
					if (word[j] != buf[i + j]) {
						flg = false;
						break;
					}
				}
				if (flg) {//������� ������������ � ������
					i += wlength;
					++count;
				}
				else
					i += stringShift(sft_2, wlength, buf[i + j]);//�������� � ������ �� �������
			}

			for (std::streamsize k = file.gcount(); k < file.gcount() + wlength - 1; ++k) //��������� wlength - 1 �������� ��������� � ������ �������, ���� ����� ������� ����� ����
				buf[k - file.gcount()] = buf[k];

		} while (file.gcount() == BLOCK_SIZE);//���� �������� ������
											  //������������ ������ � �������� ������
		delete[] buf;
		buf = nullptr;
		delete[] sft_2;
		sft_2 = nullptr;
		file.close();
		return count;
	}

	std::uint32_t checkSum(std::string name)
	{
		const auto WORD_SIZE = sizeof(uint32_t);
		std::uint32_t totalsum = 0;//������ ����� �����
		std::uint32_t wordsum = 0;//���� ���������� �����
		std::ifstream file;
		file.open(name, ios::binary);
		while (file.read(reinterpret_cast<char*>(&wordsum), WORD_SIZE)) {//�� ��� ���, ���� �������� �������� ������ � ������� 32 ����� �� �����
			totalsum += wordsum;
			wordsum = 0;
		}
		file.close();
		return totalsum += wordsum;//������������ ���������� �����
	}

	void help() {
		std::cout << "������ ��������� �������� ���������� ���������� ���������� ����� � ����� ��� �������� �����." << endl;
		std::cout << "������� �������� ���������� ����������\n���� ������������� ��� ������ ���������� ���������." << endl;
		std::cout << "���� ����������� �������� � ����������� ����� �����. � ������ ����� ������������ �������� �����-����." << endl;
		std::cout << "��� ������������ �������� ������������ ���������������� ������� ����������� ����� � �������� ���." << endl;
		std::cout << "������ ������" << endl;
		std::cout << "-h\t\t\t�������� �������������� ���������� � ���������." << endl;
		std::cout << "-v\t\t\t��������� ������������ ��������(�����) � �������� ��������� ��� ����������� ����������." << endl;
		std::cout << "-m\t\t\t��������� �������� ������ ��� ��������." << endl;
		std::cout << "-f\t\t\t��������� �������� ����� � �������� ���������." << endl;
		std::cout << "������ �������" << endl;
		std::cout << "[words]\t\t\t������ �������� ������� � �������� ���������� ��������� ����� � �����." << endl;
		std::cout << "[checksum]\t\t������ �������� ����������� � �������� 32 ������ �������� �����." << endl;
		std::cout << "������ ������� �������:test -f File.txt -m words -v <head>" << endl;
	}
}
#endif