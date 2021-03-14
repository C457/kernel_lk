#include <daudio_test.h>
#include <stdlib.h>
#include <string.h>
#include <sdmmc/emmc.h>

#ifndef CRC_CHECK_EMMC
#define CRC_CHECK_EMMC_DEBUG
#endif

#ifdef CRC_CHECK_EMMC
#define BYTE_TO_SECTOR(X)			((X + 511) >> 9)
static unsigned int crc_sum;

static unsigned int crc32(unsigned int prev_crc, unsigned char const *p, unsigned int len)
{
	int i;
	unsigned int crc = prev_crc;

	while (len--) {
		crc ^= *p++;
		for (i = 0; i < 8; i++)
			crc = (crc >> 1) ^ ((crc & 1) ? 0xedb88320 : 0);
	}
	return crc;
}

static void crc_check_emmc(void)
{
	unsigned long long emmc_size = tcc_get_storage_capacity();
	unsigned page_size = flash_page_size();
	unsigned long long data_len = (emmc_size * page_size);
	unsigned long long data_addr = 0;
	unsigned char buffer[page_size];
	int ret;
#ifdef CRC_CHECK_EMMC_DEBUG
	int i;
#endif

	crc_sum = 0;
	printf("===========================================\n");
	printf(" CRC cheking!!! Please wait!!!\n");
	printf(" eMMC size = %lld bytes (%lld sectors)\n", data_len, emmc_size);

	while(data_len) {

		memset(buffer, 0, page_size);
		ret = tcc_read(data_addr, buffer, page_size);

		if (ret != 0) {
			printf(" eMMC read error!!!\n");
			printf("===========================================\n");
			data_len = 0;
			return;
		}

		data_len = data_len - page_size;
		data_addr++;

		crc_sum = crc32(crc_sum, buffer, page_size);

		if (data_addr % 100000 == 0)
			printf("... %lld sectors done \n", data_addr);

#ifdef CRC_CHECK_EMMC_DEBUG
		for(i = 0; i < page_size; i++) {
			if((i % 16) == 0) {
				printf("\n");
				printf("addr 0x%x : ", i);
			}
#ifdef CRC_CHECK_EMMC_DEBUG
			if(buffer[i] < 0x10)
				printf("0%x ", buffer[i]);
			else
#endif
				printf("%x ", buffer[i]);
		}

#endif

	}
	printf("\n");
	printf(" CRC  = 0x%x \n", crc_sum);
	printf(" CRC cheking Done!!! \n");
	printf("===========================================\n");
#ifdef CRC_CHECK_EMMC_DEBUG
	while(1) {
	}
#endif
}
#endif

// KCH+
// 2015.07.13 DDR Memory test
char *gets(char *s)
{
	int i = 0;
	char c;
	int ret;

	do {
		if(getc(&c) == 0) {
			if(c == '\b') {
				if(i > 0) {
					--i;
					puts("\b \b");
				}
			}
			else {
				if(i < 32) {
					putc(c);
					s[i++] = c;
				}
			}
		}
	}
	while(c != '\n' && c != '\r');
	s[i] = 0;
	putc('\n');
	puts(s);
	puts("\n\n");

	return s;
}

int hexchartoi(const char ch)
{
	if((ch >= '0') && (ch <= '9')) {
		return (ch - '0');
	}
	else if((ch >= 'a') && (ch <= 'f')) {
		return (ch - 'a' + 10);
	}
	else if((ch >= 'A') && (ch <= 'F')) {
		return (ch - 'A' + 10);
	}
	else
		return 0;
}

long strtol(const char* nptr, const char** endptr, int base)
{
	long value = 0;
	int neg = 0;
	int x = 0;
	static char safechar;
	const char *safeptr = &safechar;

	if(endptr == 0) {
		endptr = &safeptr;
	}
	*endptr = nptr;

	while(isspace(**endptr)) {
		*endptr += sizeof(char);
	}

	if(**endptr == '+') {
		*endptr += sizeof(char);
	}

	switch(base) {
		case 0:
			if(**endptr == '0') {
				*endptr += sizeof(char);
				if(**endptr == 'x')
					return strtol(nptr, endptr, 16);
				if(isdigit(**endptr))
					return strtol(nptr, endptr, 8);
				return 0;
			}
			else {
				if(isdigit(**endptr))
					return strtol(nptr, endptr, 10);
				return 0;
			}
			break;

		case 8:
			while((**endptr >= '0') && (**endptr <= '7')) {
				value *= base;
				value += **endptr - '0';
				*endptr += sizeof(char);
			}
			break;

		case 10:
			while(isdigit(**endptr)) {
				value *= base;
				value += **endptr - '0';
				endptr += sizeof(char);
			}
			break;

		case 16:
			while(isxdigit(**endptr) || ((**endptr == 'x') && (x ^= 1))) {
				value = value << 4;
				value += hexchartoi(**endptr);
				*endptr += sizeof(char);
			}
			break;
	}

	if(neg)
		value = 0 - value;

	return value;
}

#define DDR_START_ADDR	0x80000000
void run_ddr_test()
{
	unsigned int StartAddr = DDR_START_ADDR;
	unsigned int EndAddr = 0xBFFFFFFF;
	int exit = 0;

	unsigned char str[32] = {0};

	while(!exit) {
		dprintf(INFO, "\n");
		dprintf(INFO, " DDR memory test\n");
		dprintf(INFO, "\n");
		dprintf(INFO, " Test Start Address = 0x%X\n", StartAddr);
		dprintf(INFO, " Test End Address = 0x%X  (If ddr is 2gb, enter 0xFFFFFFFF)\n", EndAddr);

		dprintf(INFO, "\n");
		dprintf(INFO, " (0) Start Test\n");

		dprintf(INFO, " (1) DDR Test Start Address Setting\n");
		dprintf(INFO, " (2) DDR Test End Address Setting\n");

		dprintf(INFO, " (3) Back\n");
		dprintf(INFO, "\n");

		while(1) {
			if(getc(&str[0]) == 0) {
				if((str[0] >= '0') && (str[0] <= '3'))
					break;
			}
		}

		switch(str[0]) {
			case '0':
				ddr_mem_test(StartAddr, EndAddr);
				break;

			case '1':
				while(1) {
					dprintf(INFO, " Enter new Start address : ");
					gets(str);
					StartAddr = (unsigned int)strtol(str, NULL, 16);
					if(StartAddr >= DDR_START_ADDR)
						break;
				}
				break;

			case '2':
				while(1) {
					dprintf(INFO, " Enter new End address : ");
					gets(str);
					EndAddr = (unsigned int)strtol(str, NULL, 16);
					if(EndAddr > DDR_START_ADDR)
						break;
				}
				break;

			case '3':
				exit = 1;
				break;
		}
	}
}
// KCH-

void daudio_test_start(void)
{
	int ret = 0;
	int ltc3676_test = 0;
	int is_test_mode = 1;

	dprintf(INFO, "===========================================\n");
	dprintf(INFO, "Select test mode\n");
#ifdef CRC_CHECK_EMMC
	dprintf(INFO, "(1) eMMC CRC Check\n");
#endif
	dprintf(INFO, "(2) DDR Memory Test\n");
	dprintf(INFO, "(3) DDR Over clock\n");
	dprintf(INFO, "===========================================\n");

	while (is_test_mode) {
		char input_val = 0;
		getc(&input_val);

		switch (input_val) {
#ifdef CRC_CHECK_EMMC
			case '1':
				crc_check_emmc();
				is_test_mode = 0;
				break;
#endif
			case '2':
				run_ddr_test();
				is_test_mode = 0;
				break;
			case '3':
				dram_overclock_test(0);
				is_test_mode = 0;
				break;
		}
	}
}
