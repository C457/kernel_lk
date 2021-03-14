#include <daudio_settings.h>
#include <stdio.h>
#include <string.h>

#include <dev/flash.h>
#include <sdmmc/emmc.h>
#include <sys/types.h>
#include <partition_parser.h>

#define TAG_SETTINGS		"[daudio settings]"
#define BYTE_TO_SECTOR(X)	((X + 511) >> 9)

#define DEBUG_SETTINGS		1

#if (DEBUG_SETTINGS)
#define LKPRINT(fmt, args...) dprintf(INFO, TAG_SETTINGS " " fmt, ##args)
#else
#define LKPRINT(fmt, args...) do {} while(0)
#endif

/**
 * Read Engineer mode settings from settings partition.
 * @param	info Pointer to receive the result.
 * @return	1 - success, 0 - read fail.
 */
int read_em_setting(struct em_setting_info *info)
{
	char *ptn_name = "settings";
	int ret = 0;
	unsigned long long ptn = 0;
	unsigned int ptn_index;
	const unsigned int page_size = flash_page_size();
	const unsigned int size = sizeof(struct em_setting_info);
	unsigned char data[page_size];

	ptn_index = partition_get_index(ptn_name);
	ptn = partition_get_offset(ptn_index);
	if (ptn <= 0) {
		dprintf(CRITICAL, "%s %s Not found settings partition\n", TAG_SETTINGS, __func__);
		return ret;
	}

	ret = tcc_read(ptn + OFFSET_EM_SETTINGS, (unsigned int *)data, page_size);
	if (ret == 0) {
		memcpy(info, data, size);
		ret = 1;
	}

	return ret;
}

int read_ie_setting(struct ie_setting_info *info)
{
	char *ptn_name = "settings";
	int ret = 0;
	unsigned long long ptn = 0;
	unsigned int ptn_index;
	const unsigned int page_size = flash_page_size();
	const unsigned int size = sizeof(struct ie_setting_info);
	unsigned char data[page_size];

	memset(data, 0, sizeof(struct ie_setting_info));

	ptn_index = partition_get_index(ptn_name);
	ptn = partition_get_offset(ptn_index);
	if (ptn <= 0) {
		dprintf(CRITICAL, "%s %s Not found settings partition\n", TAG_SETTINGS, __func__);
		return ret;
	}

	ret = tcc_read(ptn + OFFSET_IE_SETTINGS, (unsigned int *)data, page_size);

	if (ret == 0) {
		memcpy(info, data, size);
		ret = 1;
	}

	return ret;
}

int write_ie_setting(struct ie_setting_info *info)
{
	char *ptn_name = "settings";
	int ret = 0;
	unsigned long long ptn = 0;
	unsigned int ptn_index;
	const unsigned int page_size = flash_page_size();
	const unsigned int size = sizeof(struct ie_setting_info);
	unsigned char data[page_size];

	if (info == NULL) {
		dprintf(CRITICAL, "%s info is NULL!\n", __func__);
		return ret;
	}

	memcpy(data, info, size);

	ptn_index = partition_get_index(ptn_name);
	ptn = partition_get_offset(ptn_index);

	if (ptn <= 0) {
		dprintf(CRITICAL, "%s %s Not found settings partition\n", TAG_SETTINGS, __func__);
		return ret;
	}

	if (tcc_write(ptn_name, ptn + OFFSET_IE_SETTINGS, page_size, data)) {
		dprintf(CRITICAL, "%s IE setting write fail !!\n", __func__);
		return ret;
	}

	return 1;
}

int read_variant_setting(VARIANT_CODE_INFO_t *variant_info)
{
	char *ptn_name = "settings";
	int ret = 0;
	unsigned long long ptn = 0;
	unsigned int ptn_index;
	const unsigned int page_size = flash_page_size();
	const unsigned int size = sizeof(VARIANT_CODE_INFO_t);
	unsigned char data[page_size];

	ptn_index = partition_get_index(ptn_name);
	ptn = partition_get_offset(ptn_index);
	if (ptn <= 0) {
		dprintf(CRITICAL, "%s %d Not found settings partition\n", __func__, __LINE__);
		return -1;
	}

	ret = tcc_read(ptn + OFFSET_VARIANT_SETTINGS, (unsigned int *)data, page_size);
	if (ret == 0) {
		memset(variant_info, 0x00, size);
		memcpy(variant_info, data, size);
	}
	else {
		return -2;
	}

	return 0;
}

int read_active_partition_setting(struct boot_partition *active_partition)
{
	char *ptn_name = "settings";
	int ret = 0;
	unsigned long long ptn = 0;
	unsigned int ptn_index;
	const unsigned int page_size = flash_page_size();
	const unsigned int size = sizeof(struct boot_partition);
	unsigned char data[page_size];

	ptn_index = partition_get_index(ptn_name);
	ptn = partition_get_offset(ptn_index);
	if (ptn <= 0) {
		dprintf(CRITICAL, "%s %d Not found settings partition\n", __func__, __LINE__);
		return ret;
	}

	ret = tcc_read(ptn + OFFSET_BOOT_ACTIVE_SETTINGS, data, page_size);
	if (ret == 0) {
		memcpy(active_partition, data, size);
		ret = 1;
	}

	return ret;
}
