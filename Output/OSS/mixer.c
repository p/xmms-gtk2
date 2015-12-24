/*  XMMS - Cross-platform multimedia player
 *  Copyright (C) 1998-2000  Peter Alm, Mikael Alm, Olle Hallnas, Thomas Nilsson and 4Front Technologies
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "OSS.h"
#include <errno.h>

static char* get_mixer_device(void)
{
	char *name;
	
	if (oss_cfg.use_alt_mixer_device && oss_cfg.alt_mixer_device)
		name = g_strdup(oss_cfg.alt_mixer_device);
	else if (oss_cfg.mixer_device > 0)
		name = g_strdup_printf("%s%d", DEV_MIXER, oss_cfg.mixer_device);
	else
		name = g_strdup(DEV_MIXER);

	return name;
}

void oss_get_volume(int *l, int *r)
{
	int fd, v, devs, dspfd;
	long cmd;
	gchar *devname;

	dspfd = oss_get_fd();
	if (oss_cfg.use_master == 0 && dspfd != -1) {
		fd = dspfd;
		dspfd = 1;
	} else {
		devname = get_mixer_device();
		fd = open(devname, O_RDONLY);
		g_free(devname);
		dspfd = 0;
	}

	/*
	 * We dont show any errors if this fails, as this is called
	 * rather often
	 */
	if (fd != -1)
	{
		ioctl(fd, SOUND_MIXER_READ_DEVMASK, &devs);
		if ((devs & SOUND_MASK_PCM) && (oss_cfg.use_master==0))
			cmd = (dspfd != 0) ? SNDCTL_DSP_GETPLAYVOL :
			    SOUND_MIXER_READ_PCM;
		else if ((devs & SOUND_MASK_VOLUME) && (oss_cfg.use_master==1))
			cmd = SOUND_MIXER_READ_VOLUME;
		else
		{
			if (dspfd == 0)
				close(fd);
			return;
		}
		ioctl(fd, cmd, &v);
		*r = (v & 0xFF00) >> 8;
		*l = (v & 0x00FF);
		if (dspfd == 0)
			close(fd);
	}
}

void oss_set_volume(int l, int r)
{
	int fd, v, devs, dspfd;
	long cmd;
	gchar *devname;

	dspfd = oss_get_fd();
	if (oss_cfg.use_master == 0 && dspfd != -1) {
		fd = dspfd;
		dspfd = 1;
		devname = g_strdup("<OSS FD>");
	} else {
		devname = get_mixer_device();
		fd = open(devname, O_RDONLY);
		dspfd = 0;
	}

	if (fd != -1)
	{
		ioctl(fd, SOUND_MIXER_READ_DEVMASK, &devs);
		if ((devs & SOUND_MASK_PCM) && (oss_cfg.use_master==0))
			cmd = SOUND_MIXER_WRITE_PCM;
		else if ((devs & SOUND_MASK_VOLUME) && (oss_cfg.use_master==1))
			cmd = SOUND_MIXER_WRITE_VOLUME;
		else
		{
			if (dspfd == 0)
				close(fd);
			return;
		}
		v = (r << 8) | l;
		ioctl(fd, cmd, &v);
		if (dspfd == 0)
			close(fd);
	}
	else
		g_warning("oss_set_volume(): Failed to open mixer device (%s): %s",
			  devname, strerror(errno));
	g_free(devname);
}
