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

```
UUID=<uuid> /mnt/point ext4 defaults 0 2
