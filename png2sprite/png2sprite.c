#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <png.h>


struct color_s {
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

struct raw_image_s {
    int width;
    int height;
    int rowbytes;
    png_bytep *row_pointers;
    struct color_s palette[16];
} ;

int img_allocate(struct raw_image_s *img, int width, int height, int rowbytes) {

    img->width=width;
    img->height=height;
    img->rowbytes = rowbytes;

    img->row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    if (!img->row_pointers) return 1;

    for(int y = 0; y < height; y++) {
        img->row_pointers[y] = (png_byte*)malloc(rowbytes);
        if (!img->row_pointers[y]) return 1;
    }

    return 1;
}

void img_free(struct raw_image_s *img) {

    for(int y = 0; y < img->height; y++) {
        free(img->row_pointers[y]); 
    }

    free(img->row_pointers);
}


int load_png(char *filename, struct raw_image_s *img ) {

    FILE *fp=fopen(filename,"rb");
    if (!fp) {
        fprintf(stderr,"Cannot open %s\n",filename);
        return 0;
    }

    // check header
    unsigned char header[8];
	int bytesread = fread(header, 1, 8, fp);

	if ( (bytesread!=8) || (png_sig_cmp(header, 0, 8)) ) {
			fprintf(stderr,"Not a .png file !\n");
			fclose(fp);
			return 0;
	}
   
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr,(png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr,fp);
    
    png_set_sig_bytes(png_ptr, 8); // skip header
    png_read_info(png_ptr, info_ptr);

    int width      = png_get_image_width(png_ptr, info_ptr);
    int height     = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth  = png_get_bit_depth(png_ptr, info_ptr);

    if (width %16) {
        printf("Png width %d is not a 16 multiple\n", width);
        png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    if (color_type != PNG_COLOR_TYPE_PALETTE) {
        printf("This is not a palettized PNG\n");
        png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    if (bit_depth>8) {
        printf("Bit depth is %d, should be <=8\n",bit_depth);
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    int palette_count;
    png_colorp palette;

    png_get_PLTE(png_ptr,info_ptr,&palette,&palette_count);

    if (!img_allocate(img, width, height, png_get_rowbytes(png_ptr,info_ptr)))
        return 0;

    for(int i=1;i<palette_count;i ++) {
        if (i>16) break;

        png_color* col = &palette[i];
        img->palette[i-1].R=col->red;
        img->palette[i-1].G=col->green;
        img->palette[i-1].B=col->blue;
    }
    
   
    png_read_image(png_ptr, img->row_pointers);

    png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
    fclose(fp);

    return 1;

    
}

void export_sprite(struct raw_image_s *img,int x0,int y0,int w, int h) {

    int num_bytes = w*h/2;

    unsigned short *sprite = malloc( (size_t)num_bytes );
    if (!sprite) {
        fprintf(stderr,"Cannot allocate %d bytes\n", num_bytes);
        exit(-1);
    }

    unsigned short *spriteptr=sprite;

    for(int y=0;y<h;y++) {
        png_bytep row = img->row_pointers[y];
        
        for(int x=0;x<w;x++ ) {
              
            if  (!(x%16)) {
                *spriteptr=*(spriteptr+1)=*(spriteptr+2)=*(spriteptr+3)=0;
                if (x>x0) {
                    spriteptr+=4;
                }
            }

            png_bytep px = &row[x+x0]; 
            int col_index = (*px -1) & 0xf ;

            *spriteptr |= col_index&1;
            col_index >>= 1;
            *(spriteptr+1) |= col_index&1;
            col_index >>= 1;
            *(spriteptr+2) |= col_index&1;
            col_index >>= 1;
            *(spriteptr+3) |= col_index&1;

            *spriteptr<<=1;
			*(spriteptr+1)<<=1;
			*(spriteptr+2)<<=1;
			*(spriteptr+3)<<=1;
            
        }
        spriteptr+=4; 
    }

    for(int i=0;i<num_bytes/2;i+=4) {
		printf("0x%04x%04x, 0x%04x%04x",
				sprite[i],sprite[i+1],sprite[i+2],sprite[i+3] );

        if (i< (num_bytes/2-4)) printf(",");
        printf("\n");
	}

    free(sprite);

}

void export_palette(struct raw_image_s *img) {
    for(int i=0;i<16;i ++) {
        // STE palette bit order is b0 b3 b2 b1
        printf(" 0x%x%x%x",
            img->palette[i].R>>5 | ( (img->palette[i].R &0x10)>>1 ) ,
            img->palette[i].G>>5 | ( (img->palette[i].G &0x10)>>1 ) ,
            img->palette[i].B>>5 | ( (img->palette[i].B &0x10)>>1 ));

            if (i<15) printf(",");
            printf("\n");
    }
    
}

void show_help() {
    printf("png2sprite -i file.png [-x N] [-y N] [-w N] [-h N] [-s] [-p]\n");
    printf("\t -s : print sprite data\n");
    printf("\t -p : print palette data\n");
    exit(1);
}


int main(int argc, char *argv[]) {

    char filename[256];

    int x0=0,y0=0,w=16,h=16;

    int has_file=0,do_sprite=0,do_palette=0;

    int opt;
    while((opt = getopt(argc,argv,"i:x:y:w:h:sp")) !=-1 ) {
        switch(opt) {
            case 'i':
                strncpy(filename,optarg,255);
                has_file=1;
                break;
            case 'x':
                x0 = atoi(optarg);
                break;
            case 'y':
                y0 = atoi(optarg);
                break;
            case 'w':
                w = atoi(optarg);
                break;
            case 'h':
                h = atoi(optarg);
                break;
            case 's':
                do_sprite=1;
                break;
            case 'p':
                do_palette=1;
                break;
            default:
                show_help();

        }
    }

    if (!has_file) show_help();

    struct raw_image_s img;


    if (!(load_png(filename, &img))) {
        fprintf(stderr,"Error loading .png\n");
        exit(-1);
    }

    if (w%16) {
        fprintf(stderr,"Sprite width is not a 16 multiple\n");
        exit(-1);
    }

    if ( ((x0+w)>img.width) || ((y0+h)>img.height)) {
        fprintf(stderr,"Sprite out of bounds\n");
        exit(-1);
    }

    if (do_sprite) export_sprite(&img,x0,y0,w,h);

    if (do_palette) export_palette(&img);

    img_free(&img);

    return 0;
}