/* Taken mostly from:
http://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures#A_simple_libpng_example
*/

#ifndef LOADPNG_H
#define LOADPNG_H

#include <GL/gl.h>

class loadpng_except : public exception {
    virtual const char* what() const throw() {
        return "Could not open file.";
    }
} cant_loadpng;

/** loadTexture
 *     loads a png file into an opengl texture object, using cstdio , libpng, and opengl.
 *
 *     \param filename : the png file to be loaded
 *     \param width : width of png, to be updated as a side effect of this function
 *     \param height : height of png, to be updated as a side effect of this function
 *
 *     \return GLuint : an opengl texture id.  Will be 0 if there is a major error,
 *                                     should be validated by the client of this function.
 *
 */

GLuint loadTexture(const string filename, int &width, int &height);

#endif
