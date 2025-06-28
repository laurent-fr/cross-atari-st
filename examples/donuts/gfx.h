#ifndef GFX_H
#define GFX_H


long gemdos_necin();
long *xbios_physbase();
long *xbios_logbase();
void xbios_vsync();
void xbios_setpalette(__uint16_t *paletteptr);

void show_donuts(long *src,long *sprites,long scroll_x, long scroll_y );

#endif
