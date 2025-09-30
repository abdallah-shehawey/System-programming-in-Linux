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

- Nevertheless, **FAT** continues to be commonly used on relatively small capacity **solid-state storage** technologies such as **USB flash drives**, **SD cards**, **Multimedia-cards (MMC)** and **eMMC** <u> **because of its compatibility across operating systems and embedded systems, and ease of implementation.** </u>

- FAT32 is the most recent version of the FAT file system.

---

## Design of The FAT File System

- The FAT file system is a **file system** used on **MS-DOS** and **Windows 9x family of operating systems**.

- It continues to be used on **mobile devices and embedded systems**, and thus is a well-suited file system for data exchange between computers and devices of almost any type and age from **1981** through to the present.

## Structure of FAT32

- **FAT File system regions, it includes four main regions** :

![Tux](./img/fat16-vs-fat32.avif)

| **Region** | Reserved Sectors | FAT Region | Root Directory Region | Data Region |
|---|---|---|---|---|
| **Size in sectors**| number of reserved sectors| (number of FATs) * (sectors per FAT) | (number of root entries * 32) / (bytes per sector) | (number of clusters) * (sectors per cluster) |
| **Contents** | Boot Sector \| fs information sector(FAT32 only) \| More reserved Sectors *(optional)* | File Allocation Table #1 \| File Allocation Table #2 ... *(optional)*| Root Directory (FAT12 and FAT16 only) | Data Region (for files and directories) ... (to end of partition or disk) |
| **Notes** | The first reserved sector (logical sector 0) is the Boot Sector (also called Volume Boot Record or simply VBR). It includes an area called the BIOS Parameter Block (BPB) which contains some basic file system information, in particular its type and pointers to the location of the other sections, and usually contains the operating system's boot loader code.Important information from the Boot Sector is accessible through an operating system structure called the Drive Parameter Block (DPB) in DOS and OS/2.The total count of reserved sectors is indicated by a field inside the Boot Sector, and is usually 32 on FAT32 file systems.[4]For FAT32 file systems, the reserved sectors include a File System Information Sector at logical sector 1 and a Backup Boot Sector at logical sector 6. While many other vendors have continued to utilize a single-sector setup (logical sector 0 only) for the bootstrap loader, Microsoft's boot sector code has grown to span over logical sectors 0 and 2 since the introduction of FAT32, with logical sector 0 depending on sub-routines in logical sector 2. The Backup Boot Sector area consists of three logical sectors 6, 7, and 8 as well. In some cases, Microsoft also uses sector 12 of the reserved sectors area for an extended boot loader. | This typically contains two copies of the File Allocation Table for the sake of redundancy checking, although rarely used, even by disk repair utilities. These are maps of the Data Region, indicating which clusters are used by files and directories. In FAT12 and FAT16 they immediately follow the reserved sectors. Typically the extra copies are kept in tight synchronization on writes, and on reads they are only used when errors occur in the first FAT. The first two clusters (cluster 0 and 1) in the map contain special values.  | This is a Directory Table that stores information about the files and directories located in the root directory. It is only used with FAT12 and FAT16, and imposes on the root directory a fixed maximum size which is pre-allocated at creation of this volume. FAT32 stores the root directory in the Data Region, along with files and other directories, allowing it to grow without such a constraint. Thus, for FAT32, the Data Region starts here. | This is where the actual file and directory data is stored and takes up most of the partition. FAT32 typically commences the Root Directory Table in cluster number 2: the first cluster of the Data Region. |

### Some Notes

- The size of Sector is 512B.
- but in most hard drives manufactured today use a physical sector size of 4096B (4KB). This is known as Advanced Format (AF).
- Root directory only exist in FAT12 & FAT16.
- A key improvement in FAT32 over its predecessors is that the root directory is no longer a fixed size. It's stored in the Data Region just like any other folder, allowing it to grow as you add more files.
- FAT32 typically commences the Root Directory Table in cluster number 2: the first cluster of the Data Region.
