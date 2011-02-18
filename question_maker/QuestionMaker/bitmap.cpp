// bitmap.cpp
//
// provides the helper functions to validate bitmaps.
// Modified: 12/24/2010
// THIS_FILE_IS_NOT_TESTED

// system includes

#include <windows.h>
#include <iostream>
#include <string>
#include "bitmap.h"

int bitmapValidateBitmapByFile(std::string& strFileName)
{
  // open the file.
  HANDLE hFile=CreateFile(strFileName.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if(hFile==INVALID_HANDLE_VALUE)
  {
    return VALIDATE_BITMAP_FILE_OPEN_ERROR;
  }
  // close the handle.
  CloseHandle(hFile);

  // then check for the correct dimensions.
  // open the file as a bitmap.
  HBITMAP hBitmap=(HBITMAP)LoadImage(
    GetModuleHandle(NULL),
    strFileName.c_str(),
    IMAGE_BITMAP,
    0,
    0,
    LR_LOADFROMFILE
  );
  // handle the error codes when opening the file.
  if(hBitmap==NULL)
  {
    return VALIDATE_BITMAP_NOT_VALID_BITMAP;
  }

  // then take the dimensions of the bitmap.
  BITMAP bitmap;
  GetObject(hBitmap,sizeof(BITMAP),&bitmap);

#define BITMAP_MAX_CX    100
#define BITMAP_MAX_CY    200
  /// check for the x and y
  if(bitmap.bmWidth>BITMAP_MAX_CX || bitmap.bmHeight> BITMAP_MAX_CY)
  {

    MessageBox(NULL,TEXT("bitmap is too large"),TEXT("bitmap"),MB_OK);
    return VALIDATE_BITMAP_LARGE_ERROR;
  }
  // :TODO: close the handles that we alrady created.
  DeleteObject(hBitmap);
  return VALIDATE_BITMAP_SUCCESS;
}


HBITMAP bitmapGetBitmapIndirect(void* vpBitmapData,int cbDataSize)
{
  // :TODO: implement this.
  // Get the temporary directory.

  // Get our subdirectory.



}

HBITMAP bitmapGetBitmapByQuestionNumber(int iQuestionNumber)
{
  // make the path to our temporary directory.
  char buffer_file_path[200];
  char buffer_rest[100];
  HBITMAP hBitmap;
  GetTempPath(200,buffer_file_path);
  int path_length=strlen(buffer_file_path);
  if(buffer_file_path[path_length-1]=='/'||buffer_file_path[path_length]=='\\')
  {
    wsprintf(buffer_rest,"%i",iQuestionNumber);

  }else{
    wsprintf(buffer_rest,"\\%i",iQuestionNumber);
  }

  std::string file_name(buffer_file_path);
  file_name+=buffer_rest;

  // we got the required file path with the file name.
  // now just open the file.
  HANDLE hFile=CreateFile(
    file_name.c_str(),
    GENERIC_READ,
    FILE_SHARE_READ,
    NULL,OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,NULL);

  // check whether the file is exists or not.
  if(hFile==INVALID_HANDLE_VALUE)
  {
    MessageBox(NULL,TEXT("The required bitmap file on the temporary file can't be opened"),TEXT("Error -exitin..."),MB_OK |MB_ICONEXCLAMATION);
    exit(0);
  }

  CloseHandle(hFile);

  // then load this as a bitmap.
  hBitmap=(HBITMAP)LoadImage(GetModuleHandle(NULL),file_name.c_str(),IMAGE_BITMAP,0,0,
                    LR_LOADFROMFILE);
  if(hBitmap==NULL)
  {
    MessageBox(NULL,TEXT("The file is not a valid bitmap file"),TEXT("Error-Terminating the program"),MB_OK|MB_ICONEXCLAMATION);
    exit(0);
  }
  return hBitmap;
}

// function: bitmapCreateBitmapOnTempDir()
//
// arguments: void * vpBitmapFileData
//            int    cbFileDataSize
//            int    iQuestionNumber
//
// return bool
//        true - if success
//        false - if error occurred.
static bool bIfTempDirectoryCalculated=false; /* when the program starts it don't know where is my temporary
                                                directory is , when the first time any function  that uses that
                                                directory will be called , it will calculate the directory and save
                                                it in the static buffer bellow. and it will make the bIfTempDirectoryCalculated
                                                to true */
static char sTemporaryDirectory[500];
bool bitmapCreateBitmapOnTempDir(void *vpBitmapFileData,int cbFileDataSize,int iQuestionNumber)
{
  // if not that temporary directory is calculated then do calculate it.
  if(!bIfTempDirectoryCalculated)
  {
    if(!GetTempPath(500,sTemporaryDirectory))
      MessageBox(NULL,TEXT("Get the temporary file error"),TEXT("Error -terminating the application"),MB_OK|MB_ICONEXCLAMATION);
#define APP_TEMP_SUB_DIR TEXT("question_maker")
    strcat(sTemporaryDirectory,APP_TEMP_SUB_DIR);
  }
  // calculate the file name to open the file on the disk.
  char buffer_file_name[500];
  wsprintf(buffer_file_name,"\\%i.txt",iQuestionNumber);
  HANDLE hFile=CreateFile(buffer_file_name,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
  if(hFile==INVALID_HANDLE_VALUE)
    return false;

  WriteFile(hFile,vpBitmapFileData,cbFileDataSize,NULL,NULL);
  // close the file handle
  CloseHandle(hFile);
  return true;
}





