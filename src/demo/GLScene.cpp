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

    screenWidth = 1280;
    screenHeight = 720;
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
GLvoid GLScene::switchLevel(int level) {
    if (level < 0 || level > 2) throw std::invalid_argument("Level must be 0, 1 or 2");
    flag = level;
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
    plxFloor0->parallaxInit("assets/images/floor.png");
    plxMiddle0->parallaxInit("assets/images/middle.png");
    plxBack0->parallaxInit("assets/images/Back.png");

    //level 1
    plxFloor1->parallaxInit("assets/images/floor1.png");
    plxMiddle1->parallaxInit("assets/images/middle1.png");
    plxBack1->parallaxInit("assets/images/Back1.png");

    //level 2
    plxFront2->parallaxInit("assets/images/Front2.png");
    plxMiddle2->parallaxInit("assets/images/middle2.png");
    plxBack2->parallaxInit("assets/images/Back2.png");

    // default is level 0
    switchLevel(flag);

    // player
    pl->initPlayer(7,2, "assets/images/sonic.png");
    pl->actionTrigger = pl->STAND;

    hp->initPlayer(7,10, "assets/images/health.png");
    hp->actionTrigger = hp->HEALTH;
    hp->plPosition.y = -5.0;
    lives = 5;

    for (int i = 0; i < 8; i++) {
        enemy0[i]->initEnemy(1,1, "assets/images/arrow.png",0);
        enemy1[i]->initEnemy(1,1, "assets/images/star.png",1);
        enemy2[i] ->initEnemy(1,1, "assets/images/hook.png",2);
    }


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
        ++collisions;
        switch(flag)
        {
        case 0:
          //  enemy[i]->actions(0);
            enemy[i] ->initEnemy(1,1, "assets/images/arrow.png",0);
            break;
        case 1:
           // enemy[i]->actions(1);
            enemy[i] ->initEnemy(1,1, "assets/images/star.png",1);
            break;
        case 2:
            enemy[i] ->initEnemy(1,1, "assets/images/hook.png",2);
            break;
        }
        lives -= 1;
    }
    else if (enemy[i]->pos.y <= ground) {
        switch(flag)
        {
        case 0:
            enemy[i] ->initEnemy(1,1, "assets/images/arrow.png",0);
            break;
        case 1:
            enemy[i] ->initEnemy(1,1, "assets/images/star.png",1);
            break;
        case 2:
            enemy[i] ->initEnemy(1,1, "assets/images/hook.png",2);
            break;
        }
    }
}

   return true;
}

GLvoid GLScene::resizeScene(GLsizei width, GLsizei height)
{
    if (width <= 0 || height <= 0) return;
    screenWidth = width;
    screenHeight = height;
    GLfloat aspectRatio = (GLfloat)width/(GLfloat)height; // keep track of the ratio
    glViewport(0,0,width,height); // adjusting the viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,aspectRatio,0.1,100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLScene::handleHorizontalInput(int direction) {
    KbMs->direction = direction < 0 ? -1 : direction > 0 ? 1 : 0;
    KbMs->keyPress(filler, pl);
    if (flag == 2) {
        KbMs->keyBackground(plxFloor, 0.01f);
        KbMs->keyBackground(plxMiddle, 0.001f);
        KbMs->keyBackground(plxBack, 0.01f);
    } else {
        KbMs->keyBackground(plxFloor, 0.01f);
        KbMs->keyBackground(plxMiddle, 0.0025f);
        KbMs->keyBackground(plxBack, 0.001f);
    }
}

GLScene::State GLScene::state() const {
    return {flag, lives, pl->plPosition.x, pl->plPosition.y, collisions};
}
