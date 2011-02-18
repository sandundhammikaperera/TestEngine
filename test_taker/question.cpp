//
// question.cpp
//
//
// implements the header question.h
// Modified: 11/10/2010
//
//
#include <windows.h>      // WIN32 API calls
#include <iostream>
#include <cassert>
#include <string>         // std::string
#include <fstream>        // std::fstream
#include "question.h"

void AbstractQuestion::AddImage(void *vptrImageData,int cbDataSize)
{
  // set the pointer and the byte count.
  this->mPtrImageData=vptrImageData;
  this->mcbImageDataSize=cbDataSize;
}

void AbstractQuestion::AddBodyText(std::string text)
{
  mStrBodyText=text;
}

void AbstractQuestion::SetQuestionNumber(int iQuestionNumber)
{
  this->mQuestionNumber=iQuestionNumber;
}

void AbstractQuestion::AddSelection(Selection& selection)
{
  this->mVectorSelections.push_back(selection);
}

void AbstractQuestion::AddSelection(int index,std::string& strSelectionText)
{
  Selection selection;
  selection.index=index;
  selection.text=strSelectionText;
  this->mVectorSelections.push_back(selection);
}


void AbstractQuestion::AddAnswer(int iChoice)
{
  // check whether this already exists
  bool already_exists =false;
  for(int i=0;i<mVectorAnswers.size();i++)
  {
    if(mVectorAnswers.at(i)==iChoice)
     already_exists=true;
  }
  if(already_exists)
    return;
  this->mVectorAnswers.push_back(iChoice);
}

bool AbstractQuestion::IsAnswer(int iChoice) const
{
  for(int i=0;i<mVectorAnswers.size();i++)
    if(mVectorAnswers.at(i)==iChoice)
     return true;
  return false;
}

void AbstractQuestion::ClearAllElements()
{
  // :TODO: This should be implements at last.
}

void AbstractQuestion::RemoveAnswer(int index)
{
  for(int i=0;i < mVectorMarked.size();i++)
  {
    if(mVectorAnswers.at(i)==index)
    {
      // then delete it.
      mVectorAnswers.erase(mVectorAnswers.begin()+i);
    }
  }
}

float AbstractQuestion::GetScore()
{
  if(mQuestionType==QUESTIONTYPE_SINGLE)
  {
    for(int i=0;i<mVectorMarked.size();i++)
      if(mVectorMarked.at(i) == mVectorAnswers.at(0))
       return 100;
      else
       return 0;
  }else
  {
     int correct_marks=0;
     int wrong_marks=0;
     int correct_selections=mVectorAnswers.size();
     int wrong_selections=mVectorSelections.size()-correct_selections;


     for(int j=0;j<mVectorAnswers.size();j++)
     {
       bool is_correct=false;
       for(int k=0;k<mVectorMarked.size();k++)
          if(mVectorAnswers.at(j)==mVectorMarked.at(k))
            is_correct=true;
       if(is_correct)
         correct_marks++;
     }
     if(mVectorMarked.size()>correct_marks)
       wrong_marks=mVectorMarked.size()-correct_marks;

     return (correct_marks/correct_selections -wrong_marks/wrong_selections +1 )/2;
  }
  // NO_REACH //
  return 0.0;
}

void AbstractQuestion::MarkSelection(int index)
{
  // first search if that selection exists
  bool is_already_exists =false;
  for(int i=0;i<mVectorMarked.size();i++)
    if(mVectorMarked.at(i)==index)
        is_already_exists=true;
  if(!is_already_exists)
  {
    mVectorMarked.push_back(index);
  }
}

void AbstractQuestion::UnMarkSelection(int index)
{
  // first search if that selection exists in mVectorMarked !
  bool is_already_exists =false;
  for(int i=0;i<mVectorMarked.size();i++)
    if(mVectorMarked.at(i)==index)
    {
      is_already_exists=true;
      mVectorMarked.erase(mVectorMarked.begin()+i);
    }
}

int AbstractQuestion::GetQuestionNumber() const
{
  return mQuestionNumber;
}

std::string AbstractQuestion::GetQuestionBodyText() const
 {
  return mStrBodyText;
}

int AbstractQuestion::GetNumberOfSelections() const
{
  return mVectorSelections.size();
}

std::string AbstractQuestion::GetImagePath() const
{
  return mImagePath;
}

std::string AbstractQuestion::GetSelectionText(int index) const
{
  assert(index<=5&&index>=0);
  return mVectorSelections.at(index).text;

}

bool AbstractQuestion::GetSelectionMarkedState(int index) const
{
  assert(index<=5&&index>=0);
  for(int i=0;i<mVectorMarked.size();i++)
  {
    if(mVectorMarked.at(i)==index)
      return true;
  }
  return false;
}


bool AbstractQuestion::GetSelectionAnswerValue(int index) const
{
  assert(index<=5&&index>=0);
  return IsAnswer(index);
}


void AbstractQuestion::AddImagePath(std::string strImagePath)
{
  mImagePath=strImagePath;
}

void AbstractQuestion::LoadImageFromPath()
{
  // check whether the mImagePath consist of a valid image path.
  // if not then assert.
  HANDLE file_handle=CreateFile(mImagePath.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if(file_handle==0)
    MessageBox(NULL,TEXT("Load Image From Path failled"),TEXT("Eroror"),MB_OK|MB_ICONERROR);
  DWORD file_size;
  GetFileSize(file_handle,&file_size);
  mcbImageDataSize=file_size;
  mPtrImageData= new char[mcbImageDataSize];
  // :FILE READ::
  if(!ReadFile(file_handle,mPtrImageData,file_size,&file_size,NULL))
    MessageBox(NULL,TEXT("Load Image From path failled : File reading error"),TEXT("Error"),MB_OK|MB_ICONERROR);
  CloseHandle(file_handle);
}


void AbstractQuestion::SetQuestionType(int questionType)
{
  mQuestionType=questionType;

}

void AbstractQuestion::SetQuestionType(std::string strQuestionType)
{
  if(strQuestionType=="Single")
  {
    mQuestionType=QUESTIONTYPE_SINGLE;

  }

  if(strQuestionType=="Multiple")
  {
    mQuestionType=QUESTIONTYPE_MULTIPLE;
  }
}

bool AbstractQuestion::IsImageSet()const
{
  if(mPtrImageData==NULL)
   return false;
  return true;

}


