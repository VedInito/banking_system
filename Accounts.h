#ifndef INCLUDED_ACCOUNTS
#define INCLUDED_ACCOUNTS

#include "utils/Color_Codes.h"
#include "utils/Unique_Random_Number_Generator.h"

class Account {
public:
  Account(long long Customer_ID, int Initial_Opening_Amount)
      : m_Customer_ID(Customer_ID), m_Current_Balance(Initial_Opening_Amount) {

    m_Account_Number = s_Account_Number_Generator.Get();
    m_ATM_Card_Number = s_ATM_Number_Generator.Get();
    m_CVV_Number = s_CVV_Number_Generator.Get();

    m_Number_Of_Withdraw_This_Month = 0;
    m_Activity_This_Month = 0;

    m_Remaining_Day_Withdraw_Limit = sc_Withdraw_Amount_Per_Day_Upper_Bonund;

    m_Transaction_History.clear();
  }

public:
  long long Get_Account_Number() { return m_Account_Number; }
  long long Get_ATM_Number() { return m_ATM_Card_Number; }
  long long Get_Current_Balance() { return m_Current_Balance; }
  long long Get_Customer_ID() { return m_Customer_ID; }

public:
  long long Get_ID() { return m_Account_Number; }

public:
  bool Withdraw(int Withdraw_Amount) {
    if (m_Current_Balance < Withdraw_Amount) {
      std::cout << RED << "!! Insufficient Balance" << RESET << std::endl;
      std::cout << "Current Balance: " << m_Current_Balance << std::endl;
      std::cout << BOLD_RED << "!! Transaction Failed..." << RESET << std::endl
                << std::endl;

      return false;
    }

    if (Withdraw_Amount > sc_Withdraw_Amount_Upper_Bound) {
      std::cout
          << RED
          << "!! Withdraw Amount is exceeding upper bound for Withdraw Amount"
          << RESET << std::endl;
      std::cout << "Withdraw Amount upper bound: "
                << sc_Withdraw_Amount_Upper_Bound << std::endl;
      std::cout << BOLD_RED << "!! Transaction Failed..." << RESET << std::endl
                << std::endl;

      return false;
    }

    if (Withdraw_Amount > m_Remaining_Day_Withdraw_Limit) {
      std::cout << RED << "!! Day Withdraw Limit Exceeded" << RESET
                << std::endl;
      std::cout << "Remaining Withdraw Limit: "
                << m_Remaining_Day_Withdraw_Limit << std::endl;
      std::cout << BOLD_RED << "!! Transaction Failed..." << RESET << std::endl
                << std::endl;

      return false;
    }

    if (m_Number_Of_Withdraw_This_Month >
        sc_Free_Withdraw_Count_In_Month_Upper_Bound) {
      std::cout << CYAN
                << "! Number of Withdrawal this month exceeds Free "
                   "Withdrawal count"
                << RESET << std::endl;
      std::cout << CYAN << "! Extra Penalty will be charged" << RESET
                << std::endl;

      if (m_Current_Balance <
          Withdraw_Amount + sc_Monthly_Free_Withdraw_Count_Exceed_Penalty) {
        std::cout << RED << "!! You don't have enough balance to pay Penalty"
                  << RESET << std::endl;

        std::cout << "Your Current Balance: " << m_Current_Balance
                  << "Penalty: "
                  << sc_Monthly_Free_Withdraw_Count_Exceed_Penalty << std::endl;

        std::cout << BOLD_RED << "!! Transaction Failed..." << RESET
                  << std::endl
                  << std::endl;

        return false;
      }

      m_Current_Balance -= sc_Monthly_Free_Withdraw_Count_Exceed_Penalty;
    }

    m_Current_Balance -= Withdraw_Amount;
    ++m_Number_Of_Withdraw_This_Month;
    m_Remaining_Day_Withdraw_Limit -= Withdraw_Amount;
    m_Activity_This_Month += Withdraw_Amount;

    std::cout << "Successfully withdrawn: " << Withdraw_Amount
              << ". New Balance is: " << m_Current_Balance << std::endl;
    std::cout << BOLD_GREEN << "Withdrawal Successfull!..." << RESET
              << std::endl
              << std::endl;
    return true;
  }

  bool Deposite(int Deposite_Amount) {
    m_Current_Balance += Deposite_Amount;
    m_Activity_This_Month += Deposite_Amount;

    std::cout << GREEN << "Amount Deposited!" << RESET << std::endl;
    std::cout << Deposite_Amount << " deposited. "
              << "New Balance: " << m_Current_Balance << std::endl;
    std::cout << BOLD_GREEN << "Deposite Successfull!..." << RESET << std::endl
              << std::endl;

    return true;
  }

  bool Get_Transaction_Amount(int Transaction_Amount, Account *p_Other) {
    std::cout << "Bank: " << m_Account_Number << "(Account Number)..."
              << std::endl;

    m_Current_Balance += Transaction_Amount;
    m_Activity_This_Month += Transaction_Amount;

    std::cout << "Amount: " << Transaction_Amount << ". received..."
              << std::endl;
    std::cout << BOLD_GREEN << "Transaction Successfull!" << RESET << std::endl
              << std::endl;

    return true;
  }

  bool Send_Transaction_To(int Transaction_Amount, Account *p_Other) {
    if (m_Current_Balance < Transaction_Amount) {
      std::cout << RED << "!! Insufficient Balance..." << RESET << std::endl;
      std::cout << "!! Your Account Balance is: " << m_Current_Balance << '.'
                << std::endl;
      std::cout << BOLD_RED << "!! Transaction Failed..." << RESET << std::endl
                << std::endl;

      return false;
    }

    m_Current_Balance -= Transaction_Amount;
    std::cout << "Transaction Amount: " << Transaction_Amount << ". sent..."
              << std::endl;

    p_Other->Get_Transaction_Amount(Transaction_Amount, this);

    m_Activity_This_Month += Transaction_Amount;

    return true;
  }

public:
  void Month_End() {
    {
      // NRV
      int NRV_Shortfall = std::max(0, sc_Net_Relationship_Value_Per_Month -
                                          m_Activity_This_Month) /
                          1000;
      int NRV_Penalty = NRV_Shortfall * 1000;

      if (NRV_Penalty) {
        std::cout << BOLD_RED << "***** Lack of Activity *****" << RESET
                  << std::endl;
        std::cout << "NRV Penalty: " << NRV_Penalty << " charged." << std::endl;

        m_Current_Balance -= NRV_Shortfall * 1000;
      }
    }

    {
      // Add interest
      double Interest_Rate_Per_Month = sc_Interest_Rate / 12.0;
      int Monthly_Interest = m_Current_Balance * Interest_Rate_Per_Month;
      m_Current_Balance += Monthly_Interest;
    }

    m_Number_Of_Withdraw_This_Month = 0;
    m_Activity_This_Month = 0;
  }

  void Day_End() {
    m_Remaining_Day_Withdraw_Limit = sc_Withdraw_Amount_Per_Day_Upper_Bonund;
  }

public:
  void Dump() {
    std::cout << CYAN << "***** Dumping Account Start *****" << RESET
              << std::endl;

    std::cout << "Account Number: " << m_Account_Number << std::endl;
    std::cout << "Customer Id: " << m_Customer_ID << std::endl;
    std::cout << "ATM Card Number: " << m_ATM_Card_Number << std::endl;
    std::cout << "CVV Number: " << m_CVV_Number << std::endl;
    std::cout << "Current Balance: " << m_Current_Balance << std::endl;

    std::cout << CYAN << "***** Dumping Account End *****" << RESET << std::endl
              << std::endl;
  }

public:
  static bool Check_Age(int Age) {
    return (Age >= sc_Minimum_Age_To_Open_Account);
  }
  static bool Check_Opening_Amount(int Opening_Amount) {
    return (Opening_Amount >= sc_Minimum_Amount_To_Open_Account);
  }

private:
  static const int sc_Minimum_Age_To_Open_Account = 0;
  static const int sc_Minimum_Amount_To_Open_Account = 10000;

  static const int sc_Interest_Rate = 6;

  static const int sc_Minimum_Opening_Amount = 10000;

  static const int sc_Net_Relationship_Value_Per_Month = 100000;
  static const int sc_Net_Relationship_Value_Fall_Penalty = 1000;

  static const int sc_Free_Withdraw_Count_In_Month_Upper_Bound = 5;
  static const int sc_Monthly_Free_Withdraw_Count_Exceed_Penalty = 500;
  static const int sc_Withdraw_Amount_Upper_Bound = 20000;
  static const int sc_Withdraw_Amount_Per_Day_Upper_Bonund = 50000;

  static const int sc_Digits_In_Account_Number = 10;
  static const int sc_Digits_In_ATM_Card_Number = 16;
  static const int sc_Digits_In_CVV_Number = 3;
  static const int sc_Number_Of_Accounts_Upper_Bound = 500'000;

private:
  static Unique_Random_Number_Generator s_Account_Number_Generator;
  static Unique_Random_Number_Generator s_ATM_Number_Generator;
  static Unique_Random_Number_Generator s_CVV_Number_Generator;

private:
  int m_Customer_ID;
  int m_Current_Balance;

  int m_Number_Of_Withdraw_This_Month;
  int m_Activity_This_Month;
  int m_Remaining_Day_Withdraw_Limit;

  std::vector<std::pair<std::string, int>> m_Transaction_History;

  long long m_Account_Number;
  long long m_ATM_Card_Number;
  long long m_CVV_Number;
};

Unique_Random_Number_Generator Account::s_Account_Number_Generator(
    Account::sc_Digits_In_Account_Number,
    Account::sc_Number_Of_Accounts_Upper_Bound);

Unique_Random_Number_Generator
    Account::s_ATM_Number_Generator(Account::sc_Digits_In_ATM_Card_Number,
                                    Account::sc_Number_Of_Accounts_Upper_Bound);

Unique_Random_Number_Generator
    Account::s_CVV_Number_Generator(Account::sc_Digits_In_CVV_Number,
                                    Account::sc_Number_Of_Accounts_Upper_Bound);

#endif
