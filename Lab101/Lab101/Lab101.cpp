#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

enum Specialty
{
    COMPUTER_SCIENCE,
    INFORMATICS,
    MATH_ECONOMICS,
    PHYSICS_INFORMATICS,
    LABOR_TRAINING
};

struct Student
{
    string surname;
    int course;
    Specialty specialty;
    int physics;
    int math;
    union // the third mark depends on the specialty
    {
        int programming;      // COMPUTER_SCIENCE
        int numericalMethods; // INFORMATICS
        int pedagogy;         // all other specialties
    };
};

string SpecialtyToString(const Specialty specialty);
int ReadInt(const string& msg, const int lo, const int hi);
void Create(Student* s, const int N);
int ThirdMark(const Student& s);
void Print(const Student* s, const int N);
bool IsExcellent(const Student& s);
double PercentExcellent(const Student* s, const int N);
int SurnamesWithPhysics5(const Student* s, const int N, string* surnames);

int main()
{
    int N = ReadInt("Enter number of students N: ", 1, 1000);
    Student* s = new Student[N];

    Create(s, N);
    Print(s, N);

    cout << "Percentage of students with excellent grades: "
        << fixed << setprecision(2) << PercentExcellent(s, N) << "%" << endl
        << endl;

    string* surnames = new string[N];
    int k = SurnamesWithPhysics5(s, N, surnames);

    cout << "Students who got 5 in physics:" << endl;
    if (k == 0)
        cout << "  no such students" << endl;
    else
        for (int i = 0; i < k; i++)
            cout << setw(3) << right << i + 1 << ". " << surnames[i] << endl;

    delete[] surnames;
    delete[] s;
    return 0;
}

string SpecialtyToString(const Specialty specialty)
{
    switch (specialty)
    {
    case COMPUTER_SCIENCE:    return "Computer Science";
    case INFORMATICS:         return "Informatics";
    case MATH_ECONOMICS:      return "Math & Economics";
    case PHYSICS_INFORMATICS: return "Physics & Informatics";
    case LABOR_TRAINING:      return "Labor Training";
    }
    return "";
}

// reads an integer from [lo; hi], repeats the request on invalid input
int ReadInt(const string& msg, const int lo, const int hi)
{
    int x;
    while (true)
    {
        cout << msg;
        if (cin >> x && x >= lo && x <= hi)
            return x;
        cout << "  Error: enter an integer from " << lo << " to " << hi << endl;
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    }
}

void Create(Student* s, const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << "Student #" << i + 1 << ":" << endl;

        cout << "  surname: ";
        cin >> ws;
        getline(cin, s[i].surname);

        s[i].course = ReadInt("  course (1-6): ", 1, 6);

        cout << "  0 - Computer Science, 1 - Informatics, 2 - Math & Economics,"
            << endl
            << "  3 - Physics & Informatics, 4 - Labor Training" << endl;
        s[i].specialty = (Specialty)ReadInt("  specialty (0-4): ", 0, 4);

        s[i].physics = ReadInt("  physics mark (1-5): ", 1, 5);
        s[i].math = ReadInt("  math mark (1-5): ", 1, 5);

        switch (s[i].specialty)
        {
        case COMPUTER_SCIENCE:
            s[i].programming = ReadInt("  programming mark (1-5): ", 1, 5);
            break;
        case INFORMATICS:
            s[i].numericalMethods = ReadInt("  numerical methods mark (1-5): ", 1, 5);
            break;
        default:
            s[i].pedagogy = ReadInt("  pedagogy mark (1-5): ", 1, 5);
            break;
        }
        cout << endl;
    }
}

// returns the third mark according to the specialty
int ThirdMark(const Student& s)
{
    switch (s.specialty)
    {
    case COMPUTER_SCIENCE: return s.programming;
    case INFORMATICS:      return s.numericalMethods;
    default:               return s.pedagogy;
    }
}

void Print(const Student* s, const int N)
{
    const string line(110, '=');
    const int w[3] = { 8, 9, 8 }; // widths of the three third-mark columns

    cout << endl << "Student list" << endl;
    cout << line << endl;
    cout << "| " << setw(3) << right << "No" << " "
        << "| " << setw(14) << left << "Surname" << " "
        << "| " << setw(6) << right << "Course" << " "
        << "| " << setw(21) << left << "Specialty" << " "
        << "| " << setw(7) << right << "Physics" << " "
        << "| " << setw(6) << right << "Math" << " "
        << "| " << setw(w[0]) << right << "Program." << " "
        << "| " << setw(w[1]) << right << "Num.Meth." << " "
        << "| " << setw(w[2]) << right << "Pedagogy" << " |" << endl;
    cout << line << endl;

    for (int i = 0; i < N; i++)
    {
        cout << "| " << setw(3) << right << i + 1 << " "
            << "| " << setw(14) << left << s[i].surname << " "
            << "| " << setw(6) << right << s[i].course << " "
            << "| " << setw(21) << left << SpecialtyToString(s[i].specialty) << " "
            << "| " << setw(7) << right << s[i].physics << " "
            << "| " << setw(6) << right << s[i].math << " ";

        // only one of the three third-mark cells is filled
        int col = (s[i].specialty == COMPUTER_SCIENCE) ? 0
            : (s[i].specialty == INFORMATICS) ? 1 : 2;
        for (int c = 0; c < 3; c++)
        {
            cout << "| " << setw(w[c]) << right;
            if (c == col)
                cout << ThirdMark(s[i]);
            else
                cout << "";
            cout << " ";
        }
        cout << "|" << endl;
    }
    cout << line << endl << endl;
}

// "excellent" means all three marks are 5
bool IsExcellent(const Student& s)
{
    return s.physics == 5 && s.math == 5 && ThirdMark(s) == 5;
}

// task 1: percentage of students with excellent grades
double PercentExcellent(const Student* s, const int N)
{
    if (N == 0)
        return 0.0;
    int k = 0;
    for (int i = 0; i < N; i++)
        if (IsExcellent(s[i]))
            k++;
    return 100.0 * k / N;
}

// task 2: writes surnames of students with 5 in physics into surnames
// and returns their count (surnames must hold at least N elements)
int SurnamesWithPhysics5(const Student* s, const int N, string* surnames)
{
    int k = 0;
    for (int i = 0; i < N; i++)
        if (s[i].physics == 5)
            surnames[k++] = s[i].surname;
    return k;
}