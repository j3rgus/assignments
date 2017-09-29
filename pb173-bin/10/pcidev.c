#include <stdio.h>
#include <stdint.h>
#include <pciaccess.h>

#include "pcihdr.h"

const char *get_class_string(uint8_t class, uint8_t subclass)
{
	const PCI_CLASSCODETABLE *curr = PciClassCodeTable;

	while (curr->BaseClass != 0xff) {
		if (curr->BaseClass == class && curr->SubClass == subclass)
			break;
		curr++;
	}
	return curr->BaseDesc;
}

const char *get_ven_string(uint16_t ven)
{
	const PCI_VENTABLE *curr = PciVenTable;

	while (curr->VenId != 0xffff) {
		if (curr->VenId == ven)
			break;
		curr++;
	}
	return curr->VenFull;
}

const char *get_dev_string(uint16_t ven, uint16_t dev)
{
	const PCI_DEVTABLE *curr = PciDevTable;

	while (curr->VenId != 0xffff) {
		if ((curr->VenId == ven) && (curr->DevId == dev))
			break;
		curr++;
	}
	return curr->ChipDesc;
}

void print_data(unsigned char *data, size_t len)
{
	int i;

	for (i = 0; i < len; i++) {
		if ((i % 16) == 0)
			printf("\n%02x: ", i);
		printf("%02x ", data[i]);
	}
	printf("\n");
}

int main(void)
{
	unsigned char data[64];
	struct pci_device_iterator *it;
	struct pci_device *pci;
	uint32_t class;
	uint16_t ven, dev;
	int ret;

	pci_system_init();
	it = pci_id_match_iterator_create(NULL);

	while ((pci = pci_device_next(it)) != NULL) {
		printf("%04x:%02x:%02x.%x", pci->domain, pci->bus,
					pci->dev, pci->func);

		class = pci->device_class;
		class = 0xffff & (class >> 8);

		printf(" %s [%04x]: %s %s [%04x:%04x] (rev %02x)",
					get_class_string(class>>8, class & 0xff),
					class, pci_device_get_vendor_name(pci),
					pci_device_get_device_name(pci), pci->vendor_id,
					pci->device_id, pci->revision);

		ret = pci_device_probe(pci);
		if (ret != 0) {
			perror("pci_device_probe");
			return 1;
		}

		ret = pci_device_cfg_read(pci, data, 0, 64, NULL);
		if (ret != 0) {
			perror("pci_device_cfg_read");
			continue;
		}
		print_data(data, 64);

		/*DevID 2B, VenId 2B*/
		ven = data[0] + (data[1] << 8);
		dev = data[2] + (data[3] << 8);
		printf("Vendor: %04x (%s)\n", ven, get_ven_string(ven));
		printf("Device: %04x (%s)\n\n", dev, get_dev_string(ven, dev));
	}

	pci_iterator_destroy(it);
	pci_system_cleanup();
	return 0;
}
