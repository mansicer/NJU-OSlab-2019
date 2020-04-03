#include "types.h"
#include "utils.h"
#include "lib.h"


union DirEntry {
	uint8_t byte[128];
	struct {
		int32_t inode;
		char name[64];
	};
};

typedef union DirEntry DirEntry;

int ls(char *destFilePath) {
	// STEP 8
    // TODO: ls
    /** output format
    ls /
    boot dev usr
    */
	printf("ls %s\n", destFilePath);
	char res[1024];
	do_ls(destFilePath, res);
	printf("%s\n", res);
	return 0;
}

int cat(char *destFilePath) {
	// STEP 9
    // TODO: ls
    /** output format
    cat /usr/test
    ABCDEFGHIJKLMNOPQRSTUVWXYZ
    */
    printf("cat %s\n", destFilePath);
	char buf[1024];
	int fd = open(destFilePath, O_READ);
	int ret = read(fd, (uint8_t *)buf, 1024);
	while (ret != -1)
	{
		printf("%s", buf);
		ret = read(fd, (uint8_t *)buf, 1024);
	}
	printf("\n");
	close(fd);
	return 0;
}

int uEntry(void) {
    // STEP 2-9
    // TODO: try different test case when you finish a function
    
    /* filesystem test */
	int fd = 0;
	int i = 0;
	char tmp = 0;
	
	char root[] = "/";
	ls("/");
	ls("/boot/");
	ls("/dev/");
	ls("/usr/");

	printf("create /usr/test and write alphabets to it\n");
	fd = open("/usr/test", O_WRITE | O_READ | O_CREATE);
	// printf("create res: %d\n", fd);
	// fd = open("/usr/test2", O_WRITE | O_READ | O_CREATE);
	// printf("create res: %d\n", fd);

	//write(fd, (uint8_t*)"Hello, World!\n", 14);
	//write(fd, (uint8_t*)"This is a demo!\n", 16);
	//for (i = 0; i < 2049; i ++) {
	//for (i = 0; i < 2048; i ++) {
	//for (i = 0; i < 1025; i ++) {
	//for (i = 0; i < 512; i ++) {
	for (i = 0; i < 26; i ++) {
		tmp = (char)(i % 26 + 'A');
		write(fd, (uint8_t*)&tmp, 1);
	}
	close(fd);
	ls("/usr/");
	cat("/usr/test");
	printf("\n");
	printf("rm /usr/test\n");
	remove("/usr/test");
	ls("/usr/");
	printf("rmdir /usr/\n");
	remove("/usr/");
	//remove("/dev");
	ls(root);
	//ls("/dev");
	printf("create /usr/\n");
	fd = open("/usr/", O_CREATE | O_DIRECTORY);
	close(fd);
	ls(root);
	//fd = open("/usr/test", O_WRITE | O_READ);
	//close(fd);
	//ls("/usr");
	//fd = open("/usr/test/", O_CREATE);
	//close(fd);
	//ls("/usr/");
	//fd = open("/usr/test", O_CREATE);
	//close(fd);
	//ls("/usr/");
	
	exit();
    /**
    Output format can be found in 
    http://114.212.80.195:8170/2019oslab/lab5/
    */
	return 0;
}
