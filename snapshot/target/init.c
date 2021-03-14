//+[TCCQB] QuickBoot Command Line

#define SNAPSHOT_PARTITION "/dev/block/platform/bdm/by-name/snapshot"

extern unsigned boot_into_qb_prebuilt;

void target_cmdline_snapshot(char *cmdline)
{
	char s[128];

	sprintf(s, " resume=%s", SNAPSHOT_PARTITION);

	strcat(cmdline, s);
}

void target_cmdline_qb_prebuilt_menu(char *cmdline)
{
	strcat(cmdline, " androidboot.qb_prebuilt_mode=");
	strcat(cmdline, boot_into_qb_prebuilt ? "enable" : "disable");
}

//-[TCCQB]
//
