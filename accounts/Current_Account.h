#ifndef INCLUDED_CURRENT_ACCOUNT
#define INCLUDED_CURRENT_ACCOUNT

#include "../utils/Color_Codes.h"
#include "../utils/Unique_Random_Number_Generator.h"

class Current_Account {
public:
  Current_Account(long long Customer_ID, long double Initial_Opening_Amount)
      : m_Customer_ID(Customer_ID), m_Current_Balance(Initial_Opening_Amount) {

    m_This_Account_Type = "CURRENT_ACCOUNT";
    m_Account_Number = sp_Account_Number_Generator->Get();

    m_Number_Of_Transactions_This_Month = 0;

    m_Transaction_History.clear();

    std::cout << std::fixed;
  }

public:
  std::string Get_Account_Type() { return m_This_Account_Type; }
  long long Get_Account_Number() { return m_Account_Number; }
  long long Get_Current_Balance() { return m_Current_Balance; }
  long long Get_Customer_ID() { return m_Customer_ID; }

public:
  long long Get_ID() { return m_Account_Number; }

public:
  bool Withdraw(long double Withdraw_Amount) {
    long double Transaction_Charge =
        std::min(sc_Maximum_Charge_Per_Transaction,
                 Withdraw_Amount * sc_Transaction_Charge_Rate);

    if (m_Number_Of_Transactions_This_Month >
        sc_Number_Of_Transaction_Without_Extra_Penalty) {
      Transaction_Charge += sc_Extra_Penalty;
    }

    if (m_Current_Balance < Transaction_Charge + Withdraw_Amount) {
      std::cout << RED << "!! Insufficient Balance" << RESET << std::endl;
      std::cout << "Current Balance: " << m_Current_Balance << std::endl;
      std::cout << BOLD_RED << "!! Transaction Failed..." << RESET << std::endl
                << std::endl;

      return false;
    }

    m_Current_Balance -= Withdraw_Amount + Transaction_Charge;
    ++m_Number_Of_Transactions_This_Month;

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
                              Current_Account *p_Other) {
    std::cout << "Transaction From: " << p_Other->Get_Account_Number()
              << ". To: " << m_Account_Number << "..." << std::endl;

    m_Current_Balance += Transaction_Amount;

    std::cout << "Amount: " << Transaction_Amount << ". received..."
              << std::endl;
    std::cout << BOLD_GREEN << "Transaction Successfull!" << RESET << std::endl
              << std::endl;

    return true;
  }

  bool Send_Transaction_To(long double Transaction_Amount,
                           Current_Account *p_Other) {
    long double Transaction_Charge =
        std::min((long double)sc_Maximum_Charge_Per_Transaction,
                 Transaction_Amount * sc_Transaction_Charge_Rate);

    if (m_Number_Of_Transactions_This_Month >
        sc_Number_Of_Transaction_Without_Extra_Penalty) {
      Transaction_Charge += sc_Extra_Penalty;
    }

    if (m_Current_Balance < Transaction_Amount + Transaction_Charge) {
      std::cout << RED << "!! Insufficient Balance..." << RESET << std::endl;
      std::cout << "!! Your Account Balance is: " << m_Current_Balance << '.'
                << std::endl;
      std::cout << BOLD_RED << "!! Transaction Failed..." << RESET << std::endl
                << std::endl;

      return false;
    }

    m_Current_Balance -= Transaction_Amount + Transaction_Charge;
    ++m_Number_Of_Transactions_This_Month;

    std::cout << "Transaction Amount: " << Transaction_Amount << ". sent..."
              << std::endl;

    p_Other->Get_Transaction_Amount(Transaction_Amount, this);

    return true;
  }

public:
  void Day_End() {}

  void Month_End() {
    {
      // NRV
      long double NRV_Shortfall =
          std::max(0, sc_Net_Relationship_Value_Per_Month - m_Current_Balance) *
          sc_NRV_Rate;

      long double NRV_Penalty =
          NRV_Shortfall * sc_Net_Relationship_Value_Fall_Penalty;
      m_Current_Balance -= NRV_Penalty;
    }

    m_Number_Of_Transactions_This_Month = 0;
  }

public:
  void Dump() {
    std::cout << CYAN << "***** Dumping Account Start *****" << RESET
              << std::endl;

    std::cout << "Account Type: " << m_This_Account_Type << std::endl;
    std::cout << "Account Number: " << m_Account_Number << std::endl;
    std::cout << "Customer Id: " << m_Customer_ID << std::endl;
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
  static const int sc_Minimum_Age_To_Open_Account = 18;
  static constexpr long double sc_Minimum_Amount_To_Open_Account = 100000;

  static const int sc_Net_Relationship_Value_Per_Month = 500000;
  static constexpr long double sc_Net_Relationship_Value_Fall_Penalty = 5000;
  static constexpr long double sc_NRV_Rate = 0.01;

  static constexpr long double sc_Transaction_Charge_Rate = 0.5;
  static constexpr long double sc_Maximum_Charge_Per_Transaction = 500;
  static const int sc_Number_Of_Transaction_Without_Extra_Penalty = 3;
  static constexpr long double sc_Extra_Penalty = 500;

  static const int sc_Digits_In_Account_Number = 10;
  static constexpr long double sc_Number_Of_Accounts_Upper_Bound = 500'000;

private:
  static Unique_Random_Number_Generator *sp_Account_Number_Generator;

private:
  int m_Customer_ID;
  int m_Current_Balance;

  std::string m_This_Account_Type;

  int m_Number_Of_Transactions_This_Month;

  std::vector<std::pair<std::string, int>> m_Transaction_History;

  long long m_Account_Number;
};

Unique_Random_Number_Generator *Current_Account::sp_Account_Number_Generator =
    new Unique_Random_Number_Generator(
        Current_Account::sc_Digits_In_Account_Number,
        Current_Account::sc_Number_Of_Accounts_Upper_Bound);

#endif
