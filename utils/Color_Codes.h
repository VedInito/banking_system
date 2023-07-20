#ifndef INCLUDED_COLOR_CODES
#define INCLUDED_COLOR_CODES

#ifdef USE_COLOR_CODES

#define RESET "\033[0m"

#define BOLD_CYAN "\033[1;96m"
#define CYAN "\033[0;96m"

#define BOLD_RED "\033[1;91m"
#define RED "\033[0;91m"

#define BOLD_GREEN "\033[1;92m"
#define GREEN "\033[0;92m"

#else
#define RESET ""

#define BOLD_CYAN ""
#define CYAN ""

#define BOLD_RED ""
#define RED ""

#define BOLD_GREEN ""
#define GREEN ""

#endif

#endif
