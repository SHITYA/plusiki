#include <iostream>
#include <string>
using namespace std;


int main() {
    setlocale(LC_ALL, "Russian");
    
    cout << "21 вариант\n--- Задание на массивы ---\n";
    
    // объявляем переменные на количество и считывааем значения
    int N, K;
    cout << "Введите количество элементов N массива: ";
    cin >> N;
    cout << "Введите количество блоков K: ";
    cin >> K;
    // объявляем массив и считываем значения
    int* masiv = new int[N];
    cout << "Введите " << N << " элементов массива: ";
    for (int i = 0; i < N; i++) {
        cin >> masiv[i];
    }
    
    //размер блока
    int blockSize = N / K;
    //остаток
    int remainder = N % K;
    
    int start = 0;
    for (int block = 0; block < K; block++) {
        // в первых блоках количество может быть на 1 больше, чем в остальных
        int current_blockSize = blockSize + (block < remainder ? 1:0);
        // перемещаем точку на следующий блок
        int end = start + current_blockSize;
        // Сортировка текущего блока
        for (int i = 0; i < current_blockSize - 1; i++) {
            for (int j = start; j < end - 1 - i; j++) {
                if (masiv[j] > masiv[j + 1]) {
                    // Обмен значениями
                    int temp = masiv[j];
                    masiv[j] = masiv[j + 1];
                    masiv[j + 1] = temp;
                    }
                }
            }
        start = end;
    }
    // Вывод результата
    cout << "\nОтсортированный по блокам массив:\n";
    start = 0;
    for (int block = 0; block < K; block++) {
        int currentBlockSize = blockSize + (block < remainder ? 1 : 0);
        int end = start + currentBlockSize;

        cout << "Блок " << block + 1 << ": ";
        for (int i = start; i < end; i++) {
            cout << masiv[i] << " ";
        }
        cout << endl;
        start = end;
    }
    delete[] masiv;
    
    
    
    
    cout << "\n\n--- Задание на строки ---\n";
    int L;
    cout << "Введите количество запрещенных слов: ";
    cin >> L;
    // считываем запрещенные слова
    string* badWords = new string[L];
    cout << "Введите запрещенные слова: ";
    for (int i = 0; i < L; i++) {
        cin >> badWords[i];
    }

    // считываем текст
    cout << "Введите текст: ";
    // очищаем бувер от "\n"
    cin.ignore();
    string text;
    getline(cin, text);

    // обработка текста
    for (int i = 0; i < L; i++) {
        size_t pos = 0;

        while ((pos = text.find(badWords[i], pos)) != string::npos) {
            
            // считываем количество букв (для русского языка)
            int count = 0;
            for (int j = 0; j < badWords[i].length(); j++) {
                if ((badWords[i][j] & 0xC0) != 0x80) {
                    count++;
                }
            }
            
            // создаём строку из '*'
            string stars(count, '*');

            // замена
            text.replace(pos, badWords[i].length(), stars);
            pos += stars.length();
        }
    }
    // вывод исправленного текста
    cout << text << endl;
    delete[] badWords;

    return 0;
    
}




