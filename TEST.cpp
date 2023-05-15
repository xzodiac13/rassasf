#include <Windows.h>
#include <string>
#include <fstream>
#include <limits>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

struct Student
{
	char initials[90]; // Фамилия имя отчество
	short birth_day; // день рождения
	short birth_month; // месяц рождения
	short birth_year; // год рождения
	short admissionYear; // год поступления
	char faculty[10]; // факультет
	char department[10]; // кафедра
	char group[14]; // группа
	char sex[2]; // пол
	char recordBookNumber[10]; // номер зачетной книжки
	char subjects[2][8][50]; // предметы (2 сессии, 8 предметов)
	int marks[2][8]; // оценки за предметы (2 сессии, 8 предметов)
};

struct OtherMarks 
{
	short marks[3];
	string zachet = "Зачёт";
	string nezachet = "Незачёт";
};

bool isValidMark(int mark) {
	return mark >= 0 && mark <= 5;

}

void addStudent() {
	Student student;
	ofstream file("students.bin", ios::binary | ios::app);

	if (!file) {
		cout << "Ошибка открытия файла!" << endl;
		return;
	}

	cout << "Добавление студента в список: " << endl;
	cout << "Введите Ф.И.О. студента: ";
	cin.getline(student.initials, 90);
	cout << "Введите дату рождения студента (ДД.ММ.ГГГГ): ";

	string birth_day, birth_month, birth_year;
	getline(cin, birth_day, '.');
	getline(cin, birth_month, '.');
	getline(cin, birth_year);

	student.birth_day = stoi(birth_day);
	student.birth_month = stoi(birth_month);
	student.birth_year = stoi(birth_year);

	cout << "Введите год поступления студента: ";
	cin >> student.admissionYear;
	cin.ignore();
	cout << "Введите факультет (институт): ";
	cin.getline(student.faculty, 10);
	cout << "Введите кафедру: ";
	cin.getline(student.department, 10);
	cout << "Введите группу: ";
	cin.getline(student.group, 14);
	cout << "Введите номер зачетной книжки: ";
	cin.getline(student.recordBookNumber, 10);

	// Присваиваем порядковый номер, равный номеру зачетной книжки
	string recordBookNumber = student.recordBookNumber;
	int studentNumber = stoi(recordBookNumber);

	for (int j = 0; j < 2; j++) {
		cout << "+---Сессия " << j + 1 << "---+" << endl;
		for (int k = 0; k < 8; k++) {
			cout << "Название предмета " << k + 1 << ": ";
			cin.getline(student.subjects[j][k], 50);

			bool validMark = false;
			int mark;
			while (!validMark) {
				cout << "Оценка по предмету " << k + 1 << ": ";
				cin >> mark;
				if (cin.fail() || !isValidMark(mark)) { // Проверка оценки на валидность
					cout << "Оценка должна быть числом в диапазоне от 0 до 5. Пожалуйста, введите оценку заново." << endl;
					cin.clear();
					cin.ignore();
				}
				else {
					validMark = true;
				}
			}

			student.marks[j][k] = mark;
			cin.ignore();
		}
	}

	file.write(reinterpret_cast<char*>(&student), sizeof(Student));
	file.close();

	cout << "Студент успешно добавлен!" << endl;
}

void saveStudentsBar()
{
	cout << "Данные о студентах успешно сохранены" << endl;
}

void showStudent() {

	ifstream file("students.bin", ios::binary | ios::in);

	if (!file) {
		cout << "Ошибка открытия файла!" << endl;

		return;
	}

	cout << "Введите номер зачетной книжки студента: ";
	string recordBookNumber;
	cin >> recordBookNumber;
	cin.ignore();

	Student student;
	bool found = false;
	file.clear();

	while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) {

		if (recordBookNumber == student.recordBookNumber) {

			cout << "Найден студент с номером зачетной книжки: " << recordBookNumber << endl;
			cout << "Ф.И.О. студента: " << student.initials << endl;
			cout << "Дата рождения: " << student.birth_day << "." << student.birth_month << "." << student.birth_year << endl;
			cout << "Год поступления: " << student.admissionYear << endl;
			cout << "Факультет: " << student.faculty << endl;
			cout << "Кафедра: " << student.department << endl;
			cout << "Группа: " << student.group << endl;

			cout << "Оценки за предметы:" << endl;
			for (int i = 0; i < 2; i++) {
				cout << "+---Сессия " << i + 1 << "---+" << endl;
				for (int j = 0; j < 8; j++) 
				{
					cout << "Предмет: " << student.subjects[i][j] << ", Оценка: " << student.marks[i][j] << endl;
				}
			}

			found = true;
			break;
		}
	}

	file.close();

	if (!found) 
	{
		cout << "Студент с номером зачетной книжки " << recordBookNumber << " не найден." << endl;
	}
}

int countMarks(int marks[][8], int mark)
{
	int count = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (marks[i][j] == mark)
				count++;
		}
	}
	return count;
}


void Zadanie()
{
	const int MAX_MARKS = 8;
	ifstream file("students.bin", ios::binary | ios::in);
	if (!file)
	{
		cout << "Ошибка открытия файла!" << endl;
		return;
	}

	int option;
	cout << "Выберите вариант (1-5) для фильтрации студентов:\n";
	cout << "1. Оценки 3\n";
	cout << "2. Оценки 3 и 4\n";
	cout << "3. Оценки 5\n";
	cout << "4. Оценки 3 и 5\n";
	cout << "5. Оценки 4 и 5\n";
	cin >> option;

	// Проверка ввода пользователя
	if (option < 1 || option > 5)
	{
		cout << "Неправильный вариант. Пожалуйста, выберите число от 1 до 5." << endl;
		return;
	}

	int yearStart, yearEnd;
	cout << "Введите интервал года рождения студентов (начальный год, конечный год): ";
	cin >> yearStart >> yearEnd;

	Student student;
	int count = 0;
	int filteredCount = 0;

	while (file.read(reinterpret_cast<char*>(&student), sizeof(Student)))
	{
		count++;
		int birthYear = student.birth_year;
		if (birthYear >= yearStart && birthYear <= yearEnd)
		{
			bool filter = false;
			switch (option)
			{
			case 1: // Оценки 3
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						if (student.marks[i][j] == 3)
						{
							filter = true;
							break;
						}
					}
					if (filter)
						break;
				}
				break;
			case 2: // Оценки 3 и 4
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						if (student.marks[i][j] == 3 || student.marks[i][j] == 4)
						{
							filter = true;
							break;
						}
					}
					if (filter)
						break;
				}
				break;
			case 3: // Оценки 5
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						if (student.marks[i][j] == 5)
						{
							filter = true;
							break;
						}
					}
					if (filter)
						break;
				}
				break;
			case 4: // Оценки 3 и 5
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						if (student.marks[i][j] == 3 || student.marks[i][j] == 5)
						{
							filter = true;
							break;
						}
					}
					if (filter)
						break;
				}
			case 5: // Оценки 4 и 5
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						if (student.marks[i][j] == 4 || student.marks[i][j] == 5)
						{
							filter = true;
							break;
						}
					}
					if (filter)
						break;
				}
				break;
			}

			if (filter)
			{
				filteredCount++;
				cout << "Студент #" << filteredCount << ":\n";
				cout << "Год рождения: " << student.birth_year << endl;
				cout << "Оценки:\n";
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < MAX_MARKS; j++)
					{
						cout << student.marks[i][j] << " ";
					}
					cout << endl;
				}
				cout << endl;
			}
		}
	}

	file.close();

	cout << "Общее количество студентов: " << count << endl;
	cout << "Количество студентов после фильтрации: " << filteredCount << endl;
}





void printAllStudents()
{
	ifstream file("students.bin", ios::binary | ios::in);

	if (!file) 
	{
		cout << "Ошибка открытия файла!" << endl;
		return;
	}


	Student student;


	bool found = false;
	file.clear();


	while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) 
	{
		cout << "-----------------------------------------------" << endl;
		cout << "Найден студент с номером зачетной книжки: " << student.recordBookNumber << endl;
		cout << "Ф.И.О. студента: " << student.initials << endl;
		cout << "Дата рождения: " << student.birth_day << "." << student.birth_month << "." << student.birth_year << endl;
		cout << "Год поступления: " << student.admissionYear << endl;
		cout << "Факультет: " << student.faculty << endl;
		cout << "Кафедра: " << student.department << endl;
		cout << "Группа: " << student.group << endl;

		cout << "Оценки за предметы:" << endl;
		for (int i = 0; i < 2; i++) {
			cout << "+---Сессия " << i + 1 << "---+" << endl;
			for (int j = 0; j < 8; j++) {
				cout << "Предмет: " << student.subjects[i][j] << ", Оценка: " << student.marks[i][j] << endl;
			}
		}
		cout << "-----------------------------------------------" << endl << endl;
		//cout << student.birth_year << endl;
		cout << "-----------------------------------------------" << endl;

	}

}

void showBookNumbers()
{
	ifstream file("students.bin", ios::binary | ios::in);

	if (!file) 
	{
		cout << "Ошибка открытия файла!" << endl;
		return;
	}


	Student student;


	bool found = false;
	file.clear();

	/*cout << "-----------------------------------------------" << endl;
	cout << "Найден студент с номером зачетной книжки: " << student.recordBookNumber << endl;
	cout << "Ф.И.О. студента: " << student.name << endl;
	cout << "Дата рождения: " << student.birth_day << "." << student.birth_month << "." << student.birth_year << endl;
	cout << "Год поступления: " << student.admissionYear << endl;
	cout << "Факультет: " << student.faculty << endl;
	cout << "Кафедра: " << student.department << endl;
	cout << "Группа: " << student.group << endl;*/


	while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) 
	{
		//cout << "Найдены студенты со следующими зачетными книжками: " << endl;
		cout << "------------------------------------------------------------------------------" << endl;
		cout << "Номер зачетной книжки - " << student.recordBookNumber << " - " << student.initials << endl;

		cout << "------------------------------------------------------------------------------" << endl << endl;
		//cout << student.birth_year << endl;
		//cout << "-----------------------------------------------" << endl;
	
	}

}

void updateStudent() 
{

	fstream file("students.bin", ios::binary | ios::in | ios::out | ios::ate);

	if (!file) {
		cout <<

			"Ошибка открытия файла!" << endl;
		
		return;

	}

	cout << "Введите номер зачетной книжки студента, данные которого нужно изменить: ";
	string recordBookNumber;
	cin >> recordBookNumber;

	Student student;
	bool found = false;

	while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) 
	{
		if (recordBookNumber == student.recordBookNumber) 
		{
			cout << "Найден студент с номером зачетной книжки: " << recordBookNumber << endl;

			cout << "Введите новые данные студента: " << endl;
			cout << "Введите Ф.И.О. студента: ";
			cin.ignore();
			cin.getline(student.initials, 90);
			cout << "Введите дату рождения студента (ДД.ММ.ГГГГ): ";

			string birth_day, birth_month, birth_year;
			getline(cin, birth_day, '.');
			getline(cin, birth_month, '.');
			getline(cin, birth_year);

			student.birth_day = stoi(birth_day);
			student.birth_month = stoi(birth_month);
			student.birth_year = stoi(birth_year);

			cout << "Введите год поступления студента: ";
			cin >> student.admissionYear;
			cin.ignore();
			cout << "Введите факультет (институт): ";
			cin.getline(student.faculty, 50);
			cout << "Введите кафедру: ";
			cin.getline(student.department, 30);
			cout << "Введите группу: ";
			cin.getline(student.group, 14);

			for (int j = 0; j < 2; j++) 
			{
				cout << "+---Сессия " << j + 1 << "---+" << endl;
				for (int k = 0; k < 8; k++) 
				{
					cout << "Название предмета " << k + 1 << ": ";
					cin >> student.subjects[j][k];
					cout << "Оценка по предмету " << k + 1 << ": ";
					cin >> student.marks[j][k];
					cin.ignore();
				}
			}

			// Возвращаемся на позицию записи и перезаписываем данные студента
			file.seekp(-static_cast<int>(sizeof(Student)), ios::cur);
			file.write(reinterpret_cast<char*>(&student), sizeof(Student));

			cout << "Данные студента успешно обновлены!" << endl;

			found = true;
			break;
		}
	}

	file.close();

	if (!found) 
	{
		cout << "Студент с номером зачетной книжки " << recordBookNumber << " не найден." << endl;
	}
}

void deleteStudent() {
	ifstream fileIn("students.bin", ios::binary);
	ofstream fileOut("temp.bin", ios::binary);

	if (!fileIn || !fileOut) 
	{
		cout << "Ошибка открытия файла!" << endl;
		return;
	}

	cout << "Введите номер зачетной книжки студента, данные которого нужно удалить: ";
	string recordBookNumber;
	cin >> recordBookNumber;
	cin.ignore();

	Student student;
	bool found = false;

	while (fileIn.read(reinterpret_cast<char*>(&student), sizeof(Student))) 
	{
		if (recordBookNumber != student.recordBookNumber) 
		{
			// Записываем данные студента, кроме удаляемого, во временный файл
			fileOut.write(reinterpret_cast<char*>(&student), sizeof(Student));
		}
		else {
			found = true;
		}
	}

	fileIn.close();
	fileOut.close();

	// Удаляем исходный файл
	remove("students.bin");
	// Переименовываем временный файл в исходное имя
	rename("temp.bin", "students.bin");

	if (found) {
		cout << "Данные студента успешно удалены!" << endl;
	}
	else {
		cout << "Студент с номером зачетной книжки " << recordBookNumber << " не найден." << endl;
	}
}



void ClearConsole() 
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

bool isValidMark(int mark);
void addStudent();
void showStudent();
void updateStudent();
void deleteStudent();
void ClearConsole();

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int choice;
	do {
		ClearConsole();
		cout << "============= Меню ============" << endl;
		cout << "||1. Ввести данные студента  ||" << endl;
		cout << "||2. Вывести данные студента ||" << endl;
		cout << "||3. Изменить данные студента||" << endl;
		cout << "||4. Удалить данные студента ||" << endl;
		cout << "||5. Вывести всех студентов  ||" << endl;
		cout << "||6. Сохранить в файл        ||" << endl;
		cout << "||7. Все номера зачёток      ||" << endl;
		cout << "||8. Выполнить задание       ||" << endl;
		cout << "||0. Выход                   ||" << endl;
		cout << "===============================" << endl;
		cout << "Выберите пункт -> ";
		cin >> choice;

		switch (choice) {
		
		case 0:
			cout << "Программа завершена." << endl;
			break;
		
		case 1:
			ClearConsole();
			cout << "Вы выбрали ввод данных студента." << endl;
			cin.ignore();
			addStudent();
			break;
		
		case 2:
			ClearConsole();
			cout << "Вы выбрали вывод данных студента." << endl;
			cin.ignore();
			showStudent();
			break;
		
		case 3:
			ClearConsole();
			cout << "Вы выбрали изменение данных студента." << endl;
			cin.ignore();
			updateStudent();
			break;
		
		case 4:
			ClearConsole();
			cout << "Вы выбрали удаление данных студента:" << endl;
			deleteStudent();
			break;
		
		case 5:
			ClearConsole();
			cout << "Вы выбрали вывод всех студентов: " << endl;
			printAllStudents();
			break;

		case 6:
			ClearConsole();
			cout << "Вы выбрали сохранение всех студентов в бинарный файл. " << endl;
			saveStudentsBar();
			break;

		case 7:
			ClearConsole();
			cout << "Вы выбрали показ всех зачетных книжек для справки: " << endl;
			showBookNumbers();
			break;

		case 8:
			ClearConsole();
			Zadanie();
			cout << "Вы выбрали выполнение индивидуального задания" << endl;
			break;

		default:
			ClearConsole();
			cout << "Некорректный выбор. Попробуйте снова." << endl;
			break;
		}

		cout << "Нажмите Enter, чтобы продолжить...";

		cin.ignore();
		cin.get();

	} while (choice != 0);
	return 0;
}