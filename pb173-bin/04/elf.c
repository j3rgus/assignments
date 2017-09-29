#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <libelf.h>
#include <gelf.h>
#include <err.h>

#define HELLO			1
#define WORLD			2
#define TSIZE			1024
#define MYSEC_DATA		"This is my private section"
#define tstrcat(x, y)	{ if ((tindex + strlen(y)) >= TSIZE) \
							errx(1, "Too much sections!"); \
						strcat(&x[tindex], y); \
						tindex += strlen(y) + 1; }

char tab[TSIZE];
unsigned int tindex = 1;

char *find_string(char *in_mem, unsigned long size, char *word)
{
	char *ptr = NULL;
	char *mem = in_mem;
	unsigned long i;
	unsigned long w_size = strlen(word);

	if (size < w_size)
		return NULL;

	for (i = 0; i <= (size - w_size); i++) {
		if (mem[i] != word[0])
			continue;
		if (!memcmp(&mem[i], word, w_size)) {
			ptr = &mem[i];
			break;
		}
	}

	return ptr;
}

char *get_sec_type_str(unsigned int sh_type)
{
	switch (sh_type) {
	case SHT_NULL:			return "NULL";
	case SHT_PROGBITS:		return "PROGBITS";
	case SHT_SYMTAB:		return "SYMTAB";
	case SHT_STRTAB:		return "STRTAB";
	case SHT_RELA:			return "RELA";
	case SHT_HASH:			return "HASH";
	case SHT_DYNAMIC:		return "DYNAMIC";
	case SHT_NOTE:			return "NOTE";
	case SHT_NOBITS:		return "NOBITS";
	case SHT_REL:			return "REL";
	case SHT_SHLIB:			return "SHLIB";
	case SHT_DYNSYM:		return "DYNSYM";
	case SHT_INIT_ARRAY:	return "INIT_ARRAY";
	case SHT_FINI_ARRAY:	return "FINI_ARRAY";
	case SHT_PREINIT_ARRAY:	return "PREINIT_ARRAY";
	case SHT_GROUP:			return "GROUP";
	case SHT_SYMTAB_SHNDX:	return "SYMTAB SECTION INDICIES";
	case SHT_GNU_verdef:	return "VERDEF";
	case SHT_GNU_verneed:	return "VERNEED";
	case SHT_GNU_versym:	return "VERSYM";
	}

	return "UNKNOWN";
}

char *get_file_type(unsigned int type)
{
	switch (type) {
	case ET_NONE:	return "NONE (None)";
	case ET_REL:	return "REL (Relocatable file)";
	case ET_EXEC:	return "EXEC (Executable file)";
	case ET_DYN:	return "DYN (Shared object file)";
	case ET_CORE:	return "CORE (Core file)";
	}

	return "UNKNOWN";
}

char *get_phdr_type(unsigned int type)
{
	switch (type) {
	case PT_NULL: return "NULL";
	case PT_LOAD: return "LOAD";
	case PT_DYNAMIC: return "DYNAMIC";
	case PT_INTERP: return "INTERP";
	case PT_NOTE: return "NOTE";
	case PT_SHLIB: return "SHLIB";
	case PT_PHDR: return "PHDR";
	case PT_TLS: return "TLS";
	case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";
	case PT_GNU_STACK: return "GNU_STACK";
	case PT_GNU_RELRO: return "GNU_RELRO";
	}

	return "UNKNOWN";
}

void print_phdr_flg(unsigned int flags)
{
#define PF(x, y) (flags & y) ? putchar(x) : putchar(' ');
	PF('R', PF_R);
	PF('W', PF_W);
	PF('X', PF_X);
#undef PF
}

void print_sec_flags(unsigned long flags, short cnt)
{
	char str[16];
	char *ptr = str;

#define SF(x, y) if (flags & y) { *ptr = x; ptr++; }
	SF('W', SHF_WRITE);
	SF('A', SHF_ALLOC);
	SF('X', SHF_EXECINSTR);
	SF('M', SHF_MERGE);
	SF('S', SHF_STRINGS);
	SF('I', SHF_INFO_LINK);
	SF('L', SHF_LINK_ORDER);
	SF('O', SHF_OS_NONCONFORMING);
	SF('G', SHF_GROUP);
	SF('T', SHF_TLS);

	*ptr = 0;
	printf(" %3.*s ", cnt, str);
#undef SF
}

void printf_elf_info(Elf *elf)
{
	Elf_Scn *scn = NULL;
	Elf_Data *data = NULL;
	GElf_Shdr shdr;
	GElf_Ehdr ehdr;
	GElf_Phdr phdr;
	unsigned int i = 1, n, phnum;
	unsigned short hello = 0;

	gelf_getehdr(elf, &ehdr);

	printf("There are %u section headers, "
			"starting at offset 0x%llx;\n\n", ehdr.e_shnum, ehdr.e_shoff);

	printf("Sections Headers:\n");
	printf("  [Nr] Name              Type            Addr     "
			"Off    Size   ES Flg Lk Inf Al\n");

	elf_getshdrstrndx(elf, &n);

	while ((scn = elf_nextscn(elf, scn))) {
		gelf_getshdr(scn, &shdr);

		printf("  [%2u] %-17.17s %-15s %08llx %06llx %06llx %02llx", i++,
				elf_strptr(elf, n, shdr.sh_name), get_sec_type_str(shdr.sh_type),
				shdr.sh_addr, shdr.sh_offset, shdr.sh_size, shdr.sh_entsize);

		print_sec_flags(shdr.sh_flags, 3);

		printf("%2lu %3ld %2lu\n", (unsigned long) shdr.sh_link,
				(long) shdr.sh_info, (unsigned long) shdr.sh_addralign);

		while ((data = elf_getdata(scn, data)) != NULL) {
			if (!data->d_buf)
				continue;
			hello |= (find_string(data->d_buf, data->d_size,
						"hello")) ? HELLO : 0;
			hello |= (find_string(data->d_buf, data->d_size,
						"world")) ? WORLD : 0;
		}
	}

	elf_getphdrnum(elf, &phnum);
	if (!phnum) {
		printf("\nThere are no program headers in this file.");
		goto end;
	}

	printf("\nElf file type is %s\nEntry point 0x%llx\n",
			get_file_type(ehdr.e_type), ehdr.e_entry);

	printf("There are %u program headers, "
			"starting at offset 0x%llx\n\n", phnum, ehdr.e_phoff);

	printf("Program headers:\n"
			" Type              Offset   VirtAddr   PhysAddr   FileSiz"
			"  MemSize  Flg Align\n");

	for (i = 0; i < phnum; i++) {
		gelf_getphdr(elf, i, &phdr);

		printf(" %-17s 0x%06llx 0x%08llx 0x%08llx 0x%06llx 0x%06llx ",
				get_phdr_type(phdr.p_type),	phdr.p_offset, phdr.p_vaddr,
				phdr.p_paddr, phdr.p_filesz, phdr.p_memsz);

		print_phdr_flg(phdr.p_flags);

		printf(" 0x%llx\n", phdr.p_align);
	}

end:
	printf("\n");
	if (hello == (HELLO | WORLD))
		printf("\nIt's probably \'Hello World\' program.\n");
}

int copy_sect(GElf_Shdr *shdr, Elf_Data *data, Elf_Scn *oscn, Elf *eout)
{
	Elf_Data *odata = NULL;
	Elf32_Shdr *oshdr = NULL;
	char *ptr;

	oscn = elf_newscn(eout);
	if (oscn == NULL)
		errx(1, "elf_newscn(): %s", elf_errmsg(-1));

	odata = elf_newdata(oscn);
	if (odata == NULL)
		errx(1, "elf_newdata(): %s", elf_errmsg(-1));

	odata->d_buf = data->d_buf;
	odata->d_size = data->d_size;

	while ((ptr = find_string(odata->d_buf, odata->d_size, "hello")) != NULL) {
		ptr[1] = 'o';
		ptr[4] = 'a';
	}

	odata->d_type = data->d_type;
	odata->d_version = data->d_version;

	oshdr = elf32_getshdr(oscn);
	if (oshdr == NULL)
		errx(1, "elf32_getshdr(): %s", elf_errmsg(-1));

	oshdr->sh_name = tindex;
	oshdr->sh_type = shdr->sh_type;
	oshdr->sh_flags = shdr->sh_flags;

	return 0;
}

int copy_elf(Elf *e_in, Elf *e_out)
{
	Elf_Scn *scn = NULL, *oscn = NULL;
	Elf32_Ehdr *ehdr, *ehdr_o = NULL;
	Elf32_Shdr *oshdr = NULL;
	GElf_Shdr shdr;
	Elf_Data *data = NULL, *odata = NULL;
	unsigned int n;

	ehdr = elf32_getehdr(e_in);
	ehdr_o = elf32_newehdr(e_out);

	/* Copy Executable header */
	ehdr_o->e_ident[EI_DATA] = ehdr->e_ident[EI_DATA];
	ehdr_o->e_machine = ehdr->e_machine;
	ehdr_o->e_type = ehdr->e_type;
	ehdr_o->e_version = ehdr->e_version;

	elf_getshdrstrndx(e_in, &n);

	/* Copy sections containing "hello" */
	while ((scn = elf_nextscn(e_in, scn))) {
		gelf_getshdr(scn, &shdr);
		while ((data = elf_getdata(scn, data)) != NULL) {
			if (!data->d_buf)
				continue;
			if (find_string(data->d_buf, data->d_size, "hello")) {
				copy_sect(&shdr, data, oscn, e_out);
				tstrcat(tab, elf_strptr(e_in, n, shdr.sh_name));
			}
		}
	}

	/* Create .comment.my_objcopy section */
	oscn = elf_newscn(e_out);
	if (oscn == NULL)
		errx(1, "elf_newscn(): %s", elf_errmsg(-1));

	odata = elf_newdata(oscn);
	if (odata == NULL)
		errx(1, "elf_newdata(): %s", elf_errmsg(-1));

	odata->d_buf = MYSEC_DATA;
	odata->d_size = strlen(MYSEC_DATA);

	oshdr = elf32_getshdr(oscn);
	if (oshdr == NULL)
		errx(1, "elf32_getshdr(): %s", elf_errmsg(-1));

	oshdr->sh_name = tindex;
	oshdr->sh_flags = SHF_ALLOC;
	oshdr->sh_type = SHT_PROGBITS;

	tstrcat(tab, ".comment.my_objcopy");

	/* Create .shstrtab section */
	oscn = elf_newscn(e_out);
	if (oscn == NULL)
		errx(1, "elf_newscn(): %s", elf_errmsg(-1));

	odata = elf_newdata(oscn);
	if (odata == NULL)
		errx(1, "elf_newdata(): %s", elf_errmsg(-1));

	odata->d_buf = tab;
	odata->d_size = tindex + 10;	/* Length 10 for shstrtab */
	odata->d_version = EV_CURRENT;

	oshdr = elf32_getshdr(oscn);
	if (oshdr == NULL)
		errx(1, "elf32_getshdr(): %s", elf_errmsg(-1));

	oshdr->sh_name = tindex;
	oshdr->sh_type = SHT_STRTAB;
	oshdr->sh_flags = SHF_STRINGS | SHF_ALLOC;

	tstrcat(tab, ".shstrtab");

	/* Save index of .shstrtab section */
	ehdr_o->e_shstrndx = elf_ndxscn(oscn);

	/* Write down all sections */
	if (elf_update(e_out, ELF_C_WRITE) < 0)
		errx(1, "elf_update (WRITE): %s", elf_errmsg(-1));

	return 0;
}

int main(int argc, char *argv[])
{
	int fdi, fdo;
	Elf *e_in, *e_out;

	if (elf_version(EV_CURRENT) == EV_NONE)
		errx(1, "Init elf library");

	if (argc != 4) {
		fprintf(stderr, "Using: ./elf <in_file1> <in_file2> <out_file>\n");
		return 1;
	}

	fdi = open(argv[1], O_RDONLY);
	if (fdi < 0)
		err(1, "opening %s failed", argv[1]);

	e_in = elf_begin(fdi, ELF_C_READ, NULL);
	if (e_in == NULL)
		errx(1, "elf_begin(): %s", elf_errmsg(-1));

	if (elf_kind(e_in) != ELF_K_ELF)
		errx(1, "bad file format %s", argv[1]);

	/* Print in_file1 elf informations */
	printf_elf_info(e_in);

	elf_end(e_in);
	close(fdi);

	fdi = open(argv[2], O_RDONLY);
	if (fdi < 0)
		err(1, "opening %s failed", argv[2]);

	fdo = open(argv[3], O_RDWR | O_CREAT, S_IRWXU);
	if (fdo < 0)
		err(1, "opening %s failed", argv[3]);

	e_in = elf_begin(fdi, ELF_C_READ, NULL);
	if (e_in == NULL)
		errx(1, "elf_begin(): %s", elf_errmsg(-1));

	if (elf_kind(e_in) != ELF_K_ELF)
		errx(1, "bad file format %s", argv[2]);

	e_out = elf_begin(fdo, ELF_C_WRITE, NULL);
	if (e_out == NULL)
		errx(1, "elf_begin(): %s", elf_errmsg(-1));

	/* Copy 'hello sections' of in_file2 into out_file */
	copy_elf(e_in, e_out);

	elf_end(e_out);
	elf_end(e_in);

	close(fdo);
	close(fdi);

	return 0;
}
