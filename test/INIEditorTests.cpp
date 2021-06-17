
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
  std::string invalidName = "blah";
}

// Testing setup
// XXX the following files MUST be present in PROJECT_DIR/build/test:
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
  auto keyValuesMap = editor.getKeyValuesMap();
  std::string sectionName = "Audio";
  
  EXPECT_EQ(keyValuesMap[sectionName]["fMusicDuckingSeconds"], "6.0");
}

TEST(INIEditor, replaceEntireLine) {
  auto keyValuesMap = editor.getKeyValuesMap();
  std::string section = "Grass";
  std::string key = "bAllowCreateGrass";
  std::string value = "1";
  std::string newLine = std::string(key + "=" + value);
  editor.replaceEntireLine(12, newLine);

  EXPECT_EQ(keyValuesMap[section][key], value);
}

// This feels a bit unnecessary, but you never know
TEST(INIEditor, deleteLine) {
  auto prevNumLines = editor.getNumberOfLines();
  auto indexToDelete = 15;
  auto lineToDelete = editor.getLines()[indexToDelete];
  
  editor.deleteLine(indexToDelete);

  EXPECT_FALSE(prevNumLines == editor.getNumberOfLines());
  EXPECT_FALSE(lineToDelete == editor.getLines()[indexToDelete]);
}

TEST(INIEditor, insertLine) {
  auto prevNumLines = editor.getNumberOfLines();
  auto addAtIndex = 15;
  auto lineToAdd = std::string("testKey=testValue");
  auto shiftedLine = editor.getLines()[addAtIndex];
  
  editor.insertLine(addAtIndex, lineToAdd);

  EXPECT_EQ(prevNumLines + 1, editor.getNumberOfLines());
  EXPECT_EQ(shiftedLine, editor.getLines()[addAtIndex + 1]);
  EXPECT_EQ(editor.getLines()[addAtIndex], lineToAdd);
}

TEST(INIEditor, writeFile) {
  auto pair = editor.getKeyValuePair(1);
  pair.second = "ITALIAN";
  auto newString = std::string(pair.first) + "=" + pair.second;
  editor.replaceEntireLine(1, newString);
  editor.setWorkingFile(Testing::writeTestFile);
  editor.writeLinesToFile();

  INIEditor newEditor;
  newEditor.setWorkingFile(Testing::writeTestFile);
  newEditor.setExpectedExit();
  newEditor.parseWorkingFile();
  auto newMap = newEditor.getKeyValuesMap();

  EXPECT_EQ(newMap["General"][pair.first], pair.second);  
}

TEST(INIEditor, getSection) {
  editor.setWorkingFile(Testing::defaultFile);
  editor.parseWorkingFile();

  std::string sectionName = "Display";
  auto map = editor.getSection(sectionName);

  EXPECT_EQ(map.at("fShadowLODMaxStartFade"), "1000.0");
  EXPECT_EQ(map.at("fSpecularLODMaxStartFade"), "2000.0");
}

TEST(INIEditor, deleteSection) {
  editor.setWorkingFile(Testing::defaultFile);
  editor.parseWorkingFile();

  unsigned int beforeNum = editor.getNumberOfLines();
  std::string sectionName = "Display";

  bool result = editor.deleteSection(sectionName);

  EXPECT_TRUE(result);
  EXPECT_FALSE(editor.deleteSection(Testing::invalidName));
}

TEST(INIEditor, addSection) {
  editor.setWorkingFile(Testing::defaultFile);
  editor.parseWorkingFile();

  unsigned int beforeNum = editor.getNumberOfLines();

  std::string sectionName = "NewName";
  auto result = editor.addSection(sectionName);
  auto afterNum = editor.getNumberOfLines();

  EXPECT_EQ(beforeNum, 25);
  EXPECT_EQ(afterNum, 28); // NOTE: adding 1 line actually adds 3 because empty lines are added at the end of each section
  EXPECT_TRUE(result);
}


TEST(INIEditor, addPairToSection) {
  editor.setWorkingFile(Testing::defaultFile);
  editor.parseWorkingFile();

  std::string key = "newKey";
  std::string value = "newValue";
  std::string section = "newSection";

  editor.addSection(section);
  editor.addPairToSection(section, key, value);

  auto map = editor.getSection(section);

  EXPECT_EQ(map[key], value);
  // adding pair to non-existent section
  EXPECT_ANY_THROW(editor.addPairToSection(Testing::invalidName, key, value));
  // trying to add an already existing key
  EXPECT_FALSE(editor.addPairToSection(section, key, value));
}

TEST(INIEditor, getValueBySectionAndKey) {
  editor.setWorkingFile(Testing::defaultFile);
  editor.parseWorkingFile();

  std::string key = "newKey";
  std::string value = "newValue";
  std::string section = "newSection";

  editor.addSection(section);
  editor.addPairToSection(section, key, value);

  auto newValue = editor.getValueBySectionAndKey(section, key);

  EXPECT_EQ(newValue, value);


  EXPECT_ANY_THROW(editor.getValueBySectionAndKey(Testing::invalidName, Testing::invalidName));
}



TEST(INIEditor, renameSection) {
  editor.setWorkingFile(Testing::defaultFile);
  editor.parseWorkingFile();

  std::string toRename = "Display";
  std::string newName = "newName";
  std::string oldKey = "fShadowLODMaxStartFade";
  std::string oldValue = "1000.0";

  EXPECT_EQ(oldValue, editor.getValueBySectionAndKey(toRename, oldKey));

  editor.renameSection(toRename, newName);

  auto newValue = editor.getValueBySectionAndKey(newName, oldKey);

  EXPECT_EQ(newValue, oldValue);

  auto result = editor.renameSection(Testing::invalidName, Testing::invalidName);
  EXPECT_FALSE(result);
}



TEST(INIEditor, editValue) {
  editor.setWorkingFile(Testing::defaultFile);
  editor.parseWorkingFile();

  std::string section = "Display";
  std::string key = "fShadowLODMaxStartFade";
  std::string newValue = "2000.0";

  editor.editValue(section, key, newValue);

  EXPECT_EQ(editor.getValueBySectionAndKey(section, key), newValue);
  // Test invalid key
  EXPECT_FALSE(editor.editValue(section, Testing::invalidName, newValue));
  // Test invalid section
  EXPECT_FALSE(editor.editValue(Testing::invalidName, key, newValue));
}

 
TEST(INIEditor, deleteKey) {
  editor.setWorkingFile(Testing::defaultFile);
  editor.parseWorkingFile();

  std::string section = "Display";
  std::string key = "fShadowLODMaxStartFade";

  auto keysNum = editor.getSection(section).size();
  editor.deleteKey(section, key);
  auto newKeysNum = editor.getSection(section).size();

  EXPECT_NE(newKeysNum, keysNum);
  EXPECT_EQ(keysNum - 1, newKeysNum);
  // Test invalid section
  EXPECT_FALSE(editor.deleteKey(Testing::invalidName, key));
  // Test invalid key
  EXPECT_FALSE(editor.deleteKey(section, Testing::invalidName));
}


