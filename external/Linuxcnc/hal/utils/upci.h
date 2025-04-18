#ifndef UPCI_H
#define UPCI_H

/*************************************************************************
*
* upci - a library for user space access to PCI bus devices
*
* This library contains functions to scan the PCI bus, find a particular
* PCI card, and read or write memory and I/O locations on a PCI card.
*
* The scanning functionality is similar to lspci, but the search and
* access functions are unique.  The access functions require the calling
* program to be running as root or setuid root.  The scan and search
* functions do not.
*
* Copyright (C) 2007 John Kasunich (jmkasunich at fastmail dot fm)
*
*
**************************************************************************

This program is free software; you can redistribute it and/or
modify it under the terms of version 2 of the GNU General
Public License as published by the Free Software Foundation.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

THE AUTHORS OF THIS LIBRARY ACCEPT ABSOLUTELY NO LIABILITY FOR
ANY HARM OR LOSS RESULTING FROM ITS USE.  IT IS _EXTREMELY_ UNWISE
TO RELY ON SOFTWARE ALONE FOR SAFETY.  Any machinery capable of
harming persons must have provisions for completely removing power
from all motors, etc, before persons enter any danger area.  All
machinery must be designed to comply with local and national safety
codes, and the authors of this software can not, and do not, take
any responsibility for such compliance.

This code was written as part of the EMC HAL project.  For more
information, go to www.linuxcnc.org.

************************************************************************/

#include <linux/types.h>

enum upci_region_type { UPCI_REG_MEM, UPCI_REG_IO };

/* this struct is used to pass around info about a PCI device */
struct upci_dev_info {
    __u16 vendor_id;
    __u16 device_id;
    __u16 ss_vendor_id;
    __u16 ss_device_id;
    __u16 instance;
    __u16 bus;
    __u8 dev;
    __u8 func;
    int irq;
    enum upci_region_type region_type[6];
    __u32 base_addr[6];
    __u32 size[6];
    __u32 rom_base_addr;
    __u32 rom_size;
};

/* 'upci_reset' closes all open regions and clears the device list.  It
   can be used to force a rescan, or to prepare for program exit.
*/
void upci_reset(void);

/* 'upci_scan_bus' scans the PCI bus and builds an internal list of the
   devices it finds.  This list is used for all subsequent operations.
   Returns the number of devices found, or -1 on error.
*/
int upci_scan_bus(void);

/* 'upci_print_device_info' prints some data about a specific device,
   from the list generated by 'upci_scan_bus'.  Device numbers are
   assigned sequentially from zero based on the order in which the
   devices were detected by 'upci_scan_bus'.  If 'devnum' is outside
   the range of devices found by 'upci_scan_bus', returns -1, else
   returns 0.
*/
int upci_print_device_info(int devnum);

/* 'upci_get_device_info' fills in a user supplied upci_dev_info structure
   with information about a specific device.  Device numbers are assigned
   sequentially from zero based on the order in which the devices were
   detected by 'upci_scan_bus'.  If 'devnum' is outside the range of
   devices found by 'upci_scan_bus', returns -1, else returns 0.
*/
int upci_get_device_info(struct upci_dev_info *p, int devnum);

/* 'upci_find_device' searches the device list for a device that matches
   the vendor, device, subsystem vendor, subsystem device, and instance
   number fields of a caller supplied upci_dev_info structure, and returns
   a device number (or -1, if no match is found).
*/
int upci_find_device(struct upci_dev_info *p);

/* 'upci_open_region' prepares an I/O or memory region belonging to a
   specific device for access.  It requires root privliges.  Returns a
   region descriptor, a small non-negative integer, that refers to the
   region and is used in subsequent calls to access or close the region.
   On error, returns -1.
*/
int upci_open_region(int devnum, int region_num);

/* 'upci_close_region' closes a region previously opened for access by
   'upci_open_region'.  Subsequent attempts to access the region will
   fail.
*/
void upci_close_region(int region_descriptor);

/* 'upci_read_xx' reads data from a previously opened region on a PCI
   card.  It takes a region descriptor and an offset, and returns the
   data.  'xx' can be u8, s8, u16, s16, u32, s32.  The region can be
   either I/O or memory mapped.  It fails silently and returns zero
   if the region is not open, or if offset is greater than the size
   of the region.
*/
__u8 upci_read_u8(int region_descriptor, __u32 offset);
__s8 upci_read_s8(int region_descriptor, __u32 offset);
__u16 upci_read_u16(int region_descriptor, __u32 offset);
__s16 upci_read_s16(int region_descriptor, __u32 offset);
__u32 upci_read_u32(int region_descriptor, __u32 offset);
__s32 upci_read_s32(int region_descriptor, __u32 offset);

/* 'upci_write_xx' writes data to a previously opened region on a PCI
   card.  It takes a region descriptor, an offset, and the data.
   'xx' can be u8, s8, u16, s16, u32, s32.  The region can be
   either I/O or memory mapped.  It fails silently if the region
   is not open.
*/
void upci_write_u8(int region_descriptor, __u32 offset, __u8 data);
void upci_write_s8(int region_descriptor, __u32 offset, __s8 data);
void upci_write_u16(int region_descriptor, __u32 offset, __u16 data);
void upci_write_s16(int region_descriptor, __u32 offset, __s16 data);
void upci_write_u32(int region_descriptor, __u32 offset, __u32 data);
void upci_write_s32(int region_descriptor, __u32 offset, __s32 data);

#endif /* UPCI_H */
