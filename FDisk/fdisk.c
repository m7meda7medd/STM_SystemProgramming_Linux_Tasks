#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#pragma pack(1)
typedef struct partition {
    uint8_t Part_flag;
    uint8_t Start_CHS[3];
    uint8_t Type;
    uint8_t End_CHS[3];
    uint32_t Starting_Sector;
    uint32_t Num_Sectors;
} Part_typedef;

typedef struct EBR {
    Part_typedef FirstEntry;
    Part_typedef SecondEntry;
} EBR_typedef;
typedef struct MBR {
    Part_typedef Partitions[4];
    uint16_t signature;
} MBR_typedef;
void PrintPartitionInfo(const Part_typedef * Partition);
int i = 0;
char *dev_name = NULL;
int main(unsigned int argc, char *argv[])
{
    uint8_t buf[512], end = 0;
    int fd;
    int num_read;
    off_t l_offset = 0;
    EBR_typedef *temp = NULL;
    int j = 0;
    off_t EBR_offset, temp_offset, current_EBR;
    EBR_typedef *EBR_ptr = NULL;
    MBR_typedef *MBR_ptr = NULL;
    if (argc != 2) {
	perror("Error Invalid Number of arguments");
	return -1;
    } else {
	dev_name = argv[1];	// get device name 
	fd = open(argv[1], O_RDONLY, 777);

	if (fd != -1) {
	    num_read = read(fd, buf, 512);	// Read MBR 
	    if (num_read != 0) {
		MBR_ptr = (MBR_typedef *) & (buf[446]);
	    }
	    printf("\x1B[1m" "%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s\n"
		   "\x1B[0m", "Device", "Boot", "Start", "End", "Sectors",
		   "Size", "ID", "Type");

	    while ((MBR_ptr->Partitions[j].Type != 0)
		   && (MBR_ptr->signature == 0xAA55)) {
		PrintPartitionInfo(&(MBR_ptr->Partitions[j]));	// print Partition Info 
		if (MBR_ptr->Partitions[j].Type == 0x05)	// case of Extended Partition 
		{
		    EBR_offset = MBR_ptr->Partitions[j].Starting_Sector;	// get the offset of first EBR 
		    current_EBR = EBR_offset;
		    l_offset = lseek(fd, (uint64_t) (EBR_offset) * 512, SEEK_SET);	//seek to it 
		    if (l_offset != -1) {
			num_read = read(fd, buf, 512);	// read EBR
			if (num_read != 0) {
			    EBR_ptr = (EBR_typedef *) & (buf[446]);	//Neglect first 446 
			    while (end == 0) {
				EBR_ptr->FirstEntry.Starting_Sector += current_EBR;	// increment current_EBR_offset to offset of logical partition
				PrintPartitionInfo(&(EBR_ptr->FirstEntry));
				((EBR_ptr->SecondEntry.Starting_Sector) != 0) ? ((EBR_ptr->SecondEntry.Starting_Sector) += EBR_offset) : (end = 1);	// add first_EBR offset to the offset of second EBR , or end loop in case of 0 
				current_EBR =
				    EBR_ptr->SecondEntry.Starting_Sector;
				l_offset =
				    lseek(fd,
					  ((uint64_t) EBR_ptr->
					   SecondEntry.Starting_Sector) *
					  512, SEEK_SET);
				if (l_offset == -1) {
				    perror("Error with lseek");
				    break;
				}
				num_read = read(fd, buf, 512);
				EBR_ptr = (EBR_typedef *) & (buf[446]);
			    }
			}
		    }
		}
		j++;
	    }
	    return 0;
	}
    }
}

void PrintPartitionInfo(const Part_typedef * Partition)
{
    double size = (double) (Partition->Num_Sectors * 512);
    char unit = 0;
    char type[25];
    switch (Partition->Type) {
    case 0x83:
	strcpy(type, "linux");
	break;
    case 0x5:
	strcpy(type, "Extended");
	break;
    default:
	strcpy(type, "Invalid");

    }
    ((uint64_t) size / (1024 * 1024 * 1024)) == 0 ? (size =
						     (size /
						      (1024 * 1024)),
						     unit = 'M') : (size =
								    (size /
								     (1024
								      *
								      1024
								      *
								      1024)),
								    unit =
								    'G');
    printf("%s%-5d %-10c%-10u%-10u%-10u%02.2lf%-5c %-10X%-10s\n", dev_name,
	   ++i, Partition->Part_flag == 0x80 ? '*' : ' ',
	   Partition->Starting_Sector,
	   Partition->Starting_Sector + Partition->Num_Sectors - 1,
	   Partition->Num_Sectors, size, unit, Partition->Type, type);
}
