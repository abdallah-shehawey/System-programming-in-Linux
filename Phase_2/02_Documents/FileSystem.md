# File System & Partition Management Cheatsheet

## 📑 Table of Contents

- [MBR Layout](#mbr-layout-for-dd-cmd)
- [List Disks and Partitions](#list-disks-and-partitions)
- [List Partitions on a Specific Disk](#list-partitions-on-a-specific-disk)
- [Edit a Specific Disk](#edit-a-specific-disk)
- [Fdisk Help](#fdisk-help)
- [Create a New Partition](#create-a-new-partition)
- [Change Partition Type](#change-partition-type)
- [Delete a Partition](#delete-a-partition)
- [Print Partition Table Info](#print-partition-table-info)
- [Reload Partition Table Without Reboot](#reload-partition-table-without-reboot)
- [Advanced Topics FYI](#advanced-topics-fyi)
- [GPT Layout](#guid-partition-table-gpt-scheme-breakdown)

---

## MBR Layout for dd CMD

| Offset | Size  | Description             |
|--------|-------|-------------------------|
| 0      | 446 B | Boot Code (IPL)        |
| 446    | 64 B  | Partition Table         |
| 510    | 2 B   | Signature (0x55AA)      |

### Partition Table Entry (16 Bytes Each)

| Field          | Size  | Description               |
|----------------|-------|---------------------------|
| Active         | 1 B   | Bootable flag             |
| Starting CHS   | 3 B   | Cylinder-Head-Sector addr |
| Type           | 1 B   | Partition type            |
| Ending CHS     | 3 B   | Cylinder-Head-Sector addr |
| Start Sector   | 4 B   | LBA of first sector       |
| Num Sectors    | 4 B   | Number of sectors         |

---

## List Disks and Partitions

**Simple view:**

```bash
lsblk
```

**Detailed view:**

```bash
sudo fdisk -l
```

---

## List Partitions on a Specific Disk

```bash
sudo fdisk -l /dev/<disk_name>   # e.g. sda, nvme0n1
```

---

## Edit a Specific Disk

```bash
sudo fdisk /dev/<disk_name>
```

Follow the interactive instructions.

---

## Fdisk Help

### DOS (MBR)

- `a` → toggle bootable flag  
- `b` → edit nested BSD disklabel  
- `c` → toggle DOS compatibility flag  

### Generic

- `d` → delete a partition  
- `F` → list free space  
- `l` → list known partition types  
- `n` → add a new partition  
- `p` → print partition table  
- `t` → change partition type  
- `v` → verify partition table  
- `i` → print partition details  

### Misc

- `m` → print help menu  
- `u` → change units  
- `x` → expert mode  

### Script

- `I` → load layout from script file (sfdisk)  
- `O` → dump layout to script file  

### Save & Exit

- `w` → write changes to disk and exit  
- `q` → quit without saving  

### Create a New Label

- `g` → create empty GPT  
- `G` → create empty SGI (IRIX)  
- `o` → create empty MBR (DOS)  
- `s` → create empty Sun label  

---

## Create a New Partition

```bash
sudo fdisk /dev/sda
```

### Primary Partition

```
n → p → <Start Sector> → <End Sector> → w
```

### Extended Partition

```
n → e → <Start Sector> → <End Sector> → w
```

➡️ Once the extended partition is created, you can add **logical partitions** using:

```
n → l
```

**Check extended partition table:**

```bash
sudo dd if=/dev/sda bs=512 count=1 skip=<start_sector> | tail -c 66 | hexdump -C
```

---

## Change Partition Type

```
t → <partition_number> → <type_code>
```

List available partition types:

```
l
```

📖 Reference: [Partition type codes](https://en.wikipedia.org/wiki/Partition_type)

---

## Delete a Partition

```
d → <partition_number> → w
```

---

## Print Partition Table Info

**Read raw partition table (first sector):**

```bash
sudo dd if=/dev/sda bs=512 count=1 | tail -c 66 | hexdump -C
```

- **64B** → Partition table  
- **2B** → Magic number (0x55AA)  

---

## Reload Partition Table Without Reboot

After modifying partitions, reload without reboot:

```bash
sudo partprobe      # reload partition table
sudo kpartx -a /dev/sdX   # map partitions
```

---

## Advanced Topics FYI

### Disk Usage & UUIDs

#### Disk Usage

**Check mounted disks usage:**

```bash
df -h
```

- `-h` → human-readable (GB/MB)

**Check inode usage:**

```bash
df -i
```

**Check disk usage of a directory:**

```bash
du -sh /path/to/dir
```

- `-s` → summary only
- `-h` → human-readable

**Top disk space consumers in current dir:**

```bash
du -ah . | sort -rh | head -n 20
```

---

### UUIDs (Universally Unique Identifiers)

**List UUIDs of all partitions:**

```bash
blkid
```

**Check specific device UUID:**

```bash
blkid /dev/sda1
```

**Show UUIDs via lsblk:**

```bash
lsblk -o NAME,UUID
```

**Show UUIDs with file system type:**

```bash
lsblk -f
```

---

### Mounting Using UUIDs

**Temporary mount:**

```bash
sudo mount UUID=<uuid> /mnt/point
```

**Permanent mount (edit fstab):**

```bash
sudo nano /etc/fstab
```

Example line in `/etc/fstab`:

```bash
UUID=<uuid> /mnt/point ext4 defaults 0 2
```

---

## GUID Partition Table (GPT) Scheme Breakdown

Each **LBA (Logical Block Address)** typically represents **512 bytes** (though some disks may use 4096 bytes).
Below is the breakdown of what each LBA is used for in the GPT scheme shown.

---

### LBA Usage

| LBA | Size (bytes) | Purpose |
|-------------|--------------|---------|
| **LBA 0** | 512 B | **Protective MBR** — A legacy MBR to protect GPT disks from old tools that only understand MBR. Marks the disk as occupied. |
| **LBA 1** | 512 B | **Primary GPT Header** — Contains metadata about GPT, including disk GUID, usable LBAs, number and size of partition entries, and CRC checksums. |
| **LBA 2–33**| 32 × 512 B = 16 KiB | **Primary Partition Entries** — Array of partition entries (usually 128 entries, each 128 bytes). Describes partitions (type GUID, unique GUID, start/end LBAs, attributes, name). |
| **LBA 34–(n−34)** | variable | **Partitions Data Region** — Space allocated for partitions (Partition 1, Partition 2, etc.). |
| **LBA (n−33)–(n−2)** | 32 × 512 B = 16 KiB | **Secondary Partition Entries** — Backup copy of the partition entries array. |
| **LBA (n−1)** | 512 B | **Secondary GPT Header** — Backup copy of GPT header at the end of the disk. Points back to the start.

---

### Model Header (LBA 1)

| Offset | Length  | Contents |
|--------|---------|----------|
| 0x00   | 8 bytes | **Signature** — e.g., "GPT MODEL" identifying the file as a GPT checkpoint |
| 0x08   | 4 bytes | **Revision** — format revision number (e.g., 0x00010000 for version 1.0) |
| 0x0C   | 4 bytes | **Header Size** — size of the header in bytes |
| 0x10   | 4 bytes | **CRC-32 (Header)** — checksum of the header (with this field zeroed during calculation) |
| 0x14   | 4 bytes | **Reserved** — must be zero |
| 0x18   | 8 bytes | **Current LBA** — location of this header within the file |
| 0x20   | 8 bytes | **Backup LBA** — location of a backup header copy (if present) |
| 0x28   | 8 bytes | **First usable block** — first block available for model data (after header and tables) |
| 0x30   | 8 bytes | **Last usable block** — last usable block before extra metadata |
| 0x38   | 16 bytes| **Model GUID** — unique identifier for this model (UUID) |
| 0x48   | 8 bytes | **Partition entries start LBA** — starting block of the entries array (layers/modules) |
| 0x50   | 4 bytes | **Number of entries** — number of model units (layers/blocks) |
| 0x54   | 4 bytes | **Entry size** — size of each entry in the table (e.g., 128 bytes) |
| 0x58   | 4 bytes | **CRC-32 (entries)** — checksum of the entries array |
| 0x5C   | *       | **Reserved** — rest of the sector filled with zeroes |

---

### Layer / Module Entries (LBA 2–N)

Each entry describes a single model unit (layer, block, embedding, normalization module, etc.).

| Offset | Length  | Contents |
|--------|---------|----------|
| 0x00   | 16 bytes | **Layer type GUID** — identifies the unit type (Attention / MLP / Embedding / Norm) |
| 0x10   | 16 bytes | **Unique layer GUID** — unique identifier for this specific unit |
| 0x20   | 8 bytes  | **First LBA** — starting block of this unit’s data |
| 0x28   | 8 bytes  | **Last LBA** — ending block of this unit’s data |
| 0x30   | 8 bytes  | **Attributes** — flags (e.g., frozen weights, shared weights, trainable) |
| 0x38   | 72 bytes | **Layer name** — human-readable name (UTF-16LE), e.g., "TransformerBlock_12" |

---
---
---

```bash
man fs
```
