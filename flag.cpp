#include "flag.h"
#include <GL/gl.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
float waveAngle = 0.0f;
float getWaveOffset(float x, float y, float time)
{
    return sin((x * 0.02f) + time) * 5.0f;
}

void drawFlagBase(float cx, float cy, float w, float h, float time)
{
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(cx - w / 2, cy - h / 2);
    glVertex2f(cx + w / 2, cy - h / 2);
    glVertex2f(cx + w / 2, cy + h / 2);
    glVertex2f(cx - w / 2, cy + h / 2);
    glEnd();
}

void drawCrescent(float cx, float cy, float R, float r, float dx, float time)
{
    glPushMatrix();
    float waveOffset = getWaveOffset(cx, cy, time);
    glTranslatef(cx, cy + waveOffset, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 360; i += 5)
    {
        float a = i * M_PI / 180.0f;
        glVertex2f(R * cos(a), R * sin(a));
    }

    glEnd();
    glColor3f(0.0f, 0.6f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(dx, 0.0f);
    for (int i = 0; i <= 360; i += 5)
    {
        float a = i * M_PI / 180.0f;
        float px = dx + r * cos(a);
        float py = r * sin(a);
        glVertex2f(px, py);
    }
    glEnd();
    glPopMatrix();
}

void drawStar(float cx, float cy, float R, float r, float time)
{
    glPushMatrix();
    float waveOffset = getWaveOffset(cx, cy, time);
    glTranslatef(cx, cy + waveOffset, 0.0f);
    glColor3f(0.82f, 0.06f, 0.20f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 10; i++)
    {
        float a = i * M_PI / 5.0f - M_PI / 2.0f;
        float radius = (i % 2 == 0) ? R : r;
        glVertex2f(radius * cos(a), radius * sin(a));
    }
    glEnd();
    glPopMatrix();
}
void renderFlag()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(-2.2f, -0.65f, -0.45f);
    glScalef(0.0035f, 0.0035f, 1.0f);
    drawFlagBase(400, 200, 500, 300, waveAngle);
    drawCrescent(400, 200, 75, 60, 15, waveAngle);
    drawStar(420, 200, 30, 30 * 0.382f, waveAngle);
    glPopMatrix();
}