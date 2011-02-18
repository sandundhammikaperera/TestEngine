
#ifndef __QUESTION_H_
#define __QUESTION_H_


#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#define QUESTIONTYPE_SINGLE     0
#define QUESTIONTYPE_MULTIPLE   1

struct Selection
{
  int index;
  std::string text;
};

class AbstractQuestion
{
public:
 int mQuestionNumber; /* the number of the question */
 int mQuestionType;
 /* defines whether the type of the question eg:- multiple or single choice
                       0 -if single choice.
                       1 -if multiple choice.
 */
 std::string      mStrBodyText; /* the body of the question */
 std::string      mImagePath;

 int              mcbImageDataSize; /* if zero then this does not contain an image */
 void             *mPtrImageData; /* image of the question , if none then this is assigned to zero */

 /* the array of selections */
 std::vector<Selection>            mVectorSelections; /* selections */
 std::vector<int>                  mVectorAnswers; /* answers */
 std::vector<int>                  mVectorMarked; /* markerd questions ,intially none nothing is markered */

 /* the constructor and destructor */
 AbstractQuestion(){

  // initialize the mcbImageDataSize.
  mcbImageDataSize=0;
  mPtrImageData=NULL;
  mQuestionType=QUESTIONTYPE_SINGLE;
  mVectorMarked.clear();
  mVectorAnswers.clear();
  mVectorSelections.clear();
 };

 ~AbstractQuestion(){
          delete []mPtrImageData;
 }
 // allow the default other constructors as well.
 // copy constructor and the assignment operator.

 /* other helper functions */
 void AddImage(void *,int);
 void AddImagePath(std::string);
 void LoadImageFromPath();
 void AddBodyText(std::string);
 void AddAnswer(int);
 void SetQuestionType(int);
 void SetQuestionType(std::string);
 void SetQuestionNumber(int);
 void AddSelection(Selection&);
 void AddSelection(int,std::string&);
 void RemoveAnswer(int);
 void ClearAllElements();
 float GetScore();
 void MarkSelection(int);
 void UnMarkSelection(int);

 int         GetQuestionNumber() const;
 std::string GetQuestionBodyText() const;
 std::string GetImagePath() const;
 int         GetNumberOfSelections() const;
 std::string GetSelectionText(int) const;
 bool        GetSelectionMarkedState(int )const ;
 bool        GetSelectionAnswerValue(int) const;
 bool        IsAnswer(int) const;
 bool        IsImageSet() const;
};

#endif
