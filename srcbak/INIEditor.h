#ifndef __INIEDITOR_H__
#define __INIEDITOR_H__

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


// This is just for readablity
using mapKeyValues = std::map<std::string, std::vector<std::pair<std::string, std::string>>>;

class INIEditor {

public:
  INIEditor() : workingFile{""}, isUnexpectedQuit{true}, sectionsKeyValues{new mapKeyValues()} {}
  ~INIEditor(); // Has custom implementation for unexpected exits

  // Public APIs WITHOUT side effects
  std::vector<std::string> getLines() const; // TODO copy to UI class?
  void listSections() const;  // TODO move to UI class?
  std::string getCurrentFilename() const;
  std::pair<std::string, std::string> getKeyValuePair(unsigned int) const; // Overloaded in private utilities
  const mapKeyValues * getPtrToSectionsKeyValues() const;

  // Public APIs WITH side effects
  void setWorkingFile(std::string);
  void parseWorkingFile(); // Parses both into lines and sectionsKeyValues
  void parseMapFromLines();
  void replaceEntireLine(int, std::string);
  void replaceLineByKeyValue(int, std::pair<std::string, std::string>);
  void setExpectedExit();
  void clearAll();

private:
  // Internal attributes
  std::vector<std::string> lines;
  mapKeyValues * sectionsKeyValues; // Yes, destructor cleans this up

  std::string workingFile;
  bool isUnexpectedQuit;

  // Internal helper functions WITH side effects
  void replaceLine(int, std::string&);
  void saveCurrentLines(std::string);

  // Internal helper functions WITHOUT side effects
  std::vector<std::string> parseFileLinesToStringVector(const std::string&) const;
  std::pair<std::string, std::string> getKeyValuePair(std::string) const; // Overloaded in public APIs
  // TODO ? I don't really like the following pointer return... but the alternative is to overload operator= for that monster map
  mapKeyValues * getSectionsKeyValuePairsMap(std::vector<std::string>) const;

};


#endif // __INIEDITOR_H__