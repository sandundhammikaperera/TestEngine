#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "question.h"
#include "test.h"

/* declarations for some helper functions that helpst to
   parse the text file */
std::vector<Selection>& ExtractSelections(char *,int);
AbstractQuestion& ExtractAbstractQuestion(char *,int);


/* the constructor */
Test::Test(std::string& fileName)
{
 /* open the file here */
 std::ifstream test_file;
 test_file.open(fileName.c_str(),std::ios_base::binary); /* open the file as a binary file */



 test_file.close();
}


















