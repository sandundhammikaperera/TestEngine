//
// validate.cpp
// Copyright(C) GNU General Public license.
//
// Implements the validate.h header file functions.
//
#include "validate.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <cassert>
#include <windowsx.h>
#include "../../test_taker/question.h"
#include "childwindows.h"

extern mainChildWindows  gChildWindows[];

int validateValidateInputs(HWND hwnd)
{

  bool is_multiple =false;
  int combo_box_index=ComboBox_GetCurSel(gChildWindows[MAIN_INDEX_QUESTION_TYPE].handle);
  if(combo_box_index==1)
  {
      is_multiple=true;
  }
  assert(combo_box_index!=CB_ERR);

  // check the Question body for a valid length.
  // valid length should be 1 or more and less than or equal to
  // LIMITS_MAX_BODY_TEXT_LENGTH
  if(LIMITS_MAX_BODY_TEXT_LENGTH<GetWindowTextLength(gChildWindows[MAIN_INDEX_QUESTION_BODY].handle)|| GetWindowTextLength(gChildWindows[MAIN_INDEX_QUESTION_BODY].handle)==0)
  {
    char pstr_error_message[500];
    wsprintf(pstr_error_message,"Question Body text length should not be zero or bigger than %i.Please enter again.",LIMITS_MAX_BODY_TEXT_LENGTH);
    MessageBox(hwnd,pstr_error_message,TEXT("Error"),MB_OK|MB_ICONERROR);
    return MAIN_INDEX_QUESTION_BODY;
  }

  // get the answers.
  bool answers [5];
  for(int i=0;i<5;i++)
    answers[i]=false;  // clear the array.
  for(int i=0;i<5;i++)
  {
      if(BST_CHECKED==SendMessage(gChildWindows[MAIN_INDEX_SELECTION_1_CHECKBOX+2*i].handle,BM_GETCHECK,0,0))
        answers[i]=true;
  }
  int answer_count =0;
  for(int i=0;i<5;i++)
  {
    if(answers[i])
      answer_count++;
  }

  if( (!is_multiple) && answer_count>1)
  {
    char pstr_error_message[500];
    wsprintf(pstr_error_message,"A single typed question should only have a one answer");
    MessageBox(hwnd,pstr_error_message,TEXT("Error"),MB_OK|MB_ICONERROR);
    return MAIN_INDEX_QUESTION_TYPE;
  }

  // test selection texts for correct text lengths.
  // the first and second selections texts should not be null.
  // bellow code checks for them.
  if(GetWindowTextLength(gChildWindows[MAIN_INDEX_SELECTION_1_EDIT].handle)==0)
  {
    MessageBox(hwnd,"Selection 1 text should be not zero","Input Error",MB_OK|MB_ICONERROR);
    return MAIN_INDEX_SELECTION_1_EDIT;
  }
  if(GetWindowTextLength(gChildWindows[MAIN_INDEX_SELECTION_2_EDIT].handle)==0)
  {
    MessageBox(hwnd,"Selection2 text should be not zero","Input Error",MB_OK|MB_ICONERROR);
    return MAIN_INDEX_SELECTION_2_EDIT;
  }

  bool selection_5_have_text=false;
  bool selection_4_have_text=false;
  bool selection_3_have_text=false;
  if(IsWindowEnabled(gChildWindows[MAIN_INDEX_SELECTION_3_EDIT].handle)&& ( GetWindowTextLength(gChildWindows[MAIN_INDEX_SELECTION_3_EDIT].handle) >0 ))
    selection_3_have_text=true;
  if(IsWindowEnabled(gChildWindows[MAIN_INDEX_SELECTION_4_EDIT].handle)&&( GetWindowTextLength(gChildWindows[MAIN_INDEX_SELECTION_4_EDIT].handle)>0 ))
    selection_4_have_text=true;
  if(IsWindowEnabled( gChildWindows[MAIN_INDEX_SELECTION_5_EDIT].handle)&& ( GetWindowTextLength(gChildWindows[MAIN_INDEX_SELECTION_5_EDIT].handle)>0 ))
    selection_5_have_text=true;

  if((selection_4_have_text ||selection_5_have_text)  && !selection_3_have_text)
  {
    MessageBox(hwnd,TEXT("selection 3 should have some text"),TEXT("Input Error"),MB_OK|MB_ICONERROR);
    return MAIN_INDEX_SELECTION_3_EDIT;
  }

  if(selection_5_have_text && !selection_4_have_text)
  {
    MessageBox(hwnd,TEXT("selection 4 should have some text"),TEXT("Input Error"),MB_OK|MB_ICONERROR);
    return MAIN_INDEX_SELECTION_4_EDIT;
  }


  // check whether if each checkbox does not exceed the
  // LIMITS_MAX_SELECTION_TEXT_LENGTH
  // for selection 1 and 2
  int selection_1_text_length=GetWindowTextLength(gChildWindows[MAIN_INDEX_SELECTION_1_EDIT].handle);
  int selection_2_text_length=GetWindowTextLength(gChildWindows[MAIN_INDEX_SELECTION_2_EDIT].handle);
  if(selection_1_text_length > LIMITS_MAX_SELECTION_TEXT_LENGTH)
  {
    MessageBox(hwnd,TEXT("selection 1 text too long."),TEXT("Input Error"),MB_OK|MB_ICONERROR);
    return MAIN_INDEX_SELECTION_1_EDIT;
  }
  if(selection_2_text_length > LIMITS_MAX_SELECTION_TEXT_LENGTH)
  {
    MessageBox(hwnd,TEXT("selection 2 text too long"),TEXT("Input Error"),MB_OK|MB_ICONERROR);
    return MAIN_INDEX_SELECTION_2_EDIT;
  }

  // for selection 3
  if(IsWindowEnabled(gChildWindows[MAIN_INDEX_SELECTION_3_EDIT].handle))
  {
    if(GetWindowTextLength(gChildWindows[MAIN_INDEX_SELECTION_3_EDIT].handle)>LIMITS_MAX_SELECTION_TEXT_LENGTH)
    {
      MessageBox(hwnd,TEXT("Selection 3 text is too long"),TEXT("Input Error"),MB_OK|MB_ICONERROR);
      return MAIN_INDEX_SELECTION_3_EDIT;
    }
  }

  // for selection 4
  if(IsWindowEnabled(gChildWindows[MAIN_INDEX_SELECTION_4_EDIT].handle))
  {
    if(GetWindowTextLength(gChildWindows[MAIN_INDEX_SELECTION_4_EDIT].handle)> LIMITS_MAX_SELECTION_TEXT_LENGTH)
    {
      MessageBox(hwnd,TEXT("selection 4 text is too long"),TEXT("Input Error"),MB_OK|MB_ICONERROR);
      return MAIN_INDEX_SELECTION_4_EDIT;
    }
  }

  // for selection 5
  if(IsWindowEnabled(gChildWindows[MAIN_INDEX_SELECTION_5_EDIT].handle))
  {
    if(GetWindowTextLength(gChildWindows[MAIN_INDEX_SELECTION_5_EDIT].handle)>LIMITS_MAX_SELECTION_TEXT_LENGTH)
    {
      MessageBox(hwnd,TEXT("selection 5 text is too long"),TEXT("Input Error"),MB_OK|MB_ICONERROR);
      return MAIN_INDEX_SELECTION_5_EDIT;
    }
  }

  // if success
  return 0;
}



