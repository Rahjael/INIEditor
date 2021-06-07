
#include <gtest/gtest.h>
#include "../../src/INIEditor.cpp"

#include <filesystem>

// I was having a hard time making GTest find my files to parse, when suddenly 
// I realized Hey I can do this:
TEST(UTILITY, WhereAmI) {
  std::cout << "You are here: " << std::filesystem::current_path() << std::endl;
  EXPECT_TRUE(true);
}
// So apparently GTest runs from /home/USER/PROJECT_DIR/build/test"


namespace Testing {
  std::string TEST_PATH = "../../test/";
  std::string defaultFile = TEST_PATH + "Skyrim_short.INI";
  std::string writeTestFile = TEST_PATH + "writeTest.INI";
}

// Testing setup
// XXX the following files must be present in PROJECT_DIR/build/test:
// - Skyrim_short.INI
// - Skyrim_short_spaces.INI


TEST(INIEditor, file_parsing) {
  INIEditor editor;
  editor.setWorkingFile(Testing::defaultFile);
  editor.parseWorkingFile();
  std::vector<std::string> testLines = editor.getLines();

  // for(auto& line : testLines) {
  //   std::cout << line << std::endl;
  // }

  EXPECT_EQ(testLines.size(), 25);
  EXPECT_EQ(testLines[0], "[General]");
  EXPECT_EQ(testLines[1], "sLanguage=ENGLISH");
  EXPECT_EQ(testLines[24], "fMagnetismLookingMult=0.0");

  editor.setExpectedExit();
}

TEST(INIEditor, whitespace_removal_during_parsing) {
  std::string filename = Testing::TEST_PATH + "Skyrim_short_spaces.INI";
  INIEditor editor;
  editor.setWorkingFile(filename);
  editor.parseWorkingFile();
  std::vector<std::string> testLines = editor.getLines();

  EXPECT_EQ(testLines.size(), 14);
  EXPECT_EQ(testLines[0], "[General]");
  EXPECT_EQ(testLines[3], "uExterior Cell Buffer=36");
  EXPECT_EQ(testLines[5], "[Display]");
  EXPECT_EQ(testLines[7], "fSpecularLODMaxStartFade=2000.0");
  EXPECT_EQ(testLines[10], "fMusicDuckingSeconds=6.0");

  editor.setExpectedExit();
}



/*
*
* Public API tests
*
*/

INIEditor editor;

TEST(INIEditor, setWorkingFile) {
  editor.setExpectedExit();
  editor.setWorkingFile(Testing::defaultFile);
  EXPECT_EQ(editor.getCurrentFilename(), Testing::defaultFile);
}

TEST(INIEditor, clearAll) {
  EXPECT_EQ(editor.getCurrentFilename(), Testing::defaultFile);
  // XXX clearAll() gives a segfault if called before anything is parsed.
  // I'm pretty sure it has something to do with
  // this->sectionsKeyValues->clear();
  // called within it, but I don't really understand why
  editor.parseWorkingFile();
  editor.clearAll();
  EXPECT_EQ(editor.getCurrentFilename(), "");
}

TEST(INIEditor, getKeyValuePair) {
  editor.setWorkingFile(Testing::defaultFile);
  editor.parseWorkingFile();
  std::string key = editor.getKeyValuePair(1).first;
  std::string value = editor.getKeyValuePair(1).second;

  EXPECT_EQ(key, "sLanguage");
  EXPECT_EQ(value, "ENGLISH");

  key = editor.getKeyValuePair(16).first;
  value = editor.getKeyValuePair(16).second;

  EXPECT_EQ(key, "SGeneralMasterMismatchWarning");
  EXPECT_EQ(value, "One or more plugins could not find the correct versions of the master files they depend on. Errors may occur during load or game play. Check the \"Warnings.txt\" file for more information.");
}

TEST(INIEditor, getPtrToSectionsKeyValues) {
  auto keyValuesMap = *editor.getPtrToKeyValuesMap();
  std::string sectionName = "Audio";
  
  EXPECT_EQ(keyValuesMap[sectionName]["fMusicDuckingSeconds"], "6.0");
}

TEST(INIEditor, replaceEntireLine) {
  auto keyValuesMap = *editor.getPtrToKeyValuesMap();
  std::string section = "Grass";
  std::string key = "bAllowCreateGrass";
  std::string value = "1";
  std::string newLine = std::string(key + "=" + value);
  editor.replaceEntireLine(12, newLine);

  EXPECT_EQ(keyValuesMap[section][key], value);
}


TEST(INIEditor, writeFile) {
  auto pair = editor.getKeyValuePair(1);
  pair.second = "ITALIAN";
  editor.replaceEntireLine(1, std::string(pair.first) + "=" + pair.second);
  editor.setWorkingFile(Testing::writeTestFile);
  editor.writeLinesToFile();

  INIEditor newEditor;
  newEditor.setWorkingFile(Testing::writeTestFile);
  newEditor.setExpectedExit();
  newEditor.parseWorkingFile();
  auto newMap = *newEditor.getPtrToKeyValuesMap();

  EXPECT_EQ(newMap["General"][pair.first], pair.second);  
}


// TODO test deleteLine??? Feels unnecessary