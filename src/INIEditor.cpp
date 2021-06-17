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
  return lines;
}

std::vector<std::string> INIEditor::parseMapToStringVector(const mapKeyValues& map) const {
  std::vector<std::string> lines;
  std::string tempLine;

  for(auto& section : map) {
    tempLine = section.first;
    lines.push_back(tempLine);
    for(auto& pair : section.second) {
      tempLine = pair.first + '=' + pair.second;
      lines.push_back(tempLine);
    }
    tempLine = "";
    lines.push_back(tempLine);
  }

  return lines;  
}

INIEditor::~INIEditor() {
  //delete this->sectionsKeyValues; // TODO commented out while testing smart pointers

  if(this->isUnexpectedExit && this->workingFile != "") {
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

unsigned int INIEditor::getNumberOfLines() const {
  return this->lines.size();
}

void INIEditor::listSections() const {
  std::cout << "\nPrinting mapped sections and key/value pairs:" << std::endl;
  for(auto& it : this->sectionsKeyValues) {
    std::cout << "Section: " << it.first << std::endl;
    for(auto& jt : it.second) {
      std::cout << "\t" << jt.first << " = " << jt.second << std::endl;
    }
  }
  std::cout << "End." << std::endl;
}

void INIEditor::setWorkingFile(std::string& filename) {
  this->workingFile = filename;
}

std::string INIEditor::getCurrentFilename() const {
  return this->workingFile;
}

std::pair<std::string, std::string> INIEditor::getKeyValuePair(unsigned int index) const {
  std::pair<std::string, std::string> keyValue = this->parseStringToKeyValuePair(this->lines[index]);
  return keyValue;
}

mapKeyValues INIEditor::getKeyValuesMap() const {
  return this->sectionsKeyValues;
}

void INIEditor::insertLine(unsigned int index, std::string& newLine) {
  this->lines.insert(this->lines.begin() + index, newLine);
  this->parseMapFromLines();
}

std::pair<std::string, std::string> INIEditor::parseStringToKeyValuePair(const std::string& line) const {
  char delimiter = '=';
  std::pair<std::string, std::string> keyValue;
  std::istringstream sstream(line);
  std::getline(sstream, keyValue.first, delimiter);
  std::getline(sstream, keyValue.second, delimiter);
  return keyValue;
}

mapKeyValues INIEditor::parseStringVectorToMap(std::vector<std::string> lines) const {

  // Reminder: using mapKeyValues = std::map<std::string, std::map<std::string, std::string>>;
  mapKeyValues sectionsKeyValuesMap;
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
      // In Javascript, this would be a single line. But this is C++ (and also I'm bad) so:
      std::string tempLine = lines[i]; // Copy string to avoid messing up
      tempLine.erase(0, 1); // Erase first char (presumably '[')
      std::istringstream tempStream(tempLine); // Cast string into stringstream to allow use of std::getline()
      std::getline(tempStream, tempLine, endSectionChar); // Parse text before token ( ']' )
      currentSection = tempLine; // Assign new section name
      std::map<std::string, std::string> emptyMap;
      sectionsKeyValuesMap.insert({currentSection, emptyMap}); // Insert new section with empty vector of keyValue pairs
      continue;
    }

    if(lines[i].find(equalChar) != std::string::npos) {
      std::pair<std::string, std::string> keyValuePair;
      std::string tempLine = lines[i];
      std::istringstream tempStream(tempLine);
      std::getline(tempStream, keyValuePair.first, equalChar);
      std::getline(tempStream, keyValuePair.second, equalChar);
      sectionsKeyValuesMap[currentSection].insert(keyValuePair);
    }
  }

  return sectionsKeyValuesMap;
}

void INIEditor::parseWorkingFile() {
  this->lines = this->parseFileLinesToStringVector(this->getCurrentFilename());
  this->parseMapFromLines();
}

void INIEditor::parseMapFromLines() {
  this->sectionsKeyValues = this->parseStringVectorToMap(this->lines);
}

void INIEditor::parseLinesFromMap() {
  this->lines = this->parseMapToStringVector(this->sectionsKeyValues);
}

void INIEditor::replaceEntireLine(int index, std::string& value) {
  this->lines[index] = value;
  this->parseMapFromLines();
}

void INIEditor::replaceLineByKeyValuePair(int index, std::pair<std::string, std::string> keyValue) {
  auto newString = std::string(keyValue.first + "=" + keyValue.second);
  this->replaceEntireLine(index, newString);
}

void INIEditor::writeLinesToFile() {
  auto fileName = this->getCurrentFilename();
  this->saveCurrentLines(fileName);
}

std::string INIEditor::getValueBySectionAndKey(std::string& section, std::string& key) const {
  return this->sectionsKeyValues.at(section).at(key);
}

std::map<std::string, std::string> INIEditor::getSection(std::string& sectionName) const {
  std::map<std::string, std::string> section;
  auto it = this->sectionsKeyValues.find(sectionName);
  if(it != this->sectionsKeyValues.end()) {
    section = it->second;
  }
  return section;
}

bool INIEditor::renameSection(std::string& sectionToRename, std::string& newName) {
  auto it = this->sectionsKeyValues.find(sectionToRename);
  if(it != this->sectionsKeyValues.end()) {
    auto sectionValuesTemp = this->getSection(sectionToRename);
    this->deleteSection(sectionToRename);
    this->addSection(newName);
    this->sectionsKeyValues.at(newName) = sectionValuesTemp;
    this->parseLinesFromMap();
    return true;
  }
  return false;
}

bool INIEditor::deleteSection(std::string& sectionName) {
  auto it = this->sectionsKeyValues.find(sectionName);
  if(it != this->sectionsKeyValues.end()) {
    this->sectionsKeyValues.erase(sectionName);
    return true;
    this->parseLinesFromMap();
  }
  return false;
}

bool INIEditor::addSection(std::string& newName) {
  std::pair<std::string, std::map<std::string, std::string>> newSection;
  newSection.first = newName;
  if(this->sectionsKeyValues.emplace(newSection).second) { // map.emplace() returns a pair of <iterator to new/old value, true/false>
    this->parseLinesFromMap();
    return true;
  };
  return false;
}

bool INIEditor::addPairToSection(std::string& section, std::string& key, std::string& value) {
  if(this->sectionsKeyValues.at(section).emplace(key, value).second) {
    this->parseLinesFromMap();
    return true;
  }
  return false;
}

bool INIEditor::editValue(std::string& section, std::string& key, std::string& value) {
  auto sectionIt = this->sectionsKeyValues.find(section);
  if(sectionIt != this->sectionsKeyValues.end()) {
    auto keyIt = this->sectionsKeyValues.at(section).find(key);
    if(keyIt != this->sectionsKeyValues.at(section).end()){
      this->sectionsKeyValues.at(section).at(key) = value;
      this->parseLinesFromMap();
      return true;
    }
  }
  return false;
}

bool INIEditor::deleteKey(std::string& section, std::string& key) {
  auto sectionIt = this->sectionsKeyValues.find(section);
  if(sectionIt != this->sectionsKeyValues.end()) {
    auto keyIt = this->sectionsKeyValues.at(section).find(key);
    if(keyIt != this->sectionsKeyValues.at(section).end()){
      this->sectionsKeyValues.at(section).erase(key);
      this->parseLinesFromMap();
      return true;
    }
  }
  return false;
}

// std::string INIEditor::getValueByKey(std::string& key) const {
//   // Searches entire map for key, regardless of section (assumes every key is unique)
//   std::string value = this->sectionsKeyValues.find(key);


// TODO finish

//   return value;
// }

void INIEditor::setExpectedExit() {
  this->isUnexpectedExit = false;
}

void INIEditor::clearAll() {
  this->workingFile = "";
  this->lines.clear();
  this->sectionsKeyValues.clear();
}

void INIEditor::deleteLine(unsigned int index) {
  this->lines.erase(this->lines.begin()+index);
  this->parseMapFromLines();
}

void INIEditor::saveCurrentLines(std::string& filename) {
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

