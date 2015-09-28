//=========================================================//
//=========================================================//
//
// Supporting library
// http://www.wikihow.com/Install-Mesa-%28OpenGL%29-on-Linux-Mint
//
//=========================================================//
//=========================================================//
// WINDOW ONLY
//#include <windows.h>					// included in all Windows apps
//#include <winuser.h>          // Windows constants
// for windows GL is in small letters
//#include <gl/gl.h>						// OpenGL include
//#include <gl/glu.h>
//#include <GL/openglut.h>
// for linux GL is in capital letters
//=========================================================//
//=========================================================//

// LINUX or UNIX ONLY
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
//=========================================================//
//=========================================================//
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//=========================================================//
//=========================================================//
// person position in the environment
void move_camera(int specialKey,char normalKey);
void update_camera();

//functions



GLdouble  g_playerPos[] = { 0, 1, 15.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 0.0;
GLfloat	  g_elevation = 4.0;
GLfloat   change_collor = 1.0;
float rad =0;
const float DEFAULT_SPEED   = 0.4f;

//Snowflake variables
GLfloat height = 10;
GLfloat fallSpeed = .05;
//=========================================================//
//=========================================================//
GLvoid  DrawGround();
GLvoid  DrawGun();
GLvoid  drawSensor();
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
GLUquadricObj *g_normalObject     = NULL;
GLUquadricObj *g_wireframeObject  = NULL;
GLUquadricObj *g_flatshadedObject = NULL;
void cleanUP_data(void);

const int   WORLD_SIZE = 250;
//=========================================================//
//=========================================================//
static void text_onScreen(int row, int col, const char *fmt, ...);
//=========================================================//
//=========================================================//
typedef struct
{
    const char * const name;
    void (*solid) (void);
    void (*wire)  (void);
} entry;
//=========================================================//
//=========================================================//
static void text_onScreen (int row, int col, const char *fmt, ...)
{
    static char buf[256];
    int viewport[4];
    //void *font = GLUT_BITMAP_9_BY_15;
    va_list args;

    va_start(args, fmt);
    (void) vsprintf (buf, fmt, args);
    va_end(args);

    glGetIntegerv(GL_VIEWPORT,viewport);

    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0,viewport[2],0,viewport[3],-1,1);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
//=========================================================//
//=========================================================//
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if( ar > .5 )
        glFrustum( -ar, ar, -1.0, 1.0, 2.0, 100.0 );
    else
        glFrustum( -1.0, 1.0, -1/ar, 1/ar, 2.0, 100.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
//=========================================================//
//=========================================================//
GLvoid DrawGround()
{ // enable blending for anti-aliased lines
  glEnable(GL_BLEND);

  // set the color to a bright blue
  glColor3f(0.5f, 0.7f, 1.0f);

  // draw the lines
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

  // turn blending off
  glDisable(GL_BLEND);
} // end DrawGround()
//=========================================================//
//=========================================================//
GLvoid drawCollision()
{   GLfloat alphaTransparency = 0.5;
	GLfloat distance;

	// move forward and backward
	p1_x+=0.05f*change_direction;
	p2_x-=0.05f*change_direction;

	if(p1_x > 2){
		change_direction=1.0f-2.0f;
	}
	if(p1_x < -2){
		change_direction=1.0f;
	}
	// check-collision
	distance = sqrt(( (p1_x - p2_x) * (p1_x - p2_x) )
			         + ((p1_y - p2_y) * (p1_y - p2_y))
			         + ((p1_z - p2_z) * (p1_z - p2_z)));
	if (distance <= p2_radius + p1_radius){
	  // Red :: collision
	  change_collor = 0.0;
	}else{
	  // Yellow :: no collision
	  change_collor = 1.0;
	}
    // enable blending for transparent sphere
    glEnable(GL_BLEND);     // Turn Blending On
    glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
	   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  // first sphere collides against the other
		  glPushMatrix();
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
			glColor4f(1.0f,change_collor,0.0f,alphaTransparency);
			glTranslatef(p1_x, p1_y, p1_z);
			gluSphere(g_normalObject, p1_radius, 16, 10);
		  glPopMatrix();
		  // second sphere collides against the first
		  glPushMatrix();
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
			glColor4f(1.0f,change_collor,0.0f,alphaTransparency);
			glTranslatef(p2_x, p2_y, p2_z);
			gluSphere(g_normalObject, p2_radius, 16, 10);
		  glPopMatrix();

	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
}
//=========================================================//

GLvoid  DrawGun(){
	GLfloat alphaTransparency = 1.0;

    glEnable(GL_BLEND);     // Turn Blending On
    glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
	   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  glPushMatrix();
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
			glColor4f(1.0f,1.0f,0.0f,alphaTransparency);
			glTranslatef(1, 1, 1);
			gluSphere(g_normalObject, 1.0,4,4);
		  glPopMatrix();
	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On

}


//This function creates the sensor that is used for collision
//
GLvoid drawSensor(bool isTransparent){
	float radius = .4f;
	GLfloat alphaTransparency = 0.5;

	if(isTransparent) alphaTransparency = 0.0;

    glEnable(GL_BLEND);     // Turn Blending On
    glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
	   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  // first sphere collides against the other
		  glPushMatrix();
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
			glColor4f(1.0f,1.0f,0.0f,alphaTransparency);
			glTranslatef(1, 1, 1);
			gluSphere(g_normalObject, radius, 16, 10);
		  glPopMatrix();
	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On

}

//Draws light relative to poisition of the camera
GLvoid drawLight(int dx, int dy, int dz){
	float radius = .5f;
	GLfloat alphaTransparency = 0.5;

    glEnable(GL_BLEND);     // Turn Blending On
    glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
	   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  // first sphere collides against the other
		  glPushMatrix();
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
			glColor4f(1.0f,1.0f,0.0f,alphaTransparency);
			glTranslatef(dx,dy,dz);
			gluSphere(g_normalObject, radius, 16, 10);
		  glPopMatrix();
	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On

}

GLvoid drawRobot(void){
  srand(300); //randomize srand again
  glPushAttrib(GL_CURRENT_BIT);  // save the existing color properties
  glEnable(GL_BLEND);  // enable blending to get anti-aliased lines.

//=========================================================//
//			START DRAWING ROBOT
//=========================================================//
/*
  glPushMatrix(); //example cylinder
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3f(0.3,0.6,0.8);
    glTranslatef(-150.0, 0.5, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(g_wireframeObject, 0.0, 0.5, 3.0, 32, 4);
  glPopMatrix();
*/

  glPushMatrix();
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3f(0.3,0.6,0.8);
    glTranslatef(1, 1, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(g_normalObject, 0.0, 0.5, 3.0, 32, 4);
  glPopMatrix();

	drawSensor(true);
	drawLight(1, 1, 0);
	DrawGun();
//=========================================================//
//			STOP DRAWING ROBOT
//=========================================================//
  glDisable(GL_BLEND);

  // restore previous attributes
  glPopAttrib();

}


//=========================================================//
GLvoid drawCone(void)
{   GLfloat alphaTransparency = 0.5;
	// a transparency example: green cylinder
	// enable blending for transparent cylinder
	glEnable(GL_BLEND);     // Turn Blending On
	glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		  glPushMatrix();
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
			glColor4f(0.0f,1.0f,0.0f,alphaTransparency);
			glTranslatef(100.0, 0.0, -1.0);
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluCylinder(g_normalObject, 1.0, 0.0, 3.0, 32, 4);
		  glPopMatrix();

	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
}
//=========================================================//
GLvoid DrawTree(GLfloat dx, GLfloat dy, GLfloat dz){
	int trunkScale = 2;

	glEnable(GL_BLEND);     // Turn Blending On
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  glPushMatrix();
			//position the tree
			glTranslatef(dx, dy, dz);

			//tree base
			glPushMatrix();
				glTranslatef(0,0.5/trunkScale,0);
				glScalef(1.0/trunkScale,1.0/trunkScale,1.0/trunkScale);
				glColor3f(0.36f,.25f,0.2f);  //brown
				glRotatef(-90,1,0,0);
				gluCylinder(g_normalObject, 1.0, 1.0, 3.0, 32, 4);
			glPopMatrix();

			//leaves
			glPushMatrix();
				glTranslatef(0,.5,0);

				glPushMatrix();  //level 1
					glTranslatef(0,1,0);
					glColor3f(0.34f,.49f,0.23f);
					glRotatef(-90,1,0,0);
					gluCylinder(g_normalObject, 1.0, 0.0, 3.0, 32, 4);
				glPopMatrix();

				glPushMatrix();  //level 2
					glTranslatef(0,2,0);
					glColor3f(0.34f,.49f,0.23f);
					glRotatef(-90,1,0,0);
					gluCylinder(g_normalObject, 1.0, 0.0, 3.0, 32, 4);
				glPopMatrix();

				//level 2
			glPopMatrix();

		  glPopMatrix();
	glDisable(GL_BLEND);        // Turn Blending Off
}

GLvoid DrawTrees(){

	//foreground trees
	DrawTree(1.0f,0.0f,0.0f);
	DrawTree(1.0f,0.0f,5.0f);


	//background trees
	DrawTree(-6.0f,0.0f,0.0f);
	DrawTree(-1.0f,0.0f,4.0f);
	DrawTree(0.0f,0.0f,-12.0f);
}

GLvoid DrawSnowflake(GLfloat dx, GLfloat dy, GLfloat dz){
	glEnable(GL_BLEND);     // Turn Blending On
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  glPushMatrix();
			//position snowflake
			glTranslatef(dx,height,dz);
			glColor4f(.8f, .79f, .79f, .3);

			gluSphere(g_normalObject, 1,4,4);

		  glPopMatrix();
	glDisable(GL_BLEND);        // Turn Blending Off
}

GLvoid SnowflakeEmmitter(){
	glPushMatrix();
		//position center of snow flake emmitter
		glTranslatef(0,0,-5);

		DrawSnowflake(0,0,0);

	glPopMatrix();
}


//=========================================================//
GLvoid DrawNormalObjects(GLfloat rotation)
{
  // make sure the random color values we get are the same every time
  srand(200);

  // save the existing color properties
  glPushAttrib(GL_CURRENT_BIT);


//=========================================================//
//						START Drawing Scene
//=========================================================//

  DrawTrees();
  SnowflakeEmmitter();




//  drawCollision();
//  drawRobot();

//  drawCone();

//=========================================================//
//						END Drawing Scene
//=========================================================//

  // restore the previous color values
  glPopAttrib();
} // end DrawNormalObjects()
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
    // it's increased by one every frame
    rotation += 1.0;

	//snowflake
	//fall
		height = height - fallSpeed;

	//reset height, move to new position
		if (height <= 0) height = 10;
	//end snowflake
	//time += 1.0;
    // and ranges between 0 and 360
    if (rotation > 360.0)
    rotation = 0.0;
    // draw all of our objects in their normal position
//    DrawWireframeObjects(rotation);
    DrawNormalObjects(rotation);

    //glDisable(GL_LIGHTING);
    glColor3d(0.1,0.1,0.4);

    text_onScreen(0, 3, "Prof. Adriano Cavalcanti / A385 Computer Graphics / UAA");
    text_onScreen(2, 3, "- look up or down: A / Z");
    text_onScreen(3, 3, "- look right/left: arrows ->/<-");
    text_onScreen(4, 3, "- walk forward/backward: arrows UP/Down");

    DrawGround();

    glutSwapBuffers();
}
//=========================================================//
//=========================================================//
static void keyboard(unsigned char key, int x, int y)
{ int number=-1;

    move_camera(number,key);

    glutPostRedisplay();
}
//=========================================================//
//=========================================================//
void move_camera(int specialKEY,char normalKEY)
{
    // keyboard :: normal keys
    switch(normalKEY)
    {   // looking up
        case 'A':
        case 'a':    g_elevationAngle += 2.0; break;
       // looking down
        case 'Z':
        case 'z':g_elevationAngle -= 2.0;  break;
		//move camera up
		case 'q':
		case 'Q': g_elevation +=2.0; break;
		//move camera down
		case 'w':
		case 'W': g_elevation -=2.0; break;
		

        default:
        {    break;
        }
    }

    // special :: special keys
    switch(specialKEY)
    {   // camera setup
        // check if it is moving the view to look left
        case GLUT_KEY_LEFT:
        {
            g_viewAngle -= 2.0;
            // calculate camera rotation angle radians
            rad =  float(3.14159 * g_viewAngle / 180.0f);
            break;
        }
        // check if it is moving the view to look right
        case GLUT_KEY_RIGHT:
        {
             g_viewAngle += 2.0;
            // calculate camera rotation angle radians
            rad =  float(3.14159 * g_viewAngle / 180.0f);
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
{   char letter=' ';

    move_camera(key,letter);

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
    if (g_playerPos[0] < -(WORLD_SIZE-50))
    g_playerPos[0] = -(WORLD_SIZE-50);
    if (g_playerPos[0] > (WORLD_SIZE-50))
    g_playerPos[0] = (WORLD_SIZE-50);
    if (g_playerPos[2] < -(WORLD_SIZE-50))
    g_playerPos[2] = -(WORLD_SIZE-50);
    if (g_playerPos[2] > (WORLD_SIZE-50))
    g_playerPos[2] = (WORLD_SIZE-50);

	g_playerPos[1] = g_elevation;

  // calculate the player's angle of rotation in radians
    float rad =  float(3.14159 * g_viewAngle / 180.0f);
    // use the players view angle to correctly set up the view matrix
    g_lookAt[0] = float(g_playerPos[0] + 100*cos(rad));
    g_lookAt[2] = float(g_playerPos[2] + 100*sin(rad));

    rad = float (3.13149 * g_elevationAngle / 180.0f);

    g_lookAt[1] = float (g_playerPos[1] + 100 * sin(rad));

    // clear the modelview matrix
    glLoadIdentity();

    // setup the view matrix
    gluLookAt(g_playerPos[0], g_playerPos[1], g_playerPos[2],
              g_lookAt[0],    g_lookAt[1],    g_lookAt[2],
              0.0,            1.0,            0.0);
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
    glutInitWindowSize(640,480);
    glutInitWindowPosition(40,40);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    init_dados();

    glutCreateWindow("A385 Computer Graphics - UAA - Professor Adriano Cavalcanti");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutIdleFunc(idle);

    // environment background color
    glClearColor(0.9,0.9,0.7,1);//(1,1,1,1);
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

