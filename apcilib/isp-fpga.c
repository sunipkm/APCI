#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include <dirent.h>

#include "apcilib.h"

#define BAR_REGISTER 2
#define ofsFPGARevision 0x68
#define ofsFlashAddress 0x70
#define ofsFlashData 0x74
#define ofsFlashErase 0x78
#define bmFlashWriteBit 0x80000000

int apci;
uint32_t EraseSentinel = 0x494f0000;	   // | DeviceID
uint32_t EraseSectorSentinel = 0x0ACCE500; // | iSector
uint32_t FileBytesRead = 0;
uint8_t FlashData[0x80000];

void ReadFile(char *FileName, uint8_t FlashData[])
{
	int fd = open(FileName, O_RDONLY);
	if (fd < 0)
	{
		printf("Could not open file %s\n", FileName);
		exit(1);
	}

	FileBytesRead = read(fd, FlashData, 0x80000);
	if (FileBytesRead < 0 || FileBytesRead > 0x80000)
	{
		printf("Could not read file %s\n", FileName);
		exit(1);
	}
	printf("Read %d bytes from file %s\n", FileBytesRead, FileName);
}

void EraseFlash()
{
	for (int iSector = 0; iSector < 8; iSector++)
	{
		apci_write32(apci, 1, BAR_REGISTER, ofsFlashErase, EraseSentinel);
		apci_write32(apci, 1, BAR_REGISTER, ofsFlashErase, EraseSectorSentinel | iSector);
		printf("Erasing Flash Sector %d/8 via %08X\n", iSector + 1, EraseSectorSentinel | iSector);
		sleep(2);
	}
}

void PrimitiveWriteFlashByte(uint32_t offset, uint8_t data)
{
	apci_write32(apci, 1, BAR_REGISTER, ofsFlashData, data);
	apci_write32(apci, 1, BAR_REGISTER, ofsFlashAddress, offset | bmFlashWriteBit);
	usleep(25);
}

uint8_t PrimitiveReadFlashByte(uint32_t offset)
{
	uint32_t data;
	apci_write32(apci, 1, BAR_REGISTER, ofsFlashAddress, offset & ~bmFlashWriteBit);
	usleep(25);
	apci_read32(apci, 1, BAR_REGISTER, ofsFlashData, &data);
	return data & 0x000000FF;
}

void WriteFlash()
{
	printf("Writing %d bytes to Flash\n", FileBytesRead);
	for (int iByte = 0; iByte < FileBytesRead; iByte++)
	{
		PrimitiveWriteFlashByte(iByte, FlashData[iByte]);
		if (iByte % (FileBytesRead / 8) == 0)
		{
			printf("wrote %d of %d to flash\n", iByte, FileBytesRead);
		}
	}
	printf("Flash write complete\n");
}

int VerifyFlash()
{
	printf("Verifying %d bytes from Flash\n", FileBytesRead);
	int Result = 1;
	for (int iByte = 0; iByte < FileBytesRead; iByte++)
	{
		uint8_t data = PrimitiveReadFlashByte(iByte);
		if (data != FlashData[iByte])
		{
			printf("Verify failed at byte %8d; Got %02X, expected %02X\n", iByte, data, FlashData[iByte]);
			Result = 0;
			break;
		}

		if (iByte % (FileBytesRead / 8) == 0)
		{
			printf("Verified %d of %d to flash\n", iByte, FileBytesRead);
		}
	}
	if (Result)
		printf("\nVerification of flash contents succeeded.\n");
	else
		printf("\nVerification of flash contents failed.\nRETRYING\n");
	return Result;
}

int VerifyErase()
{
	int Result = 1;
	for (int iByte = 0; iByte < 0x80000; iByte++)
	{
		uint8_t data = PrimitiveReadFlashByte(iByte);
		if (data != 0xFF)
		{
			printf("Erase_Verify failed at byte %8d; Got %02X, expected %02X\n", iByte, data, FlashData[iByte]);
			Result = 0;
			break;
		}

		if (iByte % (FileBytesRead / 8) == 0)
		{
			printf("Verified erasure of %d/%d of the flash\n", iByte, FileBytesRead);
		}
	}
	if (Result)
		printf("\nVerification of flash erasure succeeded.\n");
	else
		printf("\nVerification of flash erasure failed.\n");
	return Result;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("\nUsage: %s <filename.rpd>\nFlashes the first device in /dev/apci.\n\n", argv[0]);
		exit(1);
	}
	char *devicepath = "/dev/apci"; // default device path

	apci = -1; // initialize as unopened

	DIR *parentdir = NULL;
	struct dirent *dir = NULL;
	parentdir = opendir(devicepath); // open directory
	if (parentdir) // valid directory
	{
		int count = 0; // subfile counts
		while ((dir = readdir(parentdir)) != NULL) // parse subfiles/dirs
		{
			if (count++ < 2)
			{
				continue; // ignore . and ..
			}
			char fullpath[1024];
			int wsz = snprintf(fullpath, sizeof(fullpath), "%s/%s", devicepath, dir->d_name); // construct full path of device
			if ((wsz >= sizeof(fullpath)) || (wsz < 0))										  // error in snprintf
			{
				printf("Could not construct full path [%s/%s]\n", devicepath, dir->d_name);
				continue;
			}
			printf("Trying to open %s ...\n", fullpath);
			apci = open(fullpath, O_RDONLY); // open the device
			if (apci > 0) // success
			{
				printf("Opened device %s .\n", fullpath);
				break;
			}
			else // failure
			{
				printf("Couldn't open device file on command line: do you need sudo? Check /dev/apci? [%s]\n", fullpath);
				break;
			}
		}
		closedir(parentdir);
	}
	else // could not find /dev/apci, is the module loaded?
	{
		printf("Could not open directory %s, is the APCI module loaded?\n\n", devicepath);
		exit(0);
	}

	if (apci < 0) // if could not get a valid handle exit program
	{
		printf("Could not open any APCI device. Exiting.\n");
		exit(0);
	}

	uint32_t Version = 0;
	apci_read32(apci, 1, BAR_REGISTER, ofsFPGARevision, &Version);

	printf("\nACCES ISP-FPGA Engineering Utility for Linux [current FPGA Rev %08X]\n", Version);

// #define DEBUG 1

#ifdef DEBUG
	close(apci);
	exit(0);
#endif

	ReadFile(argv[1], FlashData);

	/* query DeviceID for use in EraseSentinel */
	unsigned int DeviceID = 0;
	unsigned long bar[6];
	apci_get_device_info(apci, 1, &DeviceID, bar);

	/* Verify the DeviceID is on approved list for this code */
	//	if (DeviceID != 0xC2EC)
	//	{
	//		printf("\nDeviceID %08X is not supported by this utility.\n", DeviceID);
	//		exit(1);
	//	}

	EraseSentinel |= DeviceID;
	printf("EraseSentinel is %08X\n", EraseSentinel);
	do
	{
		EraseFlash();
		if (!VerifyErase())
			continue;

		/* write data to flash */
		WriteFlash();

		/* verify data read from flash */
	} while (!VerifyFlash());

	printf("Flash Update Successful.  Wrote %s to Device %04x.\n\n", argv[2], DeviceID);
	printf("\n----\nyou must COLD REBOOT to load the new FPGA from Flash!!----\n\n");

	// close(apci); // release handle, not necessary
}
