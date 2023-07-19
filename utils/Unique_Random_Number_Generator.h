#ifndef INCLUDED_UNIQUE_RANDOM_NUMBER_GENERATOR
#define INCLUDED_UNIQUE_RANDOM_NUMBER_GENERATOR

#include "Common_Headers.h"

class Unique_Random_Number_Generator {
private:
  long long m_RANDOM_MAX, m_RANDOM_MIN;

private:
  std::stack<long long> m_Randoms;

private:
  void Update_MAX_MIN(int Number_Of_Digits) {
    m_RANDOM_MAX = m_RANDOM_MIN = 0;
    for (int i_Digit = 0; i_Digit < Number_Of_Digits; ++i_Digit) {
      m_RANDOM_MIN = m_RANDOM_MIN * 10 + 1;
      m_RANDOM_MAX = m_RANDOM_MAX * 10 + 9;
    }
  }

public:
  Unique_Random_Number_Generator(int Number_Of_Digits, int Requirement_Size) {
    Update_MAX_MIN(Number_Of_Digits);

    std::vector<long long> Random_Vec;
    {
      long long Hop_Size =
          std::max(1LL, (m_RANDOM_MAX - m_RANDOM_MIN) / Requirement_Size);

      // Fill array
      Random_Vec.reserve(Requirement_Size);

      long long Random_Number = m_RANDOM_MIN;
      while ((int)Random_Vec.size() < Requirement_Size) {
        Random_Vec.push_back(Random_Number);

        Random_Number += Hop_Size;

        if (Random_Number > m_RANDOM_MAX)
          Random_Number = m_RANDOM_MIN;
      }
    }

    // Suffle Array
    std::random_device rd;
    std::mt19937 generator(rd());
    shuffle(Random_Vec.begin(), Random_Vec.end(), generator);

    // Fill Stack
    for (long long Random_Number : Random_Vec)
      m_Randoms.push(Random_Number);
  }

public:
  long long Get() {
    long long Random_Number(m_Randoms.top());
    m_Randoms.pop();
    return Random_Number;
  }
};

#endif
