/*
 * Universidade Federal do Rio de Janeiro
 * Trabalho de Introdução à Computação Gráfica (EEL882)
 * Semestre 2013.2
 * Professor: Cláudio Esperança
 * Aluno: Pedro Angelo Medeiros Fonini
 *
 */

// C++ version adapted from the C version at:
// http://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures#A_simple_libpng_example

#include <png.h>

#include "loadpng.h"

using namespace std;

GLuint loadpng(const string filename, int &width, int &height)
{
    // header for testing if it is a png
    png_byte header[8];

    // open file as binary
    FILE *fp = fopen(filename.c_str(), "rb");
    if (!fp) {
        throw loadpng_error(std::string("Can't open texture file `") +
                            filename + "'.");
    }

    // read the header
    if (fread(header, 1, 8, fp) != 8) {
        fclose(fp);
        throw loadpng_error(std::string("Error reading from texture file `") +
                            filename + "'.");
    };

    // test if it is png
    if (png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        throw loadpng_error(std::string("Texture file `") + filename +
                            "' should be PNG.");
    }

    // create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                                 NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        throw loadpng_error(
            std::string("Can't read data from the texture file `") +
            filename + "'."
        );
    }

    //create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(fp);
        throw loadpng_error(
            std::string("Error creating PNG info struct from texture file `") +
            filename + "'. (1)"
        );
    }

    //create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        throw loadpng_error(
            std::string("Error creating PNG info struct from texture file `") +
            filename + "'. (2)"
        );
    }

    //png error stuff, not sure libpng man suggests this.
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        throw loadpng_error(std::string("PNG error in texture file `") +
                            filename + "'.");
    }

    //init png reading
    png_init_io(png_ptr, fp);

    //let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    //variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 twidth, theight;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
                 NULL, NULL, NULL);

    //update width and height based on png info
    width = twidth;
    height = theight;

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte *image_data = new png_byte[rowbytes * height];
    if (!image_data) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        throw loadpng_badmem(0);
    }

    // row_pointers is for pointing to image_data for reading the png with
    // libpng
    png_bytep *row_pointers = new png_bytep[height];
    if (!row_pointers) {
        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete[] image_data;
        fclose(fp);
        throw loadpng_badmem(1);
    }
    // set the individual row_pointers to point at the correct offsets of
    // image_data
    for (int i = 0; i < height; ++i)
        row_pointers[height - 1 - i] = image_data + i * rowbytes;

    //read the png into image_data through row_pointers
        png_read_image(png_ptr, row_pointers);

        //Now generate the OpenGL texture object
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        //clean up memory and close stuff
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete[] image_data;
        delete[] row_pointers;
        fclose(fp);

        return texture;
}

