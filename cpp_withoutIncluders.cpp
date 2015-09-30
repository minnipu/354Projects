

//=========================================================//
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
//=========================================================//
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "iostream"
#include <math.h>

//=========================================================//
//=========================================================//
// person position in the environment
void move_camera(int specialKey, char normalKey);
void update_camera();

GLdouble  g_playerPos[] = { 0, 1, 15.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 0.0;
GLfloat	  g_elevation = 4.0;
GLfloat   change_collor = 1.0;
float rad = 0;
const float DEFAULT_SPEED = 0.4f;
//Snowflake variables
bool snowInitialized = false;
const int numFlakes = 10000;
static float positions[numFlakes][3];
GLfloat windmillHeight = 4.0;
GLfloat fallSpeed = .05;
float rotateBase_degrees = 0;
float rotateMult = 1;
//=========================================================//
//=========================================================//
GLvoid  DrawGround();

//New functions
GLvoid  DrawTrees();
GLvoid  DrawTree(GLfloat dx, GLfloat dy, GLfloat dz);
GLvoid  DrawSnowflake(GLfloat dx, GLfloat dy, GLfloat dz);
GLvoid  SnowflakeEmmitter();
GLvoid  DrawNormalObjects(GLfloat rotation);
GLvoid  DrawWireframeObjects(GLfloat rotation);
GLvoid  DrawFlatshadedObjects(GLfloat rotation);
//=========================================================//
//=========================================================//
// Collision detection
GLfloat p1_radius = 0.3f;
GLfloat p2_radius = 0.3f;
GLfloat p1_x = -2.0f;
GLfloat p2_x = 2.0f;
GLfloat p1_y = 2.0f;
GLfloat p2_y = 2.0f;
GLfloat p1_z = 5.0f;
GLfloat p2_z = 5.0f;
GLfloat change_direction = 1.0;
//=========================================================//
//=========================================================//
// quadric objects
void init_dados(void);
void setup_sceneEffects(void);
GLUquadricObj *g_normalObject = NULL;
GLUquadricObj *g_wireframeObject = NULL;
GLUquadricObj *g_flatshadedObject = NULL;
void cleanUP_data(void);

const int   WORLD_SIZE = 200;
//=========================================================//
//=========================================================//
static void text_onScreen(int row, int col, const char *fmt, ...);
//=========================================================//
//=========================================================//
typedef struct
{
	const char * const name;
	void(*solid) (void);
	void(*wire)  (void);
} entry;
//=========================================================//
//=========================================================//
static void text_onScreen(int row, int col, const char *fmt, ...)
{
	static char buf[256];
	int viewport[4];
	//void *font = GLUT_BITMAP_9_BY_15;
	va_list args;

	va_start(args, fmt);
	//(void)vsprintf_s(buf, fmt, args);
	va_end(args);

	glGetIntegerv(GL_VIEWPORT, viewport);

	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, viewport[2], 0, viewport[3], -1, 1);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void output(int x, int y, char *string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}
//=========================================================//
//=========================================================//
static void resize(int width, int height)
{
	const float ar = (float)width / (float)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (ar > .5)
		glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
	else
		glFrustum(-1.0, 1.0, -1 / ar, 1 / ar, 2.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//=========================================================//
//=========================================================//
GLvoid DrawGround()
{ // enable blending for anti-aliased lines
	glEnable(GL_BLEND);

	// set the color to a bright blue

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0, 0, -100.0);
	glVertex3f(-100.0, 0, 100.0);
	glVertex3f(100.0, 0, 100.0);
	glVertex3f(100.0, 0, -100.0);
	glEnd();


	// draw the lines
	/*
	glColor3f(0.5f, 0.7f, 1.0f);
	glBegin(GL_LINES);
	for (int x = -WORLD_SIZE; x < WORLD_SIZE; x += 6)
	{
	glVertex3i(x, 0, -WORLD_SIZE);
	glVertex3i(x, 0, WORLD_SIZE);
	}

	for (int z = -WORLD_SIZE; z < WORLD_SIZE; z += 6)
	{
	glVertex3i(-WORLD_SIZE, 0, z);
	glVertex3i(WORLD_SIZE, 0, z);
	}
	glEnd();
	*/
	// turn blending off
	glDisable(GL_BLEND);
} // end DrawGround()
  //=========================================================//
  //=========================================================//
GLvoid drawCollision()
{
	GLfloat alphaTransparency = 0.5;
	GLfloat distance;

	// move forward and backward
	p1_x += 0.05f*change_direction;
	p2_x -= 0.05f*change_direction;

	if (p1_x > 2) {
		change_direction = 1.0f - 2.0f;
	}
	if (p1_x < -2) {
		change_direction = 1.0f;
	}
	// check-collision
	distance = sqrt(((p1_x - p2_x) * (p1_x - p2_x))
		+ ((p1_y - p2_y) * (p1_y - p2_y))
		+ ((p1_z - p2_z) * (p1_z - p2_z)));
	if (distance <= p2_radius + p1_radius) {
		// Red :: collision
		change_collor = 0.0;
	}
	else {
		// Yellow :: no collision
		change_collor = 1.0;
	}
	// enable blending for transparent sphere
	glEnable(GL_BLEND);     // Turn Blending On
	glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// first sphere collides against the other
	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor4f(1.0f, change_collor, 0.0f, alphaTransparency);
	glTranslatef(p1_x, p1_y, p1_z);
	gluSphere(g_normalObject, p1_radius, 16, 10);
	glPopMatrix();
	// second sphere collides against the first
	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor4f(1.0f, change_collor, 0.0f, alphaTransparency);
	glTranslatef(p2_x, p2_y, p2_z);
	gluSphere(g_normalObject, p2_radius, 16, 10);
	glPopMatrix();

	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
}
//=========================================================//

//=========================================================//
GLvoid drawCone(void)
{
	GLfloat alphaTransparency = 0.5;
	// a transparency example: green cylinder
	// enable blending for transparent cylinder
	glEnable(GL_BLEND);     // Turn Blending On
	glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor4f(0.0f, 1.0f, 0.0f, alphaTransparency);
	glTranslatef(100.0, 0.0, -1.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(g_normalObject, 1.0, 0.0, 3.0, 32, 4);
	glPopMatrix();

	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
}
//=========================================================//
GLvoid DrawTree(GLfloat dx, GLfloat dy, GLfloat dz) {
	int trunkScale = 2;

	glEnable(GL_BLEND);     // Turn Blending On
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	//position the tree
	glTranslatef(dx, dy, dz);

	//tree base
	glPushMatrix();
	glTranslatef(0, 0.5 / trunkScale, 0);
	glScalef(1.0 / trunkScale, 1.0 / trunkScale, 1.0 / trunkScale);
	glColor3f(0.36f, .25f, 0.2f);  //brown
	glRotatef(-90, 1, 0, 0);
	gluCylinder(g_normalObject, 1.0, 1.0, 3.0, 32, 4);
	glPopMatrix();

	//leaves
	glPushMatrix();
	glTranslatef(0, .5, 0);

	glPushMatrix();  //level 1
	glTranslatef(0, 1, 0);
	glColor3f(0.34f, .49f, 0.23f);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(g_normalObject, 1.0, 0.0, 3.0, 32, 4);
	glPopMatrix();

	glPushMatrix();  //level 2
	glTranslatef(0, 2, 0);
	glColor3f(0.34f, .49f, 0.23f);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(g_normalObject, 1.0, 0.0, 3.0, 32, 4);
	glPopMatrix();

	//level 2
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_BLEND);        // Turn Blending Off
}

GLvoid DrawTrees() {

	//foreground trees
	DrawTree(1.0f, 0.0f, 0.0f);
	DrawTree(1.0f, 0.0f, 5.0f);


	//background trees
	DrawTree(-6.0f, 0.0f, 0.0f);
	DrawTree(-1.0f, 0.0f, 4.0f);
	DrawTree(0.0f, 0.0f, -12.0f);
}

GLvoid DrawSnow() {
	GLfloat snowSize = 0.05;
	//Initialize an array with random values within the WORLD_SIZE and no higher than 20 on the y-axis.
	//Only do it once.
	if (!snowInitialized) {
		for (int index = 0; index < numFlakes; index++) {
			for (int dimensions = 0; dimensions < 3; ++dimensions) {
				if (dimensions == 1) {
					positions[index][dimensions] = (20 * ((float)rand() / RAND_MAX));;
				}
				else
					positions[index][dimensions] = ((WORLD_SIZE - (-WORLD_SIZE))*((float)rand() / RAND_MAX)) - WORLD_SIZE;
			}
		}
		snowInitialized = true;
	}

	//draw each snowflake from vector array.
	//decrease height by by fallSpeed and reset height if goes below 0.
	glEnable(GL_BLEND);     // Turn Blending On
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int index = 0; index < numFlakes; ++index) {
		glPushMatrix();
		glTranslatef(positions[index][0], positions[index][1], positions[index][2]);
		positions[index][1] -= fallSpeed;
		if (positions[index][1] <= 0) {
			positions[index][1] = 20;
		}
		if ((rotateMult != 0) && (positions[index][1] <= windmillHeight)) {
			positions[index][2] += rotateMult / 50;
			if (positions[index][2] >= WORLD_SIZE) {
				positions[index][2] = -WORLD_SIZE;
			}
		}
		glScalef(snowSize, snowSize, snowSize);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
		glColor4f(.8f, .79f, .79f, 0.5f);
		gluSphere(g_normalObject, 1, 16, 4);
		glPopMatrix();
		glDisable(GL_BLEND);        // Turn Blending Off
		glEnable(GL_DEPTH_TEST);

	}
}


GLvoid DrawSnowman(GLfloat x, GLfloat y, GLfloat z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	// Draw body (a 20x20 spherical mesh of radius 0.75 at height 0.75)
	glColor3f(1.0, 1.0, 1.0); // set drawing color to white
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.75);
	glutSolidSphere(0.75, 20, 20);
	glPopMatrix();

	// Draw the head (a sphere of radius 0.25 at height 1.75)
	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.75); // position head
	glutSolidSphere(0.25, 20, 20); // head sphere

								   // Draw Eyes (two small black spheres)
	glColor3f(0.0, 0.0, 0.0); // eyes are black
	glPushMatrix();
	glTranslatef(0.0, -0.18, 0.10); // lift eyes to final position
	glPushMatrix();
	glTranslatef(-0.05, 0.0, 0.0);
	glutSolidSphere(0.05, 10, 10); // right eye
	glPopMatrix();
	glPushMatrix();
	glTranslatef(+0.05, 0.0, 0.0);
	glutSolidSphere(0.05, 10, 10); // left eye
	glPopMatrix();
	glPopMatrix();



	// Draw Nose (the nose is an orange cone)
	glColor3f(1.0, 0.5, 0.5); // nose is orange
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0); // rotate to point along -y
	glutSolidCone(0.08, 0.5, 10, 2); // draw cone
	glPopMatrix();
	glPopMatrix();

	// Draw a faux shadow beneath snow man (dark green circle)
	glColor3f(0.0, 0.05, 0.0);
	glPushMatrix();
	glTranslatef(0.2, 0.2, 0.005);	// translate to just above ground
	glScalef(1.0, 1.0, 0.0); // scale sphere into a flat pancake
	glutSolidSphere(0.75, 20, 20); // shadow same size as body
	glPopMatrix();
	glPopMatrix();
}

GLvoid DrawWindWheel(GLfloat x, GLfloat y, GLfloat z, GLfloat rotation)
{
	static int slices = 16;
	static int stacks = 16;
	//Wheel radii
	static float ri = 0.12;
	static float r1 = 1.0;
	static float r2 = 1.25;
	static float r3 = 1.5;
	// make sure the random color values we get are the same every time
	srand(300);

	// save the existing color properties
	//glPushAttrib(GL_CURRENT_BIT);

	// enable blending to get anti-aliased lines.
	glEnable(GL_BLEND);
	glPushMatrix();
	glMateriali(GL_FRONT_AND_BACK,
		GL_AMBIENT, 0);
	//Rotate wind wheel as a whole.
	glTranslatef(x, y, z);
	glRotatef((GLfloat)rotateBase_degrees, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	//Create mast of wind wheel
	glColor3f(0, 0, 0.90);
	glRotatef(-90., 1.0, 0., 0.);
	//glTranslatef(0.0, 0.14, 0.0);
	gluCylinder(g_normalObject, ri, ri, windmillHeight, slices, stacks);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, windmillHeight, 0.14);
	glPushMatrix();
	//Create hub of wind wheel
	glColor3f(0, 0., 0.90);
	gluDisk(g_normalObject, 0., ri, 32, 4);
	glTranslatef(0., 0.0, -0.5);
	gluCylinder(g_normalObject, ri, ri, 0.5, 32, 8);
	glPopMatrix();

	glPushMatrix();
	//Raise all 3 windwheels 4 units verticaly.
	//Create smallest wheel, as blue.
	glPushMatrix();
	//Rotates the windwheel
	glRotatef(-rotation, 0.0f, 0.0f, 1.0f);
	glColor3f(0, 0, 1);
	gluPartialDisk(g_normalObject, ri, r1, 32, 4, 0, 30);
	gluPartialDisk(g_normalObject, ri, r1, 32, 4, 90, 30);
	gluPartialDisk(g_normalObject, ri, r1, 32, 4, 180, 30);
	gluPartialDisk(g_normalObject, ri, r1, 32, 4, 270, 30);
	glColor3f(0, 0, 0);
	gluDisk(g_normalObject, r1, r1 + 0.05, 32, 4);
	glPopMatrix();

	glPushMatrix();
	//Create middle wheel, as red. made entire windwheel blue ~~ mignon
	glColor3f(0, 0.25, 0.90);
	//Move wheel a little behind the smallest wheel to avoid clipping.
	glTranslatef(0.0f, 0.0f, -0.01f);
	//Rotates the windwheel as a multiple of smallest wheel.
	glRotatef(-rotation * 1.25, 0.0f, 0.0f, 1.0f);
	gluPartialDisk(g_normalObject, ri, r2, 32, 4, 0, 30);
	gluPartialDisk(g_normalObject, ri, r2, 32, 4, 90, 30);
	gluPartialDisk(g_normalObject, ri, r2, 32, 4, 180, 30);
	gluPartialDisk(g_normalObject, ri, r2, 32, 4, 270, 30);
	glColor3f(0, 0, 0);
	gluDisk(g_normalObject, r2, r2 + 0.05, 32, 4);
	glPopMatrix();

	glPushMatrix();
	//Create largest windwheel, as green.made blue
	glColor3f(0, 0.10, 1);
	//Move wheel a little behind the middle wheel to avoid clipping.
	glTranslatef(0.0f, 0.0f, -0.02f);
	//Rotates the windwheel as a multiple of smallest wheel.
	glRotatef(-rotation * 1.5, 0.0f, 0.0f, 1.0f);
	gluPartialDisk(g_normalObject, ri, r3, 32, 4, 0, 20);
	gluPartialDisk(g_normalObject, ri, r3, 32, 4, 60, 20);
	gluPartialDisk(g_normalObject, ri, r3, 32, 4, 120, 20);
	gluPartialDisk(g_normalObject, ri, r3, 32, 4, 180, 20);
	gluPartialDisk(g_normalObject, ri, r3, 32, 4, 240, 20);
	gluPartialDisk(g_normalObject, ri, r3, 32, 4, 300, 20);
	glColor3f(0, 0, 0);
	gluDisk(g_normalObject, r3, r3 + 0.05, 64, 4);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}


//=========================================================//
//=========================================================//
//=========================================================//
static void display(void)
{
	update_camera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);

	// position the light
	GLfloat pos[4] = { 5.0, 5.0, 5.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	// rotation is used for animation
	static GLfloat rotation = 0.0;

	// and ranges between 0 and 360
	if (rotateMult != 0)
		rotation += 1.0*rotateMult;
	if (rotation > 360.0)
		rotation = 0.0;
	// draw all of our objects in their normal position
	//DrawNormalObjects();
	DrawTrees();
	DrawSnow();
	DrawSnowman(0.0, 0.0, 6.0);
	DrawWindWheel(5.0, 0.0, 5.0, rotation);
	DrawGround();

	//glDisable(GL_LIGHTING);

	glColor3f(1, 0.1, 1); //~~keyboard screen text
	output(-5, 10, "--Keyboard Controls--");
	output(-5, 9, "-Windmill Speed: 0, 1, 2, 3");
	output(-5, 8, "-Look up or down: A/Z");
	output(-5, 7, "-Walk Forward/Backward: Up/Down");
	output(-5, 6, "-Look left/right: <- ->");
	output(-5, 5, "r&l move windwheel");

	glutSwapBuffers();
}
//=========================================================//
//=========================================================//
static void keyboard(unsigned char key, int x, int y)
{
	int number = -1;

	move_camera(number, key);

	glutPostRedisplay();
}
//=========================================================//
//=========================================================//
void move_camera(int specialKEY, char normalKEY)
{
	// keyboard :: normal keys
	switch (normalKEY)
	{   // looking up
	case 'A':
	case 'a':    g_elevationAngle += 2.0; break;
		// looking down
	case 'Z':
	case 'z':g_elevationAngle -= 2.0;  break;
		//move camera up
	case 'q':
	case 'Q': g_elevation += 2.0; break;
		//move camera down
	case 'w':
	case 'W': g_elevation -= 2.0; break;
		
		case 'R':
		rotateBase_degrees+=5;
		break;
		case 'r':
		rotateBase_degrees+=5;
		break;
		case 'L':
		rotateBase_degrees-=5;
		break;
		case 'l':
		rotateBase_degrees-=5;
		break;
		
	case '0':
		rotateMult = 0;
		break;
	case '1':
		rotateMult = 1;
		break;
	case '2':
		rotateMult = 10;
		break;
	case '3':
		rotateMult = 100;
		break;
	default:
	{    break;
	}
	}

	// special :: special keys
	switch (specialKEY)
	{   // camera setup
		// check if it is moving the view to look left
	case GLUT_KEY_LEFT:
	{
		g_viewAngle -= 4.0;
		// calculate camera rotation angle radians
		rad = float(3.14159 * g_viewAngle / 180.0f);
		break;
	}
	// check if it is moving the view to look right
	case GLUT_KEY_RIGHT:
	{
		g_viewAngle += 4.0;
		// calculate camera rotation angle radians
		rad = float(3.14159 * g_viewAngle / 180.0f);
		break;
	}
	// pressing keys Up/Down, update coordinates "x" and "z"
	// based on speed and angle of view.
	case GLUT_KEY_UP:
	{
		g_playerPos[2] += sin(rad) * DEFAULT_SPEED;
		g_playerPos[0] += cos(rad) * DEFAULT_SPEED;
		break;
	}
	case GLUT_KEY_DOWN:
	{
		g_playerPos[2] -= sin(rad) * DEFAULT_SPEED;
		g_playerPos[0] -= cos(rad) * DEFAULT_SPEED;
		break;
	}
	default:
	{   break;
	}
	}
}
//=========================================================//
//=========================================================//
static void special(int key, int x, int y)
{
	char letter = ' ';

	move_camera(key, letter);

	glutPostRedisplay();
}

//=========================================================//
//=========================================================//
static void idle(void)
{
	glutPostRedisplay();
}
//=========================================================//
//=========================================================//
void update_camera()
{
	// don't allow the player to wander past the "edge of the world"
	if (g_playerPos[0] < -(WORLD_SIZE - 50))
		g_playerPos[0] = -(WORLD_SIZE - 50);
	if (g_playerPos[0] > (WORLD_SIZE - 50))
		g_playerPos[0] = (WORLD_SIZE - 50);
	if (g_playerPos[2] < -(WORLD_SIZE - 50))
		g_playerPos[2] = -(WORLD_SIZE - 50);
	if (g_playerPos[2] > (WORLD_SIZE - 50))
		g_playerPos[2] = (WORLD_SIZE - 50);

	g_playerPos[1] = g_elevation;

	// calculate the player's angle of rotation in radians
	float rad = float(3.14159 * g_viewAngle / 180.0f);
	// use the players view angle to correctly set up the view matrix
	g_lookAt[0] = float(g_playerPos[0] + 100 * cos(rad));
	g_lookAt[2] = float(g_playerPos[2] + 100 * sin(rad));

	rad = float(3.13149 * g_elevationAngle / 180.0f);

	g_lookAt[1] = float(g_playerPos[1] + 100 * sin(rad));

	// clear the modelview matrix
	glLoadIdentity();

	// setup the view matrix
	gluLookAt(g_playerPos[0], g_playerPos[1], g_playerPos[2],
		g_lookAt[0], g_lookAt[1], g_lookAt[2],
		0.0, 1.0, 0.0);
}
//=========================================================//
//=========================================================//
void setup_sceneEffects(void)
{
	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// enable using glColor to change material properties
	// we'll use the default glColorMaterial setting (ambient and diffuse)
	glEnable(GL_COLOR_MATERIAL);

	// set the default blending function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set up the fog parameters for reflections
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, -100.0);
	glFogf(GL_FOG_END, 100.0);

	// enable line anti-aliasing and make the lines slightly bigger than default
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.5f);
}
//=========================================================//
//=========================================================//
void init_dados(void)
{
	setup_sceneEffects();
	// create a normal quadric (uses default settings)
	g_normalObject = gluNewQuadric();

	g_wireframeObject = gluNewQuadric();
	gluQuadricDrawStyle(g_wireframeObject, GLU_LINE);

	// create an object that uses flat shading
	g_flatshadedObject = gluNewQuadric();
	gluQuadricNormals(g_flatshadedObject, GLU_FLAT);

}
//=========================================================//
//=========================================================//
void cleanUP_data(void)
{  // delete all quadratic objects
	if (g_normalObject)
		gluDeleteQuadric(g_normalObject);
	if (g_wireframeObject)
		gluDeleteQuadric(g_wireframeObject);
	if (g_flatshadedObject)
		gluDeleteQuadric(g_flatshadedObject);
}
//=========================================================//
//=========================================================//
int main(int argc, char *argv[])
{

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(40, 40);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("A385 Computer Graphics - UAA - Professor Adriano Cavalcanti");

	init_dados();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutIdleFunc(idle);

	// environment background color
	//glClearColor(0.9,0.9,0.7,1);//(1,1,1,1);
	glClearColor(0, 0, 0.3, 1);
	// deepth efect to objects
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// light and material in the environment
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glutMainLoop();
	cleanUP_data();

	return EXIT_SUCCESS;
}
//=========================================================//
//=========================================================//
