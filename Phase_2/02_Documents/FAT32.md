# Explanation of FAT32

- File Allocation Table

---

## At first : Some Notes

- is file system developed for personal computer and was the default file system for **MS-DOS** and **windows 9x OS** .

- it developed in **1977** .

- it used of **floppy disks** .

- it was adabted for use on hard disk and other devices .

- The increase in disk drive capacity over time drove modifications to the design that resulted in versions: **FAT12, FAT16, FAT32, and exFAT**.

- FAT was replaced with **NTFS** as the default file system on Microsoft operating systems starting with **Windows XP** .

- Nevertheless, **FAT** continues to be commonly used on relatively small capacity **solid-state storage** technologies such as **USB flash drives**, **SD cards**, **MultiMediaCards (MMC)** and **eMMC** because of its compatibility across operating systems and embedded systems, and ease of implementation.

- FAT32 is the most recent version of the FAT file system.

---

## Design of The FAT File System

- The FAT file system is a **file system** used on **MS-DOS** and **Windows 9x family of operating systems**.

- It continues to be used on **mobile devices and embedded systems**, and thus is a well-suited file system for data exchange between computers and devices of almost any type and age from **1981** through to the present.

## Structure of FAT32

- **FAT File system regions, it includes four main regions** :


| **Region** | Reserved Sectors | FAT Region | Root Directory Region | Data Region |
|---|---|---|---|---|
| **Size in sectors**| number of reserved sectors| (number of FATs) * (sectors per FAT) | (number of root entries * 32) / (bytes per sector) | (number of clusters) * (sectors per cluster) |
| **Contents** | Boot Sector  fs information sector(FAT32 only) | | |
| **Notes** | | | |

