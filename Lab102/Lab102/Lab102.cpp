// Lab_10_2_B.cpp
// Лабораторна робота № 10.2, варіант 5, рівень В
// Впорядкування та бінарний пошук в масиві структур з об'єднаннями
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <cstdlib>
#include <Windows.h> 
using namespace std;

// Спеціальності перелічені в алфавітному порядку їх назв,
// тому порівняння значень переліку еквівалентне порівнянню назв.
enum Specialty { INFORMATYKA, KOMP_NAUKY, MATEM_EKONOMIKA, TRUDOVE_NAVCHANNYA, FIZ_INFORMATYKA };
const int SPECIALTY_COUNT = 5;
const string specialtyStr[SPECIALTY_COUNT] = {
    "Інформатика",
    "Комп'ютерні науки",
    "Математика та економіка",
    "Трудове навчання",
    "Фізика та інформатика"
};

struct Student
{
    string    prizv;
    int       kurs;
    Specialty spec;
    int       physics;
    int       math;
    union // третя оцінка залежить від спеціальності
    {
        int programming;      // Комп'ютерні науки
        int numericalMethods; // Інформатика
        int pedagogy;         // Математика та економіка, Фізика та інформатика, Трудове навчання
    };
};

const int MAX_PRIZV_LEN = 14;  // ширина стовпця "Прізвище" в таблиці
const int TABLE_WIDTH = 115; // сума ширин усіх стовпців разом з роздільниками

// введення з контролем помилок користувача
int       ReadInt(const string& prompt, const int lo, const int hi);
string    ReadSurname(const string& prompt);
Specialty ReadSpecialty();
void      ReadSearchKeys(string& prizv, Specialty& spec, int& math);

// формування масиву
void Create(Student* p, const int N);

// вивід
void   PrintLine(const char c);
void   PrintHeader();
string MarkOrBlank(const bool active, const int mark);
void   PrintRow(const int number, const Student& s);
void   Print(const Student* p, const int N);
void   PrintIndexSorted(const Student* p, const int* I, const int N);
void   PrintSearchResult(const int found);

// порівняння, впорядкування, пошук
int  CompareKeys(const Student& s, const Specialty spec, const int math, const string& prizv);
int  Compare(const Student& a, const Student& b);
void SortPhysical(Student* p, const int N);
int* BuildIndex(const Student* p, const int N);
int  BinSearch(const Student* p, const int N,
    const string& prizv, const Specialty spec, const int math);
int  IndexBinSearch(const Student* p, const int* I, const int N,
    const string& prizv, const Specialty spec, const int math);

int main()
{ // забезпечення відображення кирилиці:
    SetConsoleCP(1251);       // встановлення сторінки win-cp1251 в потік вводу
    SetConsoleOutputCP(1251); // встановлення сторінки win-cp1251 в потік виводу

    const int N = ReadInt("Введіть кількість студентів N: ", 1, 1000);
    Student* p = new Student[N]();
    int* I = nullptr;               // індексний масив (nullptr, якщо не побудований)
    bool physicallySorted = false;  // чи впорядкований масив фізично

    string prizv;
    Specialty spec = INFORMATYKA;
    int math = 0;
    int menuItem;
    do
    {
        cout << endl << "Виберіть дію:" << endl << endl;
        cout << " [1] - введення даних з клавіатури" << endl;
        cout << " [2] - вивід даних на екран" << endl;
        cout << " [3] - фізичне впорядкування даних" << endl;
        cout << " [4] - індексне впорядкування та вивід даних" << endl;
        cout << " [5] - бінарний пошук у фізично впорядкованому масиві" << endl;
        cout << " [6] - бінарний пошук за індексним масивом" << endl << endl;
        cout << " [0] - вихід та завершення роботи програми" << endl << endl;
        menuItem = ReadInt("Введіть значення: ", 0, 6);
        cout << endl;

        switch (menuItem)
        {
        case 1:
            Create(p, N);
            delete[] I;               // старий індексний масив більше не відповідає даним
            I = nullptr;
            physicallySorted = false;
            break;
        case 2:
            Print(p, N);
            break;
        case 3:
            SortPhysical(p, N);
            delete[] I;               // елементи змінили позиції, індекси недійсні
            I = nullptr;
            physicallySorted = true;
            break;
        case 4:
            delete[] I;
            I = BuildIndex(p, N);
            PrintIndexSorted(p, I, N);
            break;
        case 5:
            if (!physicallySorted)
            {
                cout << "Масив не впорядковано фізично. Спочатку виконайте пункт [3]." << endl;
                break;
            }
            ReadSearchKeys(prizv, spec, math);
            PrintSearchResult(BinSearch(p, N, prizv, spec, math));
            break;
        case 6:
            if (I == nullptr)
            {
                cout << "Індексний масив не побудовано. Спочатку виконайте пункт [4]." << endl;
                break;
            }
            ReadSearchKeys(prizv, spec, math);
            PrintSearchResult(IndexBinSearch(p, I, N, prizv, spec, math));
            break;
        case 0:
            break;
        }
    } while (menuItem != 0);

    delete[] I;
    delete[] p;
    return 0;
}

// Читає ціле число з діапазону [lo; hi], повторює запит при помилці
int ReadInt(const string& prompt, const int lo, const int hi)
{
    int x;
    while (true)
    {
        cout << prompt;
        const bool ok = static_cast<bool>(cin >> x);
        if (!ok && cin.eof())
            exit(EXIT_FAILURE);   // вхідний потік закінчився
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n'); // відкидаємо решту рядка
        if (ok && x >= lo && x <= hi)
            return x;
        cout << "Помилка: введіть ціле число від " << lo << " до " << hi << "." << endl;
    }
}

// Читає непорожнє прізвище довжиною до MAX_PRIZV_LEN символів
string ReadSurname(const string& prompt)
{
    string s;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, s))
            exit(EXIT_FAILURE);
        if (!s.empty() && s.length() <= MAX_PRIZV_LEN)
            return s;
        cout << "Помилка: прізвище має містити від 1 до "
            << MAX_PRIZV_LEN << " символів." << endl;
    }
}

Specialty ReadSpecialty()
{
    for (int i = 0; i < SPECIALTY_COUNT; i++)
        cout << "    " << i << " - " << specialtyStr[i] << endl;
    return static_cast<Specialty>(
        ReadInt("  спеціальність (номер зі списку): ", 0, SPECIALTY_COUNT - 1));
}

void ReadSearchKeys(string& prizv, Specialty& spec, int& math)
{
    cout << "Введіть ключі пошуку:" << endl;
    prizv = ReadSurname("  прізвище: ");
    spec = ReadSpecialty();
    math = ReadInt("  оцінка з математики (1-5): ", 1, 5);
    cout << endl;
}

void Create(Student* p, const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << "Студент № " << i + 1 << ":" << endl;
        p[i].prizv = ReadSurname("  прізвище: ");
        p[i].kurs = ReadInt("  курс (1-6): ", 1, 6);
        p[i].spec = ReadSpecialty();
        p[i].physics = ReadInt("  оцінка з фізики (1-5): ", 1, 5);
        p[i].math = ReadInt("  оцінка з математики (1-5): ", 1, 5);
        switch (p[i].spec)
        {
        case KOMP_NAUKY:
            p[i].programming = ReadInt("  оцінка з програмування (1-5): ", 1, 5);
            break;
        case INFORMATYKA:
            p[i].numericalMethods = ReadInt("  оцінка з чисельних методів (1-5): ", 1, 5);
            break;
        case MATEM_EKONOMIKA:
        case FIZ_INFORMATYKA:
        case TRUDOVE_NAVCHANNYA:
            p[i].pedagogy = ReadInt("  оцінка з педагогіки (1-5): ", 1, 5);
            break;
        }
        cout << endl;
    }
}

void PrintLine(const char c)
{
    cout << string(TABLE_WIDTH, c) << endl;
}

void PrintHeader()
{
    PrintLine('=');
    cout << "| " << setw(3) << left << "№"
        << " | " << setw(14) << left << "Прізвище"
        << " | " << setw(4) << left << "Курс"
        << " | " << setw(23) << left << "Спеціальність"
        << " | " << setw(6) << left << "Фізика"
        << " | " << setw(10) << left << "Математика"
        << " | " << setw(9) << left << "Програм."
        << " | " << setw(9) << left << "Числ.мет."
        << " | " << setw(9) << left << "Педагог."
        << " |" << endl;
    PrintLine('-');
}

// Повертає оцінку у вигляді рядка або порожній рядок, якщо клітинка не заповнюється
string MarkOrBlank(const bool active, const int mark)
{
    return active ? to_string(mark) : "";
}

void PrintRow(const int number, const Student& s)
{
    const bool isPedagogy = (s.spec != KOMP_NAUKY && s.spec != INFORMATYKA);
    cout << "| " << setw(3) << right << number
        << " | " << setw(14) << left << s.prizv
        << " | " << setw(4) << right << s.kurs
        << " | " << setw(23) << left << specialtyStr[s.spec]
        << " | " << setw(6) << right << s.physics
        << " | " << setw(10) << right << s.math
        << " | " << setw(9) << right << MarkOrBlank(s.spec == KOMP_NAUKY, s.programming)
        << " | " << setw(9) << right << MarkOrBlank(s.spec == INFORMATYKA, s.numericalMethods)
        << " | " << setw(9) << right << MarkOrBlank(isPedagogy, s.pedagogy)
        << " |" << endl;
}

void Print(const Student* p, const int N)
{
    cout << "Список студентів (порядок елементів у масиві):" << endl;
    PrintHeader();
    for (int i = 0; i < N; i++)
        PrintRow(i + 1, p[i]);
    PrintLine('=');
    cout << endl;
}

// Те саме, що Print(), але елементи береться через індексний масив: p[I[i]]
void PrintIndexSorted(const Student* p, const int* I, const int N)
{
    cout << "Список студентів (індексне впорядкування):" << endl;
    PrintHeader();
    for (int i = 0; i < N; i++)
        PrintRow(i + 1, p[I[i]]);
    PrintLine('=');
    cout << endl;
}

void PrintSearchResult(const int found)
{
    if (found != -1)
        cout << "Студента знайдено, номер у масиві даних: " << found + 1 << endl;
    else
        cout << "Студента з такими ключами не знайдено" << endl;
}

// Порівнює студента s з ключем (спеціальність, математика, прізвище).
// Повертає -1, якщо s менший за ключ, 0, якщо рівний, 1, якщо більший.
int CompareKeys(const Student& s, const Specialty spec, const int math, const string& prizv)
{
    if (s.spec != spec)     return s.spec < spec ? -1 : 1;
    if (s.math != math)     return s.math < math ? -1 : 1;
    if (s.prizv != prizv)   return s.prizv < prizv ? -1 : 1;
    return 0;
}

int Compare(const Student& a, const Student& b)
{
    return CompareKeys(a, b.spec, b.math, b.prizv);
}

// Фізичне впорядкування: спеціальність, оцінка з математики, прізвище (за зростанням)
void SortPhysical(Student* p, const int N)
{
    Student tmp;
    for (int i0 = 0; i0 < N - 1; i0++) // метод "бульбашки"
        for (int i1 = 0; i1 < N - i0 - 1; i1++)
            if (Compare(p[i1], p[i1 + 1]) > 0)
            {
                tmp = p[i1];
                p[i1] = p[i1 + 1];
                p[i1 + 1] = tmp;
            }
}

// Індексне впорядкування (метод вставки): основний масив не змінюється.
// Повертає новий масив, який потрібно знищити через delete[].
int* BuildIndex(const Student* p, const int N)
{
    int* I = new int[N];
    for (int i = 0; i < N; i++)
        I[i] = i;
    for (int i = 1; i < N; i++)
    {
        const int value = I[i];
        int j = i - 1;
        while (j >= 0 && Compare(p[I[j]], p[value]) > 0)
        {
            I[j + 1] = I[j];
            j--;
        }
        I[j + 1] = value;
    }
    return I;
}

// Бінарний пошук у фізично впорядкованому масиві.
// Повертає індекс знайденого елемента або -1.
int BinSearch(const Student* p, const int N,
    const string& prizv, const Specialty spec, const int math)
{
    int L = 0, R = N - 1;
    while (L <= R)
    {
        const int m = (L + R) / 2;
        const int c = CompareKeys(p[m], spec, math, prizv);
        if (c == 0)
            return m;
        if (c < 0)
            L = m + 1;
        else
            R = m - 1;
    }
    return -1;
}

// Бінарний пошук за індексним масивом.
// Повертає індекс знайденого елемента в основному масиві p (тобто I[m]) або -1.
int IndexBinSearch(const Student* p, const int* I, const int N,
    const string& prizv, const Specialty spec, const int math)
{
    int L = 0, R = N - 1;
    while (L <= R)
    {
        const int m = (L + R) / 2;
        const int c = CompareKeys(p[I[m]], spec, math, prizv);
        if (c == 0)
            return I[m];
        if (c < 0)
            L = m + 1;
        else
            R = m - 1;
    }
    return -1;
}