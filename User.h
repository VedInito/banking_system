#ifndef INCLUDED_USER
#define INCLUDED_USER

#include "utils/Color_Codes.h"
#include "utils/Unique_Random_Number_Generator.h"

#include "accounts/Current_Account.h"
#include "accounts/Loan_Account.h"
#include "accounts/Saving_Account.h"

class User {
public:
  User(std::string Name, long long Phone_Number, std::string Email_Address,
       std::string Home_Address)
      : m_Name(Name), m_Phone_Number(Phone_Number),
        m_Email_Address(Email_Address), m_Home_Address(Home_Address) {

    m_Customer_ID = s_Customer_ID_Generator.Get();

    m_Total_Balance_At_Start_Of_The_Month = 0;

    m_Saving_Accounts.clear();
    m_Current_Accounts.clear();
    m_Loan_Accounts.clear();
  }

public:
  std::string Get_Name() { return m_Name; }
  long long Get_Phone_Number() { return m_Phone_Number; }
  std::string Get_Email_Address() { return m_Email_Address; }
  std::string Get_Home_Address() { return m_Home_Address; }

public:
  long long Get_ID() { return m_Customer_ID; }

public:
  bool Add_Saving_Account(Saving_Account *New_Saving_Account) {
    bool Add_Result = (m_Saving_Accounts.count(New_Saving_Account) == 0);
    m_Saving_Accounts.insert(New_Saving_Account);
    return Add_Result;
  }
  bool Add_Current_Account(Current_Account *New_Current_Account) {
    bool Add_Result = (m_Current_Accounts.count(New_Current_Account) == 0);
    m_Current_Accounts.insert(New_Current_Account);
    return Add_Result;
  }
  bool Add_Loan_Account(Loan_Account *New_Loan_Account) {
    bool Add_Result = (m_Loan_Accounts.count(New_Loan_Account) == 0);
    m_Loan_Accounts.insert(New_Loan_Account);
    return Add_Result;
  }

public:
  long double Calculate_Current_Total_Balance() {
    long double Total_Balance = 0;

    for (Saving_Account *p_Saving_Account : m_Saving_Accounts)
      Total_Balance += p_Saving_Account->Get_Current_Balance();

    for (Current_Account *p_Current_Account : m_Current_Accounts)
      Total_Balance += p_Current_Account->Get_Current_Balance();

    for (Loan_Account *p_Loan_Account : m_Loan_Accounts)
      Total_Balance -= p_Loan_Account->Get_Loan_Amount();

    return Total_Balance;
  }

  long double Get_Profit_This_Month() {
    return Calculate_Current_Total_Balance() -
           m_Total_Balance_At_Start_Of_The_Month;
  }

public:
  void Day_End() {}
  void Month_End() {
    m_Total_Balance_At_Start_Of_The_Month = Calculate_Current_Total_Balance();
  }

public:
  void Dump() {
    std::cout << CYAN << "***** Dumping User Start *****" << RESET << std::endl;

    std::cout << "Name: " << m_Name << std::endl;
    std::cout << "Age: " << m_Age << std::endl;
    std::cout << "Phone Number: " << m_Phone_Number << std::endl;
    std::cout << "Email Address: " << m_Email_Address << std::endl;
    std::cout << "Home Address: " << m_Home_Address << std::endl;
    std::cout << "Customer Id: " << m_Customer_ID << std::endl;

    std::cout << "Number of Saving Accounts: " << m_Saving_Accounts.size()
              << std::endl;
    std::cout << "Number of Current Accounts: " << m_Current_Accounts.size()
              << std::endl;
    std::cout << "Number of Loan Accounts: " << m_Loan_Accounts.size()
              << std::endl;

    std::cout << CYAN << "***** Dumping Account End *****" << RESET << std::endl
              << std::endl;
  }

private:
  static const int sc_Digits_In_Customer_ID = 10;
  static const int sc_Number_Of_Users = 500'000;

private:
  static Unique_Random_Number_Generator s_Customer_ID_Generator;

private:
  std::string m_Name;
  int m_Age;
  long long m_Phone_Number;
  std::string m_Email_Address;
  std::string m_Home_Address;

  long long m_Customer_ID;

  long double m_Total_Balance_At_Start_Of_The_Month;

  std::unordered_set<Saving_Account *> m_Saving_Accounts;
  std::unordered_set<Current_Account *> m_Current_Accounts;
  std::unordered_set<Loan_Account *> m_Loan_Accounts;
};

Unique_Random_Number_Generator
    User::s_Customer_ID_Generator(User::sc_Digits_In_Customer_ID,
                                  User::sc_Number_Of_Users);

#endif
