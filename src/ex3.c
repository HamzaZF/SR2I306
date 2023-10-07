#include <stdio.h>
#include <stdint.h>
#include <elf.h>
#include <math.h>
#include <string.h>

uint8_t buffer_temp_[8192];

void read_header_program_table(uint8_t *buffer, FILE *file)
{

    printf("###Begin of read_header_program_table function \n");
    int begin = EI_NIDENT + sizeof(Elf64_Half) + sizeof(Elf64_Half) + sizeof(Elf64_Word) + sizeof(Elf64_Addr);
    int end = begin + sizeof(Elf64_Off);
    Elf64_Off e_phoff;
    Elf64_Half e_phnum;

    memcpy(&e_phoff, buffer + begin, sizeof(Elf64_Off));
    memcpy(&e_phnum, buffer + begin + sizeof(Elf64_Off) + sizeof(Elf64_Off) + sizeof(Elf64_Word) + sizeof(Elf64_Half) + sizeof(Elf64_Half), sizeof(Elf64_Half));

    printf("e_phnum: %d\n", e_phnum);

    if (fseek(file, e_phoff, SEEK_SET) != 0)
    {
        perror("Error seeking in the file");
        fclose(file);
        return 1;
    }

    uint8_t buffer_temp[8192];
    size_t bytes_read = fread(buffer_temp, 1, e_phnum * sizeof(Elf64_Phdr), file);

    int offset_ptype = 0;
    int offset_pflags = offset_ptype + sizeof(Elf64_Word);
    int offset_poffset = offset_pflags + sizeof(Elf64_Word);
    int offset_pvirtaddr = offset_poffset + sizeof(Elf64_Off);
    int offset_pphysaddr = offset_pvirtaddr + sizeof(Elf64_Addr);
    int offset_pfilesz = offset_pphysaddr + sizeof(Elf64_Addr);
    int offset_pmemsz = offset_pfilesz + sizeof(Elf64_Xword);
    int offset_palign = offset_pmemsz + sizeof(Elf64_Xword);

    printf("program header table : \n");
    for (unsigned int i = 0; i < bytes_read; i++)
    {
        if (i % sizeof(Elf64_Phdr) == 0)
        {
            printf("\nsection number %d\n", i / sizeof(Elf64_Phdr) + 1);
            printf("	p_offset : ");
            for (int j = offset_poffset; j < offset_poffset + sizeof(Elf64_Off); j++)
            {
                printf("%02x ", buffer_temp[i + j]);
            }
            printf("\n");

            printf("	p_type : ");
            for (int j = offset_ptype; j < offset_ptype + sizeof(Elf64_Word); j++)
            {
                printf("%02x ", buffer_temp[i + j]);
            }
            printf("\n");

            printf("	p_vaddr : ");
            for (int j = offset_pvirtaddr; j < offset_pvirtaddr + sizeof(Elf64_Addr); j++)
            {
                printf("%02x ", buffer_temp[i + j]);
            }
            printf("\n");

            printf("	p_paddr : ");
            for (int j = offset_pphysaddr; j < offset_pphysaddr + sizeof(Elf64_Addr); j++)
            {
                printf("%02x ", buffer_temp[i + j]);
            }
            printf("\n");

            printf("	p_filesz : ");
            for (int j = offset_pfilesz; j < offset_pfilesz + sizeof(Elf64_Xword); j++)
            {
                printf("%02x ", buffer_temp[i + j]);
            }
            printf("\n");

            printf("	p_memsz : ");
            for (int j = offset_pmemsz; j < offset_pmemsz + sizeof(Elf64_Xword); j++)
            {
                printf("%02x ", buffer_temp[i + j]);
            }
            printf("\n");

            printf("	p_align : ");
            for (int j = offset_palign; j < offset_palign + sizeof(Elf64_Xword); j++)
            {
                printf("%02x ", buffer_temp[i + j]);
            }
        }
    }
    printf("\n");

    printf("###End of read_header_program_table function \n");
}

void print_string_from_index(int index)
{
    // divide all the elements of thi buffer by null character and print each of theses element
    int temp = index + 1;
    while (buffer_temp_[temp] != 0x00)
    {
        printf("test2");
        printf("%c", buffer_temp_[temp]);
        temp++;
    }
}

void read_header_section_table(uint8_t *buffer, FILE *file)
{

    printf("###Begin of read_header_section_table function \n");

    int begin = EI_NIDENT + sizeof(Elf64_Half) + sizeof(Elf64_Half) + sizeof(Elf64_Word) + sizeof(Elf64_Addr) + sizeof(Elf64_Off);
    int end = begin + sizeof(Elf64_Off);
    Elf64_Off e_shoff;
    Elf64_Half e_shnum;
    Elf64_Half e_shstrndx;

    memcpy(&e_shoff, buffer + begin, sizeof(Elf64_Off));
    memcpy(&e_shnum, buffer + begin + sizeof(Elf64_Off) + sizeof(Elf64_Word) + sizeof(Elf64_Half) + sizeof(Elf64_Half) + sizeof(Elf64_Half) + sizeof(Elf64_Half), sizeof(Elf64_Half));
    memcpy(&e_shstrndx, buffer + begin + sizeof(Elf64_Off) + sizeof(Elf64_Word) + sizeof(Elf64_Half) + sizeof(Elf64_Half) + sizeof(Elf64_Half) + sizeof(Elf64_Half) + sizeof(Elf64_Half), sizeof(Elf64_Half));

    printf("e_shstrndx: %d\n", e_shstrndx);

    printf("e_shnum: %d\n", e_shnum);

    if (fseek(file, e_shoff, SEEK_SET) != 0)
    {
        perror("Error seeking in the file");
        fclose(file);
        return 1;
    }

    uint8_t buffer_temp[8192];
    size_t bytes_read = fread(buffer_temp, 1, e_shnum * sizeof(Elf64_Shdr), file);

    int offset_shname = 0;
    int offset_shtype = offset_shname + sizeof(Elf64_Word);
    int offset_shflags = offset_shtype + sizeof(Elf64_Word);
    int offset_shaddr = offset_shflags + sizeof(Elf64_Xword);
    int offset_shoffset = offset_shaddr + sizeof(Elf64_Addr);
    int offset_shsize = offset_shoffset + sizeof(Elf64_Off);
    int offset_shlink = offset_shsize + sizeof(Elf64_Xword);
    int offset_shinfo = offset_shlink + sizeof(Elf64_Word);
    int offset_shaddralign = offset_shinfo + sizeof(Elf64_Word);
    int offset_shentsize = offset_shaddralign + sizeof(Elf64_Xword);

    

    for (unsigned int i = 0; i < bytes_read; i++)
    {
        if ((i / sizeof(Elf64_Shdr)) == e_shstrndx)
        {
            Elf64_Off sh_offset;
            Elf64_Xword sh_size;

            memcpy(&sh_offset, buffer_temp + i + offset_shoffset, sizeof(Elf64_Off));
            memcpy(&sh_size, buffer_temp + i + offset_shsize, sizeof(Elf64_Xword));

            printf("sh_offset: %d\n", sh_offset);
            printf("sh_size: %d\n", sh_size);

            printf("String table : \n");
            printf("start printing address ! %02x, end print is %02x", sh_offset, sh_offset + sh_size);
            // print_string_table( sh_offset, sh_size);

            FILE *file_ = fopen("./elf", "rw");

            if (fseek(file_, sh_offset, SEEK_SET) != 0)
            {
                perror("Error seeking in the file");
                fclose(file_);
                return 1;
            }

            size_t bytes_read_ = fread(buffer_temp_, 1, sh_size, file_);
            printf("\n");
            for (unsigned int i = 0; i < sh_size; i++)
            {
                printf("%02x ", buffer_temp_[i]);
            }

            for (unsigned int i = 0; i < sh_size; i++)
            {
                if (buffer_temp_[i] == 0x00)
                {
                    printf("\n");
                }
                else
                {
                    printf("%c", buffer_temp_[i]);
                }
            }
            break;  
        }
        
    }

    printf("section header table : \n");

    for (unsigned int i = 0; i < bytes_read; i++)
    {
        if (i % sizeof(Elf64_Shdr) == 0)
        {

            printf("\nsection number %d\n", i / sizeof(Elf64_Shdr));
            printf("	sh_name : ");

            Elf64_Off sh_offset;
            Elf64_Xword sh_size;
            unsigned int index_;

            memcpy(&sh_offset, buffer_temp + i + offset_shoffset, sizeof(Elf64_Off));
            memcpy(&sh_size, buffer_temp + i + offset_shsize, sizeof(Elf64_Xword));

            for (int j = offset_shname; j < offset_shname + sizeof(Elf64_Word); j++)
            {

                // printf("%02x ", buffer_temp[i + j]);
            }
            memcpy(&index_, buffer_temp + i + offset_shname, sizeof(Elf64_Word));
            printf("index :%d", index_);
            print_string_from_index(index_);
            printf("\n");

            printf("	sh_type : ");
            for (int j = offset_shaddr; j < offset_shaddr + sizeof(Elf64_Addr); j++)
            {
                printf("%02x ", buffer_temp[i + j]);
            }
            printf("\n");

            printf("	sh_offset : ");
            for (int j = offset_shoffset; j < offset_shoffset + sizeof(Elf64_Off); j++)
            {
                printf("%02x ", buffer_temp[i + j]);
            }
            printf("\n");
        }
    }
    printf("\n");
}

int main(void)
{

    FILE *file = fopen("./elf", "rw");

    if (file == NULL)
    {
        perror("error opening file");
        return 1;
    }
    /*
    if (fseek(file, sizeof(Elf64_Ehdr), SEEK_SET) != 0) {
        perror("Error seeking in the file");
        fclose(file);
        return 1;
    }
    */

    // Lire des données à partir de la position actuelle (sizeof(Elf64_Ehdr) octets à partir du début)
    uint8_t buffer[8192];
    size_t bytes_read = fread(buffer, 1, sizeof(Elf64_Ehdr), file);

    if (bytes_read == 0)
    {
        perror("Error reading from file");
        fclose(file);
        return 1;
    }
    printf("\n");

    unsigned int offset_counter = EI_NIDENT;

    // print ei_ident
    printf("e_ident : ");
    for (unsigned int i = 0; i < offset_counter; i++)
    {
        printf("%02x ", buffer[i]); // Affiche les octets lus en hexadécimal
    };
    printf("\n");

    // print file type
    printf("e_type : ");
    for (unsigned int i = offset_counter; i < offset_counter + sizeof(Elf64_Half); i++)
    {
        printf("%02x ", buffer[i]);
    };
    printf("\n");

    // print ET_EXEC
    printf("ET_EXEC : %02x\n", ET_EXEC);

    // print ET_DYN
    printf("ET_DYN : %02x\n", ET_DYN);

    // print EV_CURRENT
    printf("EV_CURRENT : %02x\n", EV_CURRENT);

    // skip e_type
    offset_counter += sizeof(Elf64_Half);

    // skip e_machine
    offset_counter += sizeof(Elf64_Half);

    // skip e_version
    offset_counter += sizeof(Elf64_Word);

    // print entry point
    printf("entry point :");
    for (unsigned char i = offset_counter; i < offset_counter + sizeof(Elf64_Addr); i++)
    {
        printf("%02x ", buffer[i]);
    };
    printf("\n");

    offset_counter += sizeof(Elf64_Addr);

    printf("offset to program header table: ");
    for (unsigned int i = offset_counter; i < offset_counter + sizeof(Elf64_Off); i++)
    {
        printf("%02x ", buffer[i]);
    };
    printf("\n");

    // skip offset to program header table
    offset_counter += sizeof(Elf64_Off);

    printf("offset to section header table :");
    for (unsigned int i = offset_counter; i < offset_counter + sizeof(Elf64_Off); i++)
    {
        printf("%02x ", buffer[i]);
    };
    printf("\n");

    // skip offset to section header table
    offset_counter += sizeof(Elf64_Off);

    // skip processor specific flags
    offset_counter += sizeof(Elf64_Word);

    // skip ELF header size
    offset_counter += sizeof(Elf64_Half);

    printf("size of program header table entry :");
    for (unsigned int i = offset_counter; i < offset_counter + sizeof(Elf64_Half); i++)
    {
        printf("%02x ", buffer[i]);
    };
    printf("\n");

    // skip program header table entry size
    offset_counter += sizeof(Elf64_Half);

    printf("number of entries in program header table :");
    for (unsigned int i = offset_counter; i < offset_counter + sizeof(Elf64_Half); i++)
    {
        printf("%02x ", buffer[i]);
    };
    printf("\n");

    // skip number of entries in program header table
    offset_counter += sizeof(Elf64_Half);

    printf("size of section header table entry :");
    for (unsigned int i = offset_counter; i < offset_counter + sizeof(Elf64_Half); i++)
    {
        printf("%02x ", buffer[i]);
    };
    printf("\n");

    // skip size of section header table entry
    offset_counter += sizeof(Elf64_Half);

    printf("number of entries in section header table :");
    for (unsigned int i = offset_counter; i < offset_counter + sizeof(Elf64_Half); i++)
    {
        printf("%02x ", buffer[i]);
    };
    printf("\n");

    // skip number of entries in section header table
    offset_counter += sizeof(Elf64_Half);

    // read_header_program_table(buffer, file);
    read_header_section_table(buffer, file);

    fclose(file);
    return 0;
}
