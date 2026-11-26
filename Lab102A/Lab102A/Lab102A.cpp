#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// спеціальності оголошені в алфавітному порядку їх назв,
// тому порівняння значень переліку відповідає порівнянню назв
enum Spec { KN, INFO, TN, ME, FI };

struct Student
{
    string prizv;    // прізвище
    int kurs;        // курс
    Spec spec;       // спеціальність
    int fizyka;      // оцінка з фізики
    int matem;       // оцінка з математики
    int inform;      // оцінка з інформатики
};

int ReadInt(const string& prompt, const int low, const int high);
Spec ReadSpec();
Student ReadKey();
void Create(Student* s, const int N);
void PrintMenu();
void PrintHeader();
void PrintRow(const Student& s, const int number);
void Print(const Student* s, const int N);
bool IsBefore(const Student& a, const Student& b);
void Sort(Student* s, const int N);
int* IndexSort(const Student* s, const int N);
void PrintIndexSorted(const Student* s, const int* I, const int N);
int BinSearch(const Student* s, const int* I, const int N, const Student& key);

int main()
{
    int N = ReadInt("Enter the number of students: ", 1, 100);
    Student* s = new Student[N];
    Create(s, N);

    int menuItem;

    do
    {
        PrintMenu();
        menuItem = ReadInt("Your choice: ", 0, 5);

        switch (menuItem)
        {
        case 1:
            Create(s, N);
            break;
        case 2:
            Print(s, N);
            break;
        case 3:
            Sort(s, N);
            cout << "The array has been sorted." << endl;
            break;
        case 4:
        {
            int* I = IndexSort(s, N);
            PrintIndexSorted(s, I, N);
            delete[] I;
            break;
        }
        case 5:
        {
            Student key = ReadKey();
            int* I = IndexSort(s, N);
            int found = BinSearch(s, I, N, key);
            delete[] I;

            if (found != -1)
                cout << "Student found in row " << found + 1
                << " of the main table." << endl;
            else
                cout << "Student not found." << endl;
            break;
        }
        }
    } while (menuItem != 0);

    delete[] s;
    return 0;
}

// зчитує ціле число з проміжку [low; high] з контролем помилок
int ReadInt(const string& prompt, const int low, const int high)
{
    int value;
    cout << prompt;

    while (!(cin >> value) || cin.peek() != '\n' || value < low || value > high)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Error! Enter a number from " << low << " to " << high << ": ";
    }

    return value;
}

// зчитує спеціальність
Spec ReadSpec()
{
    cout << " majors: 0 - Computer Science, 1 - Informatics," << endl
        << " 2 - Labor Training, 3 - Mathematics and Economics," << endl
        << " 4 - Physics and Informatics" << endl;

    return static_cast<Spec>(ReadInt(" major: ", 0, 4));
}

// зчитує ключі пошуку
Student ReadKey()
{
    Student key{};

    cout << "Enter the search keys:" << endl;
    key.spec = ReadSpec();
    key.matem = ReadInt(" Mathematics mark (2-5): ", 2, 5);
    cout << " last name: ";
    getline(cin >> ws, key.prizv);

    return key;
}

// формує масив студентів
void Create(Student* s, const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << endl << "Student No. " << i + 1 << ":" << endl;

        cout << " last name: ";
        getline(cin >> ws, s[i].prizv);

        s[i].kurs = ReadInt(" year of study (1-6): ", 1, 6);
        s[i].spec = ReadSpec();
        s[i].fizyka = ReadInt(" Physics mark (2-5): ", 2, 5);
        s[i].matem = ReadInt(" Mathematics mark (2-5): ", 2, 5);
        s[i].inform = ReadInt(" Informatics mark (2-5): ", 2, 5);
    }
}

// виводить меню
void PrintMenu()
{
    cout << endl << "Choose an action:" << endl
        << " [1] - re-enter data" << endl
        << " [2] - display data" << endl
        << " [3] - physical sorting" << endl
        << " [4] - index sorting and output" << endl
        << " [5] - binary search" << endl
        << " [0] - exit" << endl;
}

// виводить заголовок і шапку таблиці
void PrintHeader()
{
    const string line(80, '=');

    cout << endl << "List of students" << endl;
    cout << line << endl;
    cout << left
        << "| " << setw(3) << "No."
        << " | " << setw(14) << "Last name"
        << " | " << setw(4) << "Year"
        << " | " << setw(25) << "Major"
        << " | " << setw(4) << "Phys"
        << " | " << setw(4) << "Math"
        << " | " << setw(4) << "Info"
        << " |" << endl;
    cout << line << endl;
}

// виводить один рядок таблиці
void PrintRow(const Student& s, const int number)
{
    const string specName[] = { "Computer Science", "Informatics",
        "Labor Training", "Mathematics and Economics", "Physics and Informatics" };

    cout << left
        << "| " << setw(3) << number
        << " | " << setw(14) << s.prizv
        << " | " << setw(4) << s.kurs
        << " | " << setw(25) << specName[s.spec]
        << " | " << setw(4) << s.fizyka
        << " | " << setw(4) << s.matem
        << " | " << setw(4) << s.inform
        << " |" << endl;
}

// виводить масив студентів у вигляді таблиці
void Print(const Student* s, const int N)
{
    PrintHeader();

    for (int i = 0; i < N; i++)
        PrintRow(s[i], i + 1);

    cout << string(80, '=') << endl;
}

// перевіряє, чи має студент a стояти перед студентом b
bool IsBefore(const Student& a, const Student& b)
{
    if (a.spec != b.spec)
        return a.spec < b.spec;

    if (a.matem != b.matem)
        return a.matem < b.matem;

    return a.prizv < b.prizv;
}

// фізичне впорядкування методом "бульбашки"
void Sort(Student* s, const int N)
{
    for (int i = 0; i < N - 1; i++)
        for (int j = 0; j < N - i - 1; j++)
            if (IsBefore(s[j + 1], s[j]))
                swap(s[j], s[j + 1]);
}

// будує індексний масив методом вставки
int* IndexSort(const Student* s, const int N)
{
    int* I = new int[N];

    for (int i = 0; i < N; i++)
        I[i] = i;

    for (int i = 1; i < N; i++)
    {
        int value = I[i];
        int j = i - 1;

        while (j >= 0 && IsBefore(s[value], s[I[j]]))
        {
            I[j + 1] = I[j];
            j--;
        }

        I[j + 1] = value;
    }

    return I;
}

// виводить масив у порядку, заданому індексним масивом
void PrintIndexSorted(const Student* s, const int* I, const int N)
{
    PrintHeader();

    for (int i = 0; i < N; i++)
        PrintRow(s[I[i]], i + 1);

    cout << string(80, '=') << endl;
}

// бінарний пошук: повертає індекс студента в масиві s або -1
int BinSearch(const Student* s, const int* I, const int N, const Student& key)
{
    int L = 0, R = N - 1;

    while (L <= R)
    {
        int m = (L + R) / 2;

        if (IsBefore(s[I[m]], key))
            L = m + 1;
        else if (IsBefore(key, s[I[m]]))
            R = m - 1;
        else
            return I[m];
    }

    return -1;
}