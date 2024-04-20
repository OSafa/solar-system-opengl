#define _USE_MATH_DEFINES 

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include "CelestialRing.h"
#include "CelestialBody.h"

#include <GL/glew.h>
#include <GL/freeglut.h> 

// Globals.
static int width, height; // Size of the OpenGL window.
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = -10000; // Co-ordinates of the spacecraft.
static int isCollision = 0; // Is there collision between the spacecraft and an entity?
static unsigned int spacecraft; // Display lists base index.
static int frameCount = 0; // Number of frames
static float mercuryLoc = 10125;	// Distance between Mercury and Sun
static float venusLoc = 12182;	// Distance between Venus and Sun
static float earthLoc = 14238;	// Distance between Earth and Sun
static float marsLoc = 16612;	// Distance between Mars and Sun
static float jupiterLoc = 21674;	// Distance between Jupiter and Sun
static float saturnLoc = 26737;	// Distance between Saturn and Sun
static float uranusLoc = 33065;	// Distance between Uranus and Sun
static float neptuneLoc = 39393;	// Distance between Neptune and Sun
static float moonLoc = 200;	// Distance between Moon and Earth

std::vector<CelestialBody*> entities; // Vector of entities in world
CelestialBody sun; // Sun object
CelestialBody mercury;	// Mercury object
CelestialBody mars;	// Mars object
CelestialBody earth;	// Earth object
CelestialBody venus;	// Venus object
CelestialBody jupiter;	// Jupiter object
CelestialBody neptune;	// Neptune object
CelestialBody saturn;	// Saturn object
CelestialBody uranus;	// Uranus object
CelestialBody moon;	// Moon object
CelestialBody skydome;	// Skybox/Skydome object
CelestialRing ringsOfSaturn;	// Rings of Saturn object

// Routine to count the number of frames drawn every second.
void frameCounter(int value)
{
	if (value != 0) // No output the first time frameCounter() is called (from main()).
		std::cout << "FPS = " << frameCount << std::endl;
	frameCount = 0;
	glutTimerFunc(1000, frameCounter, 1);
}

// Routine to move bodies along their orbits.
void orbit(int value)
{
	// Store previous time
	static unsigned int prevTime = 0;

	// Define orbit speeds
	static float mercuryOrbitSpeed = 0.642;
	static float venusOrbitSpeed = 0.470;
	static float earthOrbitSpeed = 0.400;
	static float marsOrbitSpeed = 0.324;
	static float jupiterOrbitSpeed = 0.176;
	static float saturnOrbitSpeed = 0.130;
	static float uranusOrbitSpeed = 0.092;
	static float neptuneOrbitSpeed = 0.072;
	static float moonOrbitSpeed = -7.86;

	// Move each body along its orbit
	float rotateAngle = 0.0;

	// Mercury
	rotateAngle = mercuryOrbitSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	mercury.modOrbY(rotateAngle);

	// Venus
	rotateAngle = venusOrbitSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	venus.modOrbY(rotateAngle);

	// Earth
	rotateAngle = earthOrbitSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	earth.modOrbY(rotateAngle);

	// Mars
	rotateAngle = marsOrbitSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	mars.modOrbY(rotateAngle);

	// Jupiter
	rotateAngle = jupiterOrbitSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	jupiter.modOrbY(rotateAngle);

	// Saturn
	rotateAngle = saturnOrbitSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	saturn.modOrbY(rotateAngle);

	// Neptune
	rotateAngle = uranusOrbitSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	neptune.modOrbY(rotateAngle);

	// Uranus
	rotateAngle = neptuneOrbitSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	uranus.modOrbY(rotateAngle);

	// Moon
	rotateAngle = moonOrbitSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	moon.modOrbY(rotateAngle);

	// Update previous time
	prevTime = glutGet(GLUT_ELAPSED_TIME);

	// Refresh display
	glutPostRedisplay();

	// Call self in 20 milliseconds
	glutTimerFunc(20, orbit, 1);
}

// Routine to rotate bodies around their axis.
void planetRotate(int value)
{
	// Store previous time
	static unsigned int prevTime = 0;

	// Define rotation speeds
	static float mercurySpinSpeed = 5;
	static float venusSpinSpeed = 4;
	static float earthSpinSpeed = 10;
	static float moonSpinSpeed = 24;
	static float marsSpinSpeed = 9.2;
	static float jupiterSpinSpeed = 33;
	static float saturnSpinSpeed = 27;
	static float uranusSpinSpeed = 13.9;
	static float neptuneSpinSpeed = 15;

	// Rotate each body around its axis
	float rotateAngle = 0.0;

	// Mercury
	rotateAngle = mercurySpinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	mercury.modRotY(rotateAngle);

	// Venus
	rotateAngle = venusSpinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	venus.modRotY(rotateAngle);

	// Earth
	rotateAngle = earthSpinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	earth.modRotY(rotateAngle);

	// Moon
	rotateAngle = moonSpinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	moon.modRotY(rotateAngle);

	// Mars
	rotateAngle = marsSpinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	mars.modRotY(rotateAngle);

	// Jupiter
	rotateAngle = jupiterSpinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	jupiter.modRotY(rotateAngle);

	// Saturn
	rotateAngle = saturnSpinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	saturn.modRotY(rotateAngle);

	// Neptune
	rotateAngle = uranusSpinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	neptune.modRotY(rotateAngle);

	// Uranus
	rotateAngle = neptuneSpinSpeed * (glutGet(GLUT_ELAPSED_TIME) - prevTime) / 1000;
	uranus.modRotY(rotateAngle);

	// Update previous time
	prevTime = glutGet(GLUT_ELAPSED_TIME);

	// Refresh display
	glutPostRedisplay();

	// Set function to be called in 20 ms
	glutTimerFunc(20, planetRotate, 1);
}

// Initialization routine.
void setup(void)
{
	// Generate spacecraft display list
	spacecraft = glGenLists(1);
	glNewList(spacecraft, GL_COMPILE);

	// Push matrix on stack
	glPushMatrix();

	// To make the spacecraft point down the z-axis initially.
	glRotatef(180.0, 0.0, 1.0, 0.0);

	// Set spacecraft color to white
	glColor3f(1.0, 1.0, 1.0);

	// Draw wire cone as spacecraft
	glutWireCone(5.0, 10.0, 10, 10);

	// Pop matrix from stack
	glPopMatrix();

	// End display list
	glEndList();


	// Construct Sun
	sun = CelestialBody(0.0, 0.0, 0.0, 90.0, 0.0, -7.25, 0.0, 0.0, 0.0, 8500, "textures/sun.jpg", true);

	// Construct planets
	mercury = CelestialBody(0.0, 0.0, -1*mercuryLoc, 90.0, 0.0, -0.03, 0.0, 0.0, 0.0, 30, "textures/mercury.jpg");
	venus = CelestialBody(0, 0.0, -1*venusLoc, 90.0, 0, -2.64, 0.0, 0.0, 0.0, 85, "textures/venus.jpg");
	earth = CelestialBody(0.0, 0.0, -1*earthLoc, 90.0, 0.0, -23.44, 0.0, 0.0, 0.0, 90, "textures/earth.jpg");
	mars = CelestialBody(0.0, 0.0, -1*marsLoc, 90.0, 0, -25.19, 0.0, 0.0, 0.0, 45, "textures/mars.jpg");
	jupiter = CelestialBody(0.0, 0.0, -1*jupiterLoc, 90.0, 0, -3.13, 0.0, 0.0, 0.0, 1000, "textures/jupiter.jpg");
	saturn = CelestialBody(0.0, 0.0, -1*saturnLoc, 90.0, 0, -26.73, 0.0, 0.0, 0.0, 810, "textures/saturn.jpg");
	uranus = CelestialBody(0.0, 0.0, -1*uranusLoc, 90.0, 0, -82.23, 0.0, 0.0, 0.0, 360, "textures/uranus.jpg");
	neptune = CelestialBody(0.0, 0.0, -1*neptuneLoc, 90.0, 0, -28.32, 0.0, 0.0, 0.0, 320, "textures/neptune.jpg");
	
	// Construct moon
	moon = CelestialBody(0.0, 0, -1*moonLoc, 90.0, 0, -6.68, 0.0, 0.0, 0.0, 23, "textures/moon.jpg");
	
	// Construct Skybox/Skydome
	skydome = CelestialBody(0.0, 0.0, 0.0, 90.0, 0, 0.0, 0.0, 0.0, 0.0, 40000, "textures/skydome.jpg", true);
	
	// Construct rings of Saturn
	ringsOfSaturn = CelestialRing(0.0, 0.0, 0.0, 1010, 2250, "textures/saturn_rings.png");


	// Set Sun material properties
	sun.setAmbAndDif(0.7, 0.7, 0.7, 1);
	sun.setShine(100);
	sun.setSpec(0, 0, 0, 1);
	sun.setEmis(1, 1, 1, 1);

	// Set Mercury material properties
	mercury.setAmbAndDif(0.6, 0.6, 0.6, 1);
	mercury.setShine(95);
	mercury.setSpec(0.7, 0.7, 0.7, 1);


	// Set Venus material properties
	venus.setAmbAndDif(0.54, 0.54, 0.54, 1);
	venus.setShine(71);
	venus.setSpec(0.6, 0.6, 0.6, 1);


	// Set Moon material properties
	moon.setAmbAndDif(0.52, 0.52, 0.52, 1);
	moon.setShine(81);
	moon.setSpec(0.51, 0.51, 0.51, 1);


	// Set Earth material properties
	earth.setAmbAndDif(0.51, 0.51, 0.51, 1);
	earth.setShine(61);
	earth.setSpec(0.51, 0.51, 0.51, 1);


	// Set Mars material properties
	mars.setAmbAndDif(0.45, 0.45, 0.45, 1);
	mars.setShine(51);
	mars.setSpec(0.43, 0.43, 0.43, 1);


	// Set Jupiter material properties
	jupiter.setAmbAndDif(0.39, 0.39, 0.39, 1);
	jupiter.setShine(43);
	jupiter.setSpec(0.36, 0.36, 0.36, 1);


	// Set Saturn material properties
	saturn.setAmbAndDif(0.33, 0.33, 0.33, 1);
	saturn.setShine(37);
	saturn.setSpec(0.30, 0.30, 0.30, 1);


	// Set Uranus material properties
	uranus.setAmbAndDif(0.27, 0.27, 0.27, 1);
	uranus.setShine(31);
	uranus.setSpec(0.25, 0.25, 0.25, 1);


	// Set Neptune material properties
	neptune.setAmbAndDif(0.21, 0.21, 0.21, 1);
	neptune.setShine(11);
	neptune.setSpec(0.21, 0.21, 0.21, 1);


	// Set Skydome/Skybox material properties
	skydome.setAmbAndDif(0.1, 0.1, 0.1, 1);
	skydome.setShine(10);
	skydome.setSpec(0, 0, 0, 1);
	skydome.setEmis(0.25, 0.25, 0.25, 1);

	// Add moon to orbit Earth
	earth.addSatellite(&moon);

	// Add rings around Saturn
	saturn.addRing(&ringsOfSaturn);

	// Add entities to vector
	entities.push_back(&sun);
	entities.push_back(&moon);
	entities.push_back(&mercury);
	entities.push_back(&venus);
	entities.push_back(&earth);
	entities.push_back(&mars);
	entities.push_back(&jupiter);
	entities.push_back(&saturn);
	entities.push_back(&uranus);
	entities.push_back(&neptune);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST); // Enable depth testing.

	// Cull back faces.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutTimerFunc(0, frameCounter, 0); // Initial call of frameCounter().
	glutTimerFunc(0, orbit, 0); // Initial call of orbit().
	glutTimerFunc(0, planetRotate, 0); // Initial call of planetRotate().
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1,
	float x2, float y2, float z2, float r2)
{
	return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2) <= (r1 + r2) * (r1 + r2));
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int entityCraftCollision(float x, float z, float a)
{
	// Check for collision with each entity.
	for (int i = 0; i < entities.size(); i++)
		if (entities[i]->getRadius() > 0) // If entity exists.
			if (checkSpheresIntersection(x - 5 * sin((M_PI / 180.0) * a), 0.0,
				z - 5 * cos((M_PI / 180.0) * a), 25,
				entities[i]->getCenterX(), entities[i]->getCenterY(),
				entities[i]->getCenterZ(), entities[i]->getRadius()))
				return 1;
	return 0;
}

// Drawing routine.
void drawScene(void)
{
	frameCount++; // Increment number of frames every redraw.

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Enable scissor test to crop outside of viewport
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	// Begin main viewport.
	glViewport(0, 0, width, height);

	// Define light properties
	static float lightAmb[] = { 0.64, 0.64, 0.64, 1.0 };
	static float lightDifAndSpec[] = { 0.95, 0.95, 0.95, 1.0 };
	static float lightPos0[] = { 0.0, 0.0, -8500, 1 };
	static float lightPos1[] = { 8500.0, 0.0, 0.0, 1 };
	static float lightPos2[] = { 0.0, 0.0, 8500.0, 1 };
	static float lightPos3[] = { -8500.0, 0.0, 0.0, 1 };
	static float lightPos4[] = { 0.0, 8500.0, 0.0, 1 };
	static float lightPos5[] = { 0.0, -8500.0, 0.0, 1 };
	static float globAmb[] = { 0.3, 0.3, 0.3, 1.0 };

	// Push identity matrix on stack
	glLoadIdentity();

	// Locate the camera at the tip of the cone and pointing in the direction of the cone.
	gluLookAt(xVal - 10 * sin((M_PI / 180.0) * angle),
		0.0,
		zVal - 10 * cos((M_PI / 180.0) * angle),
		xVal - 11 * sin((M_PI / 180.0) * angle),
		0.0,
		zVal - 11 * cos((M_PI / 180.0) * angle),
		0.0,
		1.0,
		0.0);

	// Global ambient light.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); 

	// Enable local viewer
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	// Light0 properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);

	// Set attenuation constants
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0001);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00000001);

	// Enable light 0 and place in one side of Sun
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	// Light1 properties.
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec);

	// Set attenuation constants
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0001);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.00000001);

	// Enable light 1 and place in one side of Sun
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	// Light2 properties.
	glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightDifAndSpec);

	// Set attenuation constants
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0001);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.00000001);

	// Enable light 2 and place in one side of Sun
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);

	// Light3 properties.
	glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT3, GL_SPECULAR, lightDifAndSpec);

	// Set attenuation constants
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.0001);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.00000001);

	// Enable light 3 and place in one side of Sun
	glEnable(GL_LIGHT3);
	glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);

	// Light4 properties.
	glLightfv(GL_LIGHT4, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT4, GL_SPECULAR, lightDifAndSpec);

	// Set attenuation constants
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.0001);
	glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.00000001);

	// Enable light 4 and place in one side of Sun
	glEnable(GL_LIGHT4);
	glLightfv(GL_LIGHT4, GL_POSITION, lightPos4);

	// Light5 properties.
	glLightfv(GL_LIGHT5, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT5, GL_SPECULAR, lightDifAndSpec);

	// Set attenuation constants
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.0001);
	glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.00000001);

	// Enable light 5 and place in one side of Sun
	glEnable(GL_LIGHT5);
	glLightfv(GL_LIGHT5, GL_POSITION, lightPos5);


	// Draw solar system
	sun.draw();
	mercury.draw();
	venus.draw();
	earth.draw();
	mars.draw();
	jupiter.draw();
	saturn.draw();
	neptune.draw();
	uranus.draw();
	skydome.draw(1, 1, 5);

	// Draw spacecraft.
	// Push matrix on scale
	glPushMatrix();

	// Move spaceship (spaceship is placed 10 units below camera to not obscure view)
	glTranslatef(xVal, -10.0, zVal);

	// Rotate spaceship by angle
	glRotatef(angle, 0.0, 1.0, 0.0);

	// Scale up spaceship to be noticeable in map
	glScalef(4.0, 4.0, 4.0);

	// Draw spacecraft
	glCallList(spacecraft);

	// Remove matrix from stack
	glPopMatrix();

	// End main viewport.

	// Enable scissor test to crop outside of viewport
	glEnable(GL_SCISSOR_TEST);
	glScissor(0.7 * width, 0, width * 0.3, height * 0.3);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	// Begin minimap viewport.
	glViewport(0.7 * width, 0, width * 0.3, height * 0.3);

	// Push identity matrix on stack
	glLoadIdentity();

	// Disable lighting for this viewport
	glDisable(GL_LIGHTING);


	// Fixed camera. (look from above)
	gluLookAt(0.0, 7200, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Set distance scale reciprocal
	float static distanceScale = 31.641;


	// Draw scaled solar system
	sun.draw(0.035);
	mercury.draw(0.75, distanceScale);
	mercury.drawOrbit(distanceScale);
	venus.draw(0.3, distanceScale);
	venus.drawOrbit(distanceScale);
	earth.draw(0.3, distanceScale);
	earth.drawOrbit(distanceScale);
	mars.draw(0.4, distanceScale);
	mars.drawOrbit(distanceScale);
	jupiter.draw(0.05, distanceScale);
	jupiter.drawOrbit(distanceScale);
	saturn.draw(0.05, distanceScale);
	saturn.drawOrbit(distanceScale);
	neptune.draw(0.08, distanceScale);
	neptune.drawOrbit(distanceScale);
	uranus.draw(0.08, distanceScale);
	uranus.drawOrbit(distanceScale);

	// Draw spacecraft.
	// Push matrix on scale
	glPushMatrix();

	// Move spaceship considering scale
	glTranslatef(xVal/ distanceScale, 0.0, zVal/ distanceScale);

	// Rotate spaceship by angle
	glRotatef(angle, 0.0, 1.0, 0.0);

	// Scale up spaceship to be noticeable in map
	glScalef(4.0, 4.0, 4.0);

	// Draw spacecraft
	glCallList(spacecraft);

	// Remove matrix from stack
	glPopMatrix();
	// End minimap viewport.

	// Swap buffers
	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-8.0, 8.0, -4.5, 4.5, 25.0, 100000.0);
	glMatrixMode(GL_MODELVIEW);

	// Pass the size of the OpenGL window.
	width = w;
	height = h;
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;

	// Compute next position.
	if (key == GLUT_KEY_LEFT) tempAngle = angle + 5.0;
	if (key == GLUT_KEY_RIGHT) tempAngle = angle - 5.0;
	if (key == GLUT_KEY_UP)
	{
		tempxVal = xVal - 25*sin(angle * M_PI / 180.0);
		tempzVal = zVal - 25*cos(angle * M_PI / 180.0);
	}
	if (key == GLUT_KEY_DOWN)
	{
		tempxVal = xVal + 25*sin(angle * M_PI / 180.0);
		tempzVal = zVal + 25*cos(angle * M_PI / 180.0);
	}

	// Angle correction.
	if (tempAngle > 360.0) tempAngle -= 360.0;
	if (tempAngle < 0.0) tempAngle += 360.0;

	// Move spacecraft to next position only if there will not be collision with an asteroid.
	if (!entityCraftCollision(tempxVal, tempzVal, tempAngle))
	{
		isCollision = 0;
		xVal = tempxVal;
		zVal = tempzVal;
		angle = tempAngle;
	}
	else isCollision = 1;

	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press the left/right arrow keys to turn the craft." << std::endl
		<< "Press the up/down arrow keys to move the craft." << std::endl;
}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Solar System Simulation");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}