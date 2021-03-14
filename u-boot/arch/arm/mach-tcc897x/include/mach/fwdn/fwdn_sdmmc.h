int fwdn_mmc_header_write(
		unsigned long base_addr,
		unsigned long config_size,
		unsigned long rom_size,
		unsigned int boot_partition,
		int fwdn_mode);

int fwdn_mmc_config_write( unsigned int rom_size,  // ks_8930
		unsigned int config_size, unsigned char *addr, int boot_partition);

int fwdn_mmc_rom_write (
		unsigned int rom_size ,
		unsigned int config_code_size ,
		unsigned char *buf ,
		/*unsigned int buf_size ,*/
		int boot_partition );

int fwdn_mmc_update_bootloader(unsigned int rom_size, int boot_partition);

#if defined(TSBM_ENABLE)
int fwdn_mmc_update_secure_bootloader(unsigned int rom_size, int boot_partition);
#endif

#ifdef OTP_UID_INCLUDE
int fwdn_mmc_otp_uid_include_update_bootloader(unsigned int rom_size, int boot_partition);
#endif

unsigned int FwdnReadBootSDFirmware(unsigned int master);

int fwdn_mmc_get_serial_num(void);

int FwdnSetBootSDSerial(unsigned char *ucData, unsigned int overwrite);

