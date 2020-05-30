#include <iostream>

using namespace std;

struct BITMAPFILEHEADER {          /**** BMP file header structure (14 bytes total, but i've kept bfType separate) ****/
    unsigned int   Size;           /* Size of file */
    unsigned short Reserved1;      /* Reserved */
    unsigned short Reserved2;      /* ... */
    unsigned int   OffBits;        /* Offset to bitmap data */
};

struct BITMAPINFOHEADER {          /**** BMP file info structure (40 bytes total) ****/
    unsigned int   Size;           /* Size of info header */
    int            Width;          /* Width of image */
    int            Height;         /* Height of image */
    unsigned short Planes;         /* Number of color planes */
    unsigned short BitsPerPixel;   /* Number of bits per pixel */
    unsigned int   Compression;    /* Type of compression to use */
    unsigned int   SizeImage;      /* Size of image data */
    int            XPelsPerMeter;  /* X pixels per meter */
    int            YPelsPerMeter;  /* Y pixels per meter */
    unsigned int   ClrUsed;        /* Number of colors used */
    unsigned int   ClrImportant;   /* Number of important colors */
};

void generateBitmap(int width, int height) {

    int bytesPerPixel = 3;

    /*padding to apply to bitmap*/
    int paddingSize = (4 - (width*bytesPerPixel) % 4) % 4;
    unsigned char padding[3] = {0, 0, 0};

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    /* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
    unsigned short bfType=0x4d42;

    /*write bitmap file header*/
    bfh.Reserved1 = 0;
    bfh.Reserved2 = 0;
    bfh.Size = sizeof(struct BITMAPFILEHEADER) + sizeof(struct BITMAPINFOHEADER) + (bytesPerPixel*width + paddingSize)*height;
    bfh.OffBits = 0x36;

    /*write bitmap info header*/
    bih.Size = sizeof(struct BITMAPINFOHEADER);
    bih.Width = width;
    bih.Height = height;
    bih.Planes = 1;
    bih.BitsPerPixel = 8 * bytesPerPixel;
    bih.Compression = 0;
    bih.SizeImage = 0;
    bih.XPelsPerMeter = 0;
    bih.YPelsPerMeter = 0;
    bih.ClrUsed = 0;
    bih.ClrImportant = 0;

    /*open file*/
    FILE *file = fopen("bitMapImage.bmp", "wb");
    if (!file){
        cout << "Could not write file" << endl;
        return;
    }

    /*Write headers to file*/
    fwrite(&bfType,1,sizeof(bfType),file);
    fwrite(&bfh, 1, sizeof(bfh), file);
    fwrite(&bih, 1, sizeof(bih), file);

    /*Write bitmap data*/
    for (int y = bih.Height-1; y>=0; y--) { /*Scanline loop backwards*/

        for (int x = 0; x < bih.Width; x++) { /*Column loop forwards*/

            /*compute some pixel values*/
            unsigned char r = ((double)(y/height)*255);
            unsigned char g = ((double)x/width*255);
            unsigned char b = (((double)y+x)/(height+width)*255);
            fwrite(&b, 1, 1, file);
            fwrite(&g, 1, 1, file);
            fwrite(&r, 1, 1, file);
        }

        /*write padding*/
        fwrite(padding, 1, paddingSize, file);
    }

    fclose(file);
}


int main() {
    generateBitmap(640, 360);
    cout << "Image generated successfully!" << endl;
    return 0;
}
