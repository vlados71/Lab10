#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab102/Lab102.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLab102B
{
    // Створює студента так само, як Create(): третя оцінка йде в поле за спеціальністю
    static Student MakeStudent(const string& prizv, const int kurs, const Specialty spec,
        const int physics, const int math, const int third)
    {
        Student s;
        s.prizv = prizv; s.kurs = kurs; s.spec = spec; s.physics = physics; s.math = math;
        switch (spec)
        {
        case KOMP_NAUKY:  s.programming = third;      break;
        case INFORMATYKA: s.numericalMethods = third; break;
        default:          s.pedagogy = third;         break;
        }
        return s;
    }

    static void FillTestData(Student* p)
    {
        p[0] = MakeStudent("Коваль", 2, KOMP_NAUKY, 4, 5, 5);
        p[1] = MakeStudent("Бойко", 1, INFORMATYKA, 5, 4, 3);
        p[2] = MakeStudent("Адамчук", 3, KOMP_NAUKY, 3, 5, 4);
        p[3] = MakeStudent("Мельник", 2, INFORMATYKA, 4, 4, 5);
        p[4] = MakeStudent("Яремко", 1, FIZ_INFORMATYKA, 5, 3, 5);
        p[5] = MakeStudent("Гнатюк", 3, INFORMATYKA, 4, 5, 4);
        p[6] = MakeStudent("Левченко", 1, KOMP_NAUKY, 3, 3, 3);
    }

    TEST_CLASS(UnitTestLab102B)
    {
    public:
        TEST_METHOD(SortPhysical_OrdersBySpecialtyMathSurname)
        {
            Student p[7];
            FillTestData(p);
            SortPhysical(p, 7);
            const string expected[7] = { "Бойко", "Мельник", "Гнатюк", "Левченко",
                                         "Адамчук", "Коваль", "Яремко" };
            for (int i = 0; i < 7; i++)
                Assert::IsTrue(p[i].prizv == expected[i]);
        }

        TEST_METHOD(SortPhysical_KeepsThirdMarkWithStudent)
        {
            Student p[7];
            FillTestData(p);
            SortPhysical(p, 7);
            Assert::AreEqual(3, p[0].numericalMethods); // Бойко
            Assert::AreEqual(4, p[4].programming);      // Адамчук
            Assert::AreEqual(5, p[6].pedagogy);         // Яремко
        }

        TEST_METHOD(BuildIndex_OrdersWithoutChangingArray)
        {
            Student p[7];
            FillTestData(p);
            int* I = BuildIndex(p, 7);
            const int expected[7] = { 1, 3, 5, 6, 2, 0, 4 };
            for (int i = 0; i < 7; i++)
                Assert::AreEqual(expected[i], I[i]);
            Assert::IsTrue(p[0].prizv == "Коваль");
            delete[] I;
        }

        TEST_METHOD(BinSearch_FindsExistingStudents)
        {
            Student p[7];
            FillTestData(p);
            SortPhysical(p, 7);
            Assert::AreEqual(0, BinSearch(p, 7, "Бойко", INFORMATYKA, 4));
            Assert::AreEqual(5, BinSearch(p, 7, "Коваль", KOMP_NAUKY, 5));
            Assert::AreEqual(6, BinSearch(p, 7, "Яремко", FIZ_INFORMATYKA, 3));
        }

        TEST_METHOD(BinSearch_ReturnsMinusOneForMissing)
        {
            Student p[7];
            FillTestData(p);
            SortPhysical(p, 7);
            Assert::AreEqual(-1, BinSearch(p, 7, "Коваль", KOMP_NAUKY, 4));
            Assert::AreEqual(-1, BinSearch(p, 7, "Коваль", INFORMATYKA, 5));
            Assert::AreEqual(-1, BinSearch(p, 0, "Коваль", KOMP_NAUKY, 5)); // порожній масив
        }

        TEST_METHOD(IndexBinSearch_ReturnsPositionInMainArray)
        {
            Student p[7];
            FillTestData(p);
            int* I = BuildIndex(p, 7);
            Assert::AreEqual(3, IndexBinSearch(p, I, 7, "Мельник", INFORMATYKA, 4));
            Assert::AreEqual(-1, IndexBinSearch(p, I, 7, "Мельник", INFORMATYKA, 5));
            delete[] I;
        }
    };
}