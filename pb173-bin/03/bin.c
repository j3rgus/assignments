#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfd.h>

#define HELLO 1
#define WORLD 2

#define MY_SECTION ".comment.my_objcopy"

void bfd_err(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, bfd_errmsg(bfd_get_error()));
	exit(1);
}

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

void print_sect(bfd *abfd, asection *sect, void *obj)
{
	char *data;
	char *hello = (char *) obj;
	unsigned long sect_size;

	sect_size = (unsigned long) bfd_get_section_size(sect);

	printf(" %3d  %-17s %08lx   ", sect->index, sect->name, sect_size);
	bfd_printf_vma(abfd, bfd_get_section_vma(abfd, sect));
	printf("   ");
	bfd_printf_vma(abfd, sect->lma);
	printf("   %08lx", (unsigned long) sect->filepos);
	printf("   2**%u\n", bfd_get_section_alignment(abfd, sect));

#define PF(x, y) if (sect->flags & x) printf("%s, ", y);

	printf("                        ");
	PF(SEC_HAS_CONTENTS, "CONTENTS");
	PF(SEC_ALLOC, "ALLOC");
	PF(SEC_CONSTRUCTOR, "CONSTRUCTOR");
	PF(SEC_LOAD, "LOAD");
	PF(SEC_RELOC, "RELOC");
	PF(SEC_READONLY, "READONLY");
	PF(SEC_CODE, "CODE");
	PF(SEC_DATA, "DATA");
	PF(SEC_ROM, "ROM");
	PF(SEC_DEBUGGING, "DEBUGGING");
	PF(SEC_NEVER_LOAD, "NEVER_LOAD");
	PF(SEC_EXCLUDE, "EXCLUDE");
	PF(SEC_SORT_ENTRIES, "SORT_ENTRIES");
	putchar('\n');

	data = malloc(sect_size);
	if (!data)
		exit(1);

	if (bfd_get_section_contents(abfd, sect, data, 0, sect_size) == FALSE)
		bfd_err("bfd_get_section_contents");

	*hello |= (find_string(data, sect_size, "hello")) ? HELLO : 0;
	*hello |= (find_string(data, sect_size, "world")) ? WORLD : 0;

	free(data);
}

int print_bfd_info(bfd *abfd)
{
	char hello = 0;

	printf("\n%s:\tfile format %s\n", bfd_get_filename(abfd),
		abfd->xvec->name);
	printf("architecture:\t%s, flags 0x%08X\n", bfd_printable_name(abfd),
		abfd->flags);
	printf("start address 0x");
	bfd_printf_vma(abfd, abfd->start_address);

	printf("\n\nSections:\n");
	puts(" Idx  Name              Size       VMA        LMA"
			"        File off   Algn");

	bfd_map_over_sections(abfd, print_sect, &hello);

	if (hello == (HELLO | WORLD))
		printf("\nIt's probably \'Hello World\' program.\n");

	return 0;
}

void copy_section(bfd *abfd, asection *sect, void *obj)
{
	char *data;
	char *ptr;
	asection *osection;
	bfd *obfd = (bfd *) obj;
	unsigned long sect_size;

	osection = sect->output_section;
	if (!osection)
		return;

	sect_size = (unsigned long) bfd_get_section_size(osection);

	data = malloc(sect_size);
	if (!data) {
		fprintf(stderr, "Memory alloc problem! (Set limit?)");
		exit(1);
	}

	bfd_get_section_contents(abfd, sect, data, 0, sect_size);
	while ((ptr = find_string(data, sect_size, "hello")) != NULL) {
		ptr[1] = 'o';
		ptr[4] = 'a';
	}
	if (bfd_set_section_contents(obfd, osection, data, 0,
			sect_size) == FALSE)
		fprintf(stderr, "Couldn't set data for %s section!\n",
			osection->name);

	free(data);
}

void prepare_section(bfd *abfd, asection *sect, void *obj)
{
	char *data;
	asection *osection;
	unsigned long sect_size;
	bfd *obfd = (bfd *) obj;

	if (!(sect->flags & SEC_DATA))
		return;

	sect->output_section = NULL;

	sect_size = (unsigned long) bfd_get_section_size(sect);
	data = malloc(sect_size);
	if (!data) {
		fprintf(stderr, "Memory alloc problem! (Set limit?)");
		exit(1);
	}

	bfd_get_section_contents(abfd, sect, data, 0, sect_size);
	if (!find_string(data, sect_size, "hello"))
		goto skip;

	osection = bfd_make_section_with_flags(obfd, sect->name, sect->flags);
	if (!osection) {
		fprintf(stderr, "Problem while making section %s!\n",
			sect->name);
		goto skip;
	}

	if (bfd_set_section_size(obfd, osection, sect_size) == FALSE)
		fprintf(stderr, "Problem while setting size of %s section!\n",
			osection->name);

	sect->output_section = osection;

skip:
	free(data);
}

int make_bfd(bfd *in, bfd *out)
{
	asection *o_sect;
	char *data = "This is my own section!";

	if (bfd_set_format(out, in->format) == FALSE)
		return 1;

	bfd_set_arch_info(out, bfd_get_arch_info(in));

	bfd_map_over_sections(in, prepare_section, out);

	o_sect = bfd_make_section_with_flags(out, MY_SECTION, SEC_HAS_CONTENTS);
	if (!o_sect)
		fprintf(stderr, "Problem while making section %s!",
			MY_SECTION);

	if (bfd_set_section_size(out, o_sect,
			strlen(data)) == FALSE)
		fprintf(stderr, "Problem while setting size for %s!\n",
			MY_SECTION);

	bfd_map_over_sections(in, copy_section, out);

	if (bfd_set_section_contents(out, o_sect, data, 0,
			strlen(data)) == FALSE)
		fprintf(stderr, "Couldn't set data for %s section\n",
			MY_SECTION);

	return 0;
}

int main(int argc, char *argv[])
{
	bfd *abfd;
	bfd *inbfd, *outbfd;

	if (argc != 4) {
		fprintf(stderr, "Usage: ./bin <in_file1> <in_file2> <out_file>\n");
		return 1;
	}

	/* Printing in_file1 info */
	abfd = bfd_openr(argv[1], NULL);
	if (!abfd)
		bfd_err(argv[1]);

	if (bfd_check_format(abfd, bfd_object) == FALSE)
		bfd_err(argv[1]);

	print_bfd_info(abfd);

	if (bfd_close(abfd) == FALSE)
		bfd_err(argv[1]);
	/* End of printing */

	/* Copy "hello sections" of in_file2 into out_file */
	inbfd = bfd_openr(argv[2], NULL);
	if (!inbfd)
		bfd_err(argv[2]);

	if (bfd_check_format(inbfd, bfd_object) == FALSE)
		bfd_err(argv[2]);

	outbfd = bfd_openw(argv[3], NULL);
	if (!outbfd)
		bfd_err("bfd_openw");

	if (make_bfd(inbfd, outbfd))
		bfd_err("make_bfd");

	if (bfd_close(outbfd) == FALSE)
		bfd_err(argv[3]);

	if (bfd_close(inbfd) == FALSE)
		bfd_err(argv[2]);
	/* End of copying */

	return 0;
}
