#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

typedef struct
{
  uint8_t status;         // 0x80 = bootable
  uint8_t first_chs[3];   // CHS address of first sector (unused here)
  uint8_t partition_type; // Partition type
  uint8_t last_chs[3];    // CHS address of last sector (unused here)
  uint32_t lba;           // Starting LBA
  uint32_t sector_count;  // Number of sectors
} PartitionEntry;

const char *partition_type_name(uint8_t id)
{
  switch (id)
  {
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

void print_partition(const char *dev, int index, PartitionEntry *ptr)
{
  if (ptr->partition_type == 0 || ptr->sector_count == 0)
    return;

  uint64_t start = ptr->lba;
  uint64_t end = ptr->lba + ptr->sector_count - 1;
  uint64_t size = (uint64_t)ptr->sector_count * 512;

  double Val;
  char unit;
  if (size >= (1ULL << 30))
  {
    Val = (double)size / (1ULL << 30);
    unit = 'G';
  }
  else if (size >= (1ULL << 20))
  {
    Val = (double)size / (1ULL << 20);
    unit = 'M';
  }
  else if (size >= (1ULL << 10))
  {
    Val = (double)size / (1ULL << 10);
    unit = 'K';
  }
  else
  {
    Val = (double)size;
    unit = 'B';
  }

  char size_str[8];
  snprintf(size_str, sizeof(size_str), "%6.1lf%c", Val, unit);

  printf("%-15s %-8c %-10lu %-10lu %-10u %-8s %-4X %-s\n", dev, (ptr->status == 0x80 ? '*' : ' '), start, end,
         ptr->sector_count, size_str, ptr->partition_type, partition_type_name(ptr->partition_type));
}

void read_ebr(int fd, const char *dev, uint32_t ebr_lba, uint32_t ext_base, int *index)
{
  char buf[512];
  PartitionEntry *part_table;

  while (ebr_lba != 0)
  {
    lseek(fd, (off_t)ebr_lba * 512, SEEK_SET);
    if (read(fd, buf, 512) != 512)
    {
      perror("read ebr");
      return;
    }
    part_table = (PartitionEntry *)&buf[446];

    // First entry
    PartitionEntry *ext_part = &part_table[0];
    if (ext_part->partition_type != 0 && ext_part->sector_count > 0)
    {
      PartitionEntry tmp = *ext_part;
      tmp.lba += ebr_lba; // relative to EBR location
      char devname[64];
      snprintf(devname, sizeof(devname), "%s%d", dev, (*index));
      print_partition(devname, *index, &tmp);
      (*index)++;
    }

    // Second entry
    PartitionEntry *next = &part_table[1];
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

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <Partition>\n", argv[0]);
    return 1;
  }
  int x;

  char buf[512];
  int fd = open(argv[1], O_RDONLY);
  // check if open is successful
  if (fd == -1)
  {
    perror("open");
    return 1;
  }

  // check if Read is successful
  if (read(fd, buf, 512) != 512)
  {
    perror("read");
    close(fd);
    return 1;
  }

  PartitionEntry *table_entry_ptr = (PartitionEntry *)&buf[446];

  printf("%-15s %-8s %-10s %-10s %-10s %-8s %-4s %-s\n", "Device", "Boot", "Start", "End", "Sectors", "Size", "Id",
         "Type");

  int part_index = 1;
  for (int i = 0; i < 4; i++)
  {
    PartitionEntry *ptr = &table_entry_ptr[i];
    if (ptr->partition_type == 0 || ptr->sector_count == 0)
      continue;

    char devname[64];
    snprintf(devname, sizeof(devname), "%s%d", argv[1], part_index);

    print_partition(devname, part_index, ptr);

    if (ptr->partition_type == 0x05 || ptr->partition_type == 0x0F)
    {
      // Extended partition
      read_ebr(fd, argv[1], ptr->lba, ptr->lba, &part_index);
    }
    part_index++;
  }

  close(fd);
  return 0;
}
