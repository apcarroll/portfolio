#include <elf.h>
#include <stdio.h>
int q;
int weHaveD=0;
int weHaveS=0;
 FILE *fp;
 Elf32_Sym sym;
int isSym=0;
int version;

    Elf32_Ehdr ehdr;
    Elf32_Phdr phdr;
    Elf32_Shdr shdr;
char * doodle;
char * head;
int  dymHeadOff ;
    int dynamicSize=0;
    int normalSize=0;
void symPrint();

//void dymPrint(int count);
void dymPrint(    Elf32_Shdr shdr , char* stringTable);
void symPrint(int count){
    printf("Symbol Table Section:  .symtab\n");
    printf("index    value      size      type bind oth ver shndx          name \n");
    int t;
    for(t=0;t<count;t++){
        printf("[%d]\n",t);
    }
}

void dymPrint(    Elf32_Shdr shdr,char* stringTable){
   
    printf("index    value      size      type bind oth ver shndx          name\n");
    int t;  

    fseek( fp, shdr.sh_offset, SEEK_SET );
    Elf32_Sym* symtab = malloc (shdr.sh_size);
    fread( symtab, shdr.sh_entsize, shdr.sh_size/shdr.sh_entsize, fp );
    int number=shdr.sh_size/shdr.sh_entsize;
  
  



    for(t=0;t<number;t++){

      
        //printf("[%d]\t0x%.8x\t0x%.8x\t%s\t\n",t,symtab[t].st_value,symtab[t].st_size,symtab[t].st_info,symtab[t].sh_shndx,symtab[t].sh_name);
        printf("[%d]\t",t);
        printf("0x%.8x\t",symtab[t].st_value);
        printf("0x%.8x\t",symtab[t].st_size);
        int foo=ELF32_ST_TYPE(symtab[t].st_info);
        switch(foo){
            case 0:
                printf("NOTY\t");
            break;
            case 1:
                printf("OBJT\t");
            break;
            case 2:
                printf("FUNC\t");
            break;
            case 3:
                printf("SECT\t");
            break;
            case 4:
                printf("FILE\t");
            break;
            case 5:
                printf("COMM\t");
            break;
            case 6:
                printf("TLS\t");
            break;
            case 7:
                printf("NUM\t");
            break;
            case 10:
                printf("LOOS\t");
            break;
            case 12:
                printf("HIOS\t");
            break;
            case 13:
                printf("LOPR\t");
            break;
            case 15:
                printf("HIR\t");
            break;
      
        }  
        int bind = ELF32_ST_BIND(symtab[t].st_info);
        switch(bind){
            case 0:
                printf("LOCL\t");
            break;
            case 1:
                printf("GLOB\t");
            break;
            case 2:
                printf("WEAK\t");
            break;
            case 3:
                printf("NUM\t");
            break;
            case 13:
                printf("LOPR\t");
            break;
            case 15:
                printf("HIPR\t");
            break;
        }  
        int vis= ELF32_ST_VISIBILITY(symtab[t].st_other);
        switch(vis){
            case 0:
                printf("D\t");
            break;
          
            case 1:
                printf("I\t");
            break;
            case 2:
                printf("H\t");
            break;
            case 3:
                printf("P\t");
            break;
            case 4:
                printf("EX\t");
            break;
            case 5:
                printf("S\t");
            break;
            case 6:
                printf("EL\t");
            break;
            case 7:
                printf("N\t");
            break;

        }


        //version
        if(isSym==1){
            printf("0\t");
        }
        else{
        Elf32_Half versiony;
         Elf32_Shdr dope;
        fseek(fp,ehdr.e_shoff+version*ehdr.e_shentsize,SEEK_SET);
        fread(&dope,sizeof(dope),1,fp);
        fseek(fp,dope.sh_offset+t*dope.sh_entsize,SEEK_SET);
        fread(&versiony,dope.sh_entsize,1,fp);
        printf("%d\t",versiony);
        }
        if(symtab[t].st_shndx==0){
            printf("UNDEF\t");
        }      
  
        if(symtab[t].st_shndx==SHN_ABS){
            printf("ABS\t");
        }
        if(symtab[t].st_shndx > 0 && symtab[t].st_shndx < ehdr.e_shnum){
            fseek(fp,ehdr.e_shoff+symtab[t].st_shndx*ehdr.e_shentsize,SEEK_SET);
            Elf32_Shdr proxy;           
            fread(&proxy,sizeof(proxy),1,fp);
            printf("%.14s\t",&stringTable[proxy.sh_name]);
        }
       Elf32_Shdr pro;
        fseek(fp,ehdr.e_shoff+shdr.sh_link*ehdr.e_shentsize,SEEK_SET);
        fread(&pro,sizeof(pro),1,fp);
        char* names=malloc(pro.sh_size);
        fseek(fp,pro.sh_offset,SEEK_SET);
        fread(names,pro.sh_size,1,fp);
        printf("%s\n",&names[symtab[t].st_name]);  
   
  
    }
  
}


void sectionType(int x){

    switch (x){
        case 0:
            printf("[SHT_NULL ]");
        break;
        case 1:
            printf("[SHT_PROGBITS ]");
        break;
        case 2:
            printf("[SHT_SYMTAB ]");
        break;
        case 3:
            printf("[SHT_STRTAB ]");
        break;
        case 4:
            printf("[SHT_RELA  ]");
        break;
        case 5:
            printf("[SHT_HASH  ]");
        break;
        case 6:
            printf("[SHT_DYNAMIC ]");
        break;
        case 7:
            printf("[SHT_NOTE ]");
        break;
        case 8:
            printf("[SHT_NOBITS ]");
        break;
        case 9:
            printf("[SHT_REL    ]");
        break;
        case 10:
            printf("[SHT_SHLIB  ]");
        break;
        case 11:
            printf("[SHT_DYNSYM  ]");
        break;
        case 12:
            printf("[SHT_UNKNOWN12  ]");
        break;
        case 13:
            printf("[SHT_UNKNOWN13  ]");
        break;
        case 14:
            printf("[SHT_INIT_ARRAY ]");
        break;
        case 15:
            printf("[SHT_FINI_ARRAY ]");
        break;
        case 16:
            printf("[SHT_PREINIT_ARRAY]");
        break;
        case 17:
            printf("[SHT_GROUP  ]");
        break;
        case 18:
            printf("[SHT_SYMTAB_SHNDX   ]");
        break;
        case 19:
            printf("[SHT_NUM ]");
        break;


		case  0x60000000:
			 printf("[SHT_LOOS ]");
		break; 
		case  0x6fffffef:
			 printf("[SHT_LOSUNW   ]");
		break; 
		case  0x6ffffff0:
			 printf("[SHT_SUNW_capinfo  ]");
		break; 
		case  0x6ffffff1:
			 printf("[SHT_SUNW_symsort ]");
		break; 
		case  0x6ffffff2:
			 printf("[ SHT_SUNW_tlssort  ]");
		break; 
		case  0x6ffffff3:
			 printf("[SHT_SUNW_LDYNSYM   ]");
		break; 
		case  0x6ffffff4:
			 printf("[SHT_SUNW_dof ]");
		break; 
		case  0x6ffffff5:
			 printf("[SHT_SUNW_cap ]");
		break; 
		case  0x6ffffff6:
			 printf("[SHT_SUNW_SIGNATURE  ]");
		break; 
		case  0x6ffffff7:
			 printf("[SHT_SUNW_ANNOTATE  ]");
		break; 
		case  0x6ffffff8:
			 printf("[SHT_SUNW_DEBUGSTR ]");
		break; 
		case  0x6ffffff9:
			 printf("[SHT_SUNW_DEBUG   ]");
		break; 
		case  0x6ffffffa:
			 printf("[SHT_SUNW_move    ]");
		break; 
		case  0x6ffffffb:
			 printf("[SHT_SUNW_COMDAT   ]");
		break; 
		case  0x6ffffffc:
			 printf("[SHT_SUNW_syminfo   ]");
		break; 
		case  0x6ffffffd:
			 printf("[SHT_SUNW_verdef  ]");
		break; 
			case  0x6ffffffe:
			 printf("[SHT_SUNW_verneed   ]");
		break; 
			case  0x6fffffff:
			 printf("[SHT_SUNW_versym   ]");
		break; 













     

    }


}

void vexology(int flag){
/*
 SHF_WRITE               0x01    
SHF_ALLOC               0x02
SHF_EXECINSTR           0x04
 SHF_MERGE               0x10
 SHF_STRINGS             0x20
 SHF_INFO_LINK           0x40
 SHF_LINK_ORDER          0x80
 SHF_OS_NONCONFORMING    0x100
 SHF_GROUP               0x200
 SHF_TLS                 0x400
*/
 
    if(flag == 0){

        printf(" 0\n");
        return;
    }
    printf("[ ");
    if(flag & 0x01){
        printf("SHF_WRITE ");
    }
    if(flag & 0x02){
        printf("SHF_ALLOC  ");
    }
    if(flag & 0x04){
        printf("SHF_EXECINSTR");
    }
    if(flag & 0x10){
        printf("SHF_MERGE  ");
    }
    if(flag & 0x20){
        printf("SHF_STRINGS ");
    }
    if(flag & 0x40){
        printf(" SHF_INFO_LINK ");
    }
    if(flag & 0x80){
        printf("SHF_LINK_ORDER  ");
    }
    if(flag & 0x100){
        printf("SHF_OS_NONCONFORMING");
    }
    if(flag & 0x200){
        printf("SHF_GROUP ");
    }
    if(flag & 0x400){
        printf(" SHF_TLS  ");
    }
      printf(" ]\n");

}









main(argc, argv)
     int argc;
     char **argv;
{
    void sectionType(int x);
    void vexology(int flag);
 
 
    int i;
 
    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");

    if (fp == 0) {
        fprintf(stderr, "%s: can't open %s\n",argv[0], argv[1]);
         exit(1);
    }

    if ( fread( &ehdr, sizeof(ehdr), 1, fp ) != 1 ) {
        fprintf(stderr, "%s: couldn't read %s\n", argv[0], argv[1]);
        exit(1);
    }


    printf("\n");
   // printf("            ELF INFORMATION FOR \"%s\"\n\n", argv[1]);

    printf("ELF Header\n");
 
//ehrdr.eident[]











    //int operator=ehdr.e_ident[0];

    printf("ei_magic: {0x%x ,%c,%c,%c}\n",ehdr.e_ident[0],ehdr.e_ident[1],ehdr.e_ident[2],ehdr.e_ident[3]);
  
    int operator=ehdr.e_ident[4];
    switch(operator){
        case 0:
            printf("ei_class: ELFCLASSNONE ");
            break;
        case 1:
            printf("ei_class: ELFCLASS32 ");
            break;
        case 2:
            printf("ei_class: ELFCLASS64");
            break;
        case 3:
            printf("ei_class: ELFCLASSNUM ");
            break;

    }
    operator = ehdr.e_ident[5];
    switch(operator){
        case 0:
            printf("ei_data: ELFDATANONE \n");
            break;
        case 1:
            printf("ei_data: ELFDATA2LSB \n");
            break;
        case 2:
            printf("ei_data: ELFDATA2MSB \n");
            break;
        case 3:
            printf("ei_data: ELFDATANUM \n");
            break;

    }




























    operator = ehdr.e_ident[7];
    switch(operator){
        case 0:
            printf("ei_osabi: ELFOSABI_NONE");
        break;
        case 1:
            printf("ei_osabi: ELFOSABI_HPUX ");
        break;
        case 2:
            printf("ei_osabi: ELFOSABI_NETBSD  ");
        break;
        case 3:
            printf("ei_osabi: ELFOSABI_LINUX  ");
        break;
        case 4:
            printf("ei_osabi: ELFOSABI_UNKNOWN4");
        break;
        case 5:
            printf("ei_osabi: ELFOSABI_UNKNOWN5 ");
        break;
        case 6:
            printf("ei_osabi: ELFOSABI_SOLARIS  ");
        break;
        case 7:
            printf("ei_osabi: ELFOSABI_AIX ");
        break;
        case 8:
            printf("ei_osabi: ELFOSABI_IRIX  ");
        break;
        case 9:
            printf("ei_osabi: ELFOSABI_FREEBSD");
        break;
        case 10:
            printf("ei_osabi: ELFOSABI_TRU64  ");
        break;
        case 11:
            printf("ei_osabi: ELFOSABI_MODESTO ");
        break;
        case 12:
            printf("ei_osabi: ELFOSABI_OPENBSD  ");
        break;
        case 13:
            printf("ei_osabi: ELFOSABI_OPENVMS");
        break;
        case 14:
            printf("ei_osabi:  ELFOSABI_NSK   ");
        break;
        case 15:
            printf("ei_osabi: ELFOSABI_AROS");
        break;
        case 97:
            printf("ei_osabi: ELFOSABI_ARM ");
        break;
        case 255:
            printf("ei_osabi: ELFOSABI_STANDALONE ");
        break;
    
    }

    operator = ehdr.e_ident[8];
    switch(operator){
        case 0:
            printf("ei_abiversion: EAV_SUNW_NONE  \n");
        break;
        case 1:
            printf("ei_abiversion: EAV_SUNW_CURRENT  \n");
        break;
        case 2:
            printf("ei_abiversion: EAV_SUNW_NUM \n");
        break;
    }
    int theMachine= ehdr.e_machine;
    switch(theMachine){
        case 0:
            printf("e_machine: EM_NONE   ");
        break;
        case 1:
            printf("e_machine: EM_M32");
        break;
        case 2:
            printf("e_machine: EM_SPARC ");
        break;
        case 3:
            printf("e_machine: EM_386   ");
        break;
        case 4:
            printf("e_machine: EM_68K  ");
        break;
        case 5:
            printf("e_machine: EM_88K ");
        break;
        case 6:
            printf("e_machine: EM_486   ");
        break;
        case 7:
            printf("e_machine: EM_860 ");
        break;
        case 8:
            printf("e_machine: EM_MIPS   ");
        break;
        case 9:
            printf("e_machine: EM_S370  ");
        break;
        case 10:
            printf("e_machine: EM_MIPS_RS3_LE");
        break;
        case 11:
            printf("e_machine: EM_RS6000 ");
        break;
        case 12:
            printf("e_machine: EM_UNKNOWN12 ");
        break;
        case 13:
            printf("e_machine: EM_UNKNOWN13 ");
        break;
        case 14:
            printf("e_machine: EM_UNKNOWN14 ");
        break;
        case 15:
            printf("e_machine: EM_PA_RISC ");
        break;    
        case 16:
            printf("e_machine: EM_nCUBE"  );
        break;
        case 17:
            printf("e_machine:  EM_VPP500 " );
        break;
        case 18:
            printf("e_machine: EM_SPARC32PLUS " );
        break;
        case 19:
            printf("e_machine: EM_960" );
        break;
        case 20:
            printf("e_machine: EM_PPC  " );
        break;
        case 21:
            printf("e_machine: EM_PPC64 " );
        break;
        case 22:
            printf("e_machine: EM_S390 " );
        break;
        case 23:
            printf("e_machine: EM_UNKNOWN23  " );
        break;
        case 24:
            printf("e_machine:  EM_UNKNOWN24" );
        break;
        case 25:
            printf("e_machine: EM_UNKNOWN25 " );
        break;
        case 26:
            printf("e_machine: EM_UNKNOWN26  " );
        break;
        case 27:
            printf("e_machine: EM_UNKNOWN27 " );
        break;
        case 28:
            printf("e_machine: EM_UNKNOWN28" );
        break;
        case 29:
            printf("e_machine: EM_UNKNOWN29 " );
        break;
        case 30:
            printf("e_machine: EM_UNKNOWN30  " );
        break;
        case 31:
            printf("e_machine: EM_UNKNOWN31" );
        break;
        case 32:
            printf("e_machine: EM_UNKNOWN32" );
        break;
        case 33:
            printf("e_machine: EM_UNKNOWN33" );
        break;
        case 34:
            printf("e_machine: EM_UNKNOWN34" );
        break;     
        case 35:
            printf("e_machine: EM_UNKNOWN35" );
        break;
        case 36:
            printf("e_machine: EM_V800 " );
        break;
        case 37:
            printf("e_machine: EM_FR20" );
        break;
        case 38:
            printf("e_machine: EM_RH32 " );
        break;
        case 39:
            printf("e_machine: EM_RCE  " );
        break;
        case 40:
            printf("e_machine: EM_ARM " );
        break;
        case 41:
            printf("e_machine: EM_ALPHA" );
        break;
        case 42:
            printf("e_machine: EM_SH  " );
        break;
        case 43:
            printf("e_machine: EM_SPARCV9" );
        break;
        case 44:
            printf("e_machine: EM_TRICORE  " );
        break;
        case 45:
            printf("e_machine: EM_ARC " );
        break;
        case 46:
            printf("e_machine: EM_H8_300 " );
        break;
        case 47:
            printf("e_machine: EM_H8_300H " );
        break;
        case 48:
            printf("e_machine: EM_H8S " );
        break;
        case 49:
            printf("e_machine: EM_H8_500  " );
        break;
        case 50:
            printf("e_machine: EM_IA_64 " );
        break;
        case 51:
            printf("e_machine: EM_MIPS_X " );
        break;
        case 52:
            printf("e_machine: EM_COLDFIRE" );
        break;
        case 53:
            printf("e_machine: EM_68HC12  " );
        break;
        case 54:
            printf("e_machine: EM_MMA " );
        break;
        case 55:
            printf("e_machine: EM_PCP" );
        break;
        case 56:
            printf("e_machine: EM_NCPU   " );
        break;
        case 57:
            printf("e_machine: EM_NDR1 " );
        break;
        case 58:
            printf("e_machine: EM_STARCORE  " );
        break;
        case 59:
            printf("e_machine: EM_ME16 " );
        break;
        case 60:
            printf("e_machine: EM_ST100  " );
        break;
        case 61:
            printf("e_machine: EM_TINYJ " );
        break;
        case 62:
            printf("e_machine: EM_AMD64 " );
        break;
    
        case 63:
            printf("e_machine: EM_PDSP   " );
        break;
        case 64:
            printf("e_machine: EM_UNKNOWN64 " );
        break;
        case 65:
            printf("e_machine: EM_UNKNOWN65 " );
        break;
        case 66:
            printf("e_machine: EM_FX66 " );
        break;
        case 67:
            printf("e_machine: EM_ST9PLUS " );
        break;
        case 68:
            printf("e_machine: EM_ST7 " );
        break;
        case 69:
            printf("e_machine: EM_68HC16" );
        break;
        case 70:
            printf("e_machine: EM_68HC11" );
        break;
        case 71:
            printf("e_machine: EM_68HC08 " );
        break;
        case 72:
            printf("e_machine: EM_68HC05" );
        break;
        case 73:
            printf("e_machine: EM_SVX  " );
        case 74:
            printf("e_machine: EM_ST19 " );
        break;
        case 75:
            printf("e_machine: EM_VAX" );
        break;
        case 76:
            printf("e_machine: EM_CRIS" );
        break;
        case 77:
            printf("e_machine: EM_JAVELIN" );
        break;
        case 78:
            printf("e_machine: EM_FIREPATH  " );
        break;
        case 79:
            printf("e_machine:  EM_ZSP " );
        break;
        case 80:
            printf("e_machine: EM_MMIX " );
        break;
        case 81:
            printf("e_machine: EM_HUANY  " );
        break;
        case 82:
            printf("e_machine:  EM_PRISM " );
        break;
        case 83:
            printf("e_machine: EM_AVR  " );
        break;
        case 84:
            printf("e_machine: EM_FR30   " );
        break;
        case 85:
            printf("e_machine: EM_D10V  " );
        break;
        case 86:
            printf("e_machine: EM_D30V  " );
        break;
        case 87:
            printf("e_machine: EM_V850 " );
        break;
        case 88:
            printf("e_machine: EM_M32R " );
        break;
        case 89:
            printf("e_machine: EM_MN10300   " );
        break;
        case 90:
            printf("e_machine: EM_MN10200 " );
        break;
        case 91:
            printf("e_machine: EM_PJ" );
        break;
        case 92:
            printf("e_machine: EM_OPENRISC " );
        break;
        case 93:
            printf("e_machine: EM_ARC_A5" );
        break;
        case 94:
            printf("e_machine:  EM_XTENSA  " );
        break;
        case 95:
            printf("e_machine:  EM_XTENSA  ");
        break;

    }
    operator= ehdr.e_ident[6];
    
    switch(operator){
        case 0:
            printf("e_version: EV_NONE \n");
            break;
        case 1:
            printf("e_version: EV_CURRENT\n ");
            break;
        case 2:
            printf("e_version: EV_NUM \n");
            break;

    }
    int type = ehdr.e_type;
    switch(type){
        case 0:
            printf("e_type: ET_NONE  \n");
        break;
        case 1:
            printf("e_type: ET_REL   \n");
        break;
        case 2:
            printf("e_type:  ET_EXEC \n");
        break;
        case 3:
            printf("e_type: ET_DYN   \n");
        break;
        case 4:
            printf("e_type: ET_CORE   \n");
        break;
        case 5:
            printf("e_type: ET_NUM  \n");
        break;    
    }

    /*
      printf("ei_class:             ei_data:   \n");
      printf("ei_osabi:      ei_abiversion: \n");
      printf("e_machine:           e_version: \n");
      printf("e_type:    \n");
    */
    if(ehdr.e_flags==0){
      printf("e_flags:       %d              \n",ehdr.e_flags);
    }
    if(ehdr.e_flags==256){
        printf("e_flags:    [ EF_SPARC_32PLUS ]\n");
    }
      printf("e_entry:            0x%x e_ehsize:  %d     e_shstrndx:  %d\n",ehdr.e_entry,ehdr.e_ehsize,ehdr.e_shstrndx);
      printf("e_shoff:                 0x%x e_shentsize: %d   e_shnum: %d \n",ehdr.e_shoff,ehdr.e_shentsize,ehdr.e_shnum);
      printf("e_phoff:                  0x%x  e_phentsize:%d   e_phnum: %d \n",ehdr.e_phoff,ehdr.e_phentsize,ehdr.e_phnum);  

 












/*
    printf("Ident bytes:");
    for ( i = 0; i < EI_NIDENT; i++ ) {
        if ( i % 8 == 0 ) {
            printf("\n\t");
        }
        printf("%d:0x%x ", i, ehdr.e_ident[ i ] );
    }
    printf("\n");

    printf("File type:                    %d\n", ehdr.e_type );
    printf("Target machine:               %d\n", ehdr.e_machine );
    printf("File version:                 %d\n", ehdr.e_version );
    printf("Start address:                0x%x\n", ehdr.e_entry );
    */



 
    

 

 

    /* And more */

    for ( i = 0; i < ehdr.e_phnum; i++ ) {
        printf("\n");
        printf("Program Header [%d]:\n", i);
        fseek( fp, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET );
        if ( fread( &phdr, sizeof(phdr), 1, fp ) != 1 ) {
            fprintf(stderr, "%s: couldn't read %s\n", argv[0], argv[1]);
            exit(1);
        }
/*
        printf("Entry type:                    %d\n", phdr.p_type );
        printf("File offset:                   %d\n", phdr.p_offset );
        printf("Virtual address:               0x%x\n", phdr.p_vaddr );
        printf("File size:                     %d\n", phdr.p_filesz );
        printf("memory size:                   %d\n", phdr.p_memsz );
        printf("memory/file alignment:         0x%x\n", phdr.p_align );
*/
        printf("p_vaddr:      0x%x ",phdr.p_vaddr);
        int pflag=phdr.p_flags;
        switch(pflag){
            case  1:
                printf("p_flags:    [ PF_X ]\n");
            break;
            case  2:
                printf("p_flags:    [ PF_W ]\n");
            break;
            case  3:
                printf("p_flags:    [ PF_X PF_W ]\n");
            break;
            case  4:
                printf("p_flags:    [ PF_R ]\n");
            break;
            case  5:
                printf("p_flags:    [ PF_X PF_R ]\n");
            break;
            case  6:
                printf("p_flags:    [ PF_W PF_R ]\n");
            break;
            case  7:
                printf("p_flags:    [ PF_X PF_W PF_R ]\n");
            break;

        }
        printf("p_paddr:      %d       ",phdr.p_paddr);
        int ptype=phdr.p_type;
        switch(ptype){
            case 0:
                printf("p_type: [PT_NULL] \n");
            break;
            case 1:
                printf("p_type: [PT_LOAD ] \n");
            break;
            case 2:
                printf("p_type: [PT_DYNAMIC] \n");
            break;
            case 3:
                printf("p_type:  [PT_INTERP]\n");
            break;
            case 4:
                printf("p_type: [PT_NOTE ] \n");
            break;
            case 5:
                printf("p_type: [PT_SHLIB  ] \n");
            break;
            case 6:
                printf("p_type: [PT_PHDR  ] \n");
            break;
            case 7:
                printf("p_type: [PT_TLS ] \n");
            break;
            case 8:
                printf("p_type: [PT_NUM ] \n");
            break;
			default:
				printf("p_type: [PT_SUNW_UNWIND]\n");
			break;
        
        }
	if(phdr.p_memsz==0){
		  printf(" p_filesz:     0x%x       p_memsz:  0  \n",phdr.p_filesz);
	}
else{
      printf(" p_filesz:     0x%x       p_memsz:  0x%x  \n",phdr.p_filesz,phdr.p_memsz);
}     
   if(phdr.p_offset==0 && phdr.p_align!=0 ){
            printf("p_offset:       0        p_align:  0x%x  \n",phdr.p_align);
        }
        if(phdr.p_offset!=0 && phdr.p_align==0){
            printf("p_offset:       0x%x        p_align:  0  \n",phdr.p_offset);
        }
        if(phdr.p_offset!=0 && phdr.p_align>0){
            printf(" p_offset:     0x%x       p_align:  0x%x  \n",phdr.p_offset,phdr.p_align);
        }
        if(phdr.p_offset==0 && phdr.p_align==0){
            printf(" p_offset:     0       p_align:  0  \n");
        }
    }
        int secHeadOffSet = ehdr.e_shoff + ehdr.e_shstrndx*ehdr.e_shentsize;
        fseek(fp,secHeadOffSet,SEEK_SET);
        fread(&shdr,sizeof(shdr),1,fp);
        fseek(fp,shdr.sh_offset,SEEK_SET);
        char * stringTable = malloc(shdr.sh_size);
        fread(stringTable,shdr.sh_size,1,fp);
        char * secHeadname = " ";
        int dynamicPlace=0;
        int nonDynamicPlace=0;
    for ( i = 1; i < ehdr.e_shnum; i++ ) {
        printf("\n");
        int offset=shdr.sh_offset+ehdr.e_shstrndx;
        fseek(fp,ehdr.e_shoff+i*ehdr.e_shentsize,SEEK_SET);
         if ( fread( &shdr, sizeof(shdr), 1, fp ) != 1 ) {
            fprintf(stderr, "%s: couldn't read %s\n", argv[0], argv[1]);
            exit(1);
        }
        //fread(shdr,fp);
        //char * stringTable;
        secHeadname = &stringTable[shdr.sh_name];
        printf("Section Header[%d] :  sh_name: %s\n", i,secHeadname);
        //fseek( fp, ehdr.e_shoff + i * ehdr.e_shentsize, SEEK_SET );
         if(shdr.sh_addr==0){
            printf("sh_addr:     0      sh_flags:  ");   
        }
        else{
            printf("sh_addr:     0x%x      sh_flags:  ",shdr.sh_addr);
        }       
        vexology(shdr.sh_flags);
     
        //printf("\n");
        printf("sh_size:     0x%x            sh_type:    ",shdr.sh_size);
        sectionType(shdr.sh_type);
      
        printf("\n");
        int entnum=0;       
        if(shdr.sh_entsize>0){
         entnum=shdr.sh_size / shdr.sh_entsize;
        }      
        if(entnum==0){
            printf("sh_offset:   0x%x             sh_entsize: 0\n",shdr.sh_offset);   
        }
		else{
        printf("sh_offset:   0x%x             sh_entsize: 0x%x (%d entries)\n",shdr.sh_offset,shdr.sh_entsize,entnum);
		}        
		//,shdr.sh_size/shdr.sh_entsize
        printf(" sh_link:     %d                sh_info:  %d  \n",shdr.sh_link,shdr.sh_info);
        printf("sh_addralign: 0x%x\n",shdr.sh_addralign);

        if(shdr.sh_type == SHT_SUNW_versym){
            version=i;
        }

        if(strcmp(secHeadname,".dynsym")==0){
            //dynamicSize= shdr.sh_size/shdr.sh_entsize;  
            dynamicPlace=i;
            weHaveD=1;
          
        }
      
        if(strcmp(secHeadname,".symtab")==0){
            nonDynamicPlace=i;          
            weHaveS=1;
        }        
  
 
    }
    if(weHaveD==1){
    printf("\n");
    printf("Symbol Table Section:  .dynsym\n");
   fseek(fp,ehdr.e_shoff+dynamicPlace*ehdr.e_shentsize,SEEK_SET);
    fread(&shdr,sizeof(shdr),1,fp);
    dymPrint(shdr,stringTable);
    //printf("\n");
    }
    if(weHaveS==1){
    printf("\n");
    printf("Symbol Table Section:  .symtab\n");
   
       fseek(fp,ehdr.e_shoff+nonDynamicPlace*ehdr.e_shentsize,SEEK_SET);
    fread(&shdr,sizeof(shdr),1,fp);
    isSym=1;
    dymPrint(shdr,stringTable);
    }
  //fseek to correct place and read it
    //seek to ehdr offset + (index * ehdr shint size)
    //read shdr the same way you do elsewhere
    //for(0 to numents ){print them}
  
 
    //fseek(fp,ehdr.e_shoff)
/*
    fseek(fp,phdr.p_offset,SEEK_SET);
        fread(addr,phdr.p_filesz,1,fp);
  */     
         
 








   exit(0);
}
