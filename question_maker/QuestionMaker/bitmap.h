//  File: bitmap.h
//  Comment:Declares the bitmap validating and handing functions.
//
//
#ifndef __BITMAP__
#define __BITMAP__

#include <windows.h>
#include <iostream>


enum eVALIDATEBITMAPERROR
{
  VALIDATE_BITMAP_SUCCESS,
  VALIDATE_BITMAP_ERROR,
  VALIDATE_BITMAP_LARGE_ERROR,
  VALIDATE_BITMAP_COLOR_FORMAT_ERROR ,
  VALIDATE_BITMAP_FILE_OPEN_ERROR,
  VALIDATE_BITMAP_NOT_VALID_BITMAP
};

int bitmapValidateBitmapByFile(std::string&);
HBITMAP bitmapGetBitMapIndirect(void*,int);

bool bitmapIsQuestionBitmapAvaliable(int);
HBITMAP bitmapGetBitmpByQuestionNumber(int);
bool bitmapCreateBitmapOnTempDir(void*,int,int);

#endif // END OF bitmap.h //




