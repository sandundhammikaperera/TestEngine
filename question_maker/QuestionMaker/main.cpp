//
// main.cpp
//
// Copyright(C) Under the GNU GPL
// Author Sandun Dhammika Perera.
//
// Modified 11/11/2010

#include <iostream>                                                     // std::cout std::cin
#include <string>                                                       // std::string
#include <vector>                                                       // std::vector
#include <stack>                                                        // std::stack
#include <windows.h>                                                    // WIN32 API.
#include <windowsx.h>
#include <cassert>                                                      // assertions.
#include "../../test_taker/question.h"                                                   // USES AbstractQuestion
#include "bitmap.h"                                                     // bitmap routines.
#include "./resource.h"                                                 // resources
#include "./validate.h"                                                 // validate functions.
#include "./childwindows.h"                                             // defines the child window indexes and ID's.
#include <algorithm>


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);


/* save and exit dialog procedure */
BOOL CALLBACK SaveAndExitDialogProc(HWND,UINT,WPARAM,LPARAM);

/**/
void mainOnClickNext(HWND);
void mainOnClickSaveAndExit(HWND);

/* mainOnClickPrevious , when the previous button clicked */
void mainOnClickPrevious(HWND);

void mainLoadQuestion(const AbstractQuestion&);
void mainFillAbstractQuestionFromForum(AbstractQuestion*);
bool mainIsForumEmpty();

/*  Make the class name into a global variable  */
char szClassName[ ] = "QuestionMakerMainWindow";

void mainCreateNewForum();

mainChildWindows gChildWindows[]=
{
  NULL,TEXT("static"),TEXT("Question Type:"),WS_VISIBLE,701,105,10,10,11,
  NULL,TEXT("COMBOBOX"),NULL,CBS_DROPDOWNLIST|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL,702,77,100,147,8,
  NULL,TEXT("static"),TEXT("Question Number"),WS_VISIBLE,703,126,9,9,30,
  NULL,TEXT("edit"),TEXT("hi"),ES_LEFT|WS_TABSTOP|WS_VISIBLE|ES_READONLY,704,23,12,147,25,
  NULL,TEXT("static"),TEXT("Question Body Text"),WS_VISIBLE,705,116,9,9,48,
  NULL,TEXT("edit"),TEXT(""),WS_VISIBLE|ES_LEFT|WS_VSCROLL|ES_AUTOVSCROLL|WS_VSCROLL|ES_MULTILINE|WS_TABSTOP,706,186,37,147,46,
  NULL,TEXT("static"),TEXT("Image"),WS_VISIBLE|WS_TABSTOP,707,137,12,6,95,
  NULL,TEXT("edit"),TEXT("Click Here to Add a Image"),WS_VISIBLE|ES_LEFT,708,197,12,147,91,
  NULL,TEXT("static"),TEXT("section"),WS_VISIBLE|SS_CENTER,709,207,22,98,117,
  NULL,TEXT("button"),TEXT(""),BS_CHECKBOX|BS_LEFT|WS_VISIBLE,710,9,12,14,146,
  NULL,TEXT("edit"),TEXT(""),ES_LEFT|WS_VISIBLE|WS_TABSTOP,711,197,12,33,146,
  NULL,TEXT("button"),TEXT(""),BS_LEFT|BS_CHECKBOX|WS_VISIBLE|WS_TABSTOP,712,9,12,13,161,
  NULL,TEXT("edit"),TEXT(""),ES_LEFT|WS_TABSTOP|WS_VISIBLE,713,197,12,33,162,
  NULL,TEXT("button"),TEXT(""),BS_LEFT|BS_CHECKBOX|WS_VISIBLE|WS_TABSTOP,714,9,12,13,178,
  NULL,TEXT("edit"),TEXT(""),ES_LEFT|WS_VISIBLE|WS_TABSTOP,715,197,15,33,178,
  NULL,TEXT("button"),TEXT(""),BS_CHECKBOX|BS_LEFT|WS_TABSTOP|WS_VISIBLE,716,9,12,14,195,
  NULL,TEXT("edit"),TEXT(""),WS_VISIBLE|ES_LEFT|WS_TABSTOP,717,193,15,34,195,
  NULL,TEXT("button"),TEXT(""),BS_CHECKBOX|WS_TABSTOP|BS_LEFT|WS_VISIBLE,718,9,12,14,214,
  NULL,TEXT("edit"),TEXT(""),WS_TABSTOP|BS_LEFT|WS_VISIBLE,719,197,15,34,214,
  NULL,TEXT("button"),TEXT("previous"),BS_PUSHBUTTON|WS_VISIBLE|BS_VCENTER|WS_TABSTOP,720,106,28,41,251,
  NULL,TEXT("button"),TEXT("Next"),BS_PUSHBUTTON|WS_VISIBLE|BS_VCENTER|WS_TABSTOP,721,106,28,183,250,
  NULL,TEXT("button"),TEXT("Save And Exit"),BS_PUSHBUTTON|WS_VISIBLE|BS_VCENTER|WS_TABSTOP,722,106,28,320,250
};

#define NUM_OF_CHILD_WINDOWS (sizeof(gChildWindows)/sizeof(mainChildWindows))
bool mainCreateChildWindows(HWND hwnd)
{
  bool isFalled=false;
  // iterate through the array and Create each and every window.
  for(int i=0;i<NUM_OF_CHILD_WINDOWS;i++)
  {
    gChildWindows[i].handle=CreateWindow(gChildWindows[i].lpszClassName,gChildWindows[i].lpszCaption,gChildWindows[i].dwAdditionalStyles|WS_CHILD|WS_VISIBLE,
                                     0,0,0,0,hwnd,(HMENU)gChildWindows[i].iWindowID,GetModuleHandle(NULL),NULL );
    if(gChildWindows[i].handle==NULL)
    // then set the isFalled to true.
    isFalled=true;
  }

  // add the "Single" and "Multiple" combo-box items to the combobox.
  if(SendMessage(gChildWindows[1].handle,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(TEXT("Single")))==CB_ERR)
    MessageBox(NULL,"Combo Box Item adding failled","combo-box",MB_ICONERROR);
  if(SendMessage(gChildWindows[1].handle,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(TEXT("Multiple")))==CB_ERR)
    MessageBox(NULL,"Combo Box Item adding failled","combo box",MB_ICONERROR);
  if(SendMessage(gChildWindows[1].handle,CB_SETCURSEL,0,0)==CB_ERR)
    MessageBox(NULL,"Combo Box Item selection falled","combo box",MB_ICONERROR);
  return !isFalled;
}

void mainArrangeChildWindows(HWND hwnd,int x,int y)
{
 int cxChar=GetDialogBaseUnits();
 int cyChar=GetDialogBaseUnits();
 float scale=1.8;
 for(int i=0;i<NUM_OF_CHILD_WINDOWS;i++)
 {
   MoveWindow(gChildWindows[i].handle,scale*gChildWindows[i].x,scale*gChildWindows[i].y,scale*gChildWindows[i].cx,scale*gChildWindows[i].cy,TRUE);
 }
}

// mainIsSelectionGotText
//   specifies whether the selection got text or not.
// @parms -none
// @returns - bool true if selection got text , false if none.
bool mainIsSelectionGotText(int iSelection)
{
  bool return_value=false;

  // index number of the child window array.
  // index= 10+ iSelection*2
  assert(iSelection>=0&&iSelection<=5);
  HWND hwnd=gChildWindows[10+iSelection*2].handle;

  // memory buffer to retrieve text.
  char* buffer=new char[500];
  GetWindowText(hwnd,buffer,500);
  if(buffer[0]!='\0')
  {
    return_value=true;
  }
  delete [] buffer;
  return return_value;
}

// mainSetSelectionText
// @parms int index index of the selection that text should be set.
// @parms std::string text
// @returns void
//
void mainSetSelectionText(int index,std::string strSelectionText)
{
  assert(index>=0&&index<=5);

  if(!IsWindowEnabled(gChildWindows[MAIN_INDEX_SELECTION_1_EDIT+ 2*index].handle))
    MessageBox(NULL,TEXT("Selection text can't be set because selection is not enabled"),TEXT("App Error"),MB_OK|MB_ICONERROR);
  SetWindowText(gChildWindows[MAIN_INDEX_SELECTION_1_EDIT+2*index].handle,strSelectionText.c_str());
}

// Function
//   mainGetSelectionState
// @parms int index index to the selection
// @returns bool true if the selection is enabled and checked.
bool mainGetSelectionState(int index)
{
  assert( index>=0&& index<=5);
  if(IsWindowEnabled(gChildWindows[MAIN_INDEX_SELECTION_1_CHECKBOX+2*index].handle))
    if(SendMessage(gChildWindows[MAIN_INDEX_SELECTION_1_CHECKBOX+2*index].handle,BM_GETCHECK,0,0))
      return true;
    else
      return false;
  else
    return false;
}

// mainSetSelectionState
//
//@parms int index of the selection
//@parms bool state of the selection.
//@returns void
void mainSetSelectionState(int index,bool state)
{
  assert(index>=0&&index<=5);
  if(!IsWindowEnabled(gChildWindows[MAIN_INDEX_SELECTION_1_CHECKBOX+2*index].handle))
    MessageBox(NULL,TEXT("Selection checkbox can't be set because selection is not enabled"),TEXT("App Error"),MB_OK|MB_ICONERROR);
  SendMessage(gChildWindows[MAIN_INDEX_SELECTION_1_CHECKBOX+2*index].handle,BM_SETCHECK,state,0);
}


// Opens a GetFileOpenDialog and get the BITMAP file path.
//
// @parms hwnd - parent window handle.
// @returns std::string&
std::string mainOpenBitmapFileDialog(HWND hwnd)
{

  // buffer to hold the filename.
  char* buffer=new char[MAX_PATH];
  buffer[0]=NULL;

  // create and initialize the OFNFILENAME structure.
  OPENFILENAME ofn;

  static TCHAR szFilter[] = TEXT ("Bitmap Files (*.BMP)\0*.bmp\0\0") ;

  ofn.lStructSize       = sizeof (OPENFILENAME);
  ofn.hwndOwner         = hwnd ;
  ofn.hInstance         = GetModuleHandle(NULL);
  ofn.lpstrFilter       = szFilter;
  ofn.lpstrCustomFilter = NULL;
  ofn.nMaxCustFilter    = 0;
  ofn.nFilterIndex      = 0;
  ofn.lpstrFile         = buffer;          // Set in Open and Close functions
  ofn.nMaxFile          = 500;
  ofn.lpstrFileTitle    = NULL;          // Set in Open and Close functions
  ofn.nMaxFileTitle     = MAX_PATH;
  ofn.lpstrInitialDir   = NULL;
  ofn.lpstrTitle        = TEXT("Open BITMAP file");
  ofn.Flags             = 0 ;             // Set in Open and Close functions
  ofn.nFileOffset       = 0 ;
  ofn.nFileExtension    = 0 ;
  ofn.lpstrDefExt       = TEXT ("bmp") ;
  ofn.lCustData         = 0L ;
  ofn.lpfnHook          = NULL ;
  ofn.lpTemplateName    = NULL ;

  bool is_failled = true;
  // if the function does not failled.

  while(true)
  {
    if( GetOpenFileName(&ofn) ==0)
    {
      break;
    }
    std::string file_path;
    file_path="";
    file_path=buffer;
    if(bitmapValidateBitmapByFile(file_path)==0)
    {
      is_failled=false;
      break;
    }
    else
      MessageBox(NULL,"Invalid Bitmap File,Please choose back","Invalid Bitmap",MB_OK);
  }

  if(is_failled)
  {
    std::string return_str("");
    delete []buffer;
    return return_str;
  }else{
    std::string return_str(buffer);
    return return_str;
  }
}

/* enables the selection
   int iSelectionIndex index of the selection that you need to enable or disable.
   @parms iSelection index that indicates what selection to enable
   @returns void.

   @warning if the iSelection is less than the 0 or
            above than the 5 then this will fire a assertion.

*/
void mainEnableSelection(int iSelection)
{

  assert (iSelection <=5 && iSelection >= 0);
  EnableWindow(gChildWindows[2*iSelection+MAIN_INDEX_SELECTION_1_CHECKBOX].handle,TRUE);
  EnableWindow(gChildWindows[2*iSelection+MAIN_INDEX_SELECTION_1_CHECKBOX+1].handle,TRUE);
}
/*
  Disables the selection.
 @parms iSelectionIndex index of the selection that you need to disable.
 @returns void

 @warning if the iSelection is less than the 0 or above
          then the 5 then this will fire a assertion.
*/
void mainDisableSelection(int iSelection)
{
    assert(iSelection<=5&&iSelection>=0);
    EnableWindow(gChildWindows[2*iSelection+MAIN_INDEX_SELECTION_1_CHECKBOX].handle,FALSE);
    EnableWindow(gChildWindows[2*iSelection+MAIN_INDEX_SELECTION_1_CHECKBOX+1].handle,FALSE);
}

struct WindowData
{
    int cxChar;
    int cyChar;
}gWindowData;

struct AppGlobalData
{
    int iCurrentQuestionNumber; // :KLUDGE: this is a data duplication.
    int nNumberOfQuestions;
    int nNumberOfSections;
    bool bImagePathIsSet;
    std::vector<AbstractQuestion> questions;
    // :TODO: more entries to add to here.
}gAppGlobalData;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)(COLOR_WINDOW);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (//CreateWindowEx(lpszClassName,lpszAppName,style,x,y,x_width,y_width,hParent,hMenu,hInstance,additional_parameters);
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "QuestionMaker Main Window",       /* Title Text */
           WS_OVERLAPPEDWINDOW|DS_SHELLFONT, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           463*1.9,                 /* The programs width */
           285*2.1,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           LoadMenu(hThisInstance,MAKEINTRESOURCE(ID_MAIN_MENU)),                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
  switch (message)                  /* handle the messages */
  {
    case  WM_SIZE:
     mainArrangeChildWindows(hwnd,LOWORD(lParam),HIWORD(lParam));
     break;

    case WM_CREATE:
      // initialize the global data to zero
      gAppGlobalData.iCurrentQuestionNumber=1;
      gAppGlobalData.nNumberOfQuestions=1;
      mainCreateChildWindows(hwnd);
      mainCreateNewForum();
      break;

    case WM_DESTROY:
      PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
      break;
    case WM_COMMAND:
      // :TODO: handle the commands here.

      switch(LOWORD(wParam))
      {
          // add an image edit text window
          case MAIN_ID_IMAGE_EDIT:
            if(HIWORD(wParam)==EN_SETFOCUS)
            {

              EnableWindow((HWND)lParam,FALSE);
              std::string image_path;
              image_path=mainOpenBitmapFileDialog(hwnd);
              if(image_path=="")
              {
                // DO NOTHING
              } else{
               gAppGlobalData.bImagePathIsSet=true;
               SetWindowText((HWND)lParam,image_path.c_str());
              }
              EnableWindow((HWND)lParam,TRUE);
              break;
            }
            case MAIN_ID_SELECTION_1_EDIT:
            case MAIN_ID_SELECTION_2_EDIT:
            case MAIN_ID_SELECTION_3_EDIT:
            case MAIN_ID_SELECTION_4_EDIT:
            case MAIN_ID_SELECTION_5_EDIT:

              if(HIWORD(wParam)==EN_UPDATE)
              {
                if(mainIsSelectionGotText(2))
                {
                  // enable the section 4
                  mainEnableSelection(3);
                }
                if(mainIsSelectionGotText(3))
                {
                  mainEnableSelection(4);
                }
              }

              break;
            case MAIN_ID_SELECTION_1_CHECKBOX:
            case MAIN_ID_SELECTION_2_CHECKBOX:
            case MAIN_ID_SELECTION_3_CHECKBOX:
            case MAIN_ID_SELECTION_4_CHECKBOX:
            case MAIN_ID_SELECTION_5_CHECKBOX:
              SendMessage((HWND)lParam,BM_SETCHECK,(WPARAM)!SendMessage((HWND)lParam,BM_GETCHECK,0,0),0);
              break;
           case MAIN_ID_BUTTON_NEXT:case IDC_NEXT_QUESTION:
             mainOnClickNext(hwnd);
             break;
           case MAIN_ID_BUTTON_PREVIOUS:case IDC_PREVIOUS_QUESTION:
             mainOnClickPrevious(hwnd);
             break;
           case MAIN_ID_BUTTON_SAVE_AND_EXIT:case IDC_SAVE_AND_EXIT:
             DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(ID_DIALOG_SAVE_AND_EXIT),hwnd,SaveAndExitDialogProc);
             break;
           // :TODO: handle the menu commands.
           case IDC_EXIT:
             // Called when the menu IDC_EXIT was invoked //
             // :TODO:
             break;
           case IDC_MULTIPLE_CHANGES:
             // :TODO:
             break;
           default:
             break;
      }
      break;
    default:                      /* for messages that we don't deal with */
      return DefWindowProc (hwnd, message, wParam, lParam);
  }
  return 0;
}


void mainCreateNewForum()
{
  // clear all the text box values in the textboxes.
  //  tick the default checkboxes and select the default option in the
  //  drop down combobox.

  // select the default option on the question type.
  SendMessage(gChildWindows[1].handle,CB_SETCURSEL,0,0);

  // clear the text boxes.
  SetWindowText(gChildWindows[3].handle,TEXT("1"));
  SetWindowText(gChildWindows[5].handle,NULL);

  // :IMPORTANT: before change the text of the text in window handle 7 you have to disable it.
  EnableWindow(gChildWindows[7].handle,0);
  SetWindowText(gChildWindows[7].handle,TEXT("Click Here to add an image"));
  gAppGlobalData.bImagePathIsSet=false;
  EnableWindow(gChildWindows[7].handle,1);

  for(int i=0;i<5;i++)
  {
    EnableWindow(gChildWindows[MAIN_INDEX_SELECTION_1_EDIT+2*i].handle,TRUE);
    SetWindowText(gChildWindows[MAIN_INDEX_SELECTION_1_EDIT+2*i].handle,NULL);
    if(i>3)
      EnableWindow(gChildWindows[MAIN_INDEX_SELECTION_1_EDIT+2*i].handle,FALSE);
  }


  // clear all the selections.
  for ( int i=0;i<5;i++)
    SendMessage(gChildWindows[MAIN_INDEX_SELECTION_1_CHECKBOX+2* i].handle,BM_SETCHECK,0,0);

  // enable up to three sections.
  EnableWindow(gChildWindows[9].handle,TRUE);
  EnableWindow(gChildWindows[10].handle,TRUE);
  EnableWindow(gChildWindows[11].handle,TRUE);
  EnableWindow(gChildWindows[12].handle,TRUE);
  EnableWindow(gChildWindows[13].handle,TRUE);
  EnableWindow(gChildWindows[14].handle,TRUE);
  EnableWindow(gChildWindows[15].handle,FALSE);
  EnableWindow(gChildWindows[16].handle,FALSE);
  EnableWindow(gChildWindows[17].handle,FALSE);
  EnableWindow(gChildWindows[18].handle,FALSE);

  // then set the gAppGlobalData
  gAppGlobalData.nNumberOfSections=2;
}

void mainOnClickNext(HWND hwnd)
{
  // validate the forum
  int index;
  if((index=validateValidateInputs(hwnd)) != 0)
  {
    // set the focus of that window.
    SetFocus(gChildWindows[index].handle);
    return;
  }

  AbstractQuestion question;
  // read the forum and fill the vaues.
  mainFillAbstractQuestionFromForum(&question);

  if(gAppGlobalData.questions.size()<question.GetQuestionNumber())
  {
    gAppGlobalData.questions.push_back(question);
  }
  else{
    gAppGlobalData.questions.at(question.GetQuestionNumber()-1)=question;
  }

  AbstractQuestion new_question;
  if(gAppGlobalData.questions.size()<question.GetQuestionNumber()+1)
    new_question.ClearAllElements();
  else
    new_question=gAppGlobalData.questions.at(question.GetQuestionNumber() );

  // update gAppGlobalDataq
  new_question.SetQuestionNumber(question.GetQuestionNumber()+1);
  gAppGlobalData.iCurrentQuestionNumber=new_question.GetQuestionNumber();
  gAppGlobalData.nNumberOfQuestions=new_question.GetQuestionNumber();
  mainLoadQuestion(new_question);
;
}


void mainLoadQuestion(const AbstractQuestion& abstractQuestion)
{
  // first clear the forum.
  mainCreateNewForum();

  // load the question-type combo box values
  if(abstractQuestion.mQuestionType ==0)
  {
    ComboBox_SetCurSel(gChildWindows[MAIN_INDEX_QUESTION_TYPE].handle,QUESTION_TYPE_SINGLE);

  }else{
    ComboBox_SetCurSel(gChildWindows[MAIN_INDEX_QUESTION_TYPE].handle,QUESTION_TYPE_MULTIPLE);
  }

  // setting up the question number.
  char buffer[500];
  wsprintf(buffer,"%i",abstractQuestion.GetQuestionNumber());
  gAppGlobalData.iCurrentQuestionNumber=abstractQuestion.GetQuestionNumber();
  SetWindowText(gChildWindows[3].handle,buffer);

  // load the question body text
  SetWindowText(gChildWindows[MAIN_INDEX_QUESTION_BODY].handle,abstractQuestion.GetQuestionBodyText().c_str());

  // load the image path
  // :IMPORTANT: before change the text in image path edit box you
  //  have to disable that window
  EnableWindow(gChildWindows[MAIN_INDEX_IMAGE_PATH].handle,0);
  if(abstractQuestion.IsImageSet())
  {
    SetWindowText(gChildWindows[MAIN_INDEX_IMAGE_PATH].handle,abstractQuestion.GetImagePath().c_str());
    gAppGlobalData.bImagePathIsSet=true;
  }
  EnableWindow(gChildWindows[MAIN_INDEX_IMAGE_PATH].handle,1);

  // load the selection texts.
  for(int i=0;i<abstractQuestion.GetNumberOfSelections();i++)
  {
    // Load the selection text.
    std::string selection_text= abstractQuestion.GetSelectionText(i);
    mainSetSelectionText(i,selection_text.c_str());
  }

  // load the selection check boxes.
  for(int i=0;i<abstractQuestion.GetNumberOfSelections();i++)
  {
     mainSetSelectionState(i,abstractQuestion.GetSelectionAnswerValue(i));
  }


}

//  mainFillAbstractQuestionFromForum
//
// @parms AbstractQuestion a newly created AbstractQuestion
// warning before calling this function the forum should be validted for
// input errors.
void mainFillAbstractQuestionFromForum(AbstractQuestion* abstractQuestion)
{
  abstractQuestion->ClearAllElements();

  // obtain the question type and set it.
  int index_question_type_combo_box=ComboBox_GetCurSel(gChildWindows[MAIN_INDEX_QUESTION_TYPE].handle);
  int question_type ;
  if(index_question_type_combo_box == QUESTION_TYPE_SINGLE)
  {
      question_type=QUESTION_TYPE_SINGLE;
  }
  if(index_question_type_combo_box == QUESTION_TYPE_MULTIPLE)
  {
      question_type=QUESTIONTYPE_MULTIPLE;
  }
  abstractQuestion->SetQuestionType(question_type);

  // obtain the question number and set it.
  char buffer[100];
  int question_number;
  GetWindowText(gChildWindows[MAIN_INDEX_QUESTION_NUMBER].handle,buffer,100);
  sscanf(buffer,"%i",&question_number);
  abstractQuestion->SetQuestionNumber(question_number);

  // set the Question body text.
  char buffer_body_text[LIMITS_MAX_BODY_TEXT_LENGTH];
  GetWindowText(gChildWindows[MAIN_INDEX_QUESTION_BODY].handle,buffer_body_text,LIMITS_MAX_BODY_TEXT_LENGTH);
  abstractQuestion->AddBodyText(buffer_body_text);

  // set the image path
  char buffer_image_text [MAX_PATH];
  GetWindowText(gChildWindows[MAIN_INDEX_IMAGE_PATH].handle,buffer_image_text,MAX_PATH);
  abstractQuestion->AddImagePath(buffer_image_text);
  // abstractQuestion.LoadImageFromPath(); // we can do this later :KLUDGE:

  // set the selections.
  char buffer_selection_text[LIMITS_MAX_SELECTION_TEXT_LENGTH];
  std::string selection_text[5];
  int number_of_selections=0;
  for(int i=0;i<5;i++)
  {
    if(IsWindowEnabled(gChildWindows[MAIN_INDEX_SELECTION_1_EDIT+2*i].handle)){
      GetWindowText(gChildWindows[MAIN_INDEX_SELECTION_1_EDIT+2*i].handle,buffer_selection_text,LIMITS_MAX_SELECTION_TEXT_LENGTH);
      selection_text[i]=buffer_selection_text;
    }else{
      selection_text[i]="";
    }
  }
  int i=0;
  while(selection_text[i++]!=""&&i<=5)
  {
    number_of_selections++;
  }
  for(int i=0;i<number_of_selections;i++)
  {
    abstractQuestion->AddSelection(i,selection_text[i]);
  }

  bool answers[5];
  for(int i=0;i<number_of_selections;i++)
  {
    answers[i]=SendMessage(gChildWindows[MAIN_INDEX_SELECTION_1_CHECKBOX+2*i].handle,BM_GETSTATE,0,0);
    if(answers[i])
      abstractQuestion->AddAnswer(i);
  }
}

//
// mainOnClickPrevious
//
// @parms HWND hwnd handle to the main window.
//
// return void
//
void mainOnClickPrevious(HWND hwnd)
{
  // if the forum is initially clear then
  // do nothing just return
  if(!mainIsForumEmpty())
  {


    // validate the forum
    if(int index=validateValidateInputs(hwnd) != 0)
    {
      // set the focus of that window.
      SetFocus(gChildWindows[index].handle);
      return;
    }

    AbstractQuestion question;
    // read the forum and fill the vaues.
    mainFillAbstractQuestionFromForum(&question);

    if(question.GetQuestionNumber()==1)
    {
      MessageBox(hwnd,TEXT("Previous Question does not exist"),TEXT("Question Maker"),MB_OK|MB_ICONEXCLAMATION);
      return;
    }

    // insert the current question to the vector.
    if(gAppGlobalData.questions.size()<question.GetQuestionNumber())
    {
      gAppGlobalData.questions.push_back(question);
    }else{
      gAppGlobalData.questions.at(question.GetQuestionNumber() -1) = question;
    }

    // load the previous question to the forum.
    assert(question.GetQuestionNumber() >0);
    question=gAppGlobalData.questions.at(question.GetQuestionNumber() -2);
    mainLoadQuestion(question);
  }else
  {
    int current_question;
    /* :TODO: load the previous question
       to the forum */
    char buffer[20];
    GetWindowText(gChildWindows[MAIN_INDEX_QUESTION_NUMBER].handle,buffer,20);
    sscanf(buffer,"%i",&current_question);
    if(current_question==1)
    {
      MessageBox(NULL,TEXT("Previous Question Does Not Exist"),TEXT("Error"),MB_OK|MB_ICONEXCLAMATION);
      return;
    }else{
      // :TODO: load the current question to the forum.
      current_question--;
      mainLoadQuestion(gAppGlobalData.questions.at(current_question-1));
    }

  }
}

//
// mainOnClikcCancel
//
// @parms HWND hwnd the window handle of the main window
// returns NULL
void mainOnClickSaveAndExit(HWND hwnd)
{
    //:TODO: implement this method.

}


// Function
//   mainIsForumEmpty
// @parms none
// @returns bool returns ture if the forum is empty.
bool mainIsForumEmpty()
{
  bool return_value=true;
  // question body text
  if(GetWindowTextLength(gChildWindows[MAIN_INDEX_QUESTION_BODY].handle)!=0)
    return_value=false;
  // add image text box.
  if(gAppGlobalData.bImagePathIsSet)
    return_value=false;

  // selections
  for(int i=0;i<NUM_SELECTIONS;i++)
    if(IsWindowEnabled(gChildWindows[MAIN_INDEX_SELECTION_1_EDIT+2*i].handle))
      if((GetWindowTextLength(gChildWindows[MAIN_INDEX_SELECTION_1_EDIT+2*i].handle)!=0)||(mainGetSelectionState(i)!=0))
       return_value=false;
  return return_value;
}



BOOL CALLBACK SaveAndExitDialogProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
  /// :TODO: implement this procedure.
  switch(message)
  {
    case WM_COMMAND:
      // :TODO:
      break; /* if the command is not processed then return false */
    case WM_INITDIALOG:
      // set the number of questions dialog.
      std::string str_number_of_questions;
      char ch;
      std::stack<char> reverse_stack;
      int number_of_questions = gAppGlobalData.questions.size();
      while(number_of_questions!=0)
      {
          ch = number_of_questions%10 +'0';
          reverse_stack.push(ch);
          number_of_questions/=10;
      }
      if(reverse_stack.empty())
        str_number_of_questions="0";
      while(!reverse_stack.empty())
      {
        str_number_of_questions += reverse_stack.top();
        reverse_stack.pop();
      }
      SetWindowText(GetDlgItem(hwnd,ID_EDIT_NUMBER_OF_QUESTIONS),str_number_of_questions.c_str());
      // :TODO: set up the other initial texts in other windows inside the dialog box.
      return TRUE;
      //:TODO: process the messages from other buttons also.


  }
  return FALSE; // we have not processed the message.
}





















