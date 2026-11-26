#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

enum Spec { KN, INFO, ME, FI, TN };   // спеціальності

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
void Create(Student* s, const int N);
void Print(const Student* s, const int N);
bool IsExcellent(const Student& s);
double PercentExcellent(const Student* s, const int N);
void PrintPhysicsFive(const Student* s, const int N);

int main()
{
    int N = ReadInt("Enter the number of students: ", 1, 100);
    Student* s = new Student[N];

    Create(s, N);
    Print(s, N);

    double percent = PercentExcellent(s, N);
    cout << "Percentage of students with excellent marks: "
        << fixed << setprecision(2) << percent << "%" << endl;

    PrintPhysicsFive(s, N);

    delete[] s;
    return 0;
}

// зчитує ціле число з проміжку [low; high]
int ReadInt(const string& prompt, const int low, const int high)
{
    int value;
    cout << prompt;

    while (!(cin >> value) || value < low || value > high)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Error! Enter a number from " << low << " to " << high << ": ";
    }

    return value;
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

        cout << " majors: 0 - Computer Science, 1 - Informatics," << endl
            << " 2 - Mathematics and Economics, 3 - Physics and Informatics," << endl
            << " 4 - Labor Training" << endl;
        s[i].spec = static_cast<Spec>(ReadInt(" major: ", 0, 4));

        s[i].fizyka = ReadInt(" Physics mark (2-5): ", 2, 5);
        s[i].matem = ReadInt(" Mathematics mark (2-5): ", 2, 5);
        s[i].inform = ReadInt(" Informatics mark (2-5): ", 2, 5);
    }
}

// виводить масив студентів у вигляді таблиці
void Print(const Student* s, const int N)
{
    const string specName[] = { "Computer Science", "Informatics",
        "Mathematics and Economics", "Physics and Informatics", "Labor Training" };
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

    for (int i = 0; i < N; i++)
    {
        cout << "| " << setw(3) << i + 1
            << " | " << setw(14) << s[i].prizv
            << " | " << setw(4) << s[i].kurs
            << " | " << setw(25) << specName[s[i].spec]
            << " | " << setw(4) << s[i].fizyka
            << " | " << setw(4) << s[i].matem
            << " | " << setw(4) << s[i].inform
            << " |" << endl;
    }

    cout << line << endl << endl;
}

// перевіряє, чи всі оцінки студента відмінні
bool IsExcellent(const Student& s)
{
    return s.fizyka == 5 && s.matem == 5 && s.inform == 5;
}

// обчислює процент студентів, які вчаться на "відмінно"
double PercentExcellent(const Student* s, const int N)
{
    if (N <= 0)
        return 0;

    int count = 0;

    for (int i = 0; i < N; i++)
        if (IsExcellent(s[i]))
            count++;

    return 100.0 * count / N;
}

// виводить прізвища студентів з оцінкою "5" з фізики
void PrintPhysicsFive(const Student* s, const int N)
{
    cout << endl << "Students with mark \"5\" in Physics:" << endl;
    bool found = false;

    for (int i = 0; i < N; i++)
        if (s[i].fizyka == 5)
        {
            cout << " " << s[i].prizv << endl;
            found = true;
        }

    if (!found)
        cout << " there are no such students" << endl;
}