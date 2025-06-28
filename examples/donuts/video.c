#include <stdio.h>
#include <sys/types.h>
#include "gfx.h"

#define FRAMES 30

__uint16_t palette[]={
	#include "palette.dat"
};

// p 284 
__uint16_t default_palette[]={
	0x777,0x700,0x070,0x770,0x007,0x707,0x077,0x555,
	0x333,0x733,0x373,0x773,0x337,0x737,0x377,0x000
};

__uint32_t sprite[128]={
	#include "sprite1.dat"
	,
	#include "sprite2.dat"
	,
	#include "sprite3.dat"
	,
	#include "sprite4.dat"
};

__uint32_t precalc_sprite[8][128];

#define Y0 10


int main() {
	long *screen = xbios_physbase();

	int rez = Getrez();

	Setscreen(screen,screen,ST_LOW);

	xbios_setpalette(palette);

	// pre-calculate horizontal scrolling
	
	for(int j=0;j<8;j++) {
		__uint16_t *precalc_dst = (__uint16_t *)&(precalc_sprite[j][0]);
		__uint16_t *sprite_src = (__uint16_t *)sprite;
		int x = j*2;
		for(int i=0;i<256;i++) {	
			*precalc_dst =  ( (*sprite_src)>>x) | ( (*sprite_src)<<(16-x));
			precalc_dst++;sprite_src ++;
		}
	}

	long scroll_x=0;
	long scroll_y=0;
	int frame=0;
	long *dst=screen+Y0*40;

	while (!Crawio(0xff)) // while not key pressed
	{

		show_donuts(dst,(long *)precalc_sprite,scroll_x>>1,scroll_y);

		if (frame<FRAMES) { scroll_y++;
		} else if (frame<2*FRAMES) { scroll_x++; scroll_y++;
		} else if (frame<3*FRAMES) { scroll_x++;
		} else if (frame<4*FRAMES) { scroll_x++; scroll_y--;
		} else if (frame<5*FRAMES) { scroll_y--;
		} else if (frame<6*FRAMES) { scroll_y--; scroll_x--;
		} else if (frame<7*FRAMES) { scroll_x--;
		} else if (frame<8*FRAMES) { scroll_x--; scroll_y++;
		}

		frame++;

		if (scroll_y<0) scroll_y=63;
		else if (scroll_y>=64) scroll_y=0;

		if (scroll_x<0) scroll_x=15;
		else if (scroll_x>=16) scroll_x=0;

		if (frame>=8*FRAMES) frame=0;

		xbios_vsync();
	}

	// restore video settings	
	xbios_setpalette(default_palette);
	Setscreen(screen,screen,rez);

	return 0;
}
