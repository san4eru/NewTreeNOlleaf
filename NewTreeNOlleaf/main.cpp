#include <fstream>
#include <sstream>
#include <time.h>
#include "Tree23.h"

using namespace std;

bool ConvertStringToInt(const string& s, unsigned int& num);
string generateRandomString();

int main()
{
	setlocale(LC_ALL, "Russian");
	// ШАБЛОН
	Dictionary<string>* d = new Dictionary<string>();
	int choice = 0;

	while (choice != 11) {
		cout << endl;
		cout << "+===================================================+" << endl;
		cout << "|                  МЕНЮ 2-3 дерева                  |" << endl;
		cout << "+===================================================+" << endl;
		cout << "| (1) Добавать ключи и значения в дерево из файла   |" << endl;
		cout << "| (2) Добавать случайные ключи и значения           |" << endl;
		cout << "| (3) Добавать в дерево ключ и значение с клавиатуры|" << endl;
		cout << "| (4) Вывести дерево в консоль                      |" << endl;
		cout << "| (5) Сохранить дерево в файле                      |" << endl;
		cout << "| (6) Искать значение по ключу                      |" << endl;
		cout << "| (7) Удалить ключ и значение                       |" << endl;
		cout << "| (8) Удалить ВСЕ дерево                            |" << endl;
		cout << "| (9) Получить кол-во хранящихся ключей             |" << endl;
		cout << "| (10) Получить высоту дерева                       |" << endl;
		cout << "| (11) Выйти                                        |" << endl;
		cout << "+===================================================+" << endl;

		cout << "Выберите действие: "; cin >> choice;

		// добавать из файла
		if (choice == 1) {
			string FileName;
			cout << "Введите название файла: "; cin >> FileName;
			ifstream ifs; // входной файл
			ifs.open(FileName);

			unsigned int N;

			if (ifs.is_open()) {
				string nextString;
				while (ifs >> nextString)
				{
					if (ConvertStringToInt(nextString, N))
					{
						int success;
						unsigned int i;

						for (i = 0; i < N; i++)
						{
							try
							{
								// ШАБЛОН
								Template<string>* currentData = Template<string>::Read(ifs);

								if (currentData != NULL)
								{
									success = d->Insert(currentData->GetKey(), currentData);

									if (!success)
									{
										delete currentData;
									}
								}
								else // ошибка - недействительный ввод
								{
									cout << "Неверный входной файл!" << endl;

									ifs.close();
									d->FreeAllElements();
								}
							}
							catch (char* message)
							{
								cout << message << endl;
							}
						}
						break;
					}
					else
					{
						ifs.close();
						d->FreeAllElements();
					}
				}
				ifs.close();
			}
			else
				cout << "Неверное имя файла." << endl;
			system("pause");
			system("cls");
		}
		// добавать случайные
		else if (choice == 2) {
			int num = 0;
			cout << "Введите кол-во случайных пар (ключ и значение): "; cin >> num;
			srand(time(NULL));
			for (int i = 0; i < num; i++) {
				string newk = generateRandomString();
				string newdata = to_string(rand() % 100);
				// ШАБЛОН
				Template<string>* currentData = Template<string>::Readfromkeyboard(newk, newdata);
				int success;
				if (currentData != NULL)
				{
					success = d->Insert(currentData->GetKey(), currentData);

					if (!success)
					{
						delete currentData;
					}
					else {
						cout << i + 1 << " пара успешно добавлена!" << endl;
					}
				}
			}
			system("pause");
			system("cls");
		}
		// добавать с клавиатуры
		else if (choice == 3) {
			string newk;
			cout << "Введите ключ: "; cin >> newk;
			// ШАБЛОН
			string data;
			cout << "Введите data: "; cin >> data;
			// ШАБЛОН
			Template<string>* currentData = Template<string>::Readfromkeyboard(newk, data);
			int success;
			if (currentData != NULL)
			{
				success = d->Insert(currentData->GetKey(), currentData);

				if (!success)
				{
					delete currentData;
				}
				else {
					cout << "Успешно добавлено!" << endl;
				}
			}
			system("pause");
			system("cls");
		}
		// вывести в консоль
		else if (choice == 4) {
			d->PrintDictionaryConsole();

			system("pause");
			system("cls");
		}
		// сохранить дерево в файле
		else if (choice == 5) {
			string outputFile;
			cout << "Введите название нового файла: "; cin >> outputFile;

			ofstream ofs; // фвйл вывода
			ofs.open(outputFile);

			d->PrintDictionary(ofs);

			ofs.close();
			system("pause");
			system("cls");
		}
		// искать значение по ключу
		else if (choice == 6) {
			string k;
			cout << "Введите ключ: "; cin >> k;
			StringKey* sk = new StringKey(k);

			if (d->Find(sk) != NULL)
				cout << "Ключ " << k << " со значением " << *(d->Find(sk)) << endl;
			else
				cout << "Ключа " << k << " не существует!" << endl;
			
			system("pause");
			system("cls");
		}
		// удалить ключ и значение
		else if (choice == 7) {
			string kdel;
			cout << "Введите ключ, который нужно удалить: "; cin >> kdel;

			StringKey* sk = new StringKey(kdel);
			// ШАБЛОН
			Template<string>* currentData = d->Delete(sk);

			if (d->Find(sk) == NULL)
				cout << "Успешно удалено!" << endl;
			else
				cout << "Удаление не удалось!" << endl;

			delete currentData; // удаление временных
			delete sk; // переменных
			system("pause");
			system("cls");
		}
		// удалить все дерево
		else if (choice == 8) {
			string flag;
			cout << "Вы действительно хотите удалить ВСЕ дерево (Y/N): "; cin >> flag;
			if (flag == "Y") {
				d->FreeAllElements();
				if (d->IsEmpty())
					cout << "Дерево очищено!" << endl;
				else
					cout << "Ошибка! Дерево НЕ очищено." << endl;
			}
			else
				cout << "Удаление отменено!" << endl;

			system("pause");
			system("cls");
		}
		// получить кол-во хранящихся ключей
		else if (choice == 9) {
			int count_k = d->CountOfKey();
			if (count_k == 0)
				cout << "Дерево пустое! Количество хранящихся ключей: 0" << endl;
			else
				cout << "Количество хранящихся ключей: " << count_k << endl;
			system("pause");
			system("cls");
		}
		//получить высоту дерева
		else if (choice == 10) {
			int count_n = d->CountOfHeight();
			if (count_n == 0)
				cout << "Дерево пустое! Высота равна 0" << endl;
			else
				cout << "Высота дерева: " << count_n << endl;
			system("pause");
			system("cls");
		}
	}

	// Очистка памяти от данных
	d->FreeAllElements();
	delete d; // удаление словаря

	return 0;
}

bool ConvertStringToInt(const string& s, unsigned int& num)
{
	istringstream myStream(s);

	if (myStream >> num)
		return true;
	else
		return false;
}

string generateRandomString()
{
	unsigned int outputLength = 10;
	string allowedChars = "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

	int randomIndex;
	string outputString = "";

	for (unsigned int i = 0; i < outputLength; ++i) {
		randomIndex = rand() % allowedChars.length();
		outputString += allowedChars[randomIndex];
	}

	if (outputString.empty()) {
		return generateRandomString();
	}
	else {
		return outputString;
	}
}