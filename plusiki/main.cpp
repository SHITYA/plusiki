#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// Пол
enum Gender { MALE, FEMALE };

// Дата рождения
struct Birthday {
    int day;
    int month;
    int year;
};

// анкета ребёнка
struct Child {
    string surname;
    string name;
    Gender gender;
    double height;
    Birthday date;
};

// Вспомогательная структура для бинарной записи
struct ChildBinary {
    char surname[50];
    char name[50];
    Gender gender;
    double height;
    Birthday date;
};

string genderString(Gender gender) {
    return (gender == MALE) ? "М" : "Д";
}

// Вывод одного ученика
void printChild(const Child& c) {
    cout << c.surname << " " << c.name << endl;
    cout << "Пол: " << genderString(c.gender) << endl;
    cout << "Рост: " << c.height << endl;
    cout << "Дата рождения: "
         << c.date.day << "."
         << c.date.month << "."
         << c.date.year << endl;
}

// Обертка вывода массива
void printArray(Child arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "\n---\n";
        printChild(arr[i]);
    }
}

// Средний рост мальчиков
double avgHeightBoys(Child arr[], int n) {
    double sum = 0;
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i].gender == MALE) {
            sum += arr[i].height;
            count++;
        }
    }
    return (count == 0) ? 0 : sum / count;
}

// 5 самых высоких девочек
void top5Girls(Child arr[], int n, Child result[], int& size) {
    Child girls[20];
    int gCount = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i].gender == FEMALE) {
            girls[gCount++] = arr[i];
        }
    }

    // сортировка по убыванию роста
    for (int i = 0; i < gCount - 1; i++) {
        for (int j = 0; j < gCount - i - 1; j++) {
            if (girls[j].height < girls[j + 1].height) {
                swap(girls[j], girls[j + 1]);
            }
        }
    }

    size = (gCount < 5) ? gCount : 5;
    for (int i = 0; i < size; i++) {
        result[i] = girls[i];
    }
}

// Сортировка по фамилии
void sortBySurname(Child arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].surname > arr[j + 1].surname) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Поиск по фамилии
void findBySurname(Child arr[], int n, string target) {
    for (int i = 0; i < n; i++) {
        if (arr[i].surname == target) {
            printChild(arr[i]);
            return;
        }
    }
    cout << "Не найден.\n";
}

// Изменение данных
void editChild(Child arr[], int n, string target) {
    for (int i = 0; i < n; i++) {
        if (arr[i].surname == target) {
            cout << "Введите новый рост: ";
            cin >> arr[i].height;
            cout << "Введите новую дату рождения (д м г): ";
            cin >> arr[i].date.day >> arr[i].date.month >> arr[i].date.year;
            return;
        }
    }
    cout << "Не найден.\n";
}

// Дни рождения в текущем месяце
void birthdayMonth(Child arr[], int n, int month, Child result[], int& size) {
    size = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i].date.month == month) {
            result[size++] = arr[i];
        }
    }
}



// --- ПОДПРОГРАММЫ (Занятие 10) ---

// Задание 1. Чтение числового поля из текстового файла и обновление массива
void updateGenderFromFile(Child arr[], int n, string path) {
    ifstream fin(path); // Используем библиотеку fstream [cite: 5]
    if (!fin.is_open()) {
        cout << ">>> Ошибка: текстовый файл " << path << " не найден!" << endl;
        return;
    }

    cout << "\n--- Занятие 10 ---" << endl;
    string fileSurname;
    int fileGenderValue;
    bool anyChanges = false;

    // Считываем данные: фамилия и число (0 или 1) [cite: 13, 15]
    while (fin >> fileSurname >> fileGenderValue) {
        for (int i = 0; i < n; i++) {
            if (arr[i].surname == fileSurname) {
                Gender newGender = (fileGenderValue == 0) ? MALE : FEMALE;
                
                if (arr[i].gender != newGender) {
                    cout << "-- Изменения --" << endl;
                    cout  << arr[i].surname << ": "
                         << (arr[i].gender == MALE ? "М" : "Д") << " -> "
                         << (newGender == MALE ? "М" : "Д") << endl;
                    arr[i].gender = newGender;
                    anyChanges = true;
                }
                break;
            }
        }
    }
    if (!anyChanges) cout << "Изменений не зафиксировано." << endl;
    fin.close();
}

// Задание 2. Запись в бинарный файл
void saveToBinary(Child arr[], int n, string path) {
    ofstream out(path, ios::binary | ios::out);
    if (!out.is_open()) return;

    for (int i = 0; i < n; i++) {
        ChildBinary temp;
        // Ручное копирование строк в char-массивы
        for (int j = 0; j < 49; j++) {
            if (j < (int)arr[i].surname.length()) temp.surname[j] = arr[i].surname[j];
            else temp.surname[j] = '\0';
            
            if (j < (int)arr[i].name.length()) temp.name[j] = arr[i].name[j];
            else temp.name[j] = '\0';
        }
        temp.surname[49] = temp.name[49] = '\0';
        temp.gender = arr[i].gender;
        temp.height = arr[i].height;
        temp.date = arr[i].date;
        
        // Запись объекта целиком
        out.write((char*)&temp, sizeof(ChildBinary));
    }
    out.close();
    cout << "Данные сохранены в бинарный файл: " << path << endl;
}

// Задание 2. Чтение из бинарного файла
void loadFromBinary(Child arr[], int& n, string path) {
    ifstream in(path, ios::binary | ios::in);
    if (!in.is_open()) return;

    n = 0;
    ChildBinary temp;
    while (in.read((char*)&temp, sizeof(ChildBinary))) {
        arr[n].surname = temp.surname;
        arr[n].name = temp.name;
        arr[n].gender = temp.gender;
        arr[n].height = temp.height;
        arr[n].date = temp.date;
        n++;
    }
    in.close();
    cout << "Данные загружены из бинарного файла. Записей: " << n << endl;
}


int main() {
    setlocale(LC_ALL, "Russian");
    const int N = 20;

// ПУТЬ К ПАПКЕ
    string folder = "/Users/artem/coding/plusiki/plusiki/";
    string txtPath = folder + "genders.txt";
    string binPath = folder + "students.dat";

    Child students[N] = {
        {"Ivanov", "Ivan", MALE, 145.0, {10, 3, 2010}},
        {"Petrov", "Petr", MALE, 150.5, {22, 7, 2009}},
        {"Sidorova", "Anna", FEMALE, 148.0, {15, 4, 2010}},
        {"Kozlova", "Maria", FEMALE, 152.3, {5, 11, 2009}},
        {"Novikov", "Dmitry", MALE, 147.8, {18, 4, 2010}},
        {"Smirnova", "Elena", FEMALE, 149.5, {25, 4, 2009}},
        {"Volkov", "Alexey", MALE, 151.0, {12, 8, 2010}},
        {"Lebedeva", "Olga", FEMALE, 146.7, {30, 4, 2009}},
        {"Sokolov", "Nikolay", MALE, 153.2, {8, 12, 2010}},
        {"Pavlova", "Tatiana", FEMALE, 144.9, {20, 4, 2009}},
        {"Kuznetsov", "Andrey", MALE, 148.5, {15, 6, 2010}},
        {"Morozova", "Natalia", FEMALE, 150.8, {3, 4, 2009}},
        {"Fedorov", "Sergey", MALE, 149.2, {28, 9, 2010}},
        {"Mikhailova", "Irina", FEMALE, 147.3, {14, 4, 2009}},
        {"Semenov", "Vladimir", MALE, 152.7, {7, 1, 2010}},
        {"Golubeva", "Svetlana", FEMALE, 145.8, {19, 10, 2009}},
        {"Popov", "Mikhail", MALE, 150.5, {25, 4, 2010}},
        {"Vasilieva", "Yulia", FEMALE, 148.9, {11, 5, 2009}},
        {"Grigoriev", "Konstantin", MALE, 151.5, {2, 7, 2010}},
        {"Alekseeva", "Marina", FEMALE, 146.2, {16, 4, 2009}}
    };


    // 1. Средний рост мальчиков
    cout << "\nСредний рост мальчиков: " << avgHeightBoys(students, N) << endl;
        
    // 2. Топ-5 девочек
    Child topGirls[5];
    int topSize;
    top5Girls(students, N, topGirls, topSize);

    cout << "\nТоп-5 самых высоких девочек:\n";
    printArray(topGirls, topSize);

    // 3. Сортировка
    sortBySurname(students, N);
    cout << "\nОтсортированный список:\n";
    printArray(students, N);

    // 4. Поиск
    string surname;
    cout << "\nВведите фамилию: ";
    cin >> surname;
    findBySurname(students, N, surname);

    // 5. Изменение
    cout << "\nВведите фамилию для изменения: ";
    cin >> surname;
    editChild(students, N, surname);

    // 6. Фильтр по месяцу
    int month;
    cout << "\nВведите месяц (например: 7): ";
    cin >> month;

    Child birthdayList[20];
    int bSize;
    birthdayMonth(students, N, month, birthdayList, bSize);

    cout << "\nДни рождения в этом месяце:\n";
    printArray(birthdayList, bSize);
    
    
    // Занятие 10
    // Задания 1 (Текстовый файл)
    updateGenderFromFile(students, N, txtPath);

    // 2. Задания 2 (Бинарная запись)
    saveToBinary(students, N, binPath);

    // 3. Проверка: загрузка из бинарного файла в новый массив
    Child loadedStudents[N];
    int loadedCount = 0;
    loadFromBinary(loadedStudents, loadedCount, binPath);
    
    cout << "\n--- Проверка данных из бинарного файла ---" << endl;
    printArray(loadedStudents, 3); // Выведем первых трех для теста
    return 0;
}



