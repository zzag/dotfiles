#include <iostream>
#include <cstdio>
#include <png.h>


using std::cout;
using std::endl;


struct Image
{
    int width;
    int height;
    int pitch;
    unsigned char *data;

    void release()
    {
        delete[] data;
    }
};


Image *readImage(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) { return 0; }

    char header[8];
    fread(header, 1, 8, fp);
    if (png_sig_cmp((png_const_bytep)header, 0, 8)) { fclose(fp); return 0; }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png) { fclose(fp); return 0; }

    png_infop info = png_create_info_struct(png);
    if (!info) { png_destroy_read_struct(&png, 0, 0); fclose(fp); return 0; }
    if (setjmp(png_jmpbuf(png))) { png_destroy_read_struct(&png, &info, 0); fclose(fp); return 0; }

    png_init_io(png, fp);
    png_set_sig_bytes(png, 8);
    
    png_read_info(png, info);
    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    int colorType = png_get_color_type(png, info);
    int bpp = colorType == PNG_COLOR_TYPE_RGB ? 3 : 4;
    int pitch = bpp * width;
    unsigned char *data = new unsigned char[pitch * height];
    unsigned char **rows = new unsigned char*[height];
    for (int i = 0; i < height; i++) { rows[i] = data + i * pitch; }

    png_read_image(png, rows);

    png_destroy_read_struct(&png, &info, 0);
    fclose(fp);
    delete[] rows;

    Image *img = new Image;
    img->width = width;
    img->height = height;
    img->pitch = pitch;
    img->data = data;

    return img;
}


int writeImage(Image *img, const char *filename)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp) { return -1; }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png) { fclose(fp); return -1; }

    png_infop info = png_create_info_struct(png);
    if (!info) { png_destroy_write_struct(&png, 0); fclose(fp); return 0; }
    if (setjmp(png_jmpbuf(png))) { png_destroy_write_struct(&png, 0); fclose(fp); return 0; }

    png_init_io(png, fp);

    int bpp = img->pitch / img->width;
    png_set_IHDR(png, info, img->width, img->height, 8, bpp == 3 ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    unsigned char **rows = new unsigned char*[img->height];
    for (int i = 0; i < img->height; i++) { rows[i] = img->data + i * img->pitch; }

    png_set_rows(png, info, rows);
    png_write_png(png, info, PNG_TRANSFORM_IDENTITY, 0);

    if (setjmp(png_jmpbuf(png))) 
    {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return -1;
    }

    png_write_end(png, info);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
    delete[] rows;

    return 0;
}


int main()
{
    // Image *img = readImage("tst.png");
    // writeImage(img, "tst0.png");
    // img->release();
    // delete img;

    return 0;
}
