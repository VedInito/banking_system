#ifndef INCLUDED_USER_INPUT
#define INCLUDED_USER_INPUT

#include "Common_Headers.h"

#define INPUT_TYPE_ERROR "!! Error in INPUT_TYPE...\nType Again:"

template <typename Input_Type> Input_Type Get_Input() {
  Input_Type User_Input;
  while (!(std::cin >> User_Input)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << INPUT_TYPE_ERROR << std::endl;
  }
  return User_Input;
}

#endif
