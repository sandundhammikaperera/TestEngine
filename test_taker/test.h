#include <iostream>
#include <string>
#include <vector>
#include "./question.h"

#ifndef __TEST__
#define __TEST__

class Report
{
public:
  int nCorrectQuestions;
  int nAllQuestions;
  int iMarks;
  int iPrecentageMarks;
  std::string strSummary;
};

class Test
{
  public:
   std::vector<AbstractQuestion> mVecQuestions;
   long mStartTime;
   long mEndTime;
   int assignment_type;
   int marks;
   int iCurrentQuestion;

   /* the constructor */
   Test(std::string&);
   ~Test();
   void StartTest();
   void EndTest();
   Report& MarkTest();
  private:
};

#endif
