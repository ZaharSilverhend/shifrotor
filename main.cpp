#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <random>

using namespace std;

//����������� ����� � �����
bool copyTextToClipboard(const vector<int>& key)
{
	string text;
	for (int i = 0; i < key.size(); i++) {
		text += to_string(key[i]) + ":";
	}

	if (!OpenClipboard(0)) 
	{
		//���� �� ������� ������ ����� ������ ������������ false
		return false;
	}

	if (!EmptyClipboard()) 
	{
		CloseClipboard();
		// ���� �� ������� �������� ����� ������ ����������� ����� ������ � ������������ false
		return false;
	}

	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
	if (!hMem) 
	{
		CloseClipboard();
		//���� �� ������� �������� ����� ����������� ����� ������ � ������������ False
		return false;
	}
	//��������� ��������� �� ���������� ������
	char* buffer = static_cast<char*>(GlobalLock(hMem));
	//����������� ����������� ������ text � ���������� ������
	strcpy_s(buffer, text.size() + 1, text.c_str());
	//������������� ������
	GlobalUnlock(hMem);

	if (!SetClipboardData(CF_TEXT, hMem)) 
	{
		CloseClipboard();
		//���� �� ������� ���������� ������ � ����� ������, ����������� ����� ������ � ������������ false
		return false;
	}

	CloseClipboard();
	return true;
}
//������������ ������
string GetPathFile()
{
	std::string path;
	// ���������� ��������� ��� ������������� ����������� ���� ������ �����
	OPENFILENAMEA ofn;
	// ���������� ������ ��� �������� ����� �����
	char szFileName[MAX_PATH] = "";
	// ���������� ������� ������, ��������� � ���������� ofn, ������
	ZeroMemory(&ofn, sizeof(ofn));
	// ��������� ������� ��������� ����������� ����
	ofn.lStructSize = sizeof(ofn);
	// ��������� ������������� ���� ��� ����������� ���� ��� NULL
	ofn.hwndOwner = NULL;
	// ��������� ������� ��� ������ ������
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
	// ��������� ������ ��� �������� ����� ���������� �����
	ofn.lpstrFile = szFileName;
	// ��������� ������������ ����� ����� �����
	ofn.nMaxFile = sizeof(szFileName);
	// ��������� �����, ���������� ������������� ���������� �����
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn) == TRUE) {
		path = ofn.lpstrFile;
	}
	else {
		std::cout << "\n\t\x1b[91m������:\x1b[0m ���� �� ������." << std::endl;
	}
	return path;
}
//����
int Menu()
{
	int Nazhatai_Clavisha, user_command = 1;
	system("cls");
	std::cout << "\n\n\t�������� ��������:\n\n";
	printf("\t\x1b[90m<\x1b[0m����������\x1b[90m>\x1b[0m");
	std::cout << "\n\t\x1b[90m  ������������\x1b[0m";
	do
	{
		Nazhatai_Clavisha = _getch();
		switch (Nazhatai_Clavisha)
		{
		case 119: // ��� ������� 'w'
			if (user_command != 1)
			{
				user_command--;
				printf("\x1b[H");
				std::cout << "\n\n\t�������� ��������\n\n";
				std::cout << "\t\x1b[90m<\x1b[0m����������\x1b[90m>\x1b[0m";
				std::cout << "\n\t\x1b[90m  ������������ \x1b[0m";
			}
			break;
		case 115: // ��� ������� 's'
			if (user_command != 2)
			{
				user_command++;
				printf("\x1b[H");
				std::cout << "\n\n\t�������� ��������\n\n";
				std::cout << "\t\x1b[90m  ���������� \x1b[0m";
				std::cout << "\n\t\x1b[90m<\x1b[0m������������\x1b[90m>\x1b[0m";
			}
			break;
		}
	} while (Nazhatai_Clavisha != 13); // ��� ������� 'Enter'

	system("cls");
	return user_command;
}
//��������� �����
vector<int> generateKey()
{
	vector<int> key;
	try
	{
		//������������� ���������� ��������� �����
		std::random_device rd;
		//������������� ���������� ��������� ����� � �������������� rd
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
		std::cerr << "\n\t\x1b[90m������ ��� ��������� �����:\x1b[0m " << e.what() << std::endl;
		//������ ���� � ������ ������
		key.clear();
	}
	return key;
}
//���������� � �����������
string Encrypt(const string& text, const vector<int>& key)
{
	string EncryptedText = text;
	cout << EncryptedText;
	try
	{
		if (key.empty())
		{
			cerr << "\n\t\x1b[91m������: \x1b[0m\x1b[90m���� ����. ���������� ��������� ����������\x1b[0m\n";
			exit(0);
		}

		for (int i = 0; i < EncryptedText.length(); ++i)
		{
			EncryptedText[i] ^= key[i % key.size()];
		}

		cout << "\n\t\x1b[90m���������� ������� ���������!\x1b[0m\n";


		return EncryptedText;
	}
	catch (const std::exception&)
	{
		cout << "\n\t\x1b[91m������:\x1b[0m \x1b[90m���������� �� �������\x1b[0m\n";
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
			cerr << "\n\t\x1b[91m������:\x1b[0m \x1b[90m���� ����\x1b[0m\n";
			return "";
		}

		for (int i = 0; i < DecryptedText.length(); ++i)
		{
			DecryptedText[i] ^= key[i % key.size()];
		}

		cout << "\n\t\x1b[90m������������ ������� ���������\x1b[0m\n";
		return DecryptedText;
	}
	catch (const std::exception&)
	{
		cout << "\n\t\x1b[91m������:\x1b[0m \x1b[90m������������ �� �������\x1b[0m\n";
		return ""; // ���������� ������ ������ � ������ ������
	}
}
//��������� ����� ������������
vector<int> GetUserKey()
{
	vector<int> key;
	string input;
	string str;
	cout << "\n\t\x1b[90m������� ����:\x1b[0m ";
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
		cout << "\n\t\x1b[91m������:\x1b[0m \x1b[90m����������� ������ ����\x1b[0m";
		exit(0);
	}
}
//����� �����
void DisplayKey(vector<int> key)
{
	std::cout << "\n\t\x1b[90m���� <\x1b[0m";
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
//�������� ������
void ProcessFileWithEncryption(const string& path, const vector<int>& key, int user_command)
{
	ifstream read_file;
	ofstream write_file;

	string Text;

	read_file.open(path);
	if (!read_file.is_open())
	{
		cout << "\t\x1b[91m������:\x1b[0m \x1b[90m���������� ������� ����...\x1b[0m";
		exit(0);
	}
	else
	{
		getline(read_file, Text, '\0');
	}
	read_file.close();

	if (user_command == 1)
	{
		// ���������� ������
		Text = Encrypt(Text, key);
		DisplayKey(key);
		if (copyTextToClipboard(key))
		{
			std::cout << "\n\n\t\x1b[90m���� ����������\x1b[0m\n\n\n";
		}
		else
		{
			std::cout << "\n\n\t���� \x1b[90m��\x1b[0m ����������\x1b[90m...\x1b[0m\n\n\n\n";
		}
	}
	else if (user_command == 2) {
		// ������������ ������ � �������������� ���������������� ����� ��� ������������
		Text = Decrypt(Text, GetUserKey());
	}
	write_file.open(path);
	if (!write_file.is_open()) {
		cout << "\t\n\x1b[91m������ ������ � ����...\x1b[0m";
		exit(0);
	}
	else {
		write_file << Text;
	}
	write_file.close();
}
//����� ����� �������
int main()
{
	setlocale(LC_ALL, "rus");
	vector<int> key = generateKey();
	int UserCommand = Menu();
	ProcessFileWithEncryption(GetPathFile(), key, UserCommand);

	return 0;
}