/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

Device(LIBR) {
	Name(_ADR, 0x00140003)
	/* Method(_INI) {
	*	DBGO("\\_SB\\PCI0\\LpcIsaBr\\_INI\n")
	} */ /* End Method(_SB.SBRDG._INI) */

	/* Real Time Clock Device */
	Device(RTC0) {
		Name(_HID, EISAID("PNP0B00"))   /* AT Real Time Clock (not PIIX4 compatible) */
		Name(_CRS, ResourceTemplate() {
			IRQNoFlags(){8}
			IO(Decode16,0x0070, 0x0070, 0, 2)
			/* IO(Decode16,0x0070, 0x0070, 0, 4) */
		})
	} /* End Device(_SB.PCI0.LpcIsaBr.RTC0) */

	Device(TMR) {	/* Timer */
		Name(_HID,EISAID("PNP0100"))	/* System Timer */
		Name(_CRS, ResourceTemplate() {
			IRQNoFlags(){0}
			IO(Decode16, 0x0040, 0x0040, 0, 4)
			/* IO(Decode16, 0x0048, 0x0048, 0, 4) */
		})
	} /* End Device(_SB.PCI0.LpcIsaBr.TMR) */

	Device(SPKR) {	/* Speaker */
		Name(_HID,EISAID("PNP0800"))	/* AT style speaker */
		Name(_CRS, ResourceTemplate() {
			IO(Decode16, 0x0061, 0x0061, 0, 1)
		})
	} /* End Device(_SB.PCI0.LpcIsaBr.SPKR) */

	Device(PIC) {
		Name(_HID,EISAID("PNP0000"))	/* AT Interrupt Controller */
		Name(_CRS, ResourceTemplate() {
			IRQNoFlags(){2}
			IO(Decode16,0x0020, 0x0020, 0, 2)
			IO(Decode16,0x00A0, 0x00A0, 0, 2)
			/* IO(Decode16, 0x00D0, 0x00D0, 0x10, 0x02) */
			/* IO(Decode16, 0x04D0, 0x04D0, 0x10, 0x02) */
		})
	} /* End Device(_SB.PCI0.LpcIsaBr.PIC) */

	Device(MAD) { /* 8257 DMA */
		Name(_HID,EISAID("PNP0200"))	/* Hardware Device ID */
		Name(_CRS, ResourceTemplate() {
			DMA(Compatibility,BusMaster,Transfer8){4}
			IO(Decode16, 0x0000, 0x0000, 0x10, 0x10)
			IO(Decode16, 0x0081, 0x0081, 0x01, 0x03)
			IO(Decode16, 0x0087, 0x0087, 0x01, 0x01)
			IO(Decode16, 0x0089, 0x0089, 0x01, 0x03)
			IO(Decode16, 0x008F, 0x008F, 0x01, 0x01)
			IO(Decode16, 0x00C0, 0x00C0, 0x10, 0x20)
		}) /* End Name(_SB.PCI0.LpcIsaBr.MAD._CRS) */
	} /* End Device(_SB.PCI0.LpcIsaBr.MAD) */

	Device(COPR) {
		Name(_HID,EISAID("PNP0C04"))	/* Math Coprocessor */
		Name(_CRS, ResourceTemplate() {
			IO(Decode16, 0x00F0, 0x00F0, 0, 0x10)
			IRQNoFlags(){13}
		})
	} /* End Device(_SB.PCI0.LpcIsaBr.COPR) */
	#include "acpi/superio.asl"
} /* end LIBR */
