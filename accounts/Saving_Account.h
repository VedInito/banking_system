#ifndef INCLUDED_SAVING_ACCOUNTS
#define INCLUDED_SAVING_ACCOUNTS

#include "../utils/Color_Codes.h"
#include "../utils/Unique_Random_Number_Generator.h"

class Saving_Account {
public:
  Saving_Account(long long Customer_ID, long double Initial_Opening_Amount)
      : m_Customer_ID(Customer_ID), m_Current_Balance(Initial_Opening_Amount) {

    m_Account_Number = s_Account_Number_Generator.Get();
    m_ATM_Card_Number = s_ATM_Number_Generator.Get();
    m_CVV_Number = s_CVV_Number_Generator.Get();

    m_Number_Of_Withdraw_This_Month = 0;
    m_Monthly_Interest = 0;
    m_Remaining_Day_Withdraw_Limit = sc_Withdraw_Amount_Per_Day_Upper_Bonund;
    m_Net_Daily_Transaction = 0;

    m_Transaction_History.clear();

    std::cout << std::fixed;
  }

public:
  long long Get_Account_Number() { return m_Account_Number; }
  long long Get_ATM_Number() { return m_ATM_Card_Number; }
  long long Get_Current_Balance() { return m_Current_Balance; }
  long long Get_Customer_ID() { return m_Customer_ID; }

public:
  long long Get_ID() { return m_Account_Number; }

public:
  bool Withdraw(long double Withdraw_Amount) {
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

    std::cout << "Successfully withdrawn: " << Withdraw_Amount
              << ". New Balance is: " << m_Current_Balance << std::endl;
    std::cout << BOLD_GREEN << "Withdrawal Successfull!..." << RESET
              << std::endl
              << std::endl;
    return true;
  }

  bool Deposite(long double Deposite_Amount) {
    m_Current_Balance += Deposite_Amount;

    std::cout << GREEN << "Amount Deposited!" << RESET << std::endl;
    std::cout << Deposite_Amount << " deposited. "
              << "New Balance: " << m_Current_Balance << std::endl;
    std::cout << BOLD_GREEN << "Deposite Successfull!..." << RESET << std::endl
              << std::endl;

    return true;
  }

  bool Get_Transaction_Amount(long double Transaction_Amount,
                              Saving_Account *p_Other) {
    std::cout << "Bank: " << m_Account_Number << "(Account Number)..."
              << std::endl;

    m_Current_Balance += Transaction_Amount;

    std::cout << "Amount: " << Transaction_Amount << ". received..."
              << std::endl;
    std::cout << BOLD_GREEN << "Transaction Successfull!" << RESET << std::endl
              << std::endl;

    return true;
  }

  bool Send_Transaction_To(long double Transaction_Amount,
                           Saving_Account *p_Other) {
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

    return true;
  }

public:
  void Day_End() {
    {
      // Add interest
      long double Interest_Rate_Per_Day = sc_Interest_Rate / 365.0;
      long double Daily_Interest =
          m_Net_Daily_Transaction * Interest_Rate_Per_Day;
      m_Monthly_Interest += Daily_Interest;
    }
    m_Net_Daily_Transaction = 0;
    m_Remaining_Day_Withdraw_Limit = sc_Withdraw_Amount_Per_Day_Upper_Bonund;
  }

  void Month_End() {
    {
      // NRV
      long double NRV_Shortfall =
          std::max((long double)0,
                   sc_Net_Relationship_Value_Per_Month - m_Current_Balance) *
          sc_NRV_Rate;

      long double NRV_Penalty =
          NRV_Shortfall * sc_Net_Relationship_Value_Fall_Penalty;
      m_Current_Balance -= NRV_Penalty;
    }

    m_Current_Balance += m_Monthly_Interest;
    m_Monthly_Interest = 0;
    m_Number_Of_Withdraw_This_Month = 0;
  }

public:
  void Dump() {
    std::cout << CYAN << "***** Dumping Account Start *****" << RESET
              << std::endl;

    std::cout << "Account Type: Saving Account" << std::endl;
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
  static bool Check_Opening_Amount(long double Opening_Amount) {
    return (Opening_Amount >= sc_Minimum_Amount_To_Open_Account);
  }

private:
  static const int sc_Minimum_Age_To_Open_Account = 0;
  static constexpr long double sc_Minimum_Amount_To_Open_Account = 10000;

  static constexpr long double sc_Interest_Rate = 6;

  static constexpr long double sc_Minimum_Opening_Amount = 10000;

  static constexpr long double sc_Net_Relationship_Value_Per_Month = 100000;
  static constexpr long double sc_Net_Relationship_Value_Fall_Penalty = 1000;
  static constexpr double sc_NRV_Rate = 0.01;

  static const int sc_Free_Withdraw_Count_In_Month_Upper_Bound = 5;
  static constexpr long double sc_Monthly_Free_Withdraw_Count_Exceed_Penalty =
      500;
  static constexpr long double sc_Withdraw_Amount_Upper_Bound = 20000;
  static constexpr long double sc_Withdraw_Amount_Per_Day_Upper_Bonund = 50000;

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
  long double m_Current_Balance;

  int m_Number_Of_Withdraw_This_Month;
  long double m_Remaining_Day_Withdraw_Limit;
  long double m_Monthly_Interest;

  long double m_Net_Daily_Transaction;
  std::vector<std::pair<std::string, long double>> m_Transaction_History;

  long long m_Account_Number;
  long long m_ATM_Card_Number;
  long long m_CVV_Number;
};

Unique_Random_Number_Generator Saving_Account::s_Account_Number_Generator(
    Saving_Account::sc_Digits_In_Account_Number,
    Saving_Account::sc_Number_Of_Accounts_Upper_Bound);

Unique_Random_Number_Generator Saving_Account::s_ATM_Number_Generator(
    Saving_Account::sc_Digits_In_ATM_Card_Number,
    Saving_Account::sc_Number_Of_Accounts_Upper_Bound);

Unique_Random_Number_Generator Saving_Account::s_CVV_Number_Generator(
    Saving_Account::sc_Digits_In_CVV_Number,
    Saving_Account::sc_Number_Of_Accounts_Upper_Bound);

#endif
