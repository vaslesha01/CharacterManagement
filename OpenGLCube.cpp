#include <glut.h>
#include <cstdlib>
#include <math.h>

const int FPS = 60;
const float TO_RADIANS = 3.14 / 180.0;
const int width = 1600;
const int height = 800;

float angleX = 0.0, angleY = 0.0;
float posX = 0.0, posY = 0.0, posZ = 0.0;

float velocityY = 0.0f;
bool isJumping = false;
const float GRAVITY = -0.06f;
const float JUMP_STRENGTH = 1.0f;
const float GROUND_LEVEL = 0.0f; 

void display();
void reshape(int w, int h);
void timer(int);
void passive_motion(int, int);
void camera();
void keyboard(unsigned char key, int x, int y);
void keyboard_up(unsigned char key, int x, int y);

struct Motion
{
    bool Forward, Backward, Left, Right, isJump;
};

Motion motion = { false,false,false,false,false };

void drawPlane() {
    glEnable(GL_TEXTURE_2D);
    GLuint texture;
    glGenTextures(1, &texture);

    unsigned char texture_data[2][2][4] =
    {
        0,0,0,255,  255,255,255,255,
        255,255,255,255,    0,0,0,255
    };

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_NEAREST);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);  glVertex3f(-50.0, -5.0, -50.0);
    glTexCoord2f(25.0, 0.0);  glVertex3f(50.0, -5.0, -50.0);
    glTexCoord2f(25.0, 25.0);  glVertex3f(50.0, -5.0, 50.0);
    glTexCoord2f(0.0, 25.0);  glVertex3f(-50.0, -5.0, 50.0);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camera();
    drawPlane();

    glutSwapBuffers();
}

void timer(int) {
    glutPostRedisplay();
    glutWarpPointer(width / 2, height / 2);
    glutTimerFunc(1000 / FPS, timer, 0);
}

void passive_motion(int x, int y) {

    int dev_x, dev_y;
    dev_x = (width / 2) - x;
    dev_y = (height / 2) - y;

    angleY += (float)dev_x / 10.0;
    angleX += (float)dev_y / 10.0;
}

void camera() {

    if (motion.Forward) {
        posX += cos((angleY + 90) * TO_RADIANS) / 5.0;
        posZ -= sin((angleY + 90) * TO_RADIANS) / 5.0;
    }
    if (motion.Backward) {
        posX += cos((angleY + 90 + 180) * TO_RADIANS) / 5.0;
        posZ -= sin((angleY + 90 + 180) * TO_RADIANS) / 5.0;
    }
    if (motion.Left) {
        posX += cos((angleY + 90 + 90) * TO_RADIANS) / 5.0;
        posZ -= sin((angleY + 90 + 90) * TO_RADIANS) / 5.0;
    }
    if (motion.Right) {
        posX += cos((angleY + 90 - 90) * TO_RADIANS) / 5.0;
        posZ -= sin((angleY + 90 - 90) * TO_RADIANS) / 5.0;
    }

    if (isJumping) {
        velocityY += GRAVITY;
        posY += velocityY;


        if (posY <= GROUND_LEVEL) {
            posY = GROUND_LEVEL;
            isJumping = false;
            velocityY = 0.0f;
        }
    }

    if (angleX >= 70)
        angleX = 70;
    if (angleX <= -60)
        angleX = -60;

    glRotatef(-angleX, 1.0, 0.0, 0.0); // Along X axis
    glRotatef(-angleY, 0.0, 1.0, 0.0);    //Along Y axis

    glTranslatef(-posX, -posY, -posZ);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        motion.Forward = true;
        break;
    case 'a':
        motion.Left = true;
        break;
    case 's':
        motion.Backward = true;
        break;
    case 'd':
        motion.Right = true;
        break;
    case 32:
        if (!isJumping) {
            isJumping = true;
            velocityY = JUMP_STRENGTH;
        }
        break;
    case 27: // ESC для выхода
        exit(0);
        break;
    }
}

void keyboard_up(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        motion.Forward = false;
        break;
    case 'a':
        motion.Left = false;
        break;
    case 's':
        motion.Backward = false;
        break;
    case 'd':
        motion.Right = false;
        break;
    case 27: // ESC для выхода
        exit(0);
        break;
    }
}

void init()
{
    glutSetCursor(GLUT_CURSOR_NONE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glutWarpPointer(width / 2, height / 2);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 16.0 / 9.0, 1, 75);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("OpenGL");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(passive_motion);
    glutTimerFunc(0, timer, 0);  
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);

    glutMainLoop();
    return 0;
}
