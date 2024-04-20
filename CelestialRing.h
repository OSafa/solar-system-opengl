#pragma once

#include <string>
#include "stb_image.h"

#include <GL/glew.h>
#include <GL/freeglut.h> 

// CelestialRing class
class CelestialRing
{
public:
	// Constructors
	CelestialRing();
	CelestialRing(float tx, float ty, float tz, float innr, float outr, std::string texture);

	// Getters
	float getRotX() { return rotX; }
	float getRotY() { return rotY; }
	float getRotZ() { return rotZ; }
	float getInnerRadius() { return innerRadius; }
	float getOuterRadius() { return outerRadius; }

	// Attribute modification methods
	void modRotX(float value) {
		rotX += value;
		rotX = rotX > 360 ? rotX - 360 : rotX;
		rotX = rotX < 360 ? rotX + 360 : rotX;
	}
	void modRotY(float value) {
		rotY += value;
		rotY = rotY > 360 ? rotY - 360 : rotY;
		rotY = rotY < 360 ? rotY + 360 : rotY;
	}
	void modRotZ(float value) {
		rotZ += value;
		rotZ = rotZ > 360 ? rotZ - 360 : rotZ;
		rotZ = rotZ < 360 ? rotZ + 360 : rotZ;
	}

	// Draw method
	void draw(float uniformScale = 1.0);

private:
	// Attributes
	float rotX, rotY, rotZ, innerRadius, outerRadius;
	std::string texturePath;
	unsigned int texID;
	unsigned int displayID;

	// Methods
	void initBody();
};

// CelestialBody default constructor.
CelestialRing::CelestialRing()
{
	rotX = 0.0;
	rotY = 0.0;
	rotZ = 0.0;
	innerRadius = 0.0;
	outerRadius = 0.0;
	displayID = 0;
	texturePath = "";
	initBody();
}

// CelestialBody constructor.
CelestialRing::CelestialRing(float tx, float ty, float tz, float innr, float outr, std::string texture)
{
	rotX = tx;
	rotY = ty;
	rotZ = tz;
	innerRadius = innr;
	outerRadius = outr;
	displayID = 0;
	texturePath = texture;
	initBody();
}

// Method to initialize drawing lists for ring
void CelestialRing::initBody()
{
	if (outerRadius > 0.0 && innerRadius > 0.0)
	{
		if (!texturePath.empty()) {
			// Read texture file
			int texWidth, texHeight, texChannels;
			unsigned char* imgData = stbi_load(texturePath.c_str(), &texWidth, &texHeight, &texChannels, 0);

			// Enable 2D textures
			glEnable(GL_TEXTURE_2D);

			// Generate texture ID
			glGenTextures(1, &texID);

			// Bind texture
			glBindTexture(GL_TEXTURE_2D, texID);

			// Set texture wrapping to repeat
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// Set texture downscale filter to linear mipmapping
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			// Set texture upscale filter to nearest
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Generate texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);

			// Generate texture mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);

			// Free image data
			stbi_image_free(imgData);
		}


		// Create new quadric
		GLUquadricObj* disk = NULL;
		disk = gluNewQuadric();

		// Set quadric style to fill
		gluQuadricDrawStyle(disk, GLU_FILL);

		// Enable texture co-ordinate generation
		gluQuadricTexture(disk, TRUE);

		// Enable smooth shading
		gluQuadricNormals(disk, GLU_SMOOTH);

		// Generate display list id
		displayID = glGenLists(1);

		// Create new display list
		glNewList(displayID, GL_COMPILE);

		// Draw disk using quadric
		gluCylinder(disk, innerRadius, outerRadius, 10, 250, 250);

		// End drawing list
		glEndList();

		// Delete quadric data
		gluDeleteQuadric(disk);

		// Disable 2D Textures
		glDisable(GL_TEXTURE_2D);
	}
}

// Method to draw ring.
void CelestialRing::draw(float uniformScale)
{
	if (innerRadius > 0.0 && outerRadius > 0.0) // If ring exists.
	{
		// Push copy of global matrix
		glPushMatrix();

		// Rotate ring along each axis
		glRotatef(rotZ, 0.0, 0.0, 1.0);
		glRotatef(rotY, 0.0, 1.0, 0.0);
		glRotatef(rotX, 1.0, 0.0, 0.0);

		// Enable textures
		glEnable(GL_TEXTURE_2D);

		// Use ring texture
		glBindTexture(GL_TEXTURE_2D, texID);

		// Scale ring
		glScalef(uniformScale, uniformScale, uniformScale);

		// Draw ring using display list
		glCallList(displayID);

		// Disable textures
		glDisable(GL_TEXTURE_2D);

		// Pop matrix from global stack
		glPopMatrix();
	}
}
