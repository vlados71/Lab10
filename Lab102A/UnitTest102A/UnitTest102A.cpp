#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab102A/Lab102A.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest102A
{
    TEST_CLASS(UnitTest102A)
    {
    public:

        TEST_METHOD(TestBinSearch)
        {
            Student s[3] = {
                { "Melnyk", 1, ME, 4, 5, 4 },
                { "Koval",  2, KN, 5, 4, 5 },
                { "Bondar", 3, KN, 3, 4, 3 }
            };
            Student existing = { "Koval", 0, KN, 0, 4, 0 };
            Student missing = { "Koval", 0, KN, 0, 5, 0 };

            int* I = IndexSort(s, 3);
            int found = BinSearch(s, I, 3, existing);
            int notFound = BinSearch(s, I, 3, missing);
            delete[] I;

            Assert::AreEqual(1, found);
            Assert::AreEqual(-1, notFound);
        }
    };
}