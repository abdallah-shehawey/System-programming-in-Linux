#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// MBR Structures
typedef struct
{
  uint8_t status;         // 0x80 = bootable
  uint8_t first_chs[3];   // CHS address of first sector (unused here)
  uint8_t partition_type; // Partition type
  uint8_t last_chs[3];    // CHS address of last sector (unused here)
  uint32_t lba;           // Starting LBA
  uint32_t sector_count;  // Number of sectors
} MBRPartitionEntry;

// GPT Structures
typedef struct
{
  uint64_t signature; // "EFI PART" (0x5452415020494645)
  uint32_t revision;
  uint32_t header_size;
  uint32_t header_crc32;
  uint32_t reserved;
  uint64_t current_lba;
  uint64_t backup_lba;
  uint64_t first_usable_lba;
  uint64_t last_usable_lba;
  uint8_t disk_guid[16];
  uint64_t partition_entries_lba;
  uint32_t num_partition_entries;
  uint32_t partition_entry_size;
  uint32_t partition_entries_crc32;
} GPTHeader;

typedef struct
{
  uint8_t type_guid[16];
  uint8_t unique_guid[16];
  uint64_t first_lba;
  uint64_t last_lba;
  uint64_t attributes;
  uint16_t name[36];
} GPTPartitionEntry;

int is_gpt(int fd)
{
  char buf[512];
  lseek(fd, (off_t)0, SEEK_SET);

  if (read(fd, buf, 512) != 512)
  {
    return 0;
  }

  // Check if there's a GPT protective MBR entry (type 0xEE)
  MBRPartitionEntry *table_entry_ptr = (MBRPartitionEntry *)&buf[446];
  for (int i = 0; i < 4; i++)
  {
    if (table_entry_ptr[i].partition_type == 0xEE)
    {
      return 1; // GPT protective MBR found
    }
  }

  // Also check GPT signature directly
  GPTHeader gpt;
  lseek(fd, (off_t)512, SEEK_SET);
  if (read(fd, &gpt, sizeof(GPTHeader)) == sizeof(GPTHeader))
  {
    if (gpt.signature == 0x5452415020494645ULL)
    {
      return 1; // Valid GPT signature
    }
  }

  return 0; // Not GPT
}

const char *mbr_partition_type_name(uint8_t id)
{
  switch (id)
  {
  case 0x01:
    return "FAT12";
  case 0x04:
    return "FAT16 (32MB)";
  case 0x06:
    return "FAT16";
  case 0x07:
    return "HPFS/NTFS/exFAT";
  case 0x0b:
    return "W95 FAT32";
  case 0x0c:
    return "W95 FAT32 (LBA)";
  case 0x0e:
    return "W95 FAT16 (LBA)";
  case 0x83:
    return "Linux";
  case 0x82:
    return "Linux swap";
  case 0x05:
    return "Extended";
  case 0x0f:
    return "Extended (LBA)";
  case 0xee:
    return "GPT Protective";
  case 0xef:
    return "EFI System";
  case 0x00:
    return "Empty";
  default:
    return "Unknown";
  }
}

void format_size(uint64_t size, char *size_str, size_t size_str_len)
{
  double val;
  char unit;
  if (size >= (1ULL << 30))
  {
    val = (double)size / (1ULL << 30);
    unit = 'G';
  }
  else if (size >= (1ULL << 20))
  {
    val = (double)size / (1ULL << 20);
    unit = 'M';
  }
  else if (size >= (1ULL << 10))
  {
    val = (double)size / (1ULL << 10);
    unit = 'K';
  }
  else
  {
    val = (double)size;
    unit = 'B';
  }
  snprintf(size_str, size_str_len, "%6.1lf%c", val, unit);
}

void print_mbr_partition(const char *dev, int index, MBRPartitionEntry *ptr)
{
  if (ptr->partition_type == 0 || ptr->sector_count == 0)
    return;

  uint64_t start = ptr->lba;
  uint64_t end = ptr->lba + ptr->sector_count - 1;
  uint64_t size = (uint64_t)ptr->sector_count * 512;

  char size_str[16];
  format_size(size, size_str, sizeof(size_str));

  printf("%-15s %-8c %-10lu %-10lu %-10u %-8s %-4X %-s\n", dev, (ptr->status == 0x80 ? '*' : ' '), start, end,
         ptr->sector_count, size_str, ptr->partition_type, mbr_partition_type_name(ptr->partition_type));
}

void read_ebr(int fd, const char *dev, uint32_t ebr_lba, uint32_t ext_base, int *index)
{
  char buf[512];
  MBRPartitionEntry *part_table;

  while (ebr_lba != 0)
  {
    lseek(fd, (off_t)ebr_lba * 512, SEEK_SET);
    if (read(fd, buf, 512) != 512)
    {
      perror("read ebr");
      return;
    }
    part_table = (MBRPartitionEntry *)&buf[446];

    MBRPartitionEntry *ext_part = &part_table[0];
    if (ext_part->partition_type != 0 && ext_part->sector_count > 0)
    {
      MBRPartitionEntry tmp = *ext_part;
      tmp.lba += ebr_lba; // relative to EBR location
      char devname[64];
      snprintf(devname, sizeof(devname), "%s%d", dev, (*index));
      print_mbr_partition(devname, *index, &tmp);
      (*index)++;
    }

    // Second entry
    MBRPartitionEntry *next = &part_table[1];
    if (next->partition_type != 0 && next->sector_count > 0)
    {
      ebr_lba = ext_base + next->lba;
    }
    else
    {
      break;
    }
  }
}

void read_mbr(int fd, const char *dev)
{
  char buf[512];
  lseek(fd, 0, SEEK_SET);

  if (read(fd, buf, 512) != 512)
  {
    perror("read mbr");
    return;
  }

  MBRPartitionEntry *table_entry_ptr = (MBRPartitionEntry *)&buf[446];

  printf("\nMBR Partition Table:\n");
  printf("%-15s %-8s %-10s %-10s %-10s %-8s %-4s %-s\n", "Device", "Boot", "Start", "End", "Sectors", "Size", "Id",
         "Type");

  int part_index = 1;
  for (int i = 0; i < 4; i++)
  {
    MBRPartitionEntry *ptr = &table_entry_ptr[i];
    if (ptr->partition_type == 0 || ptr->sector_count == 0)
      continue;

    char devname[64];
    snprintf(devname, sizeof(devname), "%s%d", dev, part_index);

    print_mbr_partition(devname, part_index, ptr);

    if (ptr->partition_type == 0x05 || ptr->partition_type == 0x0F)
    {
      // Extended partition
      read_ebr(fd, dev, ptr->lba, ptr->lba, &part_index);
    }
    part_index++;
  }
}

void guid_to_str(const uint8_t type_guid[16], char *guid, size_t guid_size)
{
  snprintf(guid, guid_size,
           "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-"
           "%02X%02X%02X%02X%02X%02X",
           type_guid[3], type_guid[2], type_guid[1], type_guid[0], // Little endian
           type_guid[5], type_guid[4], type_guid[7], type_guid[6], type_guid[8], type_guid[9], type_guid[10],
           type_guid[11], type_guid[12], type_guid[13], type_guid[14], type_guid[15]);
}

const char *gpt_partition_type_name(const char *guid)
{
  if (strcasecmp(guid, "0FC63DAF-8483-4772-8E79-3D69D8477DE4") == 0)
    return "Linux filesystem";
  if (strcasecmp(guid, "C12A7328-F81F-11D2-BA4B-00A0C93EC93B") == 0)
    return "EFI System";
  if (strcasecmp(guid, "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7") == 0)
    return "Microsoft Basic Data";
  return "Unknown";
}

void print_gpt_partition(GPTPartitionEntry *table_entry, const char *devname, int part_index)
{
  if (table_entry->first_lba == 0 && table_entry->last_lba == 0)
    return;

  uint64_t start = table_entry->first_lba;
  uint64_t end = table_entry->last_lba;
  uint64_t sectors = end - start + 1;
  uint64_t size = sectors * 512;

  char size_str[16];
  format_size(size, size_str, sizeof(size_str));

  char guid_str[64];
  guid_to_str(table_entry->type_guid, guid_str, sizeof(guid_str));

  printf("%-10s %-12lu %-12lu %-12lu %-8s %-s\n", devname, start, end, sectors, size_str,
         gpt_partition_type_name(guid_str));
}

void read_gpt(int fd, const char *dev)
{
  GPTHeader gpt;
  lseek(fd, (off_t)512, SEEK_SET);
  if (read(fd, &gpt, sizeof(GPTHeader)) != sizeof(GPTHeader))
  {
    perror("read gpt header");
    return;
  }

  if (gpt.signature != 0x5452415020494645ULL)
  {
    printf("GPT signature not found, trying MBR...\n");
    read_mbr(fd, dev);
    return;
  }

  printf("\nGPT Partition Table:\n");
  printf("%-10s %-12s %-12s %-12s %-8s %-s\n", "Device", "Start", "End", "Sectors", "Size", "Type");

  int entries_size = gpt.num_partition_entries * gpt.partition_entry_size;
  GPTPartitionEntry *entries = malloc(entries_size);
  if (!entries)
  {
    perror("malloc");
    return;
  }

  if (lseek(fd, gpt.partition_entries_lba * 512, SEEK_SET) < 0)
  {
    perror("lseek entries");
    free(entries);
    return;
  }

  if (read(fd, entries, entries_size) != entries_size)
  {
    perror("read entries");
    free(entries);
    return;
  }

  char devname[64];
  for (int i = 0; i < gpt.num_partition_entries; i++)
  {
    snprintf(devname, sizeof(devname), "%s%d", dev, i + 1);
    print_gpt_partition(&entries[i], devname, i + 1);
  }

  free(entries);
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "%s bad usage\n", argv[0]);
    return 1;
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1)
  {
    perror("open");
    return 1;
  }

  if (is_gpt(fd))
  {
    read_gpt(fd, argv[1]);
  }
  else
  {
    read_mbr(fd, argv[1]);
  }

  close(fd);
  return 0;
}