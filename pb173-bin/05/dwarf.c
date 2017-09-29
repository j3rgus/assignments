#include <stdio.h>
#include <elfutils/libdw.h>
#include <elfutils/libdwfl.h>
#include <dwarf.h>
#include <err.h>

Dwfl_Callbacks cb = {
	.section_address = dwfl_offline_section_address,
};


char *get_type_str(Dwarf_Die *die)
{
	static char buf[256] = "";
	char *ptr = NULL;
	Dwarf_Die tdie;
	Dwarf_Attribute attr;

	if (!dwarf_attr(die, DW_AT_type, &attr))
		return "void";

	dwarf_formref_die(&attr, &tdie);

	if (dwarf_tag(&tdie) == DW_TAG_array_type)
		ptr = "[]";
	else if (dwarf_tag(&tdie) == DW_TAG_pointer_type)
		ptr = "*";
	else
		goto end_ok;

	dwarf_attr(&tdie, DW_AT_type, &attr);
	dwarf_formref_die(&attr, &tdie);

end_ok:
	sprintf(buf, "%s%s", dwarf_diename(&tdie), (ptr) ? ptr : "");

	return buf;
}

void print_dbg_info(Dwarf_Die *die, int lvl)
{
	int tag, line, i;

#define padding(x) for (i = 0; i < (x*3); i++) putchar(' ');
	tag = dwarf_tag(die);

	if (tag == DW_TAG_compile_unit)
		printf("In file %s ...\n\n", dwarf_diename(die));

	if (tag == DW_TAG_variable) {
		dwarf_decl_line(die, &line);
		printf("Variable %s on line %d (%s)\n", dwarf_diename(die),
				line, get_type_str(die));
	}

	if (tag == DW_TAG_subprogram) {
		padding(lvl);
		dwarf_decl_line(die, &line);
		printf("Function %s on line %d (%s)\n", dwarf_diename(die),
				line, get_type_str(die));
	}

	if (tag == DW_TAG_formal_parameter) {
		padding(lvl);
		printf("Formal parameter: %s (%s)\n", dwarf_diename(die),
				get_type_str(die));
	}

	if (tag == DW_TAG_typedef)
		printf("typedef %s %s\n", get_type_str(die),
				dwarf_diename(die));

	if (tag == DW_TAG_structure_type)
		printf("strucure %s\n", dwarf_diename(die));

	if (tag == DW_TAG_member) {
		padding(lvl);
		printf("%s %s\n", get_type_str(die), dwarf_diename(die));
	}
#undef padding
}

void print_die_rec(Dwarf_Die *die, int lvl)
{
	int ret;
	Dwarf_Die chdie, sdie;

	if (!die)
		return;

	print_dbg_info(die, lvl);

	ret = dwarf_child(die, &chdie);
	if (!ret)
		print_die_rec(&chdie, lvl+1);

	ret = dwarf_siblingof(die, &sdie);
	if (!ret)
		print_die_rec(&sdie, lvl);
}

int main(void)
{
	Dwfl *dw;
	Dwfl_Module *mod;
	Dwarf_Addr bias;
	Dwarf_Die *die = NULL;

	dw = dwfl_begin(&cb);
	if (!dw)
		errx(1, "dwfl_begin %s", dwfl_errmsg(-1));

	mod = dwfl_report_offline(dw, "", "tst", -1);
	if (!mod)
		errx(1, "dwfl_begin %s", dwfl_errmsg(-1));

	while ((die = dwfl_nextcu(dw, die, &bias))) {
		print_die_rec(die, -1);
		printf("\n");
	}

	dwfl_end(dw);
	return 0;
}
