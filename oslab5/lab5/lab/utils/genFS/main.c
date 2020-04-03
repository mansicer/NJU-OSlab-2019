#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "data.h"
#include "func.h"

/*
int main(int argc, char *argv[]) {
	char driver[NAME_LENGTH];
	char rootDirPath[NAME_LENGTH];
	char srcFilePath[NAME_LENGTH];
	char destDirPath[NAME_LENGTH];
	char destFilePath[NAME_LENGTH];

	stringCpy("fs.bin", driver, NAME_LENGTH - 1);
	stringCpy("/", rootDirPath, NAME_LENGTH - 1);
	stringCpy("./test", srcFilePath, NAME_LENGTH - 1);
	stringCpy("/doc", destDirPath, NAME_LENGTH - 1);
	stringCpy("/doc/test", destFilePath, NAME_LENGTH - 1);
	
	format(driver, SECTOR_NUM, SECTORS_PER_BLOCK);
	mkdir(driver, destDirPath);
	ls(driver, rootDirPath);
	cp(driver, srcFilePath, destFilePath);
	ls(driver, destDirPath);
	ls(driver, destFilePath);
	//cat(driver, destFilePath);
	rm(driver, destFilePath);
	ls(driver, destDirPath);
	ls(driver, rootDirPath);
	rmdir(driver, destDirPath);
	ls(driver, rootDirPath);

	return 0;
}
*/

int main(int argc, char *argv[]) {
	char driver[NAME_LENGTH];

	char rootDirPath[NAME_LENGTH];
	char srcFilePath[NAME_LENGTH];
	char destDirPath[NAME_LENGTH];
	char destFilePath[NAME_LENGTH];

	stringCpy("fs.bin", driver, NAME_LENGTH - 1);
	stringCpy("/", rootDirPath, NAME_LENGTH - 1);
	stringCpy("./uMain.elf", srcFilePath, NAME_LENGTH - 1);
	stringCpy("/boot/", destDirPath, NAME_LENGTH - 1);
	stringCpy("/boot/initrd", destFilePath, NAME_LENGTH - 1);
	
	
    // STEP 1
    // TODO: build file system of os.img, see lab5 4.3.
    // All functions you need have been completed
	
	format(driver, SECTOR_NUM, SECTORS_PER_BLOCK);

	mkdir(driver, destDirPath);
	cp(driver, srcFilePath, destFilePath);
	stringCpy("/usr/", destDirPath, NAME_LENGTH - 1);
	mkdir(driver, destDirPath);
	stringCpy("/dev/", destDirPath, NAME_LENGTH - 1);
	mkdir(driver, destDirPath);
	stringCpy("/dev/stdin", destFilePath, NAME_LENGTH - 1);
	touch(driver, destFilePath);
	stringCpy("/dev/stdout", destFilePath, NAME_LENGTH - 1);
	touch(driver, destFilePath);


    ls(driver, "/");
    ls(driver, "/boot/");
    ls(driver, "/dev/");
    ls(driver, "/usr/");
    
    /** output:
    ls /
    Name: boot, Type: 2, LinkCount: 1, BlockCount: 1, Size: 1024.
    Name: dev, Type: 2, LinkCount: 1, BlockCount: 1, Size: 1024.
    Name: usr, Type: 2, LinkCount: 1, BlockCount: 0, Size: 0.
    LS success.
    8185 inodes and 3052 data blocks available.
    ls /boot/
    Name: initrd, Type: 1, LinkCount: 1, BlockCount: 14, Size: 13400.
    LS success.
    8185 inodes and 3052 data blocks available.
    ls /dev/
    Name: stdin, Type: 1, LinkCount: 1, BlockCount: 0, Size: 0.
    Name: stdout, Type: 1, LinkCount: 1, BlockCount: 0, Size: 0.
    LS success.
    8185 inodes and 3052 data blocks available.
    ls /usr/
    LS success.
    8185 inodes and 3052 data blocks available.
	*/
    
	return 0;
}
