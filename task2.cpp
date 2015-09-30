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
#include <vector>

using namespace std;

//=========================================================//


struct Point{
	float x,y,z;
};

vector<Point> positions;
vector<Point> TreePos;

//=========================================================//
// person position in the environment
void move_camera(int specialKey,char normalKey);
void update_camera();

GLdouble  g_playerPos[] = { 0, 1, 15.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 0.0;
GLfloat	  g_elevation = 4.0;
GLfloat   change_collor = 1.0;
float rad =0;
const float DEFAULT_SPEED   = 0.4f;

// variables
GLfloat height = 10;
GLfloat fallSpeed = .1;
int		MaxWidth = 100;
int 	numSnowflakes = 5000;
float	snowFlakeScale = .1f;
float rotateBase_degrees=0;
float rotWheel1 = 0;
float rotWheel2 = 0;
float rotWheel3 = 0;
float windSpeed = 0;
float windWheelScale = .5f;
int		numTrees = 500;
//=========================================================//
//=========================================================//
GLvoid  DrawGround();

//New functions
GLvoid  DrawTrees();
GLvoid  DrawTree(GLfloat dx, GLfloat dy, GLfloat dz, float scale);
GLvoid  DrawSnowflake(GLfloat dx, GLfloat dy, GLfloat dz);
GLvoid  SnowflakeEmmitter(GLfloat dx, GLfloat dy, GLfloat dz);
GLvoid	DrawWindwheel(GLfloat dx, GLfloat dy, GLfloat dz, float scale);
GLvoid	DrawSnowman(GLfloat x, GLfloat y, GLfloat z);


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

  // set the to a bright blue
//  glColor3f(0.5f, 0.7f, 1.0f);


	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0, 0, -100.0);
	glVertex3f(-100.0, 0, 100.0);
	glVertex3f(100.0, 0, 100.0);
	glVertex3f(100.0, 0, -100.0);
	glEnd();

/*
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
*/
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
GLvoid DrawTree(GLfloat dx, GLfloat dy, GLfloat dz, float scale){
	int trunkScale = 2;

	glEnable(GL_BLEND);     // Turn Blending On
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  glPushMatrix();
			//position the tree
			glTranslatef(dx, dy, dz);

			glScalef(scale,scale,scale);
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
	DrawTree(-4.0f,0.0f,-2.0f, 1);
	DrawTree(7.0f,0.0f,3.0f, 1.2);


	//background trees
	DrawTree(-6.0f,0.0f,0.0f, .8);
	DrawTree(4.0f,0.0f,-4.0f, .9);

	Point p;
	for(int i = 0; i < TreePos.size(); i++){
		p = TreePos.at(i);

		DrawTree(p.x,p.y,p.z,1 + (rand() % 1000 - 500)/1000.0);
//		DrawTree(p.x,p.y,p.z,1);
	}

}

GLvoid DrawSnowflake(GLfloat dx, GLfloat dy, GLfloat dz){
	glEnable(GL_BLEND);     // Turn Blending On
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  glPushMatrix();
			glTranslatef(dx,dy,dz); 		//position snowflake
			glScalef(snowFlakeScale, snowFlakeScale, snowFlakeScale);
			glColor4f(1, 1, 1, 1);
			gluSphere(g_normalObject, 1,4,4);

		  glPopMatrix();
	glDisable(GL_BLEND);        // Turn Blending Off
}

GLvoid SnowflakeEmmitter(GLfloat dx, GLfloat dy, GLfloat dz){
	//draw each snowflake from vector
	glPushMatrix();
		//position center of snow flake emmitter
		glTranslatef(dx,dy,dz);

		Point p;
		for(int i = 0; i < positions.size(); i++){
			p = positions.at(i);
			DrawSnowflake(p.x,p.y,p.z);
		}

	glPopMatrix();
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

GLvoid DrawWindwheel(GLfloat dx, GLfloat dy, GLfloat dz,float scale){
	glPushMatrix();
		glTranslatef(dx,dy,dz);
       glRotatef ((GLfloat) rotateBase_degrees, 0.0, 1.0, 0.0); // rotate
		glScalef(scale,scale,scale);
       glTranslatef (0.0, 0, 3);//define central base of rotation
       glTranslatef (0.0, -0.3, 0.0);//define central base of rotation

		//windwheel base
       glPushMatrix();
		    glColor3d(.5,.5,.5);
			glTranslatef(0,-.75,-3);
			glScalef(.5,2,.5);
			glutSolidCube(1);
       glPopMatrix();

		glTranslatef(0,.2,0); //center of petal

		//windwheels connects to this center post
		glPushMatrix();
			glScalef(.1,.1,1);
			//quad radius, slices, stacks;
			gluSphere(g_normalObject, 1.0,32,8);

			glTranslatef(0,0,-3);			
			//quad, base, top, height, slices, stacks
			gluCylinder(g_normalObject, 1.0, 1.0, 3.0, 32, 4);

			gluSphere(g_normalObject, 1.0,32,8);

		glPopMatrix();

		//wind wheel variables
		float insetAngle = 15;
		float twistAngle = -5;
		int numPetals = 18;
		float length = 1;

		//WindWheel 1
		glPushMatrix();
		    glColor3d(0,1,1);
			glTranslatef(0,0,1);

			//rotate all petals
	    	glRotatef ((GLfloat) rotWheel1, 0.0, 0.0, 1.0); // rotate

			for(int i = 0; i < numPetals; i++){
				glPushMatrix();
					glRotatef(twistAngle, 1,0,0);
					glRotatef(insetAngle, 0,1,0);
					glTranslatef(0,length/2,-.2);
					glScalef(.3,length,.1);
					gluSphere(g_normalObject,length,6,8);
//					glutSolidCube(1);
				glPopMatrix();

				//petal 2
				glRotatef(360/numPetals, 0, 0, 1);				
			}
		glPopMatrix();
		//end wheel 1

		length = length*1.2;
		//WindWheel 2
		glPushMatrix();
		    glColor3d(1,0,1);
			glTranslatef(0,0,0);

			//rotate all petals
	    	glRotatef ((GLfloat) rotWheel2, 0.0, 0.0, 1.0); // rotate

			for(int i = 0; i < numPetals; i++){
				glPushMatrix();
					glRotatef(twistAngle, 1,0,0);
					glRotatef(insetAngle, 0,1,0);
					glTranslatef(0,length/2,-.2);
					glScalef(.3,length,.1);
					gluSphere(g_normalObject,length,6,6);
					//glutSolidCube(1);
				glPopMatrix();

				//petal 2
				glRotatef(360/numPetals, 0, 0, 1);				
			}

		glPopMatrix();
		//end Wheel 2

		length = length*1.2;
		//WindWheel 3
		glPushMatrix();
		    glColor3d(1,1,0);
			glTranslatef(0,0,-1);

			//rotate all petals
	    	glRotatef ((GLfloat) rotWheel3, 0.0, 0.0, 1.0); // rotate

			for(int i = 0; i < numPetals; i++){
				glPushMatrix();
					glRotatef(twistAngle, 1,0,0);
					glRotatef(insetAngle, 0,1,0);
					glTranslatef(0,length/2,-.2);
					glScalef(.3,length,.1);
					gluSphere(g_normalObject,length,6,6);
				glPopMatrix();
				//petal 2
				glRotatef(360/numPetals, 0, 0, 1);				
			}
		glPopMatrix();
		//end Wheel 3
	glPopMatrix();

	rotWheel1 = rotWheel1 + windSpeed*6;
	rotWheel2 = rotWheel2 + windSpeed*4;
	rotWheel3 = rotWheel3 + windSpeed*2;
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
  SnowflakeEmmitter(0,0,5);
  DrawWindwheel(4,2,3,.5);
  DrawSnowman(0,0,0);



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

	////////////////////////////////////////////
	//				SNOW Updates
	////////////////////////////////////////////

	//Update height for each snowflake (TODO)
  for(int i = 0; i < numSnowflakes; i++){
	if(positions[i].y <= 0){
		positions[i].y = 20;
		//reset position
		positions[i].x = (rand() % MaxWidth) -(MaxWidth/2);
		positions[i].z = (rand() % MaxWidth) -(MaxWidth/2);
		positions[i].x += (rand() % 1000 - 500)/1000.0;
		positions[i].z += (rand() % 1000 - 500)/1000.0;

		//move spawn position based on rotation
		positions[i].x += windSpeed*50*sin(rotateBase_degrees*M_PI/180);
		positions[i].z += windSpeed*50*cos(rotateBase_degrees*M_PI/180);
	} else{
		positions[i].y = positions[i].y - fallSpeed*(2*windSpeed + 1);
	}

	//add randomness
	positions[i].x += (rand() % 100 - 50)/10000.0;
	positions[i].z += (rand() % 100 - 50)/10000.0;

	//add wind
	positions[i].x += -windSpeed/2 * sin(rotateBase_degrees*M_PI/180);
	positions[i].z += -windSpeed/2 * cos(rotateBase_degrees*M_PI/180);
  }
	////////////////////////////////////////////
	//				END SNOW Updates
	////////////////////////////////////////////

    // and ranges between 0 and 360
    if (rotation > 360.0)
    rotation = 0.0;
    // draw all of our objects in their normal position
//    DrawWireframeObjects(rotation);
    DrawNormalObjects(rotation);

    //glDisable(GL_LIGHTING);
    glColor3d(1,1,1);

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
	    case 'l': rotateBase_degrees++; break;
		case 'L': rotateBase_degrees++; break;
		case 'r': rotateBase_degrees--; break;
		case 'R': rotateBase_degrees--; break;
		case '1': windSpeed = .25f; break;
		case '2': windSpeed = .5f; break;
		case '3': windSpeed = .75; break;
		case '0': windSpeed = 0.f; break;

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

  Point p;
  //create vector of random points
  for(int i = 0; i < numSnowflakes; i++){
	p.x = (rand() % MaxWidth) -(MaxWidth/2);
    p.y = (rand() % MaxWidth);
    p.z = (rand() % MaxWidth) -(MaxWidth/2);
	p.x += (rand() % 1000 - 500)/1000.0;
	p.y += (rand() % 1000 - 500)/1000.0;
	p.z += (rand() % 1000 - 500)/1000.0;
 
    positions.push_back(p);        

  }

  //create vector of random points
  for(int i = 0; i < numTrees; i++){
	p.x = 0;
	p.y = 0;
	p.z = 0;
	while(sqrt(p.x*p.x + p.z*p.z) <25){
		p.x = (rand() % MaxWidth) -(MaxWidth/2);
		p.z = (rand() % MaxWidth) -(MaxWidth/2);
		p.x += (rand() % 1000 - 500)/1000.0;
		p.z += (rand() % 1000 - 500)/1000.0;
	 	}
    TreePos.push_back(p);        

  }

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
    glClearColor(0,0,0,1);//(1,1,1,1);
    // deepth efect to objects
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // light and material in the environment
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glutMainLoop();
    cleanUP_data();

	Point p;
	for(int i = 0; i < 100; i++){
	  p = positions.at(i);
	  cout << "point " << i << ": " << p.x << "," << p.y << "," << p.z << endl;
	}

	cout << "hello" << endl;
    return EXIT_SUCCESS;
}
//=========================================================//
//=========================================================//

