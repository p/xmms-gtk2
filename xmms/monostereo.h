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
#ifndef MONOSTEREO_H
#define MONOSTEREO_H

typedef struct
{
	Widget ms_widget;
	int ms_num_channels;
	SkinIndex ms_skin_index;
}
MonoStereo;

MonoStereo *create_monostereo(GList ** wlist, GdkPixmap * parent, GdkGC * gc, gint x, gint y, SkinIndex si);
void monostereo_set_num_channels(MonoStereo * ms, gint nch);

#endif
