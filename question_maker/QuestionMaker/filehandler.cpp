// filehandler.cpp
//   implements the filehandler.h
//
// Modified: 1/26/2011
// Modified By: Sandun Dhammika

#include "filehandler.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <windows.h>
#include "../../test_taker/question.h"

/* local static help functions */
static int WriteSelections(const AbstractQuestion&,HANDLE);
static int WriteQuestion(const AbstractQuestion&,HANDLE);
static int WriteFileHeader(int,int,int,HANDLE);
static int WriteInteger(int,HANDLE);
static int WriteByte(char ,HANDLE);
static int GetImageFileSize(const std::string&);
static int GetImageDataFromFile(const std::string&,void*);
static int CalculateQuestionLength(const AbstractQuestion& abstractQuestion);
static int CalculateAllSelectionsLength(const AbstractQuestion& abstractQuestion);

/* save to the file */
bool FH_SaveToFile(const DataToSave&data,const std::string& filename)
{
  // try to open the file.
  // if failed then return false.
  HANDLE hFile;
  if(INVALID_HANDLE_VALUE==(hFile=CreateFile(filename.c_str(),GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL)))
    return false;
  WriteFileHeader(data.nNumberOfQuestions,data.nTimeInSeconds,data.nMarks,hFile);

  // write questions //
  for(int i=0;i<data.questions.size();i++)
  {
     WriteQuestion(data.questions.at(i),hFile);
  }
  CloseHandle(hFile);  // if no error occurred.
  return true;
}


static int WriteSelections(const AbstractQuestion&abstractQuestion,HANDLE hFile)
{
  int number_of_selections = abstractQuestion.mVectorSelections.size();
  WriteInteger(number_of_selections,hFile);
  int cbBytesWritten=4;
  for(int i=0;i<number_of_selections;i++)
  {
    int selection_text_length= abstractQuestion.mVectorSelections.at(i).text.size();
    int index                = abstractQuestion.mVectorSelections.at(i).index;
    int current_bytes_written;
    // write selection_text_length
    WriteInteger(selection_text_length,hFile);
    WriteByte(index,hFile);
    cbBytesWritten+=5;
    WriteFile(hFile,abstractQuestion.mVectorSelections.at(i).text.c_str(),selection_text_length,(DWORD*)current_bytes_written,NULL);
    cbBytesWritten+=current_bytes_written;
  }
  return cbBytesWritten;
}

#define INDEX_NUMBER_OF_QUESTIONS 0
#define INDEX_OF_TIME             4
#define INDEX_OF_MARKS            8
static int WriteFileHeader(int numberOfQuestions,int timeInMinutes,int marks,HANDLE hFile)
{
  char *out_buffer=new char[32];
  int* ptr_number_of_questions=(int*)(out_buffer+INDEX_NUMBER_OF_QUESTIONS);
  int* ptr_time= (int*)(out_buffer+INDEX_OF_TIME);
  int* ptr_marks=(int*)(out_buffer+INDEX_OF_MARKS);

  *ptr_number_of_questions=numberOfQuestions;
  *ptr_time=timeInMinutes;
  *ptr_marks=marks;

  // zero the memory of the padding fields.
  ZeroMemory(ptr_marks+4,20);

  DWORD cbBytesWritten=0;
    /* then write this structure to the file */
  if(!WriteFile(hFile,out_buffer,32,&cbBytesWritten,NULL))
    return -1;
  if(cbBytesWritten!=32)
    return -1;
  return 0;
}


static int WriteQuestion( const AbstractQuestion& abstractQuestion,HANDLE hFile)
{
  int cbBytesWritten=0;
  // write question number
  WriteInteger(abstractQuestion.GetQuestionNumber(),hFile);
  cbBytesWritten+=4;

  // write question length
  WriteInteger(CalculateQuestionLength(abstractQuestion),hFile);
  cbBytesWritten+=4;

  // write the question type.
  if(abstractQuestion.mQuestionType==QUESTIONTYPE_SINGLE)
   WriteByte('\0',hFile);
  else
   WriteByte(1,hFile);
  cbBytesWritten+=1;

  // write body text length
  WriteInteger(abstractQuestion.mStrBodyText.size(),hFile);
  cbBytesWritten+=1;

  // write body text.
  int cbLastWrittenBytes;
  WriteFile(hFile,abstractQuestion.mStrBodyText.c_str(),abstractQuestion.mStrBodyText.size(),(DWORD*)&cbLastWrittenBytes,NULL);

  // image data length.
  WriteInteger(GetImageFileSize(abstractQuestion.GetImagePath()),hFile);
  cbBytesWritten+=4;

  // write image data.
  int cbImageDataSize=GetImageFileSize(abstractQuestion.GetImagePath());
  void *buffer= malloc(cbImageDataSize);
  GetImageDataFromFile(abstractQuestion.GetImagePath(),buffer);
  WriteFile(hFile,buffer,cbImageDataSize,(DWORD*)&cbLastWrittenBytes,NULL);
  cbBytesWritten+=cbImageDataSize;
  free(buffer);

  // answers
  char answers=0;
  for(int i=1;i<5;i++)
  {
    bool is_marked=abstractQuestion.GetSelectionAnswerValue(i);
    if(is_marked)
    {
      char current_answer=1;
      current_answer<<i;
      answers|=current_answer;
    }
  }
  WriteByte(answers,hFile);
  cbBytesWritten+=1;

  // write selection length
  WriteInteger(CalculateAllSelectionsLength(abstractQuestion),hFile);
  cbBytesWritten+=4;

  // write selections.
  cbBytesWritten+=WriteSelections(abstractQuestion,hFile);
  return cbBytesWritten;
}

/*
  function static int WriteInteger
  @parms int integer the integer to write.
  @parms HANDLE hFile the file handle of the file to be written.
  @returns int -1 fails [0,) if success,number of bytes written
  */
static int WriteInteger(int integer,HANDLE hFile)
{
  char write_buffer[4];
  int *output=(int*)write_buffer;
  *output=integer;
  int cbBytesWritten=0;
  if(!WriteFile(hFile,write_buffer,4,(DWORD*)&cbBytesWritten,NULL))
    return -1;
  return cbBytesWritten;
}


/*
  function static int WriteBytes
  @parms char ch the character to write.
  @parms HANDLE hFile the file hande that data to be written.
  @returns int number of bytes written to the file -1 if faills.
  */
static int WriteByte(char ch,HANDLE hFile)
{
  int cbBytesWritten=0;
  if(!WriteFile(hFile,&ch,1,(DWORD*)&cbBytesWritten,NULL))
    return -1;
  return cbBytesWritten;
}

/*
  function int GetImageFileSize
  @parms std::string& imageFilePath file path that contains the image
  @returns number of bytes in the file.
*/
static int GetImageFileSize(const std::string& imageFilePath)
{
  HANDLE hImageFile=CreateFile(imageFilePath.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  int cbFileSize=GetFileSize(hImageFile,NULL);
  CloseHandle(hImageFile);
  return cbFileSize;
}

/*
  function static int GetImageDataFromFile
  @parms std::string& imgFilePath file path
  @parms void* buffer buffer to where to get image data.
*/
static int GetImageDataFromFile(const std::string& imgFilePath,void* buffer)
{
  HANDLE hFile=CreateFile(imgFilePath.c_str(),GENERIC_ALL,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  int file_size=GetFileSize(hFile,NULL);
  int cbBytesRead;
  if(!ReadFile(hFile,buffer,file_size,(DWORD*)&cbBytesRead,NULL))
    return -1;
  CloseHandle(hFile);
  return cbBytesRead;
}

/*
  function static int CalculateQuestionLength(AbstractQuestion& abstractQuestion)

  @parms abstractQuestion& the question to calculate the length.
  @returns returns the length of the structure when it stored inside the file.
  */
static int CalculateQuestionLength(const AbstractQuestion& abstractQuestion)
{
  // question length = length of the data in headers + body text length + image data length + length of selections.
  int question_length=22; // question_number ,question_length,question_type,body_text_length,image_data_length,answers,
                          // selection_structure_length

 question_length+=abstractQuestion.GetQuestionBodyText().size();
 question_length+=GetImageFileSize(abstractQuestion.GetImagePath());
 question_length+=CalculateAllSelectionsLength(abstractQuestion);
 return question_length;
}

/*
    function static int CalculateAllSelectionsLength
  @parms AbstractQuesation& abstractQuestion
  @returns all lengths
*/
static int CalculateAllSelectionsLength(const AbstractQuestion& abstractQuestion)
{
  int length=4;
  for(int i=0;i<abstractQuestion.GetNumberOfSelections();i++)
  {
     length+=5;
     length=abstractQuestion.mVectorSelections.at(i).text.size();
  }
  return length;
}

/* Read the first 32-bits out of the file and fill appropriate
   values in the DataToRead structure                             */
static void ReadFileHeader(DataToRead&,HANDLE);

/* Read the next question out from the file.
   And fill to the abstractQuestion data structure ,

   @returns true if success
            false if error occurred
            */
static bool ReadNextQuestion(AbstractQuestion&,HANDLE);

/*
    Read the next selection out from the file.
    ANd fill to the abstractQuestion data structure.

    @returns int number of bytes read.
                 -1 if error occurred.

*/
static int ReadNextSection(AbstractQuestion&,HANDLE);


/*
  function bool FH_ReadFromFile(DataToRead&,const std::string&);


*/
bool FH_ReadFromFile(DataToRead& data,const std::string fileName)
{
    // open the file, in the OPEN_EXISTING mode.
    HANDLE hFile = NULL;
    hFile = CreateFile(fileName.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(INVALID_HANDLE_VALUE==hFile)
    {
        MessageBox(NULL,TEXT("File Does not exists"),TEXT("Error "),MB_OK|MB_ICONERROR);
        return false;
    }

    // read the questions //
    for(int i=0;i<data.nNumberOfQuestions;i++)
    {
        AbstractQuestion abstractQuestion;
        ReadNextQuestion(abstractQuestion,hFile);
        data.questions.push_back(abstractQuestion);
    }

    // close file.
    CloseHandle(hFile);
    return true;
}


static void ReadFileHeader(DataToRead& data,HANDLE hFile)
{
    // allocate 32-bits for the read.
    char buffer[32];
    int cb_Bytes_read;
    ReadFile(hFile,buffer,32,(DWORD*)&cb_Bytes_read,NULL);
    data.nNumberOfQuestions=*(int*)(buffer+INDEX_NUMBER_OF_QUESTIONS);
    data.nTimeInSeconds=*(int*)(buffer+INDEX_OF_TIME);
    data.nMarks=*(int*)(buffer+INDEX_OF_MARKS);
    return;
}


/*
   Define the offset values of the files in question structure.
*/

#define INDEX_QUESTION_NUMBER             0
#define INDEX_QUESTION_LENGTH             4
#define INDEX_QUESTION_TYPE               8
#define INDEX_BODY_LENGTH                 9

static bool ReadNextQuestion(AbstractQuestion& abstractQuestion,HANDLE hFile)
{
    // first we must read the first 13-bytes out of the file.
    char buffer[13];
    int cb_Number_Of_Bytes_Read;
    // try to read 13 bytes out of the file.
    if(!ReadFile(hFile,buffer,13,(DWORD*)&cb_Number_Of_Bytes_Read,NULL))
    {
        if(ERROR_HANDLE_EOF==GetLastError())
        {
            MessageBox(NULL,TEXT("Error ReadNextQuestion. End of file reached"),TEXT("Error"),MB_OK|MB_ICONERROR);
            return false;
        }
    }

    abstractQuestion.SetQuestionNumber(*(int*)(buffer+INDEX_QUESTION_NUMBER));
    int cb_question_length=(*(int*)(buffer+INDEX_QUESTION_LENGTH));
    char question_type=*(char*)(buffer+INDEX_QUESTION_TYPE);
    abstractQuestion.SetQuestionType(question_type);
    int cb_body_text_length = *(int*)(buffer+INDEX_BODY_LENGTH);
    // Then read next cb_body_text_length.
    char *buffer_body_text=new char[cb_body_text_length];
    if(!ReadFile(hFile,buffer_body_text,cb_body_text_length,(DWORD*)&cb_body_text_length,NULL))
    {
        if(ERROR_HANDLE_EOF!=GetLastError())
        {
            MessageBox(NULL,TEXT("Error ReadingNextQuestion. End of file reached"),TEXT("Error"),MB_OK|MB_ICONERROR);
            return false;
        }
    }
    std::string body_text;
    body_text="";
    for(int i=0;i<cb_body_text_length;i++)
    {
        body_text+=buffer_body_text[i];
    }
    delete [] buffer_body_text;

    // read the next 4 bytes from the memory.
    int image_data_length;
    int cb_bytes_read;
    if(!ReadFile(hFile,&image_data_length,4,(DWORD*)&cb_bytes_read,NULL))
    {
        if(ERROR_HANDLE_EOF!=GetLastError())
        {
            MessageBox(NULL,TEXT("Error ReadingNextQuestion. End of file Reached."),TEXT("Error."),MB_OK|MB_ICONERROR);
            return false;
        }
    }

    // if there are image data exists then read it
    if(image_data_length!=0)
    {
        int cb_image_data_read;
        char * buffer_image_data=new char[image_data_length];
        if(!ReadFile(hFile,buffer_image_data,image_data_length,(DWORD*)&cb_image_data_read,NULL))
        {
            if(ERROR_HANDLE_EOF!=GetLastError())
            {
                // Error reading next Question.
                // Error reading the NEXT QUESTION.
                MessageBox(NULL,TEXT("Error ReadingNextQuestion.End of file Reached."),TEXT("Error"),MB_OK|MB_ICONERROR);
                return false;
            }
        }
    }

    // read next 5 bytes
    // [answers + selection_length]
    char buffer_2 [5];
    int  cb_bytes_read_buffer2;
    if(!ReadFile(hFile,buffer_2,5,(DWORD*)&cb_bytes_read_buffer2,NULL))
    {
        if(ERROR_HANDLE_EOF!=GetLastError())
        {
            MessageBox(NULL,TEXT("Error ReadingNextQuestion.End of file Reached."),TEXT("Error"),MB_OK|MB_ICONERROR);
            return false;
        }
    }
    char answers=buffer_2[0];
    int cb_selection_length;
    cb_selection_length=*(int*)(buffer_2+1);

    int more_selection_length_to_read = cb_selection_length;
    while(more_selection_length_to_read>0)
    {
        int bytes_read=ReadNextSection(abstractQuestion,hFile);
           if(bytes_read==-1)   /* if error happens */
           {
               return false;
           }
           // otherwise //
           more_selection_length_to_read-=bytes_read;
    }
    return true;
}

static int ReadNextSection(AbstractQuestion&abstractQuestion,HANDLE hFile)
{
    char buffer_1 [5];
    // read the first 5 bytes //
    int cb_bytes_read;
    if(!ReadFile(hFile,buffer_1,5,(DWORD*)&cb_bytes_read,NULL))
    {
        MessageBox(NULL,TEXT("Error ReadingNextSection."),TEXT("Error"),MB_OK|MB_ICONERROR);
        return -1;
    }
    int selection_text_length=*(int*)buffer_1;
    // read out the index and fill the AbstractQuestion //
    int selection_index=buffer_1[4];

    // read the selection text //
    char buffer_selection_text[selection_text_length];
    if(!ReadFile(hFile,buffer_selection_text,selection_text_length,(DWORD*)&cb_bytes_read,NULL))
    {
         MessageBox(NULL,TEXT("Error Reading Next Section"),TEXT("Error"),MB_OK|MB_ICONERROR);
         return -1;
    }

    // create a new selection.
    Selection selection;
    selection.index=selection_index;
    selection.text="";

    // add the buffer text to the selection //
    for(int i=0;i<selection_text_length;i++)
    {
        selection.text+= buffer_selection_text[i];
    }

    // Add the selection to the abstractQuestion //
    abstractQuestion.AddSelection(selection);

    // if success return true //
    return selection_text_length+5;
}




