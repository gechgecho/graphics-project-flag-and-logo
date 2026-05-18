#ifndef POLE_H
#define POLE_H

void drawFlagpole();
void drawGround();
void drawSky();
void drawCloud(float x, float y);
void renderEnvironment();

#endif       #include <GL/gl.h>
    glVertex2f(155.0f, 380.0f);
    glVertex2f(145.0f, 380.0f);

    glEnd();

    glColor3f(0.8f, 0.5f, 0.2f);
    glBegin(GL_QUADS);

    glVertex2f(148.0f, 50.0f);
    glVertex2f(152.0f, 50.0f);
    glVertex2f(152.0f, 380.0f);
    glVertex2f(148.0f, 380.0f);

    glEnd();

    glColor3f(1.0f, 0.843f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(150.0f, 380.0f);

    for(int i = 0; i <= 360; i += 10) {
        float angle = i * M_PI / 180.0f;
        float x = 150.0f + 12.0f * cos(angle);
        float y = 380.0f + 12.0f * sin(angle);

        glVertex2f(x, y);
    }

    glEnd();

    glColor3f(0.4f, 0.2f, 0.05f);
    glBegin(GL_QUADS);

    glVertex2f(130.0f, 45.0f);
    glVertex2f(170.0f, 45.0f);
    glVertex2f(165.0f, 55.0f);
    glVertex2f(135.0f, 55.0f);

    glEnd();
}

void renderEnvironment()
{
    drawSky();
    drawGround();

    drawCloud(100, 430);
    drawCloud(300, 400);
    drawCloud(550, 440);

    drawFlagpole();
}