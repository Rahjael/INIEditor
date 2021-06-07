#include "INIEditor.h"


std::vector<std::string> INIEditor::parseFileLinesToStringVector(const std::string& filename) const {  
  std::vector<std::string> lines;
  std::fstream fileToParse;
  std::string tempLine;

  fileToParse.open(filename);
  if(!fileToParse) {
    std::cerr << "Could not open " << filename << ". Make sure it exists or it is not already in use." << std::endl;
    exit(1);
  }
  while(getline(fileToParse, tempLine)) {
    lines.push_back(tempLine);
    tempLine.clear();
  }
  fileToParse.close();

  // Remove whitespaces from beginning of lines
  for(auto& line : lines) {
    while(line[0] == ' ') {
      line.erase(0, 1);
    }
  }
  return lines; // move semantics implemented by the compiler?
}

INIEditor::~INIEditor() {
  delete this->sectionsKeyValues;
  if(this->isUnexpectedQuit && this->workingFile != "") {
    std::cout << "Autosaving because of unexpected exit..." << std::endl;
    std::string filename = "autosave.ini";
    this->saveCurrentLines(filename);
  }
}

std::vector<std::string> INIEditor::getLines() const {
  // unsigned short index = 0;
  // for(auto& it : this->lines) {
  //   std::cout << index << ". \t" << it << std::endl;
  //   index++;
  // }

  return this->lines;
}

void INIEditor::listSections() const {
  std::cout << "\nPrinting mapped sections and key/value pairs:" << std::endl;
  for(auto& it : *(this->sectionsKeyValues)) {
    std::cout << "Section: " << it.first << std::endl;
    for(auto& jt : it.second) {
      std::cout << "\t" << jt.first << " = " << jt.second << std::endl;
    }
  }
  std::cout << "End." << std::endl;
}

void INIEditor::setWorkingFile(std::string filename) {
  this->workingFile = filename;
}

std::string INIEditor::getCurrentFilename() const {
  return this->workingFile;
}

std::pair<std::string, std::string> INIEditor::getKeyValuePair(unsigned int index) const {
  std::pair<std::string, std::string> keyValue = this->getKeyValuePair(this->lines[index]);
  return keyValue;
}

const mapKeyValues * INIEditor::getPtrToSectionsKeyValues() const {
  return this->sectionsKeyValues;
}

std::pair<std::string, std::string> INIEditor::getKeyValuePair(std::string line) const {
  char delimiter = '=';
  std::pair<std::string, std::string> keyValue;
  std::istringstream sstream(line);
  std::getline(sstream, keyValue.first, delimiter);
  std::getline(sstream, keyValue.second, delimiter);
  return keyValue;
}

mapKeyValues * INIEditor::getSectionsKeyValuePairsMap(std::vector<std::string> lines) const {
  mapKeyValues * sectionsKeyValuesMap = new mapKeyValues();
  // Char and string literals
  char commentChar = ';';
  char beginSectionChar = '[';
  char endSectionChar = ']';
  char equalChar = '=';
  std::string emptyString = "";
  std::string currentSection;

  for(int i = 0; i < lines.size(); i++) {
    // Skip comments and empty lines
    if(lines[i][0] == commentChar || lines[i] == emptyString) continue;

    // Detect section
    if(lines[i][0] == beginSectionChar) {
      // In Javascript, this would be a single line. But this is C++, so:
      std::string tempLine = lines[i]; // Copy string to avoid messing up
      tempLine.erase(0, 1); // Erase first char (presumably '[')
      std::istringstream tempStream(tempLine); // Cast string into stringstream to allow use of std::getline()
      std::getline(tempStream, tempLine, endSectionChar); // Parse text before token ( ']' )
      currentSection = tempLine; // Assign new section name
      std::vector<std::pair<std::string, std::string>> emptyVector;
      sectionsKeyValuesMap->insert({currentSection, emptyVector}); // Insert new section with empty vector of keyValue pairs
      continue;
    }

    if(lines[i].find(equalChar) != std::string::npos) {
      std::pair<std::string, std::string> keyValuePair;
      std::string tempLine = lines[i];
      std::istringstream tempStream(tempLine);
      std::getline(tempStream, keyValuePair.first, equalChar);
      std::getline(tempStream, keyValuePair.second, equalChar);
      (*sectionsKeyValuesMap)[currentSection].push_back(keyValuePair);
    }
  }

  return sectionsKeyValuesMap;  
}

void INIEditor::parseWorkingFile() {
  this->lines = this->parseFileLinesToStringVector(this->getCurrentFilename());
  this->parseMapFromLines();
}

void INIEditor::parseMapFromLines() {
  delete this->sectionsKeyValues;
  this->sectionsKeyValues = this->getSectionsKeyValuePairsMap(this->lines);  
}

void INIEditor::replaceEntireLine(int index, std::string value) {
  this->lines[index] = value;
  this->parseMapFromLines();
}

void INIEditor::replaceLineByKeyValue(int index, std::pair<std::string, std::string> keyValue) {
  this->replaceEntireLine(index, std::string(keyValue.first + "=" + keyValue.second));
}

void INIEditor::setExpectedExit() {
  this->isUnexpectedQuit = false;
}

void INIEditor::clearAll() {
  this->workingFile = "";
  this->lines.clear();
  this->sectionsKeyValues->clear();
}


void INIEditor::saveCurrentLines(std::string filename) {
  std::ofstream file;
  file.open(filename);
  for(auto& line : this->lines) {
    file << line << std::endl;
  }
  file.close();
}



void INIEditor::replaceLine(int index, std::string& newString) {
  this->lines[index] = newString;
}

