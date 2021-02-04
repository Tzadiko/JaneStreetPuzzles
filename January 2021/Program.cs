using System;
using System.Collections.Generic;
using System.Numerics;

namespace SolutionCSharp
{
    class Program
    {
        private static readonly Dictionary<int, BigInteger> _factorials;

        // This can be simplified using symmetry.
        // For example, the permutation count for 23344 is the same as that for 56677.
        // One would need to build a data structure to store the permutation count, factoring in
        // distinct number count, and frequency of each number.

        // The current runtime is around 2 hours (13! / 2! iterations), and I'm satisfied with that
        // for the sake of answering the question. So I'll leave it as is.
        static Program()
        {
            // 12 * (12 + 1) / 2 == 78. Thats how many possible numbers there are
            // so that's how many factorials I'll calculate.
            _factorials = new Dictionary<int, BigInteger>(78)
            {
                { 0 , 1 },
            };
            for (int i = 1; i <= 78; ++i)
            {
                _factorials.Add(i, _factorials[i - 1] * i);
            }
        }

        static void Main(string[] args)
        {
            var (numerator, denominator) = CalculatePermutationValues();
            Console.WriteLine($"Numerator: {numerator} / Denominator: {denominator}");
        }

        private static (BigInteger FrequencyCombinationCount, BigInteger TotalCombinationCount) CalculatePermutationValues()
        {
            BigInteger grandTotalPermutations = BigInteger.Zero, grandTotalPermutationsForN= BigInteger.Zero, iterationCount = BigInteger.Zero;
            for (int toyTwo = 0; toyTwo <= 2; toyTwo++)
                for (int toyThree = 0; toyThree <= 3; toyThree++)
                    for (int toyFour = 0; toyFour <= 4; toyFour++)
                        for (int toyFive = 0; toyFive <= 5; toyFive++)
                            for (int toySix = 0; toySix <= 6; toySix++)
                                for (int toySeven = 0; toySeven <= 7; toySeven++)
                                    for (int toyEight = 0; toyEight <= 8; toyEight++)
                                        for (int toyNine = 0; toyNine <= 9; toyNine++)
                                            for (int toyTen = 0; toyTen <= 10; toyTen++)
                                                for (int toyEleven = 0; toyEleven <= 11; toyEleven++)
                                                    for (int toyTwelve = 0; toyTwelve <= 12; toyTwelve++)
                                                    {
                                                        BigInteger permBeforeOneIsPulled = CalculatePermutations(toyTwo, toyThree, toyFour, toyFive, toySix, toySeven, toyEight, toyNine, toyTen, toyEleven, toyTwelve);
                                                        BigInteger permAfterOneIsPulled = CalculatePermutations(2 - toyTwo, 3 - toyThree, 4 - toyFour, 5 - toyFive, 6 - toySix, 7 - toySeven, 8 - toyEight, 9 - toyNine, 10 - toyTen, 11 - toyEleven, 12 - toyTwelve);

                                                        int maxNumber = MaxNumber(toyTwo, toyThree, toyFour, toyFive, toySix, toySeven, toyEight, toyNine, toyTen, toyEleven, toyTwelve);

                                                        BigInteger totalCombinationsThisArrangement = permBeforeOneIsPulled * permAfterOneIsPulled;

                                                        BigInteger totalForThisN = maxNumber * totalCombinationsThisArrangement;

                                                        grandTotalPermutations += totalCombinationsThisArrangement;

                                                        grandTotalPermutationsForN += totalForThisN;
                                                        if (iterationCount++ % 250_000 == 0)
                                                            Console.WriteLine($"Iteration Count: {iterationCount}");
                                                    }
            return (grandTotalPermutationsForN, grandTotalPermutations);
        }

        private static int MaxNumber(int toyTwo, int toyThree, int toyFour, int toyFive, int toySix, int toySeven, int toyEight, int toyNine, int toyTen, int toyEleven, int toyTwelve)
        {
            int largestNumber = int.MinValue;
            if (toyTwo > largestNumber) largestNumber = toyTwo;
            if (toyThree > largestNumber) largestNumber = toyThree;
            if (toyFour > largestNumber) largestNumber = toyFour;
            if (toyFive > largestNumber) largestNumber = toyFive;
            if (toySix > largestNumber) largestNumber = toySix;
            if (toySeven > largestNumber) largestNumber = toySeven;
            if (toyEight > largestNumber) largestNumber = toyEight;
            if (toyNine > largestNumber) largestNumber = toyNine;
            if (toyTen > largestNumber) largestNumber = toyTen;
            if (toyEleven > largestNumber) largestNumber = toyEleven;
            if (toyTwelve > largestNumber) largestNumber = toyTwelve;
            return largestNumber;

        }

        private static BigInteger CalculatePermutations(int toyTwo, int toyThree, int toyFour, int toyFive, int toySix, int toySeven, int toyEight, int toyNine, int toyTen, int toyEleven, int toyTwelve)
        {
            int totalToyCount = toyTwo + toyThree + toyFour + toyFive + toySix + toySeven + toyEight + toyNine + toyTen + toyEleven + toyTwelve;

            BigInteger numerator = _factorials[totalToyCount];
            BigInteger denominator = _factorials[toyTwo] * _factorials[toyThree] * _factorials[toyFour] * _factorials[toyFive]
                        * _factorials[toySix] * _factorials[toySeven] * _factorials[toyEight] * _factorials[toyNine]
                        * _factorials[toyTen] * _factorials[toyEleven] * _factorials[toyTwelve];

            return numerator / denominator;
        }
    }
}
