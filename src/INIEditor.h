#ifndef __INIEDITOR_H__
#define __INIEDITOR_H__

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <new>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


// This is just for readability
using mapKeyValues = std::map<std::string, std::map<std::string, std::string>>;


class INIEditor {

public:
  INIEditor() : workingFile{""}, isUnexpectedExit{true} {}
  ~INIEditor(); // XXX !!! Has custom implementation for unexpected exits !!!

  // Public APIs WITHOUT side effects
  std::vector<std::string> getLines() const;
  unsigned int getNumberOfLines() const;
  std::string getCurrentFilename() const;
  std::pair<std::string, std::string> getKeyValuePair(unsigned int) const;
  mapKeyValues getKeyValuesMap() const;

  // Public APIs WITH side effects
  void insertLine(unsigned int, std::string&); // Insert line at specified index
  void clearAll(); // Clears attributes: workingFile, lines, sectionsKeyValues
  void deleteLine(unsigned int);
  void parseWorkingFile(); // Parses both into lines and sectionsKeyValues
  void replaceEntireLine(int, std::string&);
  void replaceLineByKeyValuePair(int, std::pair<std::string, std::string>);
  void setExpectedExit();
  void setWorkingFile(std::string&);
  void writeLinesToFile();

  // Map manipulation public API
  std::string getValueBySectionAndKey(std::string&, std::string&) const; // throws by STL map.at()
  std::map<std::string, std::string> getSection(std::string&) const; // throws by STL map.at()
  bool renameSection(std::string&, std::string&);
  bool deleteSection(std::string&);
  bool addSection(std::string&);
  bool addKeyValueToSection(std::string&, std::string&, std::string&); // bool and also throws by STL map.at()
  bool editValue(std::string&, std::string&, std::string&);
  bool deleteKey(std::string&, std::string&);

  // Done:
  // Delete key in section
  // New utility: parseLinesFromMap();
  // Get section as map
  // Get pair by line number
  // Delete entire section (pairs included)
  // Add section
  // Add pair to section
  // Get value by section and key name
  // Rename section
  // Edit value by section and key name


private:
  // Internal attributes
  std::vector<std::string> lines;
  mapKeyValues sectionsKeyValues;
  std::string workingFile;
  bool isUnexpectedExit;

  // Internal helper functions WITH side effects
  void parseMapFromLines();
  void parseLinesFromMap();
  void replaceLine(int, std::string&);
  void saveCurrentLines(std::string&);

  // Internal helper functions WITHOUT side effects
  std::vector<std::string> parseFileLinesToStringVector(const std::string&) const;
  std::vector<std::string> parseMapToStringVector(const mapKeyValues&) const;
  std::pair<std::string, std::string> parseStringToKeyValuePair(const std::string&) const;
  mapKeyValues parseStringVectorToMap(std::vector<std::string>) const;





  // void listSections() const;  // TODO this doesn't really belong here, move to UI class? (was public)
};


#endif // __INIEDITOR_H__