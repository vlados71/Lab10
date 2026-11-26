#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab101A/Lab101A.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest101A
{
    TEST_CLASS(UnitTest101A)
    {
    public:

        TEST_METHOD(TestPercentExcellent)
        {
            Student s[4] = {
                { "Koval",    1, KN,   5, 5, 5 },
                { "Bondar",   2, INFO, 5, 4, 5 },
                { "Melnyk",   3, ME,   5, 5, 5 },
                { "Tkachuk",  4, FI,   3, 4, 5 }
            };

            double actual = PercentExcellent(s, 4);

            Assert::AreEqual(50.0, actual, 0.0001);
        }
    };
}