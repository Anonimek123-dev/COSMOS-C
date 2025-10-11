# `.gitignore`
## Definition
**.gitignore – a special text file used in the Git version control system, in which you enter the names of files and folders that are to be ignored by Git.**

**This means that everything on the list in .gitignore will not be:**

- tracked
- recorded in the change history,
- or sent to the repository.

**The most common items to ignore are:**

- temporary files (e.g., *.log, *.tmp),
- directories with compilation files (/build, /dist),
- editor local settings (.vscode/, .idea/),
- files containing private data (e.g., passwords, API keys).

**Thanks to .gitignore, the repository remains clean and contains only those files that are really needed for the project.**

## .gitignore in COSMOS-C
**The structure of the .gitignore file in COSMOS-C is very simple, and I think it needs no explanation:**
```gitignore
boot/kernel.bin
```
---