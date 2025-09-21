/* ===================================GPT=============================================== */
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
} PartitionEntry;

void guid_to_str(const uint8_t type_guid[16], char *guid, size_t guid_size)
{
  snprintf(guid, guid_size,
           "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-"
           "%02X%02X%02X%02X%02X%02X",
           type_guid[3], type_guid[2], type_guid[1], type_guid[0], // Little endian
           type_guid[5], type_guid[4], type_guid[7], type_guid[6], type_guid[8], type_guid[9], type_guid[10],
           type_guid[11], type_guid[12], type_guid[13], type_guid[14], type_guid[15]);
}

const char *partition_type_name(const char *guid)
{
  if (strcasecmp(guid, "0FC63DAF-8483-4772-8E79-3D69D8477DE4") == 0)
    return "Linux filesystem";
  if (strcasecmp(guid, "C12A7328-F81F-11D2-BA4B-00A0C93EC93B") == 0)
    return "EFI System";
  if (strcasecmp(guid, "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7") == 0)
    return "Microsoft Basic Data";
  return "Unknown";
}

void print_partition(PartitionEntry *table_entry, const char *devname, int part_index)
{
  if (table_entry->first_lba == 0 && table_entry->last_lba == 0)
    return;

  uint64_t start = table_entry->first_lba;
  uint64_t end = table_entry->last_lba;
  uint64_t sectors = end - start + 1;
  uint64_t size = sectors * 512;

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

  char size_str[16];
  snprintf(size_str, sizeof(size_str), "%6.1lf%c", Val, unit);

  char guid_str[64];
  guid_to_str(table_entry->type_guid, guid_str, sizeof(guid_str));

  printf("%-10s %-12lu %-12lu %-12lu %-8s %-s\n", devname, start, end, sectors, size_str,
         partition_type_name(guid_str));
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <disk_device>\n", argv[0]);
    return 1;
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1)
  {
    perror("open");
    return 1;
  }

  GPTHeader gpt;
  lseek(fd, (off_t)512, SEEK_SET);
  if (read(fd, &gpt, sizeof(GPTHeader)) != sizeof(GPTHeader))
  {
    perror("read header");
    close(fd);
    return 1;
  }

  if (gpt.signature != 0x5452415020494645ULL)
  {
    fprintf(stderr, "Not a valid GPT disk\n");
    close(fd);
    return 1;
  }

  printf("%-10s %-12s %-12s %-12s %-8s %-s\n", "Index", "Start", "End", "Sectors", "Size", "Type");

  int entries_size = gpt.num_partition_entries * gpt.partition_entry_size;
  PartitionEntry *entries = malloc(entries_size);
  if (!entries)
  {
    perror("malloc");
    close(fd);
    return 1;
  }

  if (lseek(fd, gpt.partition_entries_lba * 512, SEEK_SET) < 0)
  {
    perror("lseek entries");
    free(entries);
    close(fd);
    return 1;
  }

  if (read(fd, entries, entries_size) != entries_size)
  {
    perror("read entries");
    free(entries);
    close(fd);
    return 1;
  }

  char devname[64];
  for (int i = 0; i < gpt.num_partition_entries; i++)
  {
    snprintf(devname, sizeof(devname), "%s%d", argv[1], i + 1);
    print_partition(&entries[i], devname, i + 1);
  }

  free(entries);
  close(fd);
  return 0;
}