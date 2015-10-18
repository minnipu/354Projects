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
#include <iostream>
//=========================================================//
//=========================================================//
// person position in the environment
void move_camera(int specialKey,char normalKey);
void update_camera();

using namespace std;

float rad = 0;
float DEFAULT_SPEED = .4f;
//functions
GLdouble  g_playerPos[] = { 0, 1, 15.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 0.0;
GLfloat	  g_elevation = 4.0;
GLfloat   change_collor = 1.0;
GLfloat	  rot = 0.0;
GLfloat	  myRot = 0.0;
float	  speed = 0.f;
int		  reverse = 1;
//=========================================================//
//=========================================================//
GLvoid  DrawNormalObjects(GLfloat rotation);
GLvoid  DrawWireframeObjects(GLfloat rotation);
GLvoid  DrawFlatshadedObjects(GLfloat rotation);
//=========================================================//
//=========================================================//
GLvoid DrawTestBody();
GLvoid DrawTestJoint(float length);
GLvoid DrawLeg(float length, float LegRot);

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

struct D2Vector{
	float x;
	float y;

	//default and paramterized constructor
	D2Vector(float x=0, float y=0) : x(x), y(y){
	}

	//
	D2Vector operator+(const D2Vector v) const {
		return D2Vector(v.x + x, v.y + y);
	}

	friend std::ostream& operator<< (std::ostream& stream, const D2Vector& v){
		stream << "x: " << v.x << "\t\ty:" << v.y;
		return stream;
	}

};

//vector functions
float 		D2VectorLength(D2Vector v);		//give the vector length
D2Vector 	D2VectorUnit(D2Vector v);		//give the unit direction
float		D2VectorDot(D2Vector v1, D2Vector v2);

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

//position of body in call
GLvoid DrawTestBody(){
	float bodyScale = 3.f;
	float allScale = .4f;
	glScalef(allScale,allScale,allScale);
	glPushMatrix();
		//draw body
		glPushMatrix();
			glScalef(bodyScale,bodyScale,bodyScale);
			gluSphere(g_normalObject, 1., 32, 10);
		glPopMatrix();

		//comment for manual using [ and ]
		rot = myRot;
		glPushMatrix();
			glTranslatef(2,-2,1);	//set position of rotation
			glRotatef(90,0,0,1);
			glRotatef(-90,0,1,0);
			glRotatef(rot,0,1,0);	//set rotation variable
			DrawLeg(3,rot);		//draw limb
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-2,-2,1);	//set position of rotation
			glRotatef(90,0,0,1);
			glRotatef(-90,0,1,0);
			glRotatef(-rot,0,1,0);	//set rotation variable
			DrawLeg(3,-rot);		//draw limb
		glPopMatrix();
				
		
	glPopMatrix();
}

GLvoid DrawLeg(float length, float LegRot){
	glPushMatrix();
		DrawTestJoint(length);
		glTranslatef(0,0,length);
		glRotatef(-20 + LegRot/2,0,1,0);
		DrawTestJoint(length);
	glPopMatrix();
}

GLvoid DrawTestJoint(float length){
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
		glColor3f(0,0,1);
		gluCylinder(g_normalObject, 1.,1.0,length, 32, 10);
		glPushMatrix();
			glColor3f(1,0,0);
			glTranslatef(0,0,length);
			gluSphere(g_normalObject, 1, 32, 10);
		glPopMatrix();
		glPushMatrix();
			glColor3f(0,1,0);
			gluSphere(g_normalObject, 1, 32, 10);
		glPopMatrix();
	glPopMatrix();
	glPopAttrib();
}

float D2VectorLength(D2Vector v){
	return sqrt((v.x*v.x) + (v.y*v.y));
}

D2Vector D2VectorUnit(D2Vector v){
	D2Vector tmp;
	float len = D2VectorLength(v);
	tmp.x = v.x/len;
	tmp.y = v.y/len;
	return v;
}

float D2VectorDot(D2Vector v1, D2Vector v2){
	return (v1.x*v2.x) + (v1.y*v2.y);
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
	glPushMatrix();
		//position body
		glTranslatef(3,3,0);
		DrawTestBody();
	glPopMatrix();

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

	//put update values here
	if(myRot > 20 || myRot < -20) reverse = -reverse;	

	myRot += reverse * speed;

    DrawNormalObjects(rotation);

    //glDisable(GL_LIGHTING);
    glColor3d(0.1,0.1,0.4);

    text_onScreen(0, 3, "Dustin Mendoza / A385 Computer Graphics / UAA");
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
		case '[': rot +=4.0; break;
		case ']': rot -=4.0; break;
		case '1': speed = 0; break;
		case '2': speed = 1; break;
		case '3': speed = 2; break;
		case '4': speed = 3; break;

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

	D2Vector v1(1,1), v2(1,4);

	//cout << v1.x << " " << v1.y << endl;

	cout << "v1 is: " << v1 << endl;
	cout << "v2 is: " << v2 << endl;
	cout << "v1 dot v2 is: " << D2VectorDot(v1,v2) << endl;
	cout << "v1 + v2 is: \t" << v1 + v2 << endl;
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

