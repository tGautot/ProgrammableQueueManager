#define __MACRO_STR(name) #name
#define MACRO_STR(name) __MACRO_STR(name)

#define _VERSION_MAJOR_ 0
#define _VERSION_MINOR_ 0
#define _VERSION_MAJOR_STR_ MACRO_STR(_VERSION_MAJOR_)
#define _VERSION_MINOR_STR_ MACRO_STR(_VERSION_MINOR_)
#define _PROG_NAME_ "GMQ = The Generalized Queue Manager"
