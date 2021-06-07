
#include "UI.h"

// Non-compiler-dependent include of <filesystem>.
// I'm doing this because I'm working on this code from different machines with different OSs 
// and I can't fix all the compatibility issues. This works, so this stands.

// XXX found a bug in MINGW version 8.2 for Windows. A bunch of errors come up
// when trying to use <filesystem> or <experimental/filesystem>.
// No problems with MINGW 7.X on Windows nor g++ 9.3 on Ubuntu. This is a dirty but effective fix,
// just set CAN_AUTODETECT in the header accordingly before compiling.

#if CAN_AUTODETECT
  #ifdef __has_include
    # if __has_include(<filesystem>)
    #   include <filesystem>
    #   define have_filesystem 1
        namespace fs = std::filesystem;
    # elif __has_include(<experimental/filesystem>)
    #   include <experimental/filesystem>
    #   define have_filesystem 2
        namespace fs = std::experimental::filesystem;
    # else
    #   define have_filesystem 0
    # endif
  #endif

  #define PATH_TO_INIS "./" // TODO what will this be when the program is completed? I want the executable to scan its own directory
  // Using googletest framework forces me to either change this or put my test files in a different directory. I chose the latter and put
  // a couple INIs in PROJECT/build/test

  std::vector<std::string> UI::detectINIs() const {
    std::vector<std::string> INIFiles;

    switch(have_filesystem) {
      case 1: std::cout << "USING C++17 <filesystem>" << std::endl; break;
      case 2: std::cout << "USING C++11 <experimental/filesystem>" << std::endl; break;
      case 0: std::cout << "FILESYSTEM NOT INCLUDED, SKIPPING AUTOMATIC DETECTION" << std::endl; throw std::runtime_error("Impossible to detect files");
    }

    for(auto& item : fs::directory_iterator(PATH_TO_INIS)) {
      std::regex reg("\\.ini$", std::regex::ECMAScript | std::regex::icase);
      std::string itemName = item.path();

      if(std::regex_search(itemName, reg)) {
        // Clean name from "./"" chars at the beginning
        itemName.erase(0, 2);
        INIFiles.push_back(itemName);
      }
    }
    return INIFiles;
  }
#endif





UI::UI(INIEditor * editor)
  : editor{editor}
{
  this->mainMenu();
}

void UI::clearCin() const {
  // These two lines are necessary to flush away any non integer chars the user
  // might input, otherwise we get stuck in a loop.
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  
}

std::string UI::getName() const {
  // This is an easy fixer to parse only the first word from a user input
  std::string name;
  std::cin >> name;
  this->clearCin();
  return name;
}


void UI::mainMenu() const {
  unsigned int choice;
  do{
    std::cout << std::endl << std::endl;
    std::cout << "Main menu:" << std::endl;
    std::cout << "1. Create new INI file" << std::endl;
    std::cout << "2. Load existent INI file" << std::endl;
    std::cout << "3. Quit program" << std::endl;
    std::cout << "Select an option using a number:";

    std::cin >> choice;

    switch(choice) {
      case 1:
        this->createNew();
        break;
      case 2:
        this->loadFile();
        break;
      case 3:
        std::cout << "Exiting program..." << std::endl;
        editor->setExpectedExit();
        return;
      default:
      std::cout << "\n\nPlease use numbers to navigate the menu." << std::endl;
      this->clearCin();
    }
  } while(choice != 3);
}

void UI::createNew() const {
  std::cout << "Insert name for new file:";
  std::string filename = this->getName() + ".ini";
  std::cout << "Chosen name: " << filename << std::endl;
  this->editor->setWorkingFile(filename);
  std::cout << "\n\n working file set: " << this->editor->getCurrentFilename() << std::endl;

  this->workWithFile();
}

void UI::loadFile() const {
  unsigned int choice;

  std::cout << "\n\nDetecting files in program's folder...";

  std::vector<std::string> files;
  try {
    files = this->detectINIs();
  }
  catch(std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  
  if(files.size() < 1) {
    std::cout << "No files detected." << std::endl;

    // TODO add option to set filename manually when detection fails

    return;
  }

  std::cout << "\n\nFound " << files.size() << " .INI file" << (files.size() > 1 ? "s " : " ") << "in program directory:" << std::endl;

  do {
    this->clearCin();
    for(int i = 0; i < files.size(); i++) {
      std::cout << i + 1 << ". " << files[i] << std::endl;
    }
    std::cout << "Select file to edit: ";
    std::cin >> choice;    
  } while(choice < 1 || choice > files.size());

  std::cout << files[choice - 1] << " selected" << std::endl;

  this->editor->setWorkingFile(files[choice - 1]);
  std::cout << "\n\n Working file set: " << this->editor->getCurrentFilename() << std::endl;

  this->workWithFile();
}



void UI::workWithFile() const {

  // TODO currently working on this function

  std::cout << "\n\nWorking with file: " << editor->getCurrentFilename() << std::endl;

  std::cout << "Loading lines... ";
  this->editor->parseWorkingFile();
  std::cout << "done." << std::endl;

  // this->editor->listLines();

  this->editor->listSections();

  unsigned int choice;

  std::cin >> choice;

  std::cout << (editor->getKeyValuePair(choice)).first << " " << (editor->getKeyValuePair(choice)).second << std::endl;



  // unsigned int choice;
  // do{
  //   std::cout << std::endl << std::endl;
  //   std::cout << "Main menu:" << std::endl;
  //   std::cout << "1. Create new INI file" << std::endl;
  //   std::cout << "2. Load existent INI file" << std::endl;
  //   std::cout << "3. Quit program" << std::endl;
  //   std::cout << "Select an option using a number:";

  //   std::cin >> choice;

  //   switch(choice) {
  //     case 1:
  //       this->createNew();
  //       break;
  //     case 2:
  //       this->loadFile();
  //       break;
  //     case 3:
  //       std::cout << "Exiting program..." << std::endl;
  //       return;
  //     default:
  //     std::cout << "\n\nPlease use numbers to navigate the menu." << std::endl;
  //     this->clearCin();
  //   }
  // } while(choice != 3);


}














