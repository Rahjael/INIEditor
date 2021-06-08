
# INIEditor

## A simple command-line .INI editor

- "Why not just use Vim or even Notepad?", you may ask.
- "This is an assignment for university.", I would reply.
- "Oh, I see", you would then acknowledge.
- "But consider the following: it's a CLASS, not a fully fledged software. Probably it's meant to work inside another program which may need such a class. Or whatever.", I might add.
- "Ohhh I like that", you would reply, gently nodding.

**General overview and things to keep in mind:**

- The program autodetects .INI files, provided they are in the same folder of the executable
- It's just a single class with a few APIs wrapped in a support UI class that makes the program usable in a CLI
- In a nutshell: at startup, the program scans its own folder for .INI files and prompts the user to select one. This file is then parsed and every line is copied into a vector of strings. Every modification is applied to this vector, which is then overwritten onto the file when the user quits the program. There is also an autosave feature for unexpected exits (->autosave.ini).
- Every line is also parsed into a layered map made like this:
__std::map<std::string, std::map<std::string, std::string>>__ ("map of section names each containing a map of keys each having a value")
this map is currently not used, but it's there in case it turns out to be useful in the future.


**TODOs:**

- there are a couple raw pointers here and there and this may bring shame to me and my family. I will fix that ASAP, I promise.
- add many more input checks in the CLI (although these concern the UI more than the actual class of the exercise, so I don't know if they will be required)