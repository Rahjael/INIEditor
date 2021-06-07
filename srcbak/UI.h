#ifndef __UI_H__
#define __UI_H__

// Set this before compilation to avoid problems with certain compilers
#define CAN_AUTODETECT true

#include <climits>
#include <exception>
#include <iostream>
#include <regex>
#include <string>

#include "INIEditor.h"

class UI {
public:
  UI(INIEditor *);

private:
  INIEditor * editor;


  // Utilities
  void clearCin() const;
  std::string getName() const;

  #if CAN_AUTODETECT
  std::vector<std::string> detectINIs() const;
  #endif


  // Nav menus
  void mainMenu() const;
  void createNew() const;
  void loadFile() const;
  void workWithFile() const;



};
#endif // __UI_H__