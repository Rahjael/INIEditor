
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

std::string UI::inputName() const {
  // This is an easy fixer to parse only the first word from a user input
  std::string name;
  std::cin >> name;
  this->clearCin();
  return name;
}

unsigned int UI::inputNum() const {
  unsigned int num;
  std::cin >> num;
  this->clearCin();
  return num;
}

bool UI::confirmInputPrompt() const {
  char choice;
  do {
    std::cout << "Confirm? (y/n)" << std::endl;
    std::cin >> choice;
  } while(choice != 'y' && choice != 'n');
  
  return choice == 'y' ? true : false;
}



void UI::mainMenu() const {
  unsigned int choice;
  do{
    std::cout << std::endl << std::endl;
    std::cout << "Main menu:" << std::endl;
    std::cout << "1. Load existent INI file" << std::endl;
    std::cout << "2. Quit program" << std::endl;
    std::cout << "Select an option using a number:";

    choice = this->inputNum();

    switch(choice) {
      case 1:
        this->loadFile();
        break;
      case 2:
        std::cout << "Exiting program..." << std::endl;
        editor->setExpectedExit();
        return;
      default:
      std::cout << "\n\nPlease use numbers to navigate the menu." << std::endl;
    }
  } while(choice != 3);
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
    std::cout << "No files detected. Make sure there is at least 1 .INI file in the executable folder." << std::endl;
    exit(1);
  }

  std::cout << "\n\nFound " << files.size() << " .INI file" << (files.size() > 1 ? "s " : " ") << "in program directory:" << std::endl;

  do {
    for(int i = 0; i < files.size(); i++) {
      std::cout << i + 1 << ". " << files[i] << std::endl;
    }
    std::cout << "Select file to edit: ";
    choice = this->inputNum();
  } while(choice < 1 || choice > files.size());

  std::cout << files[choice - 1] << " selected" << std::endl;

  this->editor->setWorkingFile(files[choice - 1]);
  std::cout << "\n\n Working file set: " << this->editor->getCurrentFilename() << std::endl;

  this->workWithFile();
}

void UI::workWithFile() const {
  std::cout << "\n\nWorking with file: " << editor->getCurrentFilename() << std::endl;

  std::cout << "Loading lines... ";
  this->editor->parseWorkingFile();
  std::cout << "done." << std::endl;
  std::cout << "\nListing file lines:" << std::endl;

  auto lines = editor->getLines();

  for(int i = 0; i < lines.size(); i++) {
    std::cout << i << ". " << lines[i] << std::endl;
  }

  std::cout << "Enter the number of the line to edit: " << std::endl;

  bool keepAsking;
  unsigned int choice;

  do {
    keepAsking = false;
    choice = this->inputNum();
    if(choice >= this->editor->getNumberOfLines()) {
      std::cout << "Invalid index. Please insert another number." << std::endl;
      keepAsking = true;
    }
  } while(keepAsking);

  this->lineMenu(choice);
}

void UI::lineMenu(unsigned int num) const {
  unsigned int choice;
  do{
    std::cout << std::endl << std::endl;
    std::cout << "What do you want to do with this line?" << std::endl;
    std::cout << "1. Edit line" << std::endl;
    std::cout << "2. Delete line" << std::endl;

    choice = this->inputNum();

    switch(choice) {
      case 1:
        this->editLine(num);
        break;
      case 2:
        this->deleteLine(num);
        return;
      default:
      std::cout << "\n\nPlease choose either 1 or 2." << std::endl;
    }
  } while(choice != 1 && choice != 2);
}

void UI::deleteLine(unsigned int num) const {
  char choice;
  std::cout << "Deleting line " << num << std::endl;

  choice = this->confirmInputPrompt();
  if(choice) {
    this->editor->deleteLine(num);
    this->editor->writeLinesToFile();
    std::cout << "Line deleted." << std::endl;
  }
  else {
    std::cout << "Line not deleted." << std::endl;
  }
}

void UI::editLine(unsigned int num) const {
  std::string line = this->editor->getLines()[num];
  std::cout << "Line " << num << " contains: " << line << std::endl;

  if(line.at(0) == '[') {
    std::cout << "Selected line is a section name. Choose a new name." << std::endl;
    std::string newString = '[' + this->inputName() + ']';

    if(this->confirmInputPrompt()) {
      this->editor->replaceEntireLine(num, newString);
    }
  }
  else {
    std::pair<std::string, std::string> keyValue = this->editor->getKeyValuePair(num);
    std::cout << "Selected line is a key/value pair. Select new value for key: " << keyValue.first << std::endl;
    std::cout << "Current value: " << keyValue.second << std::endl;
    std::string newValue = this->inputName();
    std::string newString = keyValue.first + '=' + newValue;

    if(this->confirmInputPrompt()) {
      this->editor->replaceEntireLine(num, newString);
    }
  }

  this->editor->writeLinesToFile();
  std::cout << "Line edited. Lines saved to file." << std::endl;
}

