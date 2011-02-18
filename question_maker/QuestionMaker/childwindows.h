// childwindows.h
//   define the child window ID's and structures.
//
//
// Author: Sanun Dhammika
// Modified: 12/25/2010

#ifndef __CHILD_WINDOWS__
#define __CHILD_WINDOWS__

#define MAIN_INDEX_QUESTION_NUMBER       3

/* Defines the ID's with more civilized names */
#define MAIN_ID_IMAGE_EDIT             708
#define MAIN_ID_SELECTION_1_EDIT       711
#define MAIN_ID_SELECTION_1_CHECKBOX   710
#define MAIN_ID_SELECTION_2_CHECKBOX   712
#define MAIN_ID_SELECTION_2_EDIT       713
#define MAIN_ID_SELECTION_3_CHECKBOX   714
#define MAIN_ID_SELECTION_3_EDIT       715
#define MAIN_ID_SELECTION_4_CHECKBOX   716
#define MAIN_ID_SELECTION_4_EDIT       717
#define MAIN_ID_SELECTION_5_CHECKBOX   718
#define MAIN_ID_SELECTION_5_EDIT       719
#define MAIN_INDEX_SELECTION_1_CHECKBOX 9
#define MAIN_INDEX_SELECTION_1_EDIT     10
#define MAIN_INDEX_SELECTION_2_CHECKBOX 11
#define MAIN_INDEX_SELECTION_2_EDIT     12
#define MAIN_INDEX_SELECTION_3_CHECKBOX 13
#define MAIN_INDEX_SELECTION_3_EDIT     14
#define MAIN_INDEX_SELECTION_4_CHECBOOX 15
#define MAIN_INDEX_SELECTION_4_EDIT     16
#define MAIN_INDEX_SELECTION_5_CHECKBOX 17
#define MAIN_INDEX_SELECTION_5_EDIT     18

/* the combo box control */
#define MAIN_INDEX_QUESTION_TYPE       1
#define MAIN_ID_QUESTION_TYPE          702

/* the question body edit control */
#define MAIN_INDEX_QUESTION_BODY    5
#define MAIN_ID_QUESTION_BODY       706

#define MAIN_INDEX_IMAGE_PATH       7
#define MAIN_ID_IMAGE_PATH          708


#define QUESTION_TYPE_SINGLE        0
#define QUESTION_TYPE_MULTIPLE      1


#define MAIN_ID_BUTTON_NEXT               721
#define MAIN_ID_BUTTON_PREVIOUS           720
#define MAIN_ID_BUTTON_SAVE_AND_EXIT      722
#define NUM_SELECTIONS                    5
typedef struct maintagChildWindows
{
  HWND handle;
  char* lpszClassName;
  char* lpszCaption;
  DWORD dwAdditionalStyles;
  int iWindowID;
  int cx;
  int cy;
  int x;
  int y;
} mainChildWindows;

#endif // END __CHILD_WINDOWS__


