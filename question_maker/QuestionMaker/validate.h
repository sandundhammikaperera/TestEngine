// validate.h -- validates values on the edit text box.
//
//  consist the declarations of the functions that are needed to
//  validate the inputs of edit box controls.
// author: Sandun Dhammika Perera.
// Last Modefied: 11/5/2010
//

#ifndef __VALIDATE_H__
#define __VALIDATE_H__

#include <iostream>
#include <string>
#include <windows.h>

#ifndef __LIMITS__
#define __LIMITS__

#define LIMITS_MAX_BODY_TEXT_LENGTH        1000
#define LIMITS_MAX_QUESTIONS               200
#define LIMITS_MAX_SELECTION_TEXT_LENGTH   150
#define LIMITS_MIN_NUMBER_OF_SELECTIONS    2
#define LIMITS_MAX_NUMBER_OF_SELECTIONS    5
#define LIMITS_MAX_CX_BITMAP               100
#define LIMITS_MAX_CY_BITMAP               100
// :TODO: more things have to be comming soon.
#endif


/* validateValidateInputs
  @parms hwnd- handle to the parent window that contains the child windows.

  @returns integer
          Return the Child window index that contains the
          validation error. 0 if success.
*/
int validateValidateInputs(HWND);

/* validateGetLastValidateError

   @return std::string&
          Returns the last error string.
*/
std::string& validateGetLastValidateError();

#endif
