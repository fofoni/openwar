/* Taken mostly from:
http://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures#A_simple_libpng_example
*/

#ifndef LOADPNG_H
#define LOADPNG_H

#include <GL/gl.h>
#include <string>

class loadpng_except : public exception {
    virtual const char* what() const throw() {
        return "Could not open file.";
    }
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

GLuint loadpng(const string filename, int &width, int &height);

#endif
