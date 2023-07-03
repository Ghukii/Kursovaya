#include<iostream>
#include<fstream>
#include<locale.h>
#include<string.h>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
using namespace std;

enum class editType : char { onlyDigit, onlyAlpha, all };

struct Birthday {
	int day;
	int month;
	int year;
};

struct Object_mark {
	string object;
	string mark;
};

struct Session {
	int session_number;
	struct Object_mark object_mark[10];
};

struct Student {
	string name;
	string lastname;
	string surname;
	struct Birthday birthday;
	int entered;
	string faculty;
	string department;
	string group;
	string b_number;
	string gender;
	struct Session session[9];
	int session_count = 0;
	int* o_m_count = (int*)calloc(10, sizeof(int*));
};

struct Student_list {
	Student* student;
	struct Student_list* next;
};

class Student_list_class {
private:
	int countItem = 0;
	struct Student_list* head;
public:
	Student_list_class() {
		head;
		countItem = 0;
	}

	int getCountItem() {
		return countItem;
	}

	Student_list* getHead() {
		return head;
	}

	void addItem(struct Student* data)
	{
		struct Student_list* newItem = new Student_list();
		newItem->student = data;
		if (countItem == 0) {
			newItem->next = NULL;
		}
		else
		{
			newItem->next = head;
		}
		head = newItem;
		countItem++;
	}

	void deleteItem(int index) {
		if (index >= 0 and index < countItem and countItem>0) {
			struct Student_list* current = head;
			struct Student_list* old;
			if (index == 0) {
				old = head;
				head = current->next;
				delete old;
				countItem--;
			}
			else {
				int i = 0;
				while (current) {
					if (i == index - 1) {
						old = current->next->next;
						delete current->next;
						current->next = old;
						countItem--;
						break;
					}
					i++;
					current = current->next;
				}
			}
		}
	}

	void insertItem(int index, Student* data) {
		if (not (index >= 0 and index <= countItem and countItem >= 0))
			return;
		if (index == 0)
			addItem(data);
		else {
			struct Student_list* current = head;
			for (int i = 0; i < index - 1; i++) {
				current = current->next;
			}
			struct Student_list* newItem = new Student_list();
			newItem->student = data;
			newItem->next = current->next;
			current->next = newItem;
			countItem++;
		}
	}

	void print(Student* s) {
		cout << s->lastname << endl;
		cout << s->name << endl;
		cout << s->surname << endl;
		cout << s->birthday.day << '.' << s->birthday.month << '.' << s->birthday.year << endl;
		cout << s->entered << endl;
		cout << s->faculty << endl;
		cout << s->department << endl;
		cout << s->group << endl;
		cout << s->b_number << endl;
		cout << s->gender << endl;
		for (int i = 0; i < 9; i++) {
			if (s->o_m_count[i] == 0) {
				break;
			}
			cout << "Сессия №" << s->session[i].session_number << endl;
			for (int j = 0; j < s->o_m_count[i]; j++) {
				cout << s->session[i].object_mark[j].object << ':' << s->session[i].object_mark[j].mark << endl;
			}
		}
	}

	void vivod_studentov() {
		Student_list* current = head;
		for (int i = 0; i < countItem; i++) {
			cout << endl << i + 1 << " -----------------------------" << endl;
			print(current->student);
			current = current->next;
		}
	}

	void delete_all() {
		while (countItem != 0) {
			this->deleteItem(0);
		}
	}

	void save_in_file() {
		FILE* File;
		Student_list* current = this->head;

		fopen_s(&File, "Data.bin", "w");
		for (int i = 0; i < this->getCountItem(); i++) {
			Student* st = current->student;
			string name = st->name;
			fwrite(&name, sizeof(string), 1, File);
			string lastname = st->lastname;
			fwrite(&lastname, sizeof(string), 1, File);
			string surname = st->surname;
			fwrite(&surname, sizeof(string), 1, File);
			int day = st->birthday.day;
			int month = st->birthday.month;
			int year = st->birthday.month;
			int entered = st->entered;
			string faculty = st->faculty;
			string department = st->department;
			string group = st->group;
			string b_number = st->b_number;
			string gender = st->gender;
			int session_count = st->session_count;
			int* o_m_count = st->o_m_count;
			fwrite(&day, sizeof(int), 1, File);
			fwrite(&month, sizeof(int), 1, File);
			fwrite(&year, sizeof(int), 1, File);
			fwrite(&entered, sizeof(int), 1, File);
			fwrite(&faculty, sizeof(string), 1, File);
			fwrite(&department, sizeof(string), 1, File);
			fwrite(&group, sizeof(string), 1, File);
			fwrite(&b_number, sizeof(string), 1, File);
			fwrite(&gender, sizeof(string), 1, File);

			Session session[9];
			for (int i = 0; i < 9; i++) {
				if (st->o_m_count[i] == 0) {
					break;
				}
				session[i].session_number = st->session[i].session_number;
				for (int j = 0; j < st->o_m_count[i]; j++) {
					session[i].object_mark[j].object = st->session[i].object_mark[j].object;
					session[i].object_mark[j].mark = st->session[i].object_mark[j].mark;
				}
			}
			fwrite(&session_count, sizeof(&session_count), 1, File);
			fwrite(&o_m_count, sizeof(&o_m_count), 1, File);
			fwrite(&session, sizeof(Session), 1, File);
			current = current->next;
		}
		fclose(File);
	}
};

class ClassEditData
{
private:
	string label;
	string data;
public:
	ClassEditData() {
		label = "";
		data = "";
	}
	void setLabel(string _label = "") {
		if (_label.length() > 1)
			this->label = _label;
		else
			label = "";
	}
	bool isDigit(char ch) {
		if (ch >= 48 and ch <= 57)
			return true;
		else
			return false;
	}
	bool isStringDigit(string _str) {
		for (int i = 0; i < _str.length(); i++)
			if (not isDigit(_str[i]))
				return false;
		if (_str.length() == 0)
			return false;
		return true;
	}
	bool isAlpha(int ch) {
		if (ch >= 65 and ch <= 90)
			return true;
		if (ch >= 97 and ch <= 122)
			return true;
		if (ch >= -200 and ch <= -1)
			return true;
		return false;
	}
	bool isSpace(char ch) {
		if (ch == 32)
			return true;
		else
			return false;
	}
	bool isSpec(char ch) {
		if (ch >= 33 and ch <= 47)
			return true;
		else
			return false;
	}
	void clear(string _data = "") {
		system("cls");
		data = _data;
	}

	string getData(enum class editType et) {
		char ch = 0;
		cout << label << endl << data;
		while (ch != 13) {
			ch = _getch();
			if (ch == 8) {
				if (data.length() > 0) {
					data.pop_back();
					system("cls");
					cout << endl;
					cout << label << endl << data;
				}
				continue;
			}
			if (et == editType::onlyDigit)
				if (isDigit(ch)) {
					cout << ch;
					data = data + ch;
				}
			if (et == editType::onlyAlpha)
				if (isAlpha(ch)) {
					cout << ch;
					data = data + ch;
				}
			if (et == editType::all) {
				cout << ch;
				data = data + ch;
			}
		}
		return data;
	}
	int getData(enum class editType et, int min, int max) {
		if (et == editType::onlyDigit) {
			getData(et);
			int num = max + 1;
			if (isStringDigit(data))
				num = atoi(data.c_str());
			if (not (num >= min and num < max)) {
				cout << endl << "Число " << num << " не корректно, введите число в промежутке (" << min << "; " << max - 1 << ") ";
				getData(et, min, max);
			}
			if (isStringDigit(data))
				num = atoi(data.c_str());
			return num;
		}
	}
};

void new_student(Student* s) {
	ClassEditData* cl = new ClassEditData();

	cl->setLabel("Фамилия студента:");
	string lastname = cl->getData(editType::onlyAlpha);
	s->lastname = lastname;
	cl->clear();

	cl->setLabel("Имя студента:");
	string name = cl->getData(editType::onlyAlpha);
	s->name = name;
	cl->clear();

	cl->setLabel("Отчество студента:");
	string surname = cl->getData(editType::onlyAlpha);
	s->surname = surname;
	cl->clear();

	cl->setLabel("Дата рождения студента:\nДень: ");
	int day = cl->getData(editType::onlyDigit, 1, 32);
	cl->clear();
	cl->setLabel("Дата рождения студента:\nМесяц: ");
	int month = cl->getData(editType::onlyDigit, 1, 13);
	cl->clear();
	cl->setLabel("Дата рождения студента:\nГод: ");
	int year = cl->getData(editType::onlyDigit, 1936, 2006);
	cl->clear();
	s->birthday.day = day;
	s->birthday.month = month;
	s->birthday.year = year;

	cl->setLabel("Год поступления:");
	int entered = cl->getData(editType::onlyDigit, 1936, 2023);
	s->entered = entered;
	cl->clear();

	cl->setLabel("Факультет:");
	string faculty = cl->getData(editType::onlyAlpha);
	s->faculty = faculty;
	cl->clear();

	cl->setLabel("Кафедра:");
	string department = cl->getData(editType::all);
	s->department = department;
	cl->clear();

	cl->setLabel("Группа:");
	string group = cl->getData(editType::all);
	s->group = group;
	cl->clear();

	cl->setLabel("Номер зачетной книжки:");
	string b_number = cl->getData(editType::all);
	s->b_number = b_number;
	cl->clear();

	cl->setLabel("Пол:");
	string gender = cl->getData(editType::onlyAlpha);
	s->gender = gender;
	cl->clear();

	for (int i = 0; i < 9; i++) {
		s->session[i].session_number = i + 1;
		for (int j = 0; j < 10; j++) {
			s->session[i].object_mark[j].object = "";
			s->session[i].object_mark[j].mark = "";
		}
	}

	string yn = "д";
	int session_count = 0;
	int o_m_count = 0;
	int i = 0;
	while (yn == "д" && session_count < 9) {
		cl->setLabel("Добавить сессию? (д/н)");
		string yn = cl->getData(editType::onlyAlpha);
		cl->clear();
		if (yn == "н") {
			break;
		}
		string yn_om = "д";
		while (yn_om == "д" && o_m_count < 10) {
			cl->setLabel("Добавить предмет?(д/н)");
			string yn_om = cl->getData(editType::onlyAlpha);
			cl->clear();
			if (yn_om == "н") {
				break;
			}
			string obj, ma;
			cl->setLabel("Название предмета: ");
			obj = cl->getData(editType::onlyAlpha);
			cl->clear();
			cl->setLabel("Оценка:");
			ma = cl->getData(editType::onlyAlpha);
			cl->clear();
			s->session[session_count].session_number = session_count + 1;
			s->session[session_count].object_mark[o_m_count].object = obj;
			s->session[session_count].object_mark[o_m_count].mark = ma;
			o_m_count++;
		}
		session_count++;
		s->o_m_count[i] = o_m_count;
		i++;
		o_m_count = 0;
	}
	s->session_count = session_count;
}

Student_list_class sort(Student_list_class slc) {
	int n = slc.getCountItem();
	Student_list_class new_slc;
	Student_list* current = slc.getHead();
	Student** new_sl = new Student * [n];

	for (int i = 0; i < n; i++) {
		Student* sl = current->student;
		new_sl[i] = sl;
		current = current->next;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (new_sl[i]->lastname != new_sl[j]->lastname) {
				if (new_sl[i]->lastname > new_sl[j]->lastname) {
					swap(new_sl[i], new_sl[j]);
				}
			}
			else {
				if ((new_sl[i]->lastname == new_sl[j]->lastname) && (new_sl[i]->name != new_sl[j]->name)) {
					if (new_sl[i]->name > new_sl[j]->name) {
						swap(new_sl[i], new_sl[j]);
					}
				}
				else {
					if ((new_sl[i]->lastname == new_sl[j]->lastname) && (new_sl[i]->name == new_sl[j]->name) && (new_sl[i]->surname != new_sl[j]->surname)) {
						if (new_sl[i]->surname > new_sl[j]->surname) {
							swap(new_sl[i], new_sl[j]);
						}
					}
				}
			}
		}
	}

	slc.delete_all();

	for (int i = 0; i < n; i++) {
		new_slc.addItem(new_sl[i]);
	}

	return new_slc;
}

void zadanie(Student_list_class slc) {
	cout << endl << "Задание" << endl;
	Student_list_class slc_m;
	Student_list_class slc_w;

	Student_list* current = slc.getHead();

	for (int i = 0; i < slc.getCountItem(); i++) {
		if (current->student->gender == "М" || current->student->gender == "Мужской"
			|| current->student->gender == "м" || current->student->gender == "мужской") {
			slc_m.addItem(current->student);
		}
		if (current->student->gender == "Ж" || current->student->gender == "Женский"
			|| current->student->gender == "ж" || current->student->gender == "женский") {
			slc_w.addItem(current->student);
		}
		current = current->next;
	}

	cout << slc_m.getCountItem() << " Мужчин" << endl;
	cout << slc_w.getCountItem() << " Женщин" << endl;

	cout << endl;

	if (slc_m.getCountItem() != 0) {
		slc_m = sort(slc_m);
		cout << endl << "Мужчины(" << slc_m.getCountItem() << "):" << endl << endl;
		slc_m.vivod_studentov();
		cout << endl << endl;
	}
	else {
		cout << "Мужчин нет" << endl << endl;
	}

	if (slc_w.getCountItem() != 0) {
		slc_w = sort(slc_w);
		cout << endl << "Женщины(" << slc_w.getCountItem() << "):" << endl << endl;
		slc_w.vivod_studentov();
		cout << endl << endl;
	}
	else {
		cout << "Женщин нет" << endl << endl;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	ClassEditData* cl = new ClassEditData();
	Student_list_class slc;
	while (true) {
		cl->setLabel("Введите:\n1 - Добавить студента\n2 - Удалить данные о студенте\n3 - Вывесли список студентов\n4 - Выполнить задание\n5 - Выйти из программы");
		int n = cl->getData(editType::onlyDigit, 1, 6);
		cl->clear();
		if (n == 1) {
			Student* st = new Student;
			new_student(st);
			slc.addItem(st);
		}
		if (n == 2) {
			if (slc.getCountItem() == 0) {
				cout << "Студентов нет" << endl;
			}
			else {
				slc.vivod_studentov();
				cl->setLabel("Введите номер студента:");
				int index = cl->getData(editType::onlyDigit, 1, slc.getCountItem() + 1);
				cl->clear();
				slc.deleteItem(index - 1);
			}
		}
		if (n == 3) {
			cout << "Все студенты:" << endl;
			slc.vivod_studentov();
		}
		if (n == 4) {
			zadanie(slc);
		}
		if (n == 5) {
			break;
		}
	}
	slc.save_in_file();
	cout << endl << "Конец выполнения программы!!!";
}
