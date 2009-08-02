/*  Copyright (C) 2006 thoduv

    This file is part of DeSmuME

    DeSmuME is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    DeSmuME is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DeSmuME; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __DSCARD_H__
#define __DSCARD_H__

enum ECardMode
{
	CardMode_Normal = 0,
	CardMode_KEY1,
	CardMode_KEY2,
};

typedef struct
{
	
	u8 command[8];

	u32 address;
	u32 transfer_count;

	ECardMode mode;
	
} nds_dscard;

#endif /*__DSCARD_H__*/



