#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab101/Lab101.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Lab10Tests
{
    // builds a student; the third mark goes into the union field
    // that matches the specialty
    static Student Make(const string& surname, const Specialty specialty,
        const int physics, const int math, const int third)
    {
        Student s;
        s.surname = surname;
        s.course = 1;
        s.specialty = specialty;
        s.physics = physics;
        s.math = math;
        switch (specialty)
        {
        case COMPUTER_SCIENCE: s.programming = third;      break;
        case INFORMATICS:      s.numericalMethods = third; break;
        default:               s.pedagogy = third;         break;
        }
        return s;
    }

    TEST_CLASS(Task10_1Tests)
    {
    public:

        TEST_METHOD(IsExcellent_ThirdMarkReadFromUnion)
        {
            Assert::IsTrue(IsExcellent(Make("A", COMPUTER_SCIENCE, 5, 5, 5)));
            Assert::IsTrue(IsExcellent(Make("B", INFORMATICS, 5, 5, 5)));
            Assert::IsTrue(IsExcellent(Make("C", LABOR_TRAINING, 5, 5, 5)));
            Assert::IsFalse(IsExcellent(Make("D", MATH_ECONOMICS, 5, 5, 4)));
            Assert::IsFalse(IsExcellent(Make("E", COMPUTER_SCIENCE, 5, 4, 5)));
        }

        TEST_METHOD(PercentExcellent_Mixed)
        {
            Student s[4] = {
                Make("Ivanenko",   COMPUTER_SCIENCE, 5, 5, 5), // excellent
                Make("Petrenko",   INFORMATICS,      5, 5, 4),
                Make("Koval",      MATH_ECONOMICS,   5, 5, 5), // excellent
                Make("Shevchenko", LABOR_TRAINING,   4, 5, 5)
            };
            Assert::AreEqual(50.0, PercentExcellent(s, 4), 1e-9);
        }

        TEST_METHOD(PercentExcellent_All)
        {
            Student s[2] = {
                Make("A", PHYSICS_INFORMATICS, 5, 5, 5),
                Make("B", INFORMATICS,         5, 5, 5)
            };
            Assert::AreEqual(100.0, PercentExcellent(s, 2), 1e-9);
        }

        TEST_METHOD(PercentExcellent_None)
        {
            Student s[2] = {
                Make("A", COMPUTER_SCIENCE, 4, 5, 5),
                Make("B", MATH_ECONOMICS,   5, 5, 3)
            };
            Assert::AreEqual(0.0, PercentExcellent(s, 2), 1e-9);
        }

        TEST_METHOD(PercentExcellent_EmptyArray)
        {
            Assert::AreEqual(0.0, PercentExcellent(nullptr, 0), 1e-9);
        }

        TEST_METHOD(SurnamesWithPhysics5_Mixed)
        {
            Student s[4] = {
                Make("Ivanenko",   COMPUTER_SCIENCE, 5, 3, 3),
                Make("Petrenko",   INFORMATICS,      4, 5, 5),
                Make("Koval",      MATH_ECONOMICS,   5, 4, 4),
                Make("Shevchenko", LABOR_TRAINING,   3, 5, 5)
            };
            string surnames[4];
            int k = SurnamesWithPhysics5(s, 4, surnames);

            Assert::AreEqual(2, k);
            Assert::IsTrue(surnames[0] == "Ivanenko");
            Assert::IsTrue(surnames[1] == "Koval");
        }

        TEST_METHOD(SurnamesWithPhysics5_None)
        {
            Student s[2] = {
                Make("A", COMPUTER_SCIENCE, 4, 5, 5),
                Make("B", MATH_ECONOMICS,   3, 5, 5)
            };
            string surnames[2];
            Assert::AreEqual(0, SurnamesWithPhysics5(s, 2, surnames));
        }
    };
}