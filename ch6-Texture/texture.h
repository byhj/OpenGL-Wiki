#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <IL/il.h>

GLuint loadTexture(const  char* theFileName)
{
	ILuint imageID;				// Create an image ID as a ULuint
	GLuint textureID = 0;			// Create a texture ID as a GLuint
	ILboolean success;			// Create a flag to keep track of success/failure
//	ILenum error;				// Create a flag to keep track of the IL error state
	ilInit();
	ilGenImages(1, &imageID); 		// Generate the image ID
	ilBindImage(imageID); 			// Bind the image
	success = ilLoadImage(theFileName); 	// Load the image file
	
	if (success) {
	   success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		glGenTextures(1, &textureID);
		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
			0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself
	}


	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.
	cout << "Texture creation successful." << endl;
	return textureID; // Return the GLuint to the texture so you can use it!
}

#endif