#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct partition {
uint8_t Part_flag ;
uint8_t Start_CHS [3] ;
uint8_t Type ;
uint8_t End_CHS[3] ;
uint32_t Starting_Sector ;
uint32_t Num_Sectors ;

}Part_typedef ;


typedef struct MBR {
Part_typedef  Partitions[4] ;
uint16_t signature ;
}MBR_typedef ;
void PrintPartitionInfo(Part_typedef* Partition) ;

int main (unsigned int argc , char* argv[])
{
uint8_t buf[512] ;
int fd ;
int num_read  ;
MBR_typedef* MBR_ptr = NULL ;
fd = open(argv[1],O_RDONLY,00007)  ;

if (fd != -1 ) 
{
num_read = read(fd ,buf,512) ;
if (num_read != 0) 
{
MBR_ptr = (MBR_typedef*) & (buf[446]) ;
}
}
 
for (int i = 0 ; (MBR_ptr->Partitions[i].Type) != 0x00  ;i++) {
 printf("Device:%s%d\n", argv[1], i + 1);PrintPartitionInfo(&(MBR_ptr->Partitions[i])) ;
 printf("ID:%d\n",i+1) ; 

}
return 0 ;
}

void PrintPartitionInfo(Part_typedef *Partition) {
    printf("Boot:%c\n", Partition->Part_flag == 0x80 ? '*' : ' ');
    printf("Start:%u\nEnd:%u\nSectors:%u\nSize:%uG\nType:%X\n", Partition->Starting_Sector, Partition->Starting_Sector + Partition->Num_Sectors - 1,
           Partition->Num_Sectors, (uint32_t)((((uint64_t)Partition->Num_Sectors * 512)) / (1024 * 1024 * 1024)), Partition->Type);
}
