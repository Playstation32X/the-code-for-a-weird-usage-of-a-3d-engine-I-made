// simple sub-atomic physics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<iostream> 
#define GLEW_STATIC
#include<glew.h> 
#include<glut.h>  
#include<glu.h>
#include<math.h> 
#include<imgui.h>
#define RADIATION 3.14/180 
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"   

#include<al.h>
#include<alc.h>   
#define DR_WAV_IMPLEMENTATION
#include"dr_wav.h"  

float x, y, z; 
float Yrotation, Xrotation;  
float physX, physY, physZ=-50; 
float physangle;
float itemangle = 0; float clockangle2; float pendulumnANGLE; 
bool velocimaybe; 
bool startscaling;
float ballrotation; 
float ballsize=0.5;
int col1 = 1, col2 = 1, col3 = 1;
GLuint floorTex;
GLuint itemTex;
GLuint ceilingTex; 
GLuint walltex1; 
GLuint sprite1; 
GLuint sprite2; 
GLuint sprite3;
struct Motion
{
    bool Forward, Backward, Left, Right, upward, down;
};

bool yescollision(float px, float pz, float py,float x1,float x2,float y1,float y2,float z1, float z2)
{
    return (px>x1&&px<x2&&py>y1&&py<y2&&pz>z1&&pz<z2);
}

bool teleportpickup;


Motion motion = { false, false, false, false, false, false };

GLuint loadtex(const char* filename)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint textureID;  
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
     

    int texW, texH, nrchannels; 
    unsigned char* data = stbi_load(filename, &texW, &texH, &nrchannels,4); 

    if (!data)
    {
        std::cerr << "[ERROR] Failed to load texture: " << filename << std::endl;
        std::cerr << "Ensure the texture file exists and the path is correct!" << std::endl;
        exit(1); // Exit the program if texture fails to load
    }


    GLenum format = (nrchannels == 4) ? GL_RGBA :
        (nrchannels == 3) ? GL_RGB :
        (nrchannels == 1) ? GL_RED : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    std::cout << "Loaded texture: " << filename << std::endl;

    return textureID;
}

void Item()
{    
    if (teleportpickup == true)
    {
        glPushMatrix();
       
        glColor3f(col1, col2, col3);
       
        glTranslatef(0.5f, -0.5, -1.2f); // in front of camera, adjust as needed
       
        glScalef(0.5, 0.7, 2.5);

        glutWireCube(0.7);
        glPopMatrix();
    }
    
} 

void conveyor()
{
    glPushMatrix();
    
    glTranslatef(-100, -1.5, 20); 
    glRotatef(90, 0, 0, 1);
    glColor3f(1, 0, 1);
    glScalef(1, 25, 1);
    glutWireCube(1);

    glPopMatrix();
}

void machine1(float angle)
{
    glPushMatrix();   

    glTranslatef(-7, 0, 20); 
    glRotatef(angle, 0, 0, 1); 
    glColor3f(0, 0, 1);
    glScalef(1, 5, 1);
    glutWireCube(1); 
   
    glPopMatrix();
}
 
void testOBJ()
{
    glPushMatrix();  
    glTranslatef(4,0,-7); 
    glBegin(GL_POLYGON);
    
    glColor3f(0, 1, 0);  glVertex3f(1,0,1);
    glColor3f(1, 0, 0);   glVertex3f(-1,0,1);
    glColor3f(0, 0, 1);  glVertex3f(0,1,1);
    glEnd();

    glPopMatrix();
}

void scaleball()
{
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(0.5f, -0.5, -1);
    glScalef(ballsize, ballsize, ballsize - ballsize/2);
    glutWireSphere(1, 100, 100);

    glPopMatrix();
}

void player() 
{   
    Item(); 
  //  scaleball();
    float nextX = x; 
    float nextY = y;  
    float nextZ = z;


    if (motion.Forward )
    {
        nextX += cos((Xrotation+90) * RADIATION); 
        nextZ -= sin((Xrotation+90) * RADIATION); 
       
    }  

    if (motion.Backward )
    {
        nextX -= cos((Xrotation+90) * RADIATION);
        nextZ += sin((Xrotation+90) * RADIATION);
    } 

    if (motion.Left )
    {
        Xrotation += 2;
    }

    if (motion.Right)
    {
        Xrotation -= 2;
    }

   
    if (yescollision(x, z, y, 3, 5, -1, 1, -8, -6))
    {
        std::cout << "you got teleporter\n";
        teleportpickup = true; 
        startscaling = true;
    } 

    if (!yescollision(x, z, y, 3, 5, -1, 1, -8, -6))
    {
        x = nextX; 
        z = nextZ;
    }
 
    if (yescollision(x, z, y, -9, -7, -1, 1, -14, -12))
    {
        std::cout << "wall is here\n"; 
        teleportpickup = false; 
        startscaling = false;
    } 
    if (!yescollision(x, z, y, -9, -7, -1, 1, -14, -12))
    {
        x = nextX;
        z = nextZ;
    } 

    if (yescollision(x, z, y, 21, 24, -3, 1, 67, 72))
    {
        std::cout << "I'm easy for others to forget and ignore, like you\n";
    }


    glRotatef(-Yrotation, 1, 0, 0);
    glRotatef(-Xrotation, 0, 1, 0);  
    glTranslatef(-x, -y, -z); 
    
    
  
}

void wall(float wallx,float wally,float wallz)
{
    glPushMatrix(); 
    glTranslatef(wallx,  wally, wallz);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, walltex1); 

   
    glColor3f(1, 1, 1);
     glScalef(10,12,1);

    glBegin(GL_POLYGON);

    glTexCoord2f(0, 0); glVertex3f(1, 0, 1);
    glTexCoord2f(1, 0); glVertex3f(-1, 0, 1);
    glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);
    glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
    glEnd();



    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void floor(int Y,GLuint texture)
{
    glPushMatrix();   

    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D,texture);
    glColor3f(1, 1, 1);  
    glTranslatef(0,Y, 0); 
    glScalef(1, 1, 3);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3f(-50.0, -5.0, -50.0);
    glTexCoord2f(1,0); glVertex3f(50.0, -5.0, -50.0);
    glTexCoord2f(1,1); glVertex3f(50.0, -5.0, 50.0);
    glTexCoord2f(0,1); glVertex3f(-50.0, -5.0, 50.0);
    glEnd(); 

    glDisable(GL_TEXTURE_2D); 

    glPopMatrix();
}

void sprite(GLuint texture,float sx, float sy, float sz)
{
    glPushMatrix();
    glTranslatef(sx, sy, sz);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glRotatef(Xrotation,0,1,0);
    glColor3f(1, 1, 1);
    glScalef(2, 7, 7);

    glBegin(GL_POLYGON);

    glTexCoord2f(1, 1); glVertex3f(1, 0, 1);
    glTexCoord2f(0, 1); glVertex3f(-1, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-1, 1, 1);
    glTexCoord2f(1, 0); glVertex3f(1, 1, 1);
    glEnd();



    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void mechanicalCLOCK()
{
    glPushMatrix();
    glTranslatef(20, -1, -80);
    glRotatef(90, 0, 1, 0);
    glPushMatrix(); 
 
    machine1(itemangle);    
    glTranslatef(-6, 0, 20); 
    glScalef(2, 3, 2); 
    glColor3f(0, 1, 0); 
    glTranslatef(0, 1, 0);
    glutWireCube(4); 
    glPopMatrix();
    glTranslatef(3,1.5,0);
    machine1(clockangle2);    
    glTranslatef(0, 4, 0);
    machine1(pendulumnANGLE); 
    glPopMatrix();
    
    

    glPopMatrix();
}  

float lx = 20, ly = 0, lz = 30;
void physicsBALL()
{
    glPushMatrix(); 
    glColor3f(0, 0, 1); 
    glTranslatef(physX,physY,physZ); 
    glRotatef(physangle, 1, 0, 0); 
   
    glutWireSphere(4,100,100);
  
    glPopMatrix();
}

void launcher()
{
    glPushMatrix(); 
    glColor3f(1, 0, 0); 
    glTranslatef(lx,ly,lz);
    glutWireCube(2);

    glPopMatrix();
}



void displaying()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);   
    glLoadIdentity();      
   
    player();   
   
    floor(0,floorTex);   
    conveyor();
    floor(13,ceilingTex); 
    launcher();
     testOBJ();
     wall(-8,-5,-13);  
     wall(10,-5,-100);
     physicsBALL(); 
     
     mechanicalCLOCK(); 
     sprite(sprite1,10,-2,14); 
     sprite(sprite2,-50,-2,45); 
     sprite(sprite3,23,-2,70);
    glutSwapBuffers(); 
      glFlush();
} 

void initialize() 
{    
   glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    startscaling = false;
    x = 0; 
    y = 0; 
    Yrotation = 0, Xrotation = 0; 
    velocimaybe = false;
    teleportpickup = false;

    floorTex = loadtex("C:/Users/Andrew Alexander/source/repos/opengl stuff 2/opengl stuff 2/painting2.png"); 
    itemTex = loadtex("C:/Users/Andrew Alexander/source/repos/opengl stuff 2/opengl stuff 2/gunTest.png"); 
    ceilingTex= loadtex("C:/Users/Andrew Alexander/source/repos/opengl stuff 2/opengl stuff 2/083.png"); 
    walltex1 = loadtex("C:/Users/Andrew Alexander/source/repos/opengl stuff 2/opengl stuff 2/antikythera.jpg"); 
    sprite1 = loadtex("C:/Users/Andrew Alexander/source/repos/opengl stuff 2/opengl stuff 2/metalsonic.png"); 
    sprite2 = loadtex("C:/Users/Andrew Alexander/source/repos/opengl stuff 2/opengl stuff 2/robot.png"); 
    sprite3 = loadtex("C:/Users/Andrew Alexander/source/repos/opengl stuff 2/opengl stuff 2/forgotten.png");
}
 
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   gluPerspective(60, 16 / 9, 1, 75);
   glMatrixMode(GL_MODELVIEW);
}

void clockingIN(int)
{
    glutPostRedisplay(); 
    glutTimerFunc(1000/60,clockingIN,0); 
    itemangle += 2; 
    clockangle2 += 0.033; 
    pendulumnANGLE += 0.0005;
    if (physY > -1)
    {
        physY -= 0.1;
   }  

    if (x > physX - 3 && x<physX + 3 && z>physZ - 3 && z < physZ + 3)
    {
        physX += cos((Xrotation + 90) * RADIATION)*4;
        physZ -= sin((Xrotation + 90) * RADIATION)*4; 
        physangle += 1;  
        
        
    }
  
    
    if (yescollision(x,z,y,-120,-85,-2,1,18,22))
    {
        std::cout << "conveying away\n"; 
        x += cos((Xrotation + 90) * RADIATION);
        z -= sin((Xrotation + 90) * RADIATION);
    } 

    if (yescollision(physX, physZ, physY, -120, -85, -2, 1, 18, 22))
    {
        std::cout << "conveying away\n";
        physX += cos((Xrotation + 90) * RADIATION);
        physZ -= sin((Xrotation + 90) * RADIATION);
    }
   

    if (yescollision(physX, physZ, physY, lx - 2, lx + 2, ly - 2, ly + 2, lz - 2, lz + 2))
    {
        physY += 50; 
        velocimaybe = true;
        std::cout << "ball incoming\n"; 
        ballrotation += 20;
    }  
    
    if (startscaling == true)
    {
        ballsize += 0.005;
    } 
    if (ballsize > 5)
    {
        ballsize = 2;
    }

    if(velocimaybe==true) 
    {  
        
        physangle += 2; 
        ballrotation -= 0.02;
    }

    if (ballrotation < 0)
    {
        velocimaybe = false;
    }
           
        
}

void TYPE(unsigned char key,int,int)
{   
    if (key == 'w')
    {
        motion.Forward = true;
    } 
    if (key == 's')
    {
        motion.Backward = true;
    } 

    if (key == 'a')
    {
        motion.Left=true;
    }
    if (key == 'd')
    {
        motion.Right = true;
    }

    if (teleportpickup == true)
    {
        if (key == 'e')
        {
            int distance = 90; 

            x += cos((Xrotation + 90) * RADIATION)*distance;
            z -= sin((Xrotation + 90) * RADIATION)*distance; 
           
            col1 = 0;col2 = 0; col3 = 1;
        }
    }

    glutPostRedisplay();
} 

void TYPE2(unsigned char key, int, int)
{
    if (key == 'w')
    {
        motion.Forward = false;
    }
    if (key == 's')
    {
        motion.Backward = false;
    }

    if (key == 'a')
    {
        motion.Left = false;
    }
    if (key == 'd')
    {
        motion.Right = false;
    }

    if (key == 'e')
    {
        col1 = 1, col3 = 1; col2 = 1;
    }
    glutPostRedisplay();
}

int main(int argc,char** argv)
{     
    std::cout << "Hello World!\n";
    glutInit(&argc,argv);  
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH|GLUT_STENCIL|GLUT_ALPHA);  
    glutInitWindowSize(1000, 500);
    glutInitWindowPosition(50,50);
    glutCreateWindow("educational window 3D"); 
    glewInit(); 
    initialize(); 
    glutTimerFunc(0,clockingIN,0);
    glutDisplayFunc(displaying); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(TYPE); 
    glutKeyboardUpFunc(TYPE2);
    glutMainLoop(); 
    return 0;
}
