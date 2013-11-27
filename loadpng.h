/* Taken mostly from:
http://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures#A_simple_libpng_example
*/

#ifndef LOADPNG_H
#define LOADPNG_H

#include <GL/gl.h>
#include <string>
#include <stdexcept>

#define E_MEMBLOCK "Cannot allocate memory for the given texture file."
#define E_ROWS "Cannot allocate memory for reading through the texture image."

class loadpng_badmem : public std::exception {
    int e;
public:
    loadpng_badmem(int err) : e(err) {}
    virtual const char* what() const throw() {
        switch (e) {
            case 0:
                return E_MEMBLOCK;
            case 1:
                return E_ROWS;
        }
    }
};

class loadpng_error : public std::runtime_error {
public:
    loadpng_error(std::string const& error) : std::runtime_error(error) {}
};

/** loadpng
 *     loads a png file into an opengl texture object
 *
 *     \param filename : the png file to be loaded
 *     \param width : width of png, to be updated as a side effect of this function
 *     \param height : height of png, to be updated as a side effect of this function
 *
 *     \return GLuint : an opengl texture id
 *
 */

GLuint loadpng(const std::string filename, int &width, int &height);

#endif
