#define USE_COLOR_CODES
#include "accounts/Current_Account.h"
#include "accounts/Loan_Account.h"
#include "accounts/Saving_Account.h"

#include "database/Database.h"

#include "utils/Color_Codes.h"
#include "utils/Common_Headers.h"
#include "utils/User_Input.h"

#include "User.h"

int main() {
  Database<Loan_Account> *p_DB = new Database<Loan_Account>();

  Loan_Account *p_account1 = new Loan_Account(2, "CAR", 2'000'000, 9'000'000);
  Loan_Account *p_account2 = new Loan_Account(3, "CAR", 2'100'000, 9'000'000);

  p_DB->Add_Element(p_account1);
  p_DB->Add_Element(p_account2);

  p_DB->Dump();

  p_account1->Month_End();
  p_account1->Month_End();
  p_account1->Month_End();
  p_account1->Month_End();
  p_account1->Month_End();
  p_account1->Month_End();

  /*
  p_account1->Deposite(10000);

  p_account1->Withdraw(10000);
  p_account1->Withdraw(10000);
  p_account1->Withdraw(10000);
  p_account1->Withdraw(10000);
  p_account1->Withdraw(10000);

  p_account1->Day_End();

  p_account1->Withdraw(10000);
  p_account1->Withdraw(10000);
  p_account1->Withdraw(10000);

  p_account1->Month_End();

  p_account1->Withdraw(100);

  p_account1->Withdraw(500);

  DB.Dump();

  p_account2->Send_Transaction_To(100, p_account1);

  */

  p_DB->Dump();
}
