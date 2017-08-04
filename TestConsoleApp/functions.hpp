#ifndef COMMAND_FUNCTIONS
#define COMMAND_FUNCTIONS
#include <fstream>
#include <iostream>
using namespace std;

//по заданной таблице смещения и символу выдаёт смещение
size_t stringShift(size_t* sft, size_t length, char ch)
{
	for (size_t i = 0; i <= length; ++i)//проход по массиву смещения
		if (sft[i] == ch)
			return i + 1;
	return length;
}

namespace command {
	//находит количество включений слова в файле
	size_t substrFind(std::string name, std::string word)
	{
		const short BLOCK_SIZE = 4096;//предположительный размер кластера в системе
		size_t wlength = word.length();

		//создание и инициализация таблицы смещения
		size_t *sft_2 = new size_t[wlength];
		for (size_t i = 0; i < wlength; ++i)
			sft_2[i] = word[wlength - 1];

		//заполнение таблицы смещения
		for (int i = wlength - 1; i >= 0; --i)
			if (sft_2[wlength - 2 - i] != word[i] && sft_2[wlength - 2 - i] == word[wlength - 1])
				sft_2[wlength - 2 - i] = word[i];

		std::ifstream file;
		file.open("File.txt", ios::binary);
		char *buf = new char[BLOCK_SIZE + wlength - 1];//формируем буфер с добавлением места для конца блока
		size_t count = 0;//число нахождений слова в файле

		do {
			file.read(&buf[wlength - 1], BLOCK_SIZE);//заполнение содержимым файла, начиная с индекса, следующего после остатка предыдущего блока
			buf[file.gcount() + wlength - 1] = '\0';
			size_t i = 0;//точка прохода по буферу
			int j;//индекс сравнения со словом
			bool flg;//флаг совпадения слова со строкой

			while (i < file.gcount()) {//проход по прочитанному буферу
				flg = true;
				for (j = wlength - 1; j >= 0; --j) {//сравнение начиная с конца слова со строкой
					if (word[j] != buf[i + j]) {
						flg = false;
						break;
					}
				}
				if (flg) {//найдено соответствие в строке
					i += wlength;
					++count;
				}
				else
					i += stringShift(sft_2, wlength, buf[i + j]);//смещение в строке по таблице
			}

			for (std::streamsize k = file.gcount(); k < file.gcount() + wlength - 1; ++k) //последние wlength - 1 символов переносим в начало массива, куда будет записан новый блок
				buf[k - file.gcount()] = buf[k];

		} while (file.gcount() == BLOCK_SIZE);//пока доступно чтение
											  //освобождение памяти и закрытие потока
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
		std::uint32_t totalsum = 0;//полная сумма битов
		std::uint32_t wordsum = 0;//биты отдельного слова
		std::ifstream file;
		file.open(name, ios::binary);
		while (file.read(reinterpret_cast<char*>(&wordsum), WORD_SIZE)) {//до тех пор, пока возможно успешное чтение в размере 32 битов из файла
			totalsum += wordsum;
			wordsum = 0;
		}
		file.close();
		return totalsum += wordsum;//присваивание остаточных битов
	}

	void help() {
		std::cout << "Данная программа способна просчитать количество нахождений слова в файле или чексумму файла." << endl;
		std::cout << "Краткое описание внутренней реализации\nВвод анализируется при помощи регулярных выражений." << endl;
		std::cout << "Файл считывается поблочно с сохранением конца блока. В поиске слова используется алгоритм Боера-Мура." << endl;
		std::cout << "При высчитывании чексуммы производится последовательный перевод содержимого файла в бинарный вид." << endl;
		std::cout << "Список команд" << endl;
		std::cout << "-h\t\t\tпечатает характеристику параметрам и программе." << endl;
		std::cout << "-v\t\t\tпринимает совокупность символов(слово) в качестве параметра для дальнейшего нахождения." << endl;
		std::cout << "-m\t\t\tпринимает название режима как параметр." << endl;
		std::cout << "-f\t\t\tпринимает название файла в качестве параметра." << endl;
		std::cout << "Список режимов" << endl;
		std::cout << "[words]\t\t\tданный параметр считает и печатает количество включений слова в файле." << endl;
		std::cout << "[checksum]\t\tданный параметр высчитывает и печатает 32 битную чексумму файла." << endl;
		std::cout << "Пример верного запроса:test -f File.txt -m words -v <head>" << endl;
	}
}
#endif