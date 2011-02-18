
// filehanlder.h
//
// handles the file IO
//
//

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

/* --    Includes -- */

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <windowsx.h>

#include "../../test_taker/question.h"

/*  --- structures --- */
typedef struct tagDataToSave
{
  int nNumberOfQuestions;
  int nTimeInSeconds;
  int nMarks;
  std::vector<AbstractQuestion> questions;
} DataToSave,DataToRead;


bool FH_SaveToFile(const DataToSave&,const std::string&);
bool FH_ReadFromFile(DataToRead&,const std::string&);

#endif   // END OF FILEHANDLER_H

