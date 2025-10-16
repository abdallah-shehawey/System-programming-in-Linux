# Linux User Management and File Permissions Guide

## 1. User Identification Basics

These commands help you identify who you are and who else is active on the system:

| Command    | Description                                                                                                                            |
| ---------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| `whoami`   | Displays the **Effective User ID** (username) of the current user.                                                                     |
| `w`        | Lists users currently logged in to the machine and their session information.                                                          |
| `id`       | Displays all current user identities, including the **User ID (UID)** number and the primary/supplementary **Group ID (GID)** numbers. |
| `hostname` | Displays the current machine's hostname.                                                                                               |

## 2. Managing Users and Groups

### Adding Users

There are two main ways to add a user:

| Command                   | Description                                                                                                                                                                          |
| ------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `sudo adduser [username]` | **Interactive Method:** The easiest and most common way. It automatically creates the user's home directory, sets up a shell, and prompts for additional information and a password. |
| `sudo useradd [username]` | **Non-Interactive Method:** More complex, often used in scripts. It requires manually specifying options (like path, groups, etc.) or relying on system defaults.                    |
| `groupadd [groupname]`    | To add a new group to the system.                                                                                                                                                    |

### Changing Passwords

```bash
passwd # To change the password for the current user.
sudo passwd [username] # To change the password for another user (requires root privileges).
```

### Modifying Users

```bash
man usermod # Displays the full manual for the user modification command.
```

**Adding a user to a group while preserving existing groups:**

```bash
# WARNING: This command is dangerous! It will REPLACE all existing groups with ONLY the groups you list (-G).
sudo usermod username -G groupsname 

# **The Correct Way:** To APPEND the user to a new group while preserving existing groups (-aG).
sudo usermod username -aG groupname
```

### Deleting Users

```bash
sudo userdel username
# To delete the user AND their home directory (recommended)
# sudo userdel -r username
```

## 3. Root Privileges and `sudo`

| Command              | Description                                                                                                                                                    |
| -------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `sudo`               | **Super User Do:** Allows authorized users to execute commands with **Root** privileges or as another specified user.                                          |
| `su`                 | **Switch User:** Used to switch to another user; it prompts for the target user's password.                                                                    |
| `sudo su - username` | To log in as a new user completely, including setting up their environment variables and moving to their home directory (a **Login Shell**).                   |
| `sudo visudo`        | Opens the `sudo` configuration file (`/etc/sudoers`). You **MUST** use this command instead of other editors to ensure syntax checking before saving the file. |

## 4. System Information Files

These files contain critical user and group definitions.

| Command                 | Description                                                                                                                                 |
| ----------------------- | ------------------------------------------------------------------------------------------------------------------------------------------- |
| `sudo nvim /etc/passwd` | Displays all user accounts and their details (excluding the password hash). Each line is split into seven fields separated by colons.       |
| `man 5 passwd`          | To view the details and explanation of the seven fields in this file.                                                                       |
| `nvim /etc/group`       | Displays information about the groups known to the system.                                                                                  |
| `man 5 group`           | To view the details and explanation of the group file contents.                                                                             |
| `nvim /etc/shadow`      | Displays the encrypted (Hashed) passwords for users and password aging information. **This file should only be readable by the Root user.** |
| `man 5 shadow`          | To view the details and explanation of the fields in this sensitive file.                                                                   |

## 5. Shell Exit Status and Hash Functions

### Last Command Status (Exit Status)

In the Bash environment, the exit status (Success/Failure) of the last executed command is stored in the special variable `$?`.

- **Success (True):** Represented by the code `0`.
    
- **Failure (False) or Error:** Represented by the code `1` or any other non-zero number.
    

```bash
# Displays the exit status of the very last command executed
echo $?
```

### What is a Hash Function?

A hash function is a mathematical algorithm that takes an input of arbitrary length (a password, file, or message) and converts it into a fixed-length output called a "hash value" or **digest**. In Linux, this is used to securely store passwords in the `/etc/shadow` file.

**Important Properties of Cryptographic Hash Functions:**

1. **One-Way Function / Pre-image Resistance:** It's virtually impossible to deduce the original input (password) from the resulting hash value.
    
2. **Collision Resistance:** It is computationally difficult to find two different inputs that produce the exact same hash value.
    

|Input|Process|Resulting Hash (Digest)|
|---|---|---|
|Fox|$\rightarrow$ Cryptographic Hash Function $\rightarrow$|DFCD 3454 8B8A 788A 751A 696C 24D9 7009 CA99 2017|
|The red fox jumps over the blue dog|$\rightarrow$ Cryptographic Hash Function $\rightarrow$|D086 46BB FB7D CB82 823C ACC7 6CD1 90B1 EE6E 3ABC|
|The red fox jumps ouer the blue dog|$\rightarrow$ Cryptographic Hash Function $\rightarrow$|BFD8 7550 7851 4F32 D1C6 76B1 79A9 0DA4 ABF2 4819|

**Common Examples:**

- **Outdated and Insecure:** DES, MD5, SHA1.
    
- **Currently Recommended:** SHA-256, SHA-512.
    

**For information on the `crypt` function (used for encryption/hashing):**

```bash
man 3 crypt && man 5 crypt
```

## 6. File Permissions (Next Steps)

This section will cover the core concepts of file and directory permissions in Linux, including viewing and changing them.

### Viewing Permissions

```bash
ls -l # Detailed listing of files, including permissions, ownership, and size.
# and you can use stat to any file to show file status and it's details
stat file.txt
```

### Understanding the Permission String

The output of `ls -l` shows a 10-character string (e.g., `drwxr-xr--` or `-rw-r--r--`).

| Character Position | Meaning                                | Options                                                                          |
| ------------------ | -------------------------------------- | -------------------------------------------------------------------------------- |
| **1st**            | File Type                              | `-` (Regular File), `d` (Directory), `l` (Symbolic Link), `c` (character device) |
| **2nd - 4th**      | **User (Owner)** Permissions           | `r` (Read), `w` (Write), `x` (Execute), `-` (No Permission)                      |
| **5th - 7th**      | **Group** Permissions                  | `r`, `w`, `x`, `-`                                                               |
| **8th - 10th**     | **Others** (Everyone Else) Permissions | `r`, `w`, `x`, `-`                                                               |

### Changing Ownership

The `chown` command changes the owner and/or group of a file or directory.

```bash
# Change only the owner of 'file.txt' to 'newuser'
sudo chown newuser file.txt

# Change only the group of 'file.txt' to 'newgroup'
sudo chown :newgroup file.txt

# Change both the owner and group
sudo chown newuser:newgroup file.txt

# Recursively change ownership for a directory and its contents (-R)
sudo chown -R newuser:newgroup /path/to/directory
```

### Changing Permissions (`chmod`)

Permissions can be changed using two main methods: **Symbolic** (letters) or **Octal** (numbers).

#### Octal (Numeric) Method

Each permission type (`r`, `w`, `x`) is assigned a numeric value:

- **r** (Read) $= 4$
    
- **w** (Write) $= 2$
    
- **x** (Execute) $= 1$
    
- **-** (No Permission) $= 0$
    

Permissions are summed for each set (User, Group, Others).

|Permission String|Calculation|Octal Code|
|---|---|---|
|`rwx` (Read, Write, Execute)|$4 + 2 + 1$|**7**|
|`rw-` (Read, Write)|$4 + 2 + 0$|**6**|
|`r-x` (Read, Execute)|$4 + 0 + 1$|**5**|
|`r--` (Read Only)|$4 + 0 + 0$|**4**|

**Example using Octal Code:**

```bash
# Set permissions to: User=rwx (7), Group=r-x (5), Others=r-- (4)
# Resulting permission string: -rwxr-xr--
chmod 754 script.sh
```

#### Symbolic (Alphabetic) Method

This method uses letters to specify:

- **Who** (`u`=User, `g`=Group, `o`=Others, `a`=All)
    
- **Action** (`+`=Add, `-`=Remove, `=`=Set Exactly)
    
- **Permission** (`r`, `w`, `x`)
    

**Examples using Symbolic Method:**

```bash
# Add execute permission to the User
chmod u+x script.sh

# Remove write permission from Others
chmod o-w file.txt

# Set Group permissions exactly to Read and Write
chmod g=rw data.log

# Add read permission to all (User, Group, Others)
chmod a+r config.ini
```

### Special Permissions (SUID, SGID, Sticky Bit)

**To read everything about these bits:**

```
man inode # Displays the manual page for the inode, which is the data structure that stores all file metadata, including permissions and ownership.
```

Linux provides three Special Permission Bits that extend the standard `rwx` permission model:

| Octal Code | Bit Name                | Symbol | Description and Purpose                                                                                                                                                                                                             |
| ---------- | ----------------------- | ------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `4000`     | **Set-User-ID (SUID)**  | `u+s`  | When an executable file with SUID set is run, the process executes with the permissions of the file's **owner**, not the user who ran it. This is essential for programs requiring elevated privileges to function.                 |
| `2000`     | **Set-Group-ID (SGID)** | `g+s`  | **For Files:** The process executes with the permissions of the file's **group**. **For Directories:** Any new files or subdirectories created inside the folder automatically inherit the group ownership of the parent directory. |
| `1000`     | **Sticky Bit (SVTX)**   | `o+t`  | Prevents non-owners of a file from deleting or renaming it inside a shared directory, even if they have Write permission to the directory.                                                                                          |

#### Set-User-ID (SUID) Example: The `passwd` Command

The `passwd` command is the most famous example of SUID:

- **Need:** To change your password, the program needs to modify the sensitive `/etc/shadow` file, which only the **`root`** user has write access to.
    
- **Application:** The SUID bit (octal code 4) is set on the `/usr/bin/passwd` file. When you run it, the process temporarily runs with the permissions of `root` (the file's owner), allowing it to modify the password file successfully.
    
- **In `ls -l`:** The lowercase `s` appears in the owner's execute position (`-rwsr-xr-x`).
    

#### Sticky Bit Example: The `/tmp` Directory

The `/tmp` directory often has the Sticky Bit set:

- **Permissions:** The permissions for `/tmp` are usually `drwxrwxrwt` (or octal code `1777`), meaning everyone has Read, Write, and Execute permissions.
    
- **Effect:** Users can create, read, and write their own files. However, the presence of the Sticky Bit (`t`) ensures that **only the file's owner (or root) can delete or rename the file** within this shared directory, preventing users from tampering with others' files.
    
- **In `ls -l`:** The lowercase `t` appears in the others' execute position (`drwxrwxrwt`).
    

#### How to Set Special Permissions (Using `chmod`)

To set a special permission, the specific octal code (4, 2, or 1) is combined with the standard 3-digit octal code (User, Group, Others).

**Syntax: `chmod [SUID/SGID/SBIT Code] [Standard Permissions Code] [File/Directory]`**

|Scenario|Octal Command|Resulting Permissions in `ls -l`|
|---|---|---|
|Add SUID (4) and set standard 755 permissions|`chmod 4755 script.sh`|`-rwsr-xr-x`|
|Add Sticky Bit (1) and set standard 777 permissions|`chmod 1777 /shared_folder`|`drwxrwxrwt`|
|Add SGID (2) and set standard 770 permissions (for directories)|`chmod 2770 /project`|`drwxrws---`|

_Note: If the execute permission (`x`) is not already set for the owner or group, a capital letter (`'S'` or `'T'`) will appear instead of the lowercase letter (`'s'` or `'t'`). This indicates that the special bit is set but is currently non-functional._