
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

**TODOs:**

- there are a couple raw pointers here and there and this may bring shame to me and my family. I will fix that ASAP, I promise.
- "ADD LINE" METHOD. HOW COULD YOU NOT THINK ABOUT IT???
- add many more input checks in the CLI (although these concern the UI more than the actual class of the exercise, so I don't know if they will be required)