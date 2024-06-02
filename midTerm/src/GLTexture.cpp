#include "GLTexture.h"

GLTexture::GLTexture()
{
    //ctor
}

GLTexture::~GLTexture()
{
    //dtor
}
void GLTexture::loadTexture(char* fileName)
{
    glGenTextures(1,&tex);               //generate image handler
    glBindTexture(GL_TEXTURE_2D,tex);    //pointing handler to the texture data format

    image= SOIL_load_image(fileName,&width,&height,0,SOIL_LOAD_RGBA); // read image

    if(!image)cout<<"ERROR: image not found"<<endl; // error check

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);//store image data
    SOIL_free_image_data(image); //delete image holder

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // how to wrap on object in x direction
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // how to wrap on object in y direction
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//decide the quality of the image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//decide the quality of the image

}

void GLTexture::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D,tex);
}
