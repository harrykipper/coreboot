#include <pci.h>
#include <pc80/keyboard.h>
#include <printk.h>

void keyboard_on()
{
	volatile unsigned char regval;

	/*  regval = intel_conf_readb(0x8000385A); */
	/*regval |= 0x01; */
	regval = 0xff;
	intel_conf_writeb(0x8000385A, regval);

	/* disable USB1 */
	intel_conf_writeb(0x80003A3C, 0x00);
	intel_conf_writeb(0x80003A04, 0x00);
	regval = intel_conf_readb(0x80003848);
	regval |= 0x04;
	intel_conf_writeb(0x80003848, regval);

	/* disable USB2 */
	intel_conf_writeb(0x80003B3C, 0x00);
	intel_conf_writeb(0x80003B04, 0x00);
	regval = intel_conf_readb(0x80003885);
	regval |= 0x10;
	intel_conf_writeb(0x80003885, regval);

	pc_keyboard_init();

}

void nvram_on()
{
	/* the VIA 686A South has a very different nvram setup than the piix4e ... */
	/* turn on ProMedia nvram. */
	/* TO DO: use the PciWriteByte function here. */
	intel_conf_writeb(0x80003843, 0xFF);
}

void southbridge_fixup()
{
	unsigned int devfn;
	unsigned char enables;

	// enable the internal I/O decode
	// to do: use the pcibios_find function here, instead of 
	// hard coding the devfn. 
	devfn = PCI_DEVFN(7, 0);
	enables = pcibios_read_config_byte(0, devfn, 0x81, &enables);
	enables |= 0x80;
	pcibios_write_config_byte(0, devfn, 0x81, enables);

	// enable com1 and com2. 
	enables = pcibios_read_config_byte(0, devfn, 0x83, &enables);
	// 0x80 is enable com port b, 0x1 is to make it com2, 0x8 is enable com port a as com1
	enables = 0x80 | 0x1 | 0x8 ;
	pcibios_write_config_byte(0, devfn, 0x83, enables);
	// note: this is also a redo of some port of assembly, but we want everything up. 
	// set com1 to 115 kbaud
	// not clear how to do this yet. 
	// forget it; done in assembly. 
	// enable IDE, since Linux won't do it.
	// First do some more things to devfn (7,0)
	// note: this should already be cleared, according to the book. 
	pcibios_read_config_byte(0, devfn, 0x48, &enables);
	printk("IDE enable in reg. 48 is 0x%x\n", enables);
	enables &= ~2; // need manifest constant here!
	printk("set IDE reg. 48 to 0x%x\n", enables);
	pcibios_write_config_byte(0, devfn, 0x48, enables);

	// set default interrupt values (IDE)
	pcibios_read_config_byte(0, devfn, 0x4a, &enables);
	printk("IRQs in reg. 4a are 0x%x\n", enables & 0xf);
	// clear out whatever was there. 
	enables &= ~0xf;
	enables |= 4;
	printk("setting reg. 4a to 0x%x\n", enables);
	pcibios_write_config_byte(0, devfn, 0x4a, enables);
	
	// set up the serial port interrupts. 
	// com2 to 3, com1 to 4
	pcibios_write_config_byte(0, devfn, 0x52, 0x34);

	devfn = PCI_DEVFN(7, 1);
	pcibios_read_config_byte(0, devfn, 0x40, &enables);
	printk("enables in reg 0x40 0x%x\n", enables);
	enables |= 3;
	pcibios_write_config_byte(0, devfn, 0x40, enables);
	pcibios_read_config_byte(0, devfn, 0x40, &enables);
	printk("enables in reg 0x40 read back as 0x%x\n", enables);
	// address decoding. 
	// we want "flexible", i.e. 1f0-1f7 etc. or native PCI
        pcibios_read_config_byte(0, devfn, 0x9, &enables);
        printk("enables in reg 0x9 0x%x\n", enables);
	// by the book, set the low-order nibble to 0xa. 
	enables &= ~0xf;
	// cf/cg silicon needs an 'f' here. 
        enables |= 0xf;
        pcibios_write_config_byte(0, devfn, 0x9, enables);
        pcibios_read_config_byte(0, devfn, 0x9, &enables);
        printk("enables in reg 0x9 read back as 0x%x\n", enables);

	// standard bios sets master bit. 
	pcibios_read_config_byte(0, devfn, 0x4, &enables);
	printk("command in reg 0x4 0x%x\n", enables);
	enables |= 5;
	pcibios_write_config_byte(0, devfn, 0x4, enables);
	pcibios_read_config_byte(0, devfn, 0x4, &enables);
	printk("command in reg 0x4 reads back as 0x%x\n", enables);

	// oh well, the PCI BARs don't work right. 
	// This chip will not work unless IDE runs at standard legacy
	// values. 

	pcibios_write_config_dword(0, devfn, 0x10, 0x1f1);
	pcibios_write_config_dword(0, devfn, 0x14, 0x3f5);
	pcibios_write_config_dword(0, devfn, 0x18, 0x171);
	pcibios_write_config_dword(0, devfn, 0x1c, 0x375);
	pcibios_write_config_dword(0, devfn, 0x20, 0xcc0);


}
