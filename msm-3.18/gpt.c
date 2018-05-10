#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#define CHAR(x) ((x) & 0xff)
#define SHORT(x) ((x) & 0xffff)

typedef unsigned long ulong;
typedef unsigned int uint;

struct partition_entry {
	char physical_driver_status;//0x0 means inactive
	char start_CHS[3];//1B header, 1B sector, 1B cylinder
	char partition_type;//SB 0xee for GPT's protective MBR(LBA 0)
	char last_CHS[3];//CHS address of last absolute sector in partition. The format is described by 3 bytes, see the above
	uint partition_first_abs_sector_LBA;//LBA of first absolute sector in the partition
	uint partition_sectors;//number of sectors in partition
} __attribute__((packed));

struct classical_generic_mbr {
	char bootstap_code[0x1be];
	struct partition_entry entries[4];
	short boot_signatrue;//SB 0x55 0xaa
} __attribute__((packed));

struct patition_table_header {
	char signature[8];
	int version;
	int header_size;
	int crc32_header;
	int reserved_SBZ;
	ulong header_LBA;
	ulong backup_LBA;
	ulong patition_start_LBA;//primary partition table last LBA + 1
	ulong last_usable_LBA;//secondary patition table first LBA - 1
	char disk_guid[16];
	ulong patition_entries_LBA;//AL 2 in primary copy
	int partition_entries_nr;
	int partition_entry_size;
	int crc32_partition_array;
	char reserved[420];
} __attribute__((packed));

struct gpt_partition_entry {
	char partition_type_GUID[16];
	char partition_GUID[16];
	ulong first_LBA;
	ulong last_LBA;
	ulong attr;
	char partition_name[72];
} __attribute__((packed));//128B

char buf[10<<20];//10M
uint buf_size;
char tmp[10<<20];
void dump_mbr()
{
	struct classical_generic_mbr *pmbr = (struct classical_generic_mbr *)buf;
	int i;
	int sig = SHORT(pmbr->boot_signatrue);
	for (i = 0; i < ARRAY_SIZE(pmbr->entries); i++) {
		struct partition_entry *p = &pmbr->entries[i];
		char *pCHS = p->start_CHS;
		printf("###partition %d\n", i);
		printf("status(SBZ): 0x%x\n", p->physical_driver_status);
		printf("start C-%x H-%x S-%x\n", CHAR(pCHS[0]), CHAR(pCHS[1]), CHAR(pCHS[2]));
		printf("part type(SB 0xEE): %x\n", CHAR(p->partition_type));
		pCHS = p->last_CHS;
		printf("end C-%x H-%x S-%x\n", CHAR(pCHS[0]), CHAR(pCHS[1]), CHAR(pCHS[2]));
		printf("part:[%d, +0x%x\n", p->partition_first_abs_sector_LBA, p->partition_sectors);
	}
	if (sig != 0xaa55)
		printf("***");
	printf("MBR signature %04x\n", sig);
}

#define GPT_HEADER_SIG "EFI PART"
#define dump(fmt, var) __dump(fmt, &(var), sizeof(var))
void __dump(const char *fmt, void *addr, uint size)
{
	int i;
	char *s = addr;
	printf("%s", fmt);
	for (i = 0; i < size; i++)
		printf("%02x ", s[i] & 0xff);
	printf("\n");
}

void dump_gpt_header(const char *name)
{
	struct patition_table_header *p = (struct patition_table_header*)(buf + sizeof(struct classical_generic_mbr));
	int ret = strncmp("EFI PART", p->signature, sizeof(p->signature));
	int i, j, fd;
	uint sectors;
	struct gpt_partition_entry *pe = (struct gpt_partition_entry*)(p + 1);
	printf("Signature [%s]\n", ret? "***wrong": "OK");
	dump("Revision(v1.0 SB 00 00 01 00):", p->version);
	printf("header size in bytes(usu. 92): %d\n", p->header_size);
	dump("CRC32/zlib of header (offset +0 up to header size) in little endian, with this field zeroed during calculation:", p->crc32_header);
	dump("Reserved 4B(SBZ):", p->reserved_SBZ);
	printf("This GPT head's LBA(SB 1?):%ld\n", p->header_LBA);
	printf("BK GPT head's LBA(usu 0?):%ld\n", p->backup_LBA);
	printf("First usable LBA for partitions (primary partition table last LBA + 1):%ld\n", p->patition_start_LBA);
	printf("\t*512=%ld, ==file size %d?\n", p->patition_start_LBA * 512, buf_size);
	printf("Last usable LBA (secondary partition table first LBA - 1)(usu 0?):%ld\n", p->last_usable_LBA);
	dump("disk GUID:", p->disk_guid);
	printf("Starting LBA of array of partition entries (AL 2 in primary copy): %ld\n", p->patition_entries_LBA);
	printf("Number of partition entries in array: %d\n", p->partition_entries_nr);
	printf("Size of a single partition entry (usually 80h or 128): %d\n", p->partition_entry_size);
	dump("CRC32/zlib of partition array in little endian:", p->crc32_partition_array);
	for (i = 0; i < sizeof(p->reserved); i++) {
		if (p->reserved[i]) {
			printf("***wrong reserverd area, SBZ!\n");
			break;
		}
	}
	printf("Remaing reserved 420 bytes area SB all 0:[%s]\n", i == sizeof(p->reserved)? "OK": "Fail");
	sprintf(tmp, "%s.xml", name);
	fd = open(tmp, O_RDWR|O_CREAT, 0644);
	if (fd < 0) {
		perror("open file err");
		return;
	}
	tmp[0] = 0;
	for (i = 0;;i++) {
		char label[72] = {0};
		for (j = 0; j < sizeof(pe->partition_type_GUID); j++) {
			if (pe->partition_type_GUID[j])
				break;
		}
		if (j == sizeof(pe->partition_type_GUID))
			break;
		dump("Partition type GUID:", pe->partition_type_GUID);
		dump("Partition GUID:", pe->partition_GUID);
		printf("------------\"");
		for (j = 0; j < sizeof pe->partition_name; j += 2) {
			if (pe->partition_name[j])
				snprintf(label + strlen(label), sizeof(label) - strlen(label), \
					"%c", pe->partition_name[j]);
			else
				break;
		}
		printf("%s\":", label);
		sectors = pe->last_LBA + 1 - pe->first_LBA;
		printf("%d, KB %d, start bytes hex %lx, [%ld, %ld]\n", sectors, sectors * 512 / 1024, pe->first_LBA * 512, \
			pe->first_LBA, pe->last_LBA);
		printf("%s:%08lx %08lx ", label, pe->first_LBA, pe->last_LBA + 1 - pe->first_LBA);
		//
		sprintf(tmp + strlen(tmp),
			"<program SECTOR_SIZE_IN_BYTES=\"512\" file_sector_offset=\"0\" filename=\"\" label=\"%s\" "
			"num_partition_sectors=\"%d\" physical_partition_number=\"0\" size_in_KB=\"%d.0\" sparse=\"false\" "
			"start_byte_hex=\"0x%lx\" start_sector=\"%ld\"/>\n", \
			label, sectors, sectors * 512 / 1024, pe->first_LBA * 512, pe->first_LBA);
		//
		if (pe->attr) {
			printf("%lx(BIT ", pe->attr);
			int k;
			for (k = 0; k < sizeof(pe->attr) * 8; k++) {
				if ((1UL << k) & pe->attr)
					printf("%d ", k);//BIT60 set means RO
			}
			printf("set)");
		}
		printf("\n\n");
		pe++;
	}
	ret = write(fd, tmp, strlen(tmp));
	if (ret != strlen(tmp))
		printf("***");
	printf("write %d bytes\n", ret);
	close(fd);
	printf("total %d gpt partition entries found, ==%d in gpt header?\n", i, p->partition_entries_nr);
}

int main(int argc, char **argv)
{
	int fd;
	if (sizeof(long) != 8) {
		printf("build on 64bit!\n");
		return 0;
	}
	if (argc == 1) {
		printf("usage:./a.out gpt-bin-file");
		return 0;
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open error:");
		return 0;
	}
	buf_size = read(fd, buf, sizeof(buf));
	close(fd);
	printf("%s is %d bytes\n", argv[1], buf_size);
	dump_mbr();
	dump_gpt_header(argv[1]);
	return 0;
}
