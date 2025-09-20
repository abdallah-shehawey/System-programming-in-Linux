# 📜 Utilities Documentation

## 1️⃣ pwd
**Description:** Prints the absolute path of the current working directory.  

**Syntax:**
```bash
pwd
```

**Example:**
```bash
$ ./pwd
/home/user/projects
```

**Output:**
```
/home/user/projects
```

---

## 2️⃣ echo
**Description:** Prints the arguments passed to it, separated by spaces.  

**Syntax:**
```bash
echo [STRING...]
```

**Example:**
```bash
$ ./echo Hello World from C
Hello World from C
```

**Output:**
```
Hello World from C
```

---

## 3️⃣ cp
**Description:** Copies a file from a source path to a destination path.  
If the destination file exists, it will be overwritten.  

**Syntax:**
```bash
cp SOURCE_FILE DESTINATION_FILE
```

**Example:**
```bash
$ ./cp file.txt /tmp/file_copy.txt
$ cat /tmp/file_copy.txt
Hello from file.txt
```

**Output:**
```
(no output if successful, destination file is created with the same content)
```

---

## 4️⃣ mv
**Description:** Moves or renames a file to a new location or name.  
If the destination file exists, it will be overwritten.  

**Syntax:**
```bash
mv SOURCE_FILE DESTINATION_FILE
```

**Example (rename):**
```bash
$ ./mv file.txt newfile.txt
$ ls
newfile.txt
```

**Example (move):**
```bash
$ ./mv newfile.txt /tmp/newfile.txt
$ ls /tmp
newfile.txt
```

**Output:**
```
(no output if successful, source file is moved or renamed)
```
