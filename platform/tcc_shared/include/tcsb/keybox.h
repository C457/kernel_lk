/* ###############################################################################
 * Secure Key Store : keybox.h
 * Copyright (C) 2012 - 2015 Telechips, Inc.
 * ############################################################################### */
#define MAX_DRM			5

struct secure_key_box {
	char key_tag[8];
	unsigned int total_size;
	char Reserved0 [4];
	unsigned int drm_flags[MAX_DRM];
	unsigned int drm_size[MAX_DRM];
	unsigned int drm_offset[MAX_DRM];
	unsigned int Reserved1;
	unsigned int dtcpipsrm_offset;		// Do not used!!
	unsigned int dtcpipsrm_size;		// Do not used!!
	unsigned int hdcpsrm_offset;		// Do not used!!
	unsigned int hdcpsrm_size;		// Do not used!!
} SECURE_KEY_STORE_T;
