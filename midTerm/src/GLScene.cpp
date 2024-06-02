#include <GLScene.h>
#include<GLLight.h>
#include<GLTexture.h>
#include<GLModel.h>
#include<GLInputs.h>
#include<GLParallax.h>
#include<GLPlayer.h>
#include<GLEnms.h>
#include<GLTimer.h>
#include<GLCheckCollision.h>

GLParallax *plxFloor = new GLParallax(); // Parallax of floor picture
GLParallax *plxMiddle = new GLParallax(); // Parallax of floor picture
GLParallax *plxBack = new GLParallax(); // Parallax of background picture

GLParallax *plxFloor0 = new GLParallax(); // Parallax of floor picture
GLParallax *plxMiddle0 = new GLParallax(); // Parallax of floor picture
GLParallax *plxBack0 = new GLParallax(); // Parallax of background picture

GLParallax *plxFloor1 = new GLParallax(); // Parallax of floor picture
GLParallax *plxMiddle1 = new GLParallax(); // Parallax of floor picture
GLParallax *plxBack1 = new GLParallax(); // Parallax of background picture

GLParallax *plxFront2 = new GLParallax(); // Parallax of floor picture
GLParallax *plxMiddle2 = new GLParallax(); // Parallax of floor picture
GLParallax *plxBack2 = new GLParallax(); // Parallax of background picture

GLPlayer *pl = new GLPlayer(); // Player
GLModel *filler = new GLModel(); // filler for the models
GLInputs *KbMs = new GLInputs(); // inputs
GLPlayer *hp = new GLPlayer(); // our lives

GLCheckCollision *check = new GLCheckCollision();
GLEnms *enemy[8] = {new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms()}; // enemy
GLEnms *enemy0[8] = {new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms()}; // enemy
GLEnms *enemy1[8] = {new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms()}; // enemy
GLEnms *enemy2[8] = {new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms(), new GLEnms()}; // enemy

GLScene::GLScene()
{
    //ctor

    screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    flag = 0;
    radius = 0.6;
    lives = 5;
    ground = -3.0;
    none = -1;

}

GLScene::~GLScene()
{
    //dtor
}
GLvoid GLScene::switchLevel(int flag) { // how we switch levels
    switch(flag)
    {
    case 0:
    plxFloor=plxFloor0;
    plxMiddle=plxMiddle0;
    plxBack=plxBack0;
    for(int i = 0; i < 8; i++){
        enemy[i] = enemy0[i];
    }
        break;
    case 1:
    plxFloor=plxFloor1;
    plxMiddle=plxMiddle1;
    plxBack=plxBack1;
    for(int i = 0; i < 8; i++){
        enemy[i] = enemy1[i];
    }
        break;
    case 2:
    plxFloor=plxFront2;
    plxMiddle=plxMiddle2;
    plxBack=plxBack2;
    for(int i = 0; i < 8; i++){
        enemy[i] = enemy2[i];
    }
        break;
    }
}
GLint GLScene::initGL()
{
    glShadeModel(GL_SMOOTH); // for smooth render
    glClearColor(0.0f,0.0f,0.0f,0.0f); // clear background to black
    glClearDepth(2.0f);            // depth test for layers
    glEnable(GL_DEPTH_TEST);       // activate depth test
    glDepthFunc(GL_LEQUAL);         // depth function type

    GLLight Light(GL_LIGHT0);
    Light.setLight(GL_LIGHT0);

    glEnable(GL_BLEND);             // Transparent effect of pngs
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);  //enable textures

    //level 0
    plxFloor0->parallaxInit("images/floor.png");
    plxMiddle0->parallaxInit("images/middle.png");
    plxBack0->parallaxInit("images/Back.png");

    //level 1
    plxFloor1->parallaxInit("images/floor1.png");
    plxMiddle1->parallaxInit("images/middle1.png");
    plxBack1->parallaxInit("images/Back1.png");

    //level 2
    plxFront2->parallaxInit("images/Front2.png");
    plxMiddle2->parallaxInit("images/middle2.png");
    plxBack2->parallaxInit("images/Back2.png");

    // default is level 0
    switchLevel(flag);

    // player
    pl->initPlayer(7,2, "images/sonic.png");
    pl->actionTrigger = pl->STAND;

    hp->initPlayer(7,10, "images/health.png");
    hp->actionTrigger = hp->HEALTH;
    hp->plPosition.y = -5.0;
    lives = 5;

    for (int i = 0; i < 8; i++) {
        enemy0[i]->initEnemy(1,1, "images/arrow.png",0);
        enemy1[i]->initEnemy(1,1, "images/star.png",1);
        enemy2[i] ->initEnemy(1,1, "images/hook.png",2);
    }
    // falling enemies
    vec3 spawn;
    spawn.x = 1;
    spawn.y = 1;
    spawn.z = 1;


    return true;
}

GLint GLScene::drawScene()    // this function runs on a loop
                              // DO NOT ABUSE ME
{
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);// clear bits in each itterration
   glLoadIdentity();
   glTranslatef(0.0,0.0,-15.0);
   glColor3f(0.5,1.0,1.0);     //color the object red

    glPushMatrix(); // background
        glScalef(10,10,1.0);
        //glTranslatef(0.0,.109,0.0);
        glDisable(GL_LIGHTING);
        plxBack->parallaxDraw(screenWidth, screenHeight);
        if (flag == 1)
        {
            glTranslatef(0.0,.109,0.0);
            plxBack->parallaxScroll(true,"level1",0.0008);
            pl->plPosition.y = -4.4; // grounding to certain y value
            ground = -6.0;
            radius = 0.7;
        }
        else if (flag == 2)
        {
            plxFloor->parallaxScroll(true,"down",0.008);
            plxMiddle->parallaxScroll(true,"level2",0.008);
            glTranslatef(0.0,0.0,0.0);
            pl->plPosition.y = -4.4;
            ground = -5.0;
            radius = 0.7;
        }
        else // level 0
       {
            glTranslatef(0.0,.109,0.0);
            plxBack->parallaxScroll(false,"left",0.001);
            pl->plPosition.y = -2.4;
            radius = 0.6;
            ground = -3.0;

        }
        glEnable(GL_LIGHTING);
   glPopMatrix();

    glPushMatrix(); // middleground
        glScalef(10,10,1.0);
        glTranslatef(0.0,-0.01,0.0);
        glDisable(GL_LIGHTING);
        plxMiddle->parallaxDraw(screenWidth, screenHeight);
        plxMiddle->parallaxScroll(false,"left",0.0025);
        glEnable(GL_LIGHTING);
    glPopMatrix();

   glPushMatrix(); // floor
    glScalef(10,10,1.0);
    glDisable(GL_LIGHTING);
    plxFloor->parallaxDraw(screenWidth, screenHeight);
    plxFloor->parallaxScroll(false,"left",0.005);
    glEnable(GL_LIGHTING);
   glPopMatrix();


    glPushMatrix(); //image for lives
     glDisable(GL_LIGHTING);
        hp->drawPlayer();
        hp->actions(lives);
   glPopMatrix();


   glPushMatrix(); // player
  // glScalef(10,10,-1.0);
    glDisable(GL_LIGHTING);
    pl->drawPlayer();
    pl->actions(none);
    if (pl->plPosition.x < -10.5) pl->plPosition.x = -10.2; //no going left
    if (pl->plPosition.x > 12.0) {
        pl->plPosition.x = -10.2;
        flag = (flag + 1 )% 3; // returns 0, 1, 2 for our levels
        switchLevel(flag);
        lives = 5; // reset LIVES when they proceed to the next scene
        //hp->actions(5);
    }
    if (lives <= 0) {
        pl->plPosition.x = -10.2;
        lives = 5;
    }
    glEnable(GL_LIGHTING);
   glPopMatrix();



for (int i = 0; i < 8; i++)
{
    enemy[i]->drawEnemy(flag);
//enemy[i]->actions(0);
        switch(flag)
        {
        case 0:
            enemy[i]->actions(0);
            break;
        case 1:
            enemy[i]->actions(1);
            break;
        case 2:
            enemy[i]->actions(2);
            break;
        }
    if (check->isRadialCollision({pl->plPosition.x ,pl->plPosition.y},{enemy[i]->pos.x,enemy[i]->pos.y},radius, radius, 0))
    {
        cout << "hit!" << endl;
        cout << "enemy: " << enemy[i]->pos.x<< endl;
        cout << "player " << pl->plPosition.x<< endl;
        switch(flag)
        {
        case 0:
          //  enemy[i]->actions(0);
            enemy[i] ->initEnemy(1,1, "images/arrow.png",0);
            break;
        case 1:
           // enemy[i]->actions(1);
            enemy[i] ->initEnemy(1,1, "images/star.png",1);
            break;
        case 2:
            enemy[i] ->initEnemy(1,1, "images/hook.png",2);
            break;
        }
        lives -= 1;
    }
    else if (enemy[i]->pos.y <= ground) {
        switch(flag)
        {
        case 0:
            enemy[i] ->initEnemy(1,1, "images/arrow.png",0);
            break;
        case 1:
            enemy[i] ->initEnemy(1,1, "images/star.png",1);
            break;
        case 2:
            enemy[i] ->initEnemy(1,1, "images/hook.png",2);
            break;
        }
    }
}

   return true;
}

GLvoid GLScene::resizeScene(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio = (GLfloat)width/(GLfloat)height; // keep track of the ratio
    glViewport(0,0,width,height); // adjusting the viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,aspectRatio,0.1,100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int GLScene::windMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)   // check for inputs
    {

    case WM_KEYDOWN:
         KbMs->wParam = wParam;
         KbMs->keyPress(filler, pl);
         if (flag == 2) {
         KbMs->keyBackground(plxFloor,0.01);
         KbMs->keyBackground(plxMiddle,0.001);
         KbMs->keyBackground(plxBack,0.01);
         }
         else {
         KbMs->keyBackground(plxFloor,0.01);
         KbMs->keyBackground(plxMiddle,0.0025);
         KbMs->keyBackground(plxBack,0.001);
         }
         break;

    case WM_KEYUP:
         KbMs->wParam = wParam;
         KbMs->keyUP(pl);
         break;

    case WM_LBUTTONDOWN:

         break;

    case WM_RBUTTONDOWN:

         break;

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:

        break;

    case WM_MOUSEMOVE:

         break;
    case WM_MOUSEWHEEL:


       break;
    }
}



