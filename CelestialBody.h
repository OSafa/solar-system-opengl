#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include "stb_image.h"

#include <GL/glew.h>
#include <GL/freeglut.h> 

// CelestialBody class.
class CelestialBody
{
public:
	// Constructors
	CelestialBody();
	CelestialBody(float x, float y, float z, float tx, float ty, float tz, float ox, float oy, float oz, float r, std::string texture, bool invertNormals = false);

	// Getters
	float getCenterX() { return centerX; }
	float getCenterY() { return centerY; }
	float getCenterZ() { return centerZ; }
	float getRotX() { return rotX; }
	float getRotY() { return rotY; }
	float getRotZ() { return rotZ; }
	float getOrbX() { return orbX; }
	float getOrbY() { return orbY; }
	float getOrbZ() { return orbZ; }
	float getRadius() { return radius; }

	// Modification methods
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
	void modOrbX(float value) {
		orbX += value;
		orbX = orbX > 360 ? orbX - 360 : orbX;
		orbX = orbX < 360 ? orbX + 360 : orbX;
	}
	void modOrbY(float value) {
		orbY += value;
		orbY = orbY > 360 ? orbY - 360 : orbY;
		orbY = orbY < 360 ? orbY + 360 : orbY;
	}
	void modOrbZ(float value) {
		orbZ += value;
		orbZ = orbZ > 360 ? orbZ - 360 : orbZ;
		orbZ = orbZ < 360 ? orbZ + 360 : orbZ;
	}

	// Method to draw body
	void draw(float uniformScale = 1.0, float scaleOrbit = 1.0, float texRepeat = 1.0);

	// Method to draw body's orbit
	void drawOrbit(float scaleOrbit = 1.0);

	// Method to add satellite to orbit around body
	void addSatellite(CelestialBody* satellite) { satellites.push_back(satellite); }

	// Method to add ring to body
	void addRing(CelestialRing* ring) { rings.push_back(ring); }

	// Method to set material ambient and diffuse parameters
	void setAmbAndDif(float r, float g, float b, float alpha) {
		matAmbAndDif[0] = r;
		matAmbAndDif[1] = g;
		matAmbAndDif[2] = b;
		matAmbAndDif[3] = alpha;
	}

	// Method to set material specular parameters
	void setSpec(float r, float g, float b, float alpha) {
		matSpec[0] = r;
		matSpec[1] = g;
		matSpec[2] = b;
		matSpec[3] = alpha;
	}

	// Method to set material emission parameters
	void setEmis(float r, float g, float b, float alpha) {
		matEmis[0] = r;
		matEmis[1] = g;
		matEmis[2] = b;
		matEmis[3] = alpha;
	}

	// Method to set material shine parameters
	void setShine(float val) {
		matShine[0] = val;
	}

private:
	// Attributes
	float centerX, centerY, centerZ, rotX, rotY, rotZ, orbX, orbY, orbZ, orbitRadius, radius;
	std::vector<CelestialBody*> satellites;
	std::vector<CelestialRing*> rings;
	std::string texturePath;
	unsigned int texID;
	unsigned int displayID, orbitID;
	float matAmbAndDif[4];
	float matSpec[4];
	float matEmis[4];
	float matShine[1];

	// Methods
	void initBody(bool invertNormals = false);
};

// CelestialBody default constructor.
CelestialBody::CelestialBody()
{
	centerX = 0.0;
	centerY = 0.0;
	centerZ = 0.0;
	rotX = 0.0;
	rotY = 0.0;
	rotZ = 0.0;
	orbX = 0.0;
	orbY = 0.0;
	orbZ = 0.0;
	radius = 0.0;
	orbitRadius = abs(centerZ);
	matAmbAndDif[0] = 0.4;
	matAmbAndDif[1] = 0.4;
	matAmbAndDif[2] = 0.4;
	matAmbAndDif[3] = 1;
	matSpec[0] = 0.2;
	matSpec[1] = 0.2;
	matSpec[2] = 0.2;
	matSpec[3] = 1;
	matEmis[0] = 0;
	matEmis[1] = 0;
	matEmis[2] = 0;
	matEmis[3] = 1;
	matShine[0] = 3;
	displayID = 0;
	orbitID = 0;
	texturePath = "";
	initBody();
}

// CelestialBody constructor.
CelestialBody::CelestialBody(float x, float y, float z, float tx, float ty, float tz, float ox, float oy, float oz, float r, std::string texture, bool invertNormals)
{
	centerX = x;
	centerY = y;
	centerZ = z;
	rotX = tx;
	rotY = ty;
	rotZ = tz;
	orbX = ox;
	orbY = oy;
	orbZ = oz;
	radius = r;
	orbitRadius = centerZ;
	matAmbAndDif[0] = 0.4;
	matAmbAndDif[1] = 0.4;
	matAmbAndDif[2] = 0.4;
	matAmbAndDif[3] = 1;
	matSpec[0] = 0.2;
	matSpec[1] = 0.2;
	matSpec[2] = 0.2;
	matSpec[3] = 1;
	matEmis[0] = 0;
	matEmis[1] = 0;
	matEmis[2] = 0;
	matEmis[3] = 1;
	matShine[0] = 3;
	displayID = 0;
	orbitID = 0;
	texturePath = texture;
	initBody(invertNormals);
}

// Method to initialize body and orbit display lists
void CelestialBody::initBody(bool invertNormals)
{
	if (radius > 0.0)
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

			// Set texture upscale filter to linear
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Generate texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);

			// Generate texture mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);

			// Free image data
			stbi_image_free(imgData);
		}

		// Create new quadric
		GLUquadricObj* sphere = NULL;
		sphere = gluNewQuadric();

		// Set quadric style to fill
		gluQuadricDrawStyle(sphere, GLU_FILL);

		// Enable texture co-ordinate generation
		gluQuadricTexture(sphere, TRUE);
		gluQuadricNormals(sphere, GLU_SMOOTH);
		if (invertNormals) gluQuadricOrientation(sphere, GLU_INSIDE);

		//Generate display list id
		displayID = glGenLists(1);

		// Create new display list
		glNewList(displayID, GL_COMPILE);

		// Mirror sphere (to apply textures correctly)
		glScalef(1.0, -1.0, 1.0);

		// Properly align sphere
		glRotatef(180, 0, 0, 1);

		// Draw sphere using quadric
		gluSphere(sphere, radius, 250, 250);

		// End drawing list
		glEndList();

		// Delete quadric data
		gluDeleteQuadric(sphere);

		// Disable 2D Textures
		glDisable(GL_TEXTURE_2D);

		//Generate display list id
		orbitID = glGenLists(1);

		// Create new display list
		glNewList(orbitID, GL_COMPILE);

		// Push matrix on stack for orbit transformations
		glPushMatrix();

		// Create new quadric
		GLUquadricObj* orbit = NULL;
		orbit = gluNewQuadric();

		// Set quadric style to line
		gluQuadricDrawStyle(orbit, GLU_LINE);

		// Rotate orbit to align with camera
		glRotatef(90, 1, 0, 0);

		// Color orbit as white
		glColor3f(1, 1, 1);

		// Draw sphere using quadric
		gluDisk(orbit, abs(orbitRadius), abs(orbitRadius), 500, 500);

		// Delete quadric data
		gluDeleteQuadric(orbit);

		// Remove matrix containing orbit transformations
		glPopMatrix();

		// End drawing list
		glEndList();
	}
}


// Method to draw body's orbit using display list
void CelestialBody::drawOrbit(float scaleOrbit)
{
	if (radius > 0.0) // If CelestialBody exists.
	{
		// Push new matrix on stack
		glPushMatrix();

		// Scale orbit by reciprocal of provided amount
		glScalef(1 / scaleOrbit, 1 / scaleOrbit, 1 / scaleOrbit);

		// Draw orbit
		glCallList(orbitID);

		// Pop matrix from stack
		glPopMatrix();
	}
}

// Function to draw CelestialBody.
void CelestialBody::draw(float uniformScale, float scaleOrbit, float texRepeat)
{
	if (radius > 0.0) // If CelestialBody exists.
	{
		// Set material properties
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmis);


		// Push matrix on stack
		glPushMatrix();

		// Move body along orbit path taking into consideration scale
		centerZ = orbitRadius * cos(orbY * M_PI / 180);
		centerX = orbitRadius * sin(orbY * M_PI / 180);
		glTranslatef(centerX / scaleOrbit, centerY / scaleOrbit, centerZ / scaleOrbit);

		// Draw satellite bodies
		for (int i = 0; i < satellites.size(); i++) {
			satellites[i]->draw(uniformScale, scaleOrbit);
		}

		// Rotate body
		glRotatef(rotZ, 0.0, 0.0, 1.0);
		glRotatef(rotY, 0.0, 1.0, 0.0);
		glRotatef(rotX, 1.0, 0.0, 0.0);

		// Draw attached rings
		for (int i = 0; i < rings.size(); i++) {
			rings[i]->draw(uniformScale);
		}

		// Enable textures
		glEnable(GL_TEXTURE_2D);

		// Use bound texture
		glBindTexture(GL_TEXTURE_2D, texID);

		// Scale body by provided amount
		glScalef(uniformScale, uniformScale, uniformScale);

		// Switch to texture matrix
		glMatrixMode(GL_TEXTURE);

		// Push new matrix on texture stack
		glPushMatrix();

		// Scale textures to repeat them on body
		glScalef(texRepeat, texRepeat, 0);

		// Draw body
		glCallList(displayID);

		// Pop texture matrix from texture stack
		glPopMatrix();

		// Switch back to model view stack
		glMatrixMode(GL_MODELVIEW);

		// Disable textures
		glDisable(GL_TEXTURE_2D);

		// Pop matrix from model view stack
		glPopMatrix();
	}
}