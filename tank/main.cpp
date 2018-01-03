//!Includes
#include <GL/glew.h>                            //OpenGL Related Functions
#include <GL/glut.h>                            //OpenGL Related Functions
#include <Shader.h>                             //Functions for compiling
#include <Vector.h>                             //Functions for Vectors
#include <Matrix.h>
#include <Mesh.h>
#include <Texture.h>
#include <SphericalCameraManipulator.h>
#include <iostream>                             //Writing to the Console using std::cout
#include <math.h>                               // Includes Math functions
#include <string>
#include <fstream>
#include <unistd.h>

//!Function Prototypes
bool initGL(int argc, char** argv);             //Function to init OpenGL
void display(void);                             //OpenGL Display Function
void keyboard(unsigned char key, int x, int y); //Keyboard Function
void keyUp(unsigned char key, int x, int y);
void handleKeys();
void mouse(int button, int state, int x, int y);//Mouse Function
void motion(int x, int y);                      //Mouse Motion Function
void Timer(int value);

void initShader();		                //Function to init Shader
void initGeometry();		                //Function to init Geometry 
void drawGeometry();		                //Function to draw Geometry
void initTexture(std::string filename, GLuint & textureID);
void render2dText(std::string text, float r, float g, float b, float x, float y);
void levelup();
float toRadian(float degree);

//! Screen size
int screenWidth   	        = 720;
int screenHeight   	        = 720;
int zoom                        = 5;
int view                        = 1;

//! Array of key states
bool keyStates[256];

// Function for degree
float toRadian(float degree)
{
        return (degree*3.1415/180);
}

// Map array
int level = 1;
#define MapHeight 10
#define MapWidth 10
int map[MapHeight][MapWidth];

// Tank postition
int tanky = 0;
int tankrot = 0;
float tankscale = 0;
float tankmovex = 0;
float tankmovey = 0;
int tanktempx = 0;
int tanktempy = 0;
int turretrot = 0;
int turretreset = 0;
float ballmovex = 0;
float ballmovey = 0;
int coinrot = 0;
float t = 0;
int mousestate;

//Score
int s = 0;
int count = 1;
int shoot = 0;
int shoottime = 0;
int endgame = 0;
int countdown = 1;
int temp;
int showinfo = 0;
int totalscore = 0;

//Global Variables
GLuint shaderProgramID;			        //Shader Program ID
GLuint vertexPositionAttribute;		        //Vertex Position Attribute Location

//Material Properties
GLuint LightPositionUniformLocation;                // Light Position Uniform   
GLuint AmbientUniformLocation;
GLuint SpecularUniformLocation;
GLuint SpecularPowerUniformLocation;

Vector3f lightPosition= Vector3f(20.0,20.0,20.0);   // Light Position 
Vector3f ambient    = Vector3f(0.1,0.1,0.1);
Vector3f specular   = Vector3f(1.0,1.0,1.0);
float specularPower = 10.0;

//Viewing
SphericalCameraManipulator cameraManip;
Matrix4x4 ModelViewMatrix;		        //ModelView Matrix
GLuint MVMatrixUniformLocation;		        //ModelView Matrix Uniform
Matrix4x4 ProjectionMatrix;		        //Projection Matrix
GLuint ProjectionUniformLocation;               //Projection Matrix Uniform Location

GLuint vertexNormalAttribute;
GLuint vertexTexcoordAttribute;                 //Vertex Texcoord Attribute Location
GLuint TextureMapUniformLocation;               //Texture Map Location
GLuint texture1;
GLuint texture2;
GLuint texture3;
GLuint texture4;

//Mesh
Mesh mesh1;                                     //Mesh - Cube
Mesh mesh2;                                     //Mesh - Coin
Mesh mesh3;                                     //Mesh - Tank
Mesh mesh4;                                     //Mesh - front_wheel
Mesh mesh5;                                     //Mesh - back_wheel
Mesh mesh6;                                     //Mesh - turret
Mesh mesh7;                                     //Mesh - ball


//! Main Program Entry
int main(int argc, char** argv)
{	

	//Init OpenGL
	if(!initGL(argc, argv))
		return -1;

        //Init Key States to false;    
        for(int i = 0 ; i < 256; i++)
                keyStates[i] = false;
    
        //
        //Main program
        //

        //Init Map Array to 0
        for (int i = 0 ; i < MapHeight; i++)
        {
                for (int j = 0 ; j < MapWidth; j++) map[i][j] = 0;
        }

        //Loading level tutorial
        if (level == 1)
        {
                //Init Map array from input.txt
                using namespace std;
                ifstream file("tutorial.txt");

                for (int j = 0 ; j < MapHeight; j++)
                {
                        for (int i = 0 ; i < MapWidth; i++) file >> map[i][j];
                }
        }

        //Init OpenGL Shader
        initShader();
    
        
	//Init Mesh Geometry
        mesh1.loadOBJ("../models/cube.obj");
        mesh2.loadOBJ("../models/coin.obj");
        mesh3.loadOBJ("../models/chassis.obj");
        mesh4.loadOBJ("../models/front_wheel.obj");
        mesh5.loadOBJ("../models/back_wheel.obj");
        mesh6.loadOBJ("../models/turret.obj");
        mesh7.loadOBJ("../models/ball.obj");

        initTexture("../models/crate.bmp", texture1); 
        initTexture("../models/coin.bmp", texture2);
        initTexture("../models/hamvee.bmp", texture3);
        initTexture("../models/ball.bmp", texture4);

        //Init Camera Manipultor
	cameraManip.setPanTiltRadius(0.f, -1.0f, 5.f);
        cameraManip.setFocus(Vector3f(tankmovex, 0.9f, tankmovey));
	//cameraManip.setFocus(mesh1.getMeshCentroid());
        //cameraManip.setFocus(mesh3.getMeshCentroid());
     
        //
        // End of Set Main program
        //

        //Enter main loop
        glutMainLoop();

        //Delete shader program
	glDeleteProgram(shaderProgramID);

        return 0;
}

//! Function to Initlise OpenGL
bool initGL(int argc, char** argv)
{
	//Init GLUT
        glutInit(&argc, argv);
    
	//Set Display Mode
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	//Set Window Size
        glutInitWindowSize(screenWidth, screenHeight);
    
        // Window Position
        glutInitWindowPosition(200, 200);

	//Create Window
        glutCreateWindow("Tank Assignment");
    
        // Init GLEW
	if (glewInit() != GLEW_OK) 
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}
	
	//Set Display function
        glutDisplayFunc(display);

        //Set Colour
        glClearColor(0.2,0.6,0.7,1.0);
	
	//Set Keyboard Interaction Functions
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyUp); 

	//Set Mouse Interaction Functions
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(motion);
	glutMotionFunc(motion);

        glEnable(GL_TEXTURE_2D);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
        glEnable(GL_DEPTH_TEST);

        //Start start timer function after 100 milliseconds
        glutTimerFunc(100,Timer, 0);

	return true;
}

//Init Shader
void initShader()
{
	//Create shader
        shaderProgramID = Shader::LoadFromFile("shader.vert","shader.frag");
    
        // Get a handle for our vertex position buffer
	vertexPositionAttribute         = glGetAttribLocation(shaderProgramID,  "aVertexPosition");
	vertexNormalAttribute           = glGetAttribLocation(shaderProgramID,  "aVertexNormal");
	vertexTexcoordAttribute         = glGetAttribLocation(shaderProgramID,  "aVertexTexcoord");

        //!
	MVMatrixUniformLocation         = glGetUniformLocation(shaderProgramID, "MVMatrix_uniform"); 
	ProjectionUniformLocation       = glGetUniformLocation(shaderProgramID, "ProjMatrix_uniform"); 
	LightPositionUniformLocation    = glGetUniformLocation(shaderProgramID, "LightPosition_uniform"); 
	AmbientUniformLocation          = glGetUniformLocation(shaderProgramID, "Ambient_uniform"); 
	SpecularUniformLocation         = glGetUniformLocation(shaderProgramID, "Specular_uniform"); 
	SpecularPowerUniformLocation    = glGetUniformLocation(shaderProgramID, "SpecularPower_uniform");
        TextureMapUniformLocation       = glGetUniformLocation(shaderProgramID, "TextureMap_uniform");
}

void initTexture(std::string filename, GLuint & textureID)
{
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        //Get texture Data
        int width, height;
        char * data;
        Texture::LoadBMP(filename, width, height, data);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);

        //Cleanup data as copied to GPU
        delete[] data;
}

//! Display Loop
void display(void)
{
        //Handle keys
        handleKeys();

	//Set Viewport
	glViewport(0,0,screenWidth, screenHeight);
	if (count == 0)
        {
                glClearColor(0.2,0.7,0.2,1.0);
        }

        if (endgame == 1 || countdown <= 0)
        {
                glClearColor(1.0,0.2,0.2,0.0);
        }
        
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        //
        //Draw your scene
        //

        //Use shader
	glUseProgram(shaderProgramID);
            
        //Projection Matrix - Perspective Projection
        ProjectionMatrix.perspective(90, 1.0, 0.0001, 100.0);

        //Set Projection Matrix
        glUniformMatrix4fv(	
                ProjectionUniformLocation,      //Uniform location
                1,                              //Number of Uniforms
                false,                          //Transpose Matrix
                ProjectionMatrix.getPtr());	//Pointer to ModelViewMatrixValues
        
        int tempy;
        int tempx;
        int tempcoiny;
        int tempcoinx;

        if (shoot == 0)
        {
                //Array and position detection
                tempy = (tankmovey/2);
                tempx = (tankmovex/2);
                if (fmod(tankmovey, 2) > 1) tempy++;
                if (fmod(tankmovex, 2) > 1) tempx++;

                
                //Coin detection
                if (map[tempx][tempy] == 2)
                {
                        map[tempx][tempy] = 1;
                        s++;
                        totalscore++;
                }
        }

        if (shoot == 1)
        {
                //Array and position detection for shooting
                tempcoiny = (ballmovey/2);
                tempcoinx = (ballmovex/2);
                if (fmod(ballmovey, 2) > 1) tempcoiny++;
                if (fmod(ballmovex, 2) > 1) tempcoinx++;

                //Coin detection
                if (map[tempcoinx][tempcoiny] == 2)
                {
                        map[tempcoinx][tempcoiny] = 1;
                        s++;
                        totalscore++;
                }
        }




        count = 0;

        for (int i = 0 ; i < MapHeight; i++)
        {
                for (int j = 0 ; j < MapWidth; j++)
                {
                        if (map[i][j] == 2) count++;
                }
        }

        //Fall detection
        tankscale = 0.25f;
        if (map[tempx][tempy] == 0 || tankmovex < -1 || tankmovey < -1 || tankmovex > 19 || tankmovey > 19)
        {
                tankscale = 0.f;
                endgame = 1;
        }

        //Testing
        //printf("remaindery = %f\n", fmod(tankmovey, 2));
        //printf("remainderx = %f\n", fmod(tankmovey, 2));
        //printf("tankmovey = %f\n", tankmovey);
        //printf("tankmovex = %f\n", tankmovex);
        //printf("map at [%i][%i] is %i\n", tempy, tempx, map[tempy][tempx]);
        
        //Set Tank Movement
        if (tanky == 1)
        {
                tankmovey += 0.05 * cosf(toRadian(tankrot));
                tankmovex += 0.05 * sinf(toRadian(tankrot));
                ballmovey += 0.05 * cosf(toRadian(tankrot));
                ballmovex += 0.05 * sinf(toRadian(tankrot));
        }

        if (tanky == -1)
        {
                tankmovey -= 0.05 * cosf(toRadian(tankrot));
                tankmovex -= 0.05 * sinf(toRadian(tankrot));
                ballmovey -= 0.05 * cosf(toRadian(tankrot));
                ballmovex -= 0.05 * sinf(toRadian(tankrot));
        }
        
        
        //Shooting
        if (shoot == 1 && shoottime <= 50)
        {
                ballmovey += 0.10 * cosf(toRadian(turretrot));
                ballmovex += 0.10 * sinf(toRadian(turretrot));
                shoottime++;
        }
        if (shoottime == 50)
        {               
                ballmovey = tankmovey;
                ballmovex = tankmovex;
                shoottime = 1;
                shoot = 0;
        }

        //Rotate coin
        coinrot++;

        //Reset turret
        if (turretreset == 1 || turretreset == 2)
        {
                while (turretrot > tankrot)
                {
                        turretrot--;
                }
                
                while (turretrot < tankrot)
                {
                        turretrot++;
                }

                if (turretreset == 1) turretreset = 0;
        }
        
        //test point
        //float test = cosf(tankrot);
        printf("tanky = %i\n", tanky);
        printf("%i\n", shoot);
        printf("%i\n", shoottime);
        //printf("tankrot = %i\n", tankrot);
        //printf("test = %f\n", test);
        printf("tankmovey = %f\n", tankmovey);
        printf("tankmovex = %f\n", tankmovex);
        printf("ballmovey = %f\n", ballmovey);
        printf("ballmovex = %f\n", ballmovex);
        
        //Draw items
        //drawitem();

        //Set Colour after program is in use
	//Apply Camera Manipluator to Set Model View Matrix on GPU
        //ModelViewMatrix.toIdentity();
        for (int i = 0 ; i < MapHeight; i++)
        {
                for (int j = 0 ; j < MapWidth; j++)
                {
                        if (map[i][j] == 1 || map[i][j] == 2)
                        {
                                //-------------------------------------------------------------------Cube
                                glActiveTexture(GL_TEXTURE0);
                                glBindTexture(GL_TEXTURE_2D, texture1);
                                glUniform1i(TextureMapUniformLocation, 0);

                                Matrix4x4 cube = cameraManip.apply(ModelViewMatrix);
                                cube.translate((float)(i * 2), 0.f, (float)(j * 2));
	                        glUniformMatrix4fv(MVMatrixUniformLocation, 1, false, cube.getPtr());
                                mesh1.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
                        }

                        if (map[i][j] == 2)
                        {
                                //-------------------------------------------------------------------Coin
                                glActiveTexture(GL_TEXTURE0);
                                glBindTexture(GL_TEXTURE_2D, texture2);
                                glUniform1i(TextureMapUniformLocation, 0);

                                Matrix4x4 coin = cameraManip.apply(ModelViewMatrix);
                                coin.translate((float)(i * 2), 1.5f, (float)(j * 2));
                                coin.scale(0.5f, 0.5f, 0.5f);
                                coin.rotate((float)(coinrot), 0.f, 1, 0.f);
	                        glUniformMatrix4fv(MVMatrixUniformLocation, 1, false, coin.getPtr());
                                mesh2.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
                        }
                }
        }
        
         //-------------------------------------------------------------------Chassis
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture3);
        glUniform1i(TextureMapUniformLocation, 0);

        Matrix4x4 chassis = cameraManip.apply(ModelViewMatrix);
        chassis.translate(tankmovex, 0.9f, tankmovey);
        chassis.scale(tankscale, tankscale, tankscale);
        chassis.rotate((float)(tankrot), 0.f, 1, 0.f);
	glUniformMatrix4fv(MVMatrixUniformLocation, 1, false, chassis.getPtr());
        mesh3.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

        //-------------------------------------------------------------------Front_wheel
        Matrix4x4 front_wheel = cameraManip.apply(ModelViewMatrix);
        front_wheel.translate(tankmovex, 0.9f, tankmovey);
        front_wheel.scale(tankscale, tankscale, tankscale);
        front_wheel.rotate((float)(tankrot), 0.f, 1, 0.f);
	glUniformMatrix4fv(MVMatrixUniformLocation, 1, false, front_wheel.getPtr());
        mesh4.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

        //-------------------------------------------------------------------back_wheel
        Matrix4x4 back_wheel = cameraManip.apply(ModelViewMatrix);
        back_wheel.translate(tankmovex, 0.9f, tankmovey);
        back_wheel.scale(tankscale, tankscale, tankscale);
        back_wheel.rotate((float)(tankrot), 0.f, 1, 0.f);
	glUniformMatrix4fv(MVMatrixUniformLocation, 1, false, back_wheel.getPtr());
        mesh5.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

        //-------------------------------------------------------------------turret
        Matrix4x4 turret = cameraManip.apply(ModelViewMatrix);
        turret.translate(tankmovex, 0.9f, tankmovey);
        turret.scale(tankscale, tankscale, tankscale);
        turret.rotate((float)(turretrot), 0.f, 1, 0.f);
	glUniformMatrix4fv(MVMatrixUniformLocation, 1, false, turret.getPtr());
        mesh6.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
        
        //-------------------------------------------------------------------ball
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture4);
        glUniform1i(TextureMapUniformLocation, 0);

        Matrix4x4 ball = cameraManip.apply(ModelViewMatrix);
        ball.translate(ballmovex, 1.6f, ballmovey);
        ball.scale(0.02f, 0.02f, 0.02f);
	glUniformMatrix4fv(MVMatrixUniformLocation, 1, false, ball.getPtr());
        mesh7.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

        //------------------------------------------------------------------

        glUniform3f(LightPositionUniformLocation, lightPosition.x,lightPosition.y,lightPosition.z);
        glUniform4f(AmbientUniformLocation, ambient.x, ambient.y, ambient.z, 1.0);
        glUniform4f(SpecularUniformLocation, specular.x, specular.y, specular.z, 1.0);
        glUniform1f(SpecularPowerUniformLocation, specularPower);

        //------------------------------------------------------------------
        if (mousestate == 0)
        {
                if (view%2 == 1)
                {
                        cameraManip.setFocus(Vector3f(tankmovex, 0.9f, tankmovey));
                        cameraManip.setPanTiltRadius((float)(tankrot * 0.0175), -1.0f, (float)(zoom));
                }

                else if (view%2 == 0)
                {
                        cameraManip.setFocus(Vector3f(tankmovex, 1.8f, tankmovey));
                        cameraManip.setPanTiltRadius((float)(turretrot * 0.0175), -1.08f, 0.1f);
                }
        }
        //Call Draw Geometry Function

	//Unuse Shader
	glUseProgram(0);
        char time[100];
        char score[100];
        char coinleft[100];
        char levelnow[100];
        
        if (showinfo == 1)
        {
                render2dText("Manually Level Select(1-6)", 1.0, 1.0, 1.0, -1.0, -0.70);
                render2dText("Tank Move(WSAD)", 1.0, 1.0, 1.0, -1.0, -0.75);
                render2dText("turret Rotate(JK) Reset(L) Lock(^L)", 1.0, 1.0, 1.0, -1.0, -0.80);
                render2dText("Change View (V)", 1.0, 1.0, 1.0, -1.0, -0.85);
                render2dText("Manual View On(E) Reset(^E)", 1.0, 1.0, 1.0, -1.0, -0.90);
                render2dText("Mesh View On(Q) Reset(^Q)", 1.0, 1.0, 1.0, -1.0, -0.95);
                render2dText("Zoom In(=) Out(-)", 1.0, 1.0, 1.0, -1.0, -1.00);
        }
        
        sprintf(levelnow, "Level %i", level);
        render2dText(levelnow, 1.0, 1.0, 1.0, -1.0, 0.95);

        countdown = int(40 - t);
        sprintf(time, "Time = %i", countdown);
	render2dText(time, 1.0, 1.0, 1.0, -1.0, 0.90);

        sprintf(score, "Total Score = %i", totalscore);
        render2dText(score, 1.0, 1.0, 1.0, -1.0, 0.85);

        sprintf(coinleft, "Coin(s) Left = %i/%i", count, (count + s));
        render2dText(coinleft, 1.0, 1.0, 1.0, -1.0, 0.80);

        render2dText("Info Open(I) Close(O)", 1.0, 1.0, 1.0, -1.0, 0.75);

        if (temp ==1) render2dText("Level Completed", 1.0, 1.0, 1.0, 0.0, 0.75);
        if (temp ==2) render2dText("Game Over", 1.0, 1.0, 1.0, 0.0, 0.70);

        //reset 
        tanky = 0;

        //
        //End of Draw your scene
        //

        //Swap Buffers and post redisplay
	glutSwapBuffers();
	glutPostRedisplay();

        if (temp == 1)
        {
                sleep(1);
                level++;
                levelup();
        }

        if (temp == 2)
        {
                sleep(1);
                level = 1;
                levelup();
        }

        temp = 0;

        if (count == 0)
        {
                temp = 1;
        }

        if (endgame == 1 || countdown <= 0)
        {
                temp = 2;
        }
}

//! Keyboard Interaction
void keyboard(unsigned char key, int x, int y)
{
	//Quits program when esc is pressed
	if (key == 27)	//esc key code
	{
		exit(0);
	}
        else if(key == 'w')
	{
		std::cout << "w key pressed" << std::endl;
	}
        else if(key == 's')
	{
		std::cout << "s key pressed" << std::endl;
	}
        else if(key == 'a')
	{
		std::cout << "a key pressed" << std::endl;
	}
        else if(key == 'd')
	{
		std::cout << "d key pressed" << std::endl;
	}
        else if(key == 'j')
	{
		std::cout << "j key pressed" << std::endl;
	}
        else if(key == 'k')
	{
		std::cout << "k key pressed" << std::endl;
	}
        else if(key == 'l')
	{
		std::cout << "l key pressed" << std::endl;
	}
        else if(key == 'L')
	{
		std::cout << "L key pressed" << std::endl;
	}
        else if(key == '-')
	{
		std::cout << "- key pressed" << std::endl;
                zoom++;
	}
        else if(key == '=')
	{
		std::cout << "+ key pressed" << std::endl;
                zoom--;
	}
        else if(key == '1')
	{
		std::cout << "1 key pressed" << std::endl;
	}
        else if(key == '2')
	{
		std::cout << "2 key pressed" << std::endl;
	}
        else if(key == '3')
	{
		std::cout << "3 key pressed" << std::endl;
	}
        else if(key == '4')
	{
		std::cout << "4 key pressed" << std::endl;
	}
        else if(key == '5')
	{
		std::cout << "5 key pressed" << std::endl;
	}
        else if(key == '6')
	{
		std::cout << "6 key pressed" << std::endl;
	}
        else if(key == ' ')
	{
		std::cout << "space key pressed" << std::endl;
                shoot = 1;
	}
        else if(key == 'q')
        {
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
         else if(key == 'Q')
        {
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
         else if(key == 'e')
        {
                std::cout << "e key pressed" << std::endl;
                mousestate = 1;
        }
         else if(key == 'E')
        {
                std::cout << "E key pressed" << std::endl;
                mousestate = 0;
        }
         else if(key == 'v')
        {
                std::cout << "v key pressed" << std::endl;
                view++;
        }
         else if(key == 'i')
        {
                std::cout << "i key pressed" << std::endl;
                showinfo = 1;
        }
         else if(key == 'o')
        {
                std::cout << "o key pressed" << std::endl;
                showinfo = 0;
        }

	//Tell opengl to redraw frame
	glutPostRedisplay();
    
        //Set key status
        keyStates[key] = true;
}

//! Handle key up situation
void keyUp(unsigned char key, int x, int y)
{
        keyStates[key] = false;
}


//! Handle Keys
void handleKeys()
{
        //keys should be handled here
	if(keyStates['w'])tanky=1;
        if(keyStates['s'])tanky=-1;
        if(keyStates['a'])tankrot++;
        if(keyStates['d'])tankrot--;
        if(keyStates['j'])turretrot++;
        if(keyStates['k'])turretrot--;
        if(keyStates['l'])turretreset=1;
        if(keyStates['l'])turretreset=2;
        if(keyStates['1'])
        {
                level = 1;
                levelup();
        }
        if(keyStates['2'])
        {
                level = 2;
                levelup();
        }
        if(keyStates['3'])
        {
                level = 3;
                levelup();
        }
        if(keyStates['4'])
        {
                level = 4;
                levelup();
        }
        if(keyStates['5'])
        {
                level = 5;
                levelup();
        }
        if(keyStates['6'])
        {
                level = 6;
                levelup();
        }
}

//! Mouse Interaction
void mouse(int button, int state, int x, int y)
{       
        if (mousestate == 1) cameraManip.handleMouse(button, state,x,y);
        glutPostRedisplay(); 
}

//! Motion
void motion(int x, int y)
{
        if (mousestate == 1) cameraManip.handleMouseMotion(x,y);
        glutPostRedisplay();     
}

//! Timer Function
void Timer(int value)
{
        //Call function again after 10 milli seconds
        glutTimerFunc(10,Timer, 0);
        t += 0.015;
        glutPostRedisplay();
}

// Headup display
void render2dText(std::string text, float r, float g, float b,
float x, float y)
{
glBindTexture(GL_TEXTURE_2D, 0);
glColor3f(r,g,b);
glRasterPos2f(x, y); // window coordinates
for(unsigned int i = 0; i < text.size(); i++)
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

// Next level
void levelup()
{
        //Reset Everything
        tanky = 0;
        tankrot = 0;
        tankscale = 0;
        tankmovex = 0;
        tankmovey = 0;
        tanktempx = 0;
        tanktempy = 0;
        turretrot = 0;
        turretreset = 0;
        s = 0;
        t = 0;
        count = 1;
        countdown = 1;
        endgame = 0;
        ballmovex = 0;
        ballmovey = 0;
        shoot = 0;
        shoottime = 0;
        glClearColor(0.2,0.6,0.7,1.0);
        
        //Loading level tutorial
        if (level == 1)
        {
                totalscore = 0;
                //Init Map array from input.txt
                using namespace std;
                ifstream file("tutorial.txt");

                for (int j = 0 ; j < MapHeight; j++)
                {
                        for (int i = 0 ; i < MapWidth; i++) file >> map[i][j];
                }
        }

        //Loading level 1
        if (level == 2)
        {
                //Init Map array from input.txt
                using namespace std;
                ifstream file("input1.txt");

                for (int j = 0 ; j < MapHeight; j++)
                {
                        for (int i = 0 ; i < MapWidth; i++) file >> map[i][j];
                }
        }

        //Loading level 2
        if (level == 3)
        {
                //Init Map array from input.txt
                using namespace std;
                ifstream file("input2.txt");

                for (int j = 0 ; j < MapHeight; j++)
                {
                        for (int i = 0 ; i < MapWidth; i++) file >> map[i][j];
                }
        }

        //Loading level 3
        if (level == 3)
        {
                //Init Map array from input.txt
                using namespace std;
                ifstream file("input3.txt");

                for (int j = 0 ; j < MapHeight; j++)
                {
                        for (int i = 0 ; i < MapWidth; i++) file >> map[i][j];
                }
        }

        //Loading level 4
        if (level == 4)
        {
                //Init Map array from input.txt
                using namespace std;
                ifstream file("input4.txt");

                for (int j = 0 ; j < MapHeight; j++)
                {
                        for (int i = 0 ; i < MapWidth; i++) file >> map[i][j];
                }
        }

        //Loading level 5
        if (level == 5)
        {
                //Init Map array from input.txt
                using namespace std;
                ifstream file("input5.txt");

                for (int j = 0 ; j < MapHeight; j++)
                {
                        for (int i = 0 ; i < MapWidth; i++) file >> map[i][j];
                }
        }

        //Loading Coin Rush
        if (level == 6)
        {
                for (int i = 0 ; i < MapHeight; i++)
                {
                        for (int j = 0 ; j < MapWidth; j++) map[i][j] = 2;
                }
        }
}
