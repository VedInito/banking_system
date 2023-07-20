#ifndef INCLUDED_LOAN_ACCOUNT
#define INCLUDED_LOAN_ACCOUNT

#include "../utils/Color_Codes.h"
#include "../utils/Unique_Random_Number_Generator.h"

class Loan_Account {
public:
  Loan_Account(long long Customer_ID, const char *Account_Type,
               long double Initial_Loan_Amount, int Total_Balance_Of_Customer)

      : m_Customer_ID(Customer_ID), m_Account_Type(Account_Type),
        m_Initial_Loan_Amount(Initial_Loan_Amount),
        m_Current_Compounded_Loan_Amount(Initial_Loan_Amount) {

    m_Account_Number = s_Account_Number_Generator.Get();

    m_Maximum_Repay_Installment =
        sc_Account_Opening_Upper_Bound_In_Percent_Of_Deposite *
        Total_Balance_Of_Customer;

    m_Account_Duration_In_Months = 0;

    m_Six_Months_Loan_Amount_Increment_Factor =
        Six_Months_Loan_Amount_Increment_Factor_Calculator(Account_Type);

    std::cout << std::fixed;
  }

public:
  long long Get_Account_Number() { return m_Account_Number; }
  long double Get_Loan_Amount() { return m_Current_Compounded_Loan_Amount; }
  long long Get_Customer_ID() { return m_Customer_ID; }
  long double Get_Initial_Loan_Amount() { return m_Initial_Loan_Amount; }

public:
  long long Get_ID() { return m_Account_Number; }

public:
  bool Pay_Installment(long double Installment_Amount) {
    if (Installment_Amount > m_Maximum_Repay_Installment) {
      std::cout << RED << "!! Installment Exceeds the Maximum Repay Installment"
                << RESET << std::endl;
      std::cout << "Maximum Repay Installment: " << m_Maximum_Repay_Installment
                << std::endl;
      std::cout << BOLD_RED << "!! Transaction Failed..." << RESET << std::endl
                << std::endl;

      return false;
    }

    if (Installment_Amount >=
        m_Current_Compounded_Loan_Amount + sc_Precision_Error) {
      std::cout << RED << "!! Money Exceeds the Current Compounded Loan Amount"
                << RESET << std::endl;
      std::cout << "Current Compounded Loan Amount: "
                << m_Current_Compounded_Loan_Amount << std::endl;
      std::cout << BOLD_RED << "!! Transaction Failed..." << RESET << std::endl
                << std::endl;

      return false;
    }

    if (m_Account_Duration_In_Months < sc_Minimum_Loan_Duration_In_Months) {
      std::cout << RED << "!! Minimum Loan Duration Critera Voilated." << RESET
                << std::endl;
      std::cout << "Minimum Loan Duration(in Monts): "
                << sc_Minimum_Loan_Duration_In_Months << std::endl;
      std::cout << BOLD_RED << "!! Transaction Failed..." << RESET << std::endl
                << std::endl;

      return false;
    }

    m_Current_Compounded_Loan_Amount -= Installment_Amount;

    std::cout << "Your new Current Compounded Loan Amount is"
              << m_Current_Compounded_Loan_Amount << '.' << std::endl;

    std::cout << BOLD_GREEN << "Installment Paid Successfully!..." << RESET
              << std::endl
              << std::endl;

    return true;
  }

public:
  void Day_End() {}
  void Month_End() {
    ++m_Account_Duration_In_Months;

    bool Six_Months_Cycle_Completed =
        (bool)(m_Account_Duration_In_Months %
                   sc_Interest_Update_Duration_In_Months ==
               0);

    if (Six_Months_Cycle_Completed) {
      m_Current_Compounded_Loan_Amount *=
          m_Six_Months_Loan_Amount_Increment_Factor;
    }
  }

public:
  void Dump() {
    std::cout << CYAN << "***** Dumping Account Start *****" << RESET
              << std::endl;

    std::cout << "Account Type: Loan Account" << std::endl;
    std::cout << "Account Number: " << m_Account_Number << std::endl;
    std::cout << "Customer Id: " << m_Customer_ID << std::endl;
    std::cout << "Current Compounded Loan Amount: "
              << m_Current_Compounded_Loan_Amount << std::endl;

    std::cout << CYAN << "***** Dumping Account End *****" << RESET << std::endl
              << std::endl;
  }

public:
  static bool Check_Age(int Age) {
    return (Age >= sc_Minimum_Age_To_Open_Account);
  }

  static bool Check_Initial_Loan_Amount(long double Initial_Loan_Amount,
                                        long double Total_Balance_Of_Customer) {
    if (Initial_Loan_Amount < sc_Account_Opening_Lower_Bound)
      return false;

    long double Account_Opening_Upper_Bound =
        Total_Balance_Of_Customer *
        sc_Account_Opening_Upper_Bound_In_Percent_Of_Deposite;

    if (Initial_Loan_Amount > Account_Opening_Upper_Bound)
      return false;

    return true;
  }

  static long double
  Six_Months_Loan_Amount_Increment_Factor_Calculator(const char *Account_Type) {
    long double Interest_Rate = -1;

    std::string loanType(Account_Type);
    if (loanType == "HOME") {
      Interest_Rate = sc_Home_Loan_Interest_Rate;
    } else if (loanType == "CAR") {
      Interest_Rate = sc_Car_Loan_Interest_Rate;
    } else if (loanType == "PERSONAL") {
      Interest_Rate = sc_Personal_Loan_Interest_Rate;
    } else {
      std::cout << BOLD_RED << "!! Wrong Loan Type." << std::endl << std::endl;
    }

    long double Six_Months_Loan_Amount_Increment_Factor =
        (1 + Interest_Rate / 100);

    return Six_Months_Loan_Amount_Increment_Factor;
  }

private:
  static constexpr long double sc_Home_Loan_Interest_Rate = 7;
  static constexpr long double sc_Car_Loan_Interest_Rate = 8;
  static constexpr long double sc_Personal_Loan_Interest_Rate = 12;

  static const int sc_Interest_Update_Duration_In_Months = 6;

  static const int sc_Minimum_Age_To_Open_Account = 25;
  static const int sc_Account_Opening_Lower_Bound = 500'000;
  static const int sc_Account_Opening_Upper_Bound_In_Percent_Of_Deposite = 40;
  static const int sc_Business_Loan_Interest_Rate = 15;

  static const int
      sc_Installment_Lower_Bound_In_Percent_Of_Initial_Loan_Amount = 10;
  static const int sc_Minimum_Loan_Duration_In_Months = 24;

  static const int sc_Digits_In_Account_Number = 10;
  static constexpr long double sc_Number_Of_Accounts_Upper_Bound = 500'000;

  static constexpr long double sc_Precision_Error = 1e-6;

private:
  static Unique_Random_Number_Generator s_Account_Number_Generator;

private:
  long long m_Customer_ID;
  std::string m_Account_Type;
  long double m_Initial_Loan_Amount;

  long long m_Account_Number;

  long double m_Current_Compounded_Loan_Amount;
  long double m_Maximum_Repay_Installment;

  long long m_Account_Duration_In_Months;
  long double m_Six_Months_Loan_Amount_Increment_Factor;
};

Unique_Random_Number_Generator Loan_Account::s_Account_Number_Generator(
    Loan_Account::sc_Digits_In_Account_Number,
    Loan_Account::sc_Number_Of_Accounts_Upper_Bound);

#endif
