#ifndef __INIEDITOR_H__
#define __INIEDITOR_H__

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


// This is just for readablity
using mapKeyValues = std::map<std::string, std::map<std::string, std::string>>;
// I'm still not really sure if this map thing can be helpful at all. I thought it'd make it easier
// to edit the values, but I can probably do fine with just an array of lines and some string manipulation.
// It's an INI file, does it make sense to even talk about performance?


class INIEditor {

public:
  INIEditor() : workingFile{""}, isUnexpectedExit{true}, sectionsKeyValues{new mapKeyValues()} {}
  ~INIEditor(); // Has custom implementation for unexpected exits

  // Public APIs WITHOUT side effects
  std::vector<std::string> getLines() const;
  unsigned int getNumberOfLines() const;
  void listSections() const;  // TODO move to UI class?
  std::string getCurrentFilename() const;
  std::pair<std::string, std::string> getKeyValuePair(unsigned int) const; // Overloaded in private utilities
  const mapKeyValues * getPtrToKeyValuesMap() const;

  // Public APIs WITH side effects
  void clearAll();
  void deleteLine(unsigned int);
  void parseMapFromLines();
  void parseWorkingFile(); // Parses both into lines and sectionsKeyValues
  void replaceEntireLine(int, std::string);
  void replaceLineByKeyValuePair(int, std::pair<std::string, std::string>);
  void setExpectedExit();
  void setWorkingFile(std::string);
  void writeLinesToFile();


private:
  // Internal attributes
  std::vector<std::string> lines;
  mapKeyValues * sectionsKeyValues; // Yes, destructor cleans this up
  std::string workingFile;
  bool isUnexpectedExit;


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