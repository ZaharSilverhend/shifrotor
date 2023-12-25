#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <random>

using namespace std;

//Копирование ключа в буфер
bool copyTextToClipboard(const vector<int>& key)
{
	string text;
	for (int i = 0; i < key.size(); i++) {
		text += to_string(key[i]) + ":";
	}

	if (!OpenClipboard(0)) 
	{
		//Если не удалось отрыть буфер обмена возвращается false
		return false;
	}

	if (!EmptyClipboard()) 
	{
		CloseClipboard();
		// Если не удалось очистить буфер обмена закрывается буфер обмена и возвращается false
		return false;
	}

	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
	if (!hMem) 
	{
		CloseClipboard();
		//Если не удалось выделить памят закрывается буфер обмена и возвращается False
		return false;
	}
	//Получение указателя на выделенную память
	char* buffer = static_cast<char*>(GlobalLock(hMem));
	//Копирование содержимого строки text в выделенную память
	strcpy_s(buffer, text.size() + 1, text.c_str());
	//Разблокировка памяти
	GlobalUnlock(hMem);

	if (!SetClipboardData(CF_TEXT, hMem)) 
	{
		CloseClipboard();
		//Если не удалось установить данные в буфер обмена, закрывается буфер обмена и возвращается false
		return false;
	}

	CloseClipboard();
	return true;
}
//Обозреватель файлов
string GetPathFile()
{
	std::string path;
	// Объявление структуры для использования диалогового окна выбора файла
	OPENFILENAMEA ofn;
	// Объявление буфера для хранения имени файла
	char szFileName[MAX_PATH] = "";
	// Заполнение области памяти, связанной с структурой ofn, нулями
	ZeroMemory(&ofn, sizeof(ofn));
	// Установка размера структуры диалогового окна
	ofn.lStructSize = sizeof(ofn);
	// Установка родительского окна для диалогового окна как NULL
	ofn.hwndOwner = NULL;
	// Установка фильтра для выбора файлов
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
	// Установка буфера для хранения имени выбранного файла
	ofn.lpstrFile = szFileName;
	// Установка максимальной длины имени файла
	ofn.nMaxFile = sizeof(szFileName);
	// Установка флага, требующего существования выбранного файла
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn) == TRUE) {
		path = ofn.lpstrFile;
	}
	else {
		std::cout << "\n\t\x1b[91mОшибка:\x1b[0m Файл не выбран." << std::endl;
	}
	return path;
}
//Меню
int Menu()
{
	int Nazhatai_Clavisha, user_command = 1;
	system("cls");
	std::cout << "\n\n\tВыберите действие:\n\n";
	printf("\t\x1b[90m<\x1b[0mШифрование\x1b[90m>\x1b[0m");
	std::cout << "\n\t\x1b[90m  Дешифрование\x1b[0m";
	do
	{
		Nazhatai_Clavisha = _getch();
		switch (Nazhatai_Clavisha)
		{
		case 119: // Код клавиши 'w'
			if (user_command != 1)
			{
				user_command--;
				printf("\x1b[H");
				std::cout << "\n\n\tВыберите действие\n\n";
				std::cout << "\t\x1b[90m<\x1b[0mШифрование\x1b[90m>\x1b[0m";
				std::cout << "\n\t\x1b[90m  Дешифрование \x1b[0m";
			}
			break;
		case 115: // Код клавиши 's'
			if (user_command != 2)
			{
				user_command++;
				printf("\x1b[H");
				std::cout << "\n\n\tВыберите действие\n\n";
				std::cout << "\t\x1b[90m  Шифрование \x1b[0m";
				std::cout << "\n\t\x1b[90m<\x1b[0mДешифрование\x1b[90m>\x1b[0m";
			}
			break;
		}
	} while (Nazhatai_Clavisha != 13); // Код клавиши 'Enter'

	system("cls");
	return user_command;
}
//Генерация ключа
vector<int> generateKey()
{
	vector<int> key;
	try
	{
		//Инициализация генератора случайных чисел
		std::random_device rd;
		//Инициализация генератора случайных чисел с использованием rd
		std::mt19937 gen(rd());

		for (int i = 0; i < 10; ++i)
		{
			if (i % 2 == 0)
			{
				key.push_back(rd());
			}
			else
			{
				key.push_back(gen());
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "\n\t\x1b[90mОшибка при генерации ключа:\x1b[0m " << e.what() << std::endl;
		//Очищаю ключ в случае ошибки
		key.clear();
	}
	return key;
}
//Шифрование и дешифровани
string Encrypt(const string& text, const vector<int>& key)
{
	string EncryptedText = text;
	cout << EncryptedText;
	try
	{
		if (key.empty())
		{
			cerr << "\n\t\x1b[91mОшибка: \x1b[0m\x1b[90mКлюч пуст. Невозможно выполнить шифрование\x1b[0m\n";
			exit(0);
		}

		for (int i = 0; i < EncryptedText.length(); ++i)
		{
			EncryptedText[i] ^= key[i % key.size()];
		}

		cout << "\n\t\x1b[90mШифрование успешно завершено!\x1b[0m\n";


		return EncryptedText;
	}
	catch (const std::exception&)
	{
		cout << "\n\t\x1b[91mОшибка:\x1b[0m \x1b[90mШифрование не удалось\x1b[0m\n";
		exit(0);
	}

}
string Decrypt(const string& EncryptedText, const vector<int>& key)
{
	string DecryptedText = EncryptedText;

	try
	{
		if (key.empty())
		{
			cerr << "\n\t\x1b[91mОшибка:\x1b[0m \x1b[90mКлюч пуст\x1b[0m\n";
			return "";
		}

		for (int i = 0; i < DecryptedText.length(); ++i)
		{
			DecryptedText[i] ^= key[i % key.size()];
		}

		cout << "\n\t\x1b[90mДешифрование успешно завершено\x1b[0m\n";
		return DecryptedText;
	}
	catch (const std::exception&)
	{
		cout << "\n\t\x1b[91mОшибка:\x1b[0m \x1b[90mДешифрование не удалось\x1b[0m\n";
		return ""; // Возвращаем пустую строку в случае ошибки
	}
}
//Обработка ключа пользователя
vector<int> GetUserKey()
{
	vector<int> key;
	string input;
	string str;
	cout << "\n\t\x1b[90mВведите ключ:\x1b[0m ";
	cin >> input;
	try
	{
		for (int i = 0; i < input.length(); i++)
		{
			if (input[i] == ':')
			{
				key.push_back(stoi(str));
				str = "";
			}
			else
			{
				str += input[i];
			}
		}
		return key;
	}
	catch (const std::exception&)
	{
		cout << "\n\t\x1b[91mОшибка:\x1b[0m \x1b[90mНеправильно введен ключ\x1b[0m";
		exit(0);
	}
}
//Вывод ключа
void DisplayKey(vector<int> key)
{
	std::cout << "\n\t\x1b[90mКлюч <\x1b[0m";
	for (size_t i = 0; i < key.size(); i++)
	{
		if (i != (key.size() - 1))
		{
			cout << key[i] << ":";
		}
		else
		{
			cout << key[i];
		}
	}
	std::cout << "\x1b[90m>\x1b[0m";
}
//Основная логика
void ProcessFileWithEncryption(const string& path, const vector<int>& key, int user_command)
{
	ifstream read_file;
	ofstream write_file;

	string Text;

	read_file.open(path);
	if (!read_file.is_open())
	{
		cout << "\t\x1b[91mОшибка:\x1b[0m \x1b[90mневозможно открыть файл...\x1b[0m";
		exit(0);
	}
	else
	{
		getline(read_file, Text, '\0');
	}
	read_file.close();

	if (user_command == 1)
	{
		// Шифрование текста
		Text = Encrypt(Text, key);
		DisplayKey(key);
		if (copyTextToClipboard(key))
		{
			std::cout << "\n\n\t\x1b[90mКлюч скопирован\x1b[0m\n\n\n";
		}
		else
		{
			std::cout << "\n\n\tКлюч \x1b[90mНЕ\x1b[0m скопирован\x1b[90m...\x1b[0m\n\n\n\n";
		}
	}
	else if (user_command == 2) {
		// Дешифрование текста с использованием предоставленного ключа для дешифрования
		Text = Decrypt(Text, GetUserKey());
	}
	write_file.open(path);
	if (!write_file.is_open()) {
		cout << "\t\n\x1b[91mОшибка записи в файл...\x1b[0m";
		exit(0);
	}
	else {
		write_file << Text;
	}
	write_file.close();
}
//Основ вызов функций
int main()
{
	setlocale(LC_ALL, "rus");
	vector<int> key = generateKey();
	int UserCommand = Menu();
	ProcessFileWithEncryption(GetPathFile(), key, UserCommand);

	return 0;
}