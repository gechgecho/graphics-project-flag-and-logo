#ifdef _WIN32
  #include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const int CURVE_STEPS = 512;
static const double SVG_W = 204.0;
static const double SVG_H = 192.0;

static float waveAngle = 0.0f;
static float waveSpeed = 0.25f;
static bool isWaving = true;
static float xamppRotation = 0.0f;
static float rotationSpeed = 1.0f;
static bool isRotating = false;
static bool rotationDirection = 1;

struct Pt { double x, y; };

static inline Pt svgPt(double x, double y)
{
    return { (x / SVG_W) * 2.0 - 1.0,
            -((y / SVG_H) * 2.0 - 1.0) };
}

static Pt bezier(Pt p0, Pt p1, Pt p2, Pt p3, double t)
{
    double u   = 1.0 - t;
    double u2  = u * u,  t2  = t * t;
    double u3  = u2 * u, t3  = t2 * t;
    return { u3*p0.x + 3*u2*t*p1.x + 3*u*t2*p2.x + t3*p3.x,
             u3*p0.y + 3*u2*t*p1.y + 3*u*t2*p2.y + t3*p3.y };
}

typedef std::vector<Pt> Contour;

static void cbez(Contour& c, Pt p0, Pt p1, Pt p2, Pt p3, bool skipFirst = true)
{
    for (int i = skipFirst ? 1 : 0; i <= CURVE_STEPS; ++i)
        c.push_back( bezier(p0, p1, p2, p3, (double)i / CURVE_STEPS) );
}

static void lnTo(Contour& c, Pt p) { c.push_back(p); }
static void reverseContour(Contour& c) { std::reverse(c.begin(), c.end()); }


static Contour makeBackground()
{
    Contour c;
    const double R  = 17.0;
    const double X0 = 0.5,  Y0 = 0.5;
    const double X1 = 203.5, Y1 = 191.5;
    const double k  = R * 0.5522847498;

    c.push_back(svgPt(X0, Y0+R));
    cbez(c, svgPt(X0,    Y0+R), svgPt(X0,    Y0+R-k), svgPt(X0+R-k,Y0), svgPt(X0+R,  Y0));
    lnTo(c, svgPt(X1-R, Y0));
    cbez(c, svgPt(X1-R,  Y0), svgPt(X1-R+k,Y0), svgPt(X1,    Y0+R-k), svgPt(X1,    Y0+R));
    lnTo(c, svgPt(X1, Y1-R));
    cbez(c, svgPt(X1,    Y1-R), svgPt(X1,    Y1-R+k), svgPt(X1-R+k,Y1), svgPt(X1-R,  Y1));
    lnTo(c, svgPt(X0+R, Y1));
    cbez(c, svgPt(X0+R,  Y1), svgPt(X0+R-k,Y1), svgPt(X0,    Y1-R+k), svgPt(X0,    Y1-R));
    lnTo(c, svgPt(X0, Y0+R));
    return c;
}

static Contour makeSymbolOuter()
{
    Contour c;
    double px = 71.257756, py = 158.34418;
    c.push_back( svgPt(px, py) );

#define REL_C(dx1,dy1, dx2,dy2, dx,dy) \
    cbez(c, svgPt(px,py), svgPt(px+(dx1),py+(dy1)), svgPt(px+(dx2),py+(dy2)), svgPt(px+(dx), py+(dy))); \
    px+=(dx); py+=(dy)

#define REL_L(dx,dy) \
    lnTo(c, svgPt(px+(dx), py+(dy))); \
    px+=(dx); py+=(dy)

    REL_C(13.64777,-2.23521,  25.10968,-12.55956,  29.155504,-26.26189);
    REL_L(1.22587,-4.15176);
    REL_L(1.6745,4.93779);
    REL_C(2.58535,7.62375,  8.18338,14.96641,  14.75713,19.35619);
    REL_C(8.21715,5.48721,  11.57882,6.37727,  22.31306,5.90779);
    REL_C(7.67637,-0.33574,  10.15446,-0.88278,  14.91894,-3.29339);
    REL_C(18.26946,-9.2435,  25.64948,-31.80334,  16.43771,-50.24812);
    REL_L(-2.14606,-4.29705);
    REL_L(2.18875,-6.743809);
    REL_C(6.89715,-21.250971,  4.92391,-36.111101,  -6.4031,-48.220594);
    REL_C(-8.83361,-9.443843,  -22.59504,-13.831828,  -34.55093,-11.016946);
    REL_C(-13.68742,3.222554,  -23.12367,12.125594,  -28.0021,26.419801);
    REL_C(-0.95241,2.790662,  -1.76358,4.173931,  -1.80258,3.073931);
    REL_C(-0.1605,-4.526501,  -5.843134,-14.092567,  -11.374794,-19.148194);
    REL_C(-8.89933,-8.133489,  -13.29368,-9.851806,  -25.19455,-9.851806);
    REL_C(-8.85604,0,  -10.633,0.310877,  -15.53334,2.717544);
    REL_C(-7.06375,3.469165,  -14.27996,10.685374,  -17.74912,17.749123);
    REL_C(-2.43286,4.953673,  -2.71754,6.63329,  -2.71754,16.033333);
    REL_C(0,9.281891,  0.30885,11.158859,  2.66231,16.179308);
    REL_L(2.66231,5.679307);
    REL_L(-2.65149,6.320693);
    REL_C(-2.42292,5.775819,  -2.65242,7.484439,  -2.66231,19.820689);
    REL_C(-0.01,12.33211,  0.202,13.93255,  2.44925,18.5);
    REL_C(5.02607,10.21528,  14.55486,17.88848,  25.01217,20.14146);
    REL_C(7.04114,1.51697,  8.29207,1.54934,  15.33041,0.3966);

#undef REL_C
#undef REL_L
    return c;
}

static Contour makeInnerDetail()
{
    Contour c;
    double px = 58.356366, py = 132.40907;
    c.push_back( svgPt(px, py) );

#define REL_C(dx1,dy1, dx2,dy2, dx,dy) \
    cbez(c, svgPt(px,py), svgPt(px+(dx1),py+(dy1)), svgPt(px+(dx2),py+(dy2)), svgPt(px+(dx), py+(dy))); \
    px+=(dx); py+=(dy)
#define REL_L(dx,dy) \
    lnTo(c, svgPt(px+(dx), py+(dy))); \
    px+=(dx); py+=(dy)

    REL_C(-4.11764,-2.51089,  -6.7557,-9.0456,  -5.83992,-14.46602);
    REL_C(1.96491,-11.63024,  7.58418,-21.576913,  15.11792,-26.760263);
    REL_C(7.92742,-5.454211,  14.46758,-6.875917,  31.65983,-6.882241);
    REL_C(22.223944,-0.0082,  25.230264,-1.317418,  26.857864,-11.696523);
    REL_C(1.75383,-11.184013,  7.69736,-15.994473,  16.20754,-13.117747);
    REL_C(6.00445,2.029705,  8.10993,5.63193,  8.02341,13.727139);
    REL_C(-0.0785,7.345118,  -1.97752,13.27672,  -6.24581,19.508801);
    REL_C(-2.03852,2.97642,  -2.35249,4.026096,  -1.30849,4.374533);
    REL_C(0.75548,0.252143,  2.78048,1.714764,  4.5,3.250271);
    REL_C(3.05016,2.72374,  3.13471,3.03547,  3.46722,12.78295);
    REL_C(0.26985,7.91074,  -0.0185,10.74834,  -1.38485,13.62769);
    REL_C(-3.86173,8.13799,  -15.16717,8.89298,  -20.74017,1.38505);
    REL_C(-1.42777,-1.92349,  -2.14907,-4.97937,  -2.5964,-11);
    REL_L(-0.6194,-8.33659);
    REL_L(-15.5,-0.30547);
    REL_C(-20.264784,-0.39938,  -25.339194,0.23475,  -29.102764,3.63683);
    REL_C(-2.26411,2.04664,  -3.16489,3.99804,  -3.86476,8.37234);
    REL_C(-1.14689,7.16829,  -2.34628,9.40708,  -6.34631,11.84607);
    REL_C(-4.0254,2.45446,  -8.31655,2.47304,  -12.28491,0.0532);

#undef REL_C
#undef REL_L
    return c;
}

static Contour makeSmallOval()
{
    Contour c;
    double px = 60.596476, py = 83.277917;
    c.push_back( svgPt(px, py) );

#define REL_C(dx1,dy1, dx2,dy2, dx,dy) \
    cbez(c, svgPt(px,py), svgPt(px+(dx1),py+(dy1)), svgPt(px+(dx2),py+(dy2)), svgPt(px+(dx), py+(dy))); \
    px+=(dx); py+=(dy)

    REL_C(-5.60252,-1.678923,  -8.2525,-6.226721,  -7.78123,-13.353864);
    REL_C(0.42835,-6.477993,  7.00435,-11.388245,  13.77922,-10.288836);
    REL_C(6.99719,1.135488,  10.35946,6.663327,  10.35946,17.031799);
    REL_C(0,4.936387,  -0.36651,6.565772,  -1.58179,7.032118);
    REL_C(-2.23583,0.857967,  -11.37661,0.597387,  -14.77566,-0.421217);

#undef REL_C
    return c;
}

static std::vector<double*> g_tempVerts;

static void CALLBACK cb_vertex (void* d)             { glVertex2dv((double*)d); }
static void CALLBACK cb_begin  (GLenum t)            { glBegin(t); }
static void CALLBACK cb_end    ()                    { glEnd(); }
static void CALLBACK cb_error  (GLenum e)            { fprintf(stderr,"Tess error: %s\n", gluErrorString(e)); }
static void CALLBACK cb_combine(double c[3], void*[4], GLfloat[4], void** out)
{
    double* v = new double[3]{ c[0], c[1], c[2] };
    g_tempVerts.push_back(v);
    *out = v;
}

static void freeTempVerts()
{
    for (double* v : g_tempVerts) delete[] v;
    g_tempVerts.clear();
}

static void tessellateWithHoles(const Contour& outer, const std::vector<Contour>& holes)
{
    freeTempVerts();
    GLUtesselator* t = gluNewTess();
    gluTessProperty(t, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
    gluTessCallback(t, GLU_TESS_BEGIN,   (void(CALLBACK*)())cb_begin);
    gluTessCallback(t, GLU_TESS_END,     (void(CALLBACK*)())cb_end);
    gluTessCallback(t, GLU_TESS_VERTEX,  (void(CALLBACK*)())cb_vertex);
    gluTessCallback(t, GLU_TESS_COMBINE, (void(CALLBACK*)())cb_combine);
    gluTessCallback(t, GLU_TESS_ERROR,   (void(CALLBACK*)())cb_error);

    std::vector<std::vector<double*>> allVerts(1 + holes.size());

    gluTessBeginPolygon(t, NULL);
    gluTessBeginContour(t);
    allVerts[0].reserve(outer.size());
    for (const Pt& p : outer) {
        double* v = new double[3]{ p.x, p.y, 0.0 };
        allVerts[0].push_back(v);
        gluTessVertex(t, v, v);
    }
    gluTessEndContour(t);

    for (size_t hi = 0; hi < holes.size(); ++hi) {
        gluTessBeginContour(t);
        allVerts[1+hi].reserve(holes[hi].size());
        for (const Pt& p : holes[hi]) {
            double* v = new double[3]{ p.x, p.y, 0.0 };
            allVerts[1+hi].push_back(v);
            gluTessVertex(t, v, v);
        }
        gluTessEndContour(t);
    }

    gluTessEndPolygon(t);
    gluDeleteTess(t);

    for (auto& vv : allVerts)
        for (double* v : vv) delete[] v;
}

static void tessellateSingle(const Contour& contour)
{
    freeTempVerts();
    GLUtesselator* t = gluNewTess();
    gluTessProperty(t, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
    gluTessCallback(t, GLU_TESS_BEGIN,   (void(CALLBACK*)())cb_begin);
    gluTessCallback(t, GLU_TESS_END,     (void(CALLBACK*)())cb_end);
    gluTessCallback(t, GLU_TESS_VERTEX,  (void(CALLBACK*)())cb_vertex);
    gluTessCallback(t, GLU_TESS_COMBINE, (void(CALLBACK*)())cb_combine);
    gluTessCallback(t, GLU_TESS_ERROR,   (void(CALLBACK*)())cb_error);

    std::vector<double*> verts;
    verts.reserve(contour.size());

    gluTessBeginPolygon(t, NULL);
    gluTessBeginContour(t);
    for (const Pt& p : contour) {
        double* v = new double[3]{ p.x, p.y, 0.0 };
        verts.push_back(v);
        gluTessVertex(t, v, v);
    }
    gluTessEndContour(t);
    gluTessEndPolygon(t);
    gluDeleteTess(t);

    for (double* v : verts) delete[] v;
}


float getWaveOffset(float x, float y, float time) {

    float t = (x - 150.0f) / 500.0f;

    if (t < 0) t = 0;
    if (t > 1) t = 1;

    return sin(x * 0.03f + time) * 10.0f * t;
}

void drawFlagBase(float cx, float cy, float w, float h, float time) {
    glColor3f(0.0f, 0.384f, 0.20f);

    glBegin(GL_QUADS);
    for(float x = cx - w/2; x <= cx; x += 5.0f) {
        float x1 = x;
        float x2 = (x + 5.0f > cx) ? cx : x + 5.0f;

        float yOffset1 = getWaveOffset(x1, cy, time);
        float yOffset2 = getWaveOffset(x2, cy, time);

        glVertex2f(x1, cy - h/2 + yOffset1);
        glVertex2f(x2, cy - h/2 + yOffset2);
        glVertex2f(x2, cy + h/2 + yOffset2);
        glVertex2f(x1, cy + h/2 + yOffset1);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    for(float x = cx; x <= cx + w/2; x += 5.0f) {
        float x1 = x;
        float x2 = (x + 5.0f > cx + w/2) ? cx + w/2 : x + 5.0f;

        float yOffset1 = getWaveOffset(x1, cy, time);
        float yOffset2 = getWaveOffset(x2, cy, time);

        glVertex2f(x1, cy - h/2 + yOffset1);
        glVertex2f(x2, cy - h/2 + yOffset2);
        glVertex2f(x2, cy + h/2 + yOffset2);
        glVertex2f(x1, cy + h/2 + yOffset1);
    }
    glEnd();
}

void drawCrescent(float cx, float cy, float R, float r, float dx, float time) {
    glPushMatrix();

    float waveOffset = getWaveOffset(cx, cy, time);
    glTranslatef(cx, cy + waveOffset, 0.0f);

    glColor3f(0.82f, 0.06f, 0.20f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 25; i <= 335; i += 5) {
        float a = i * M_PI / 180.0;
        glVertex2f(R * cos(a), R * sin(a));
    }
    glEnd();

    glColor3f(0.0f, 0.384f, 0.20f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(dx, 0.0f);
    for (int i = 25; i <= 335; i += 5) {
        float a = i * M_PI / 180.0;
        float px = dx + r * cos(a);
        float py = r * sin(a);
        if (px > 0) px = 0;
        glVertex2f(px, py);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(dx, 0.0f);
    for (int i = 25; i <= 335; i += 5) {
        float a = i * M_PI / 180.0;
        float px = dx + r * cos(a);
        float py = r * sin(a);
        if (px < 0) px = 0;
        glVertex2f(px, py);
    }
    glEnd();

    glPopMatrix();
}

void drawStar(float cx, float cy, float R, float r, float time) {
    glPushMatrix();

    float waveOffset = getWaveOffset(cx, cy, time);
    glTranslatef(cx, cy + waveOffset, 0.0f);

    glColor3f(0.82f, 0.06f, 0.20f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 10; i++) {
        float a = i * M_PI / 5.0 - M_PI / 2.0;
        float radius = (i % 2 == 0) ? R : r;
        glVertex2f(radius * cos(a), radius * sin(a));
    }
    glEnd();

    glPopMatrix();
}

void drawFlagpole() {

    glColor3f(0.545f, 0.271f, 0.075f);
    glBegin(GL_QUADS);
    glVertex2f(145.0f, 50.0f);
    glVertex2f(155.0f, 50.0f);
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

    glColor3f(1.0f, 1.0f, 0.6f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(150.0f, 380.0f);
    for(int i = -45; i <= 45; i += 10) {
        float angle = i * M_PI / 180.0f;
        float x = 150.0f + 6.0f * cos(angle);
        float y = 380.0f + 6.0f * sin(angle);
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

static float g_zoom   = 0.7f;
static bool  g_wire   = false;
static Contour g_bg, g_outer, g_inner, g_oval;

static void renderFlag()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(-2.2f, -0.65f, -0.45f);
    glScalef(0.0035f, 0.0035f, 1.0f);

    drawFlagpole();
    drawFlagBase(400, 200, 500, 300, waveAngle);
    drawCrescent(400, 200, 75, 60, 15, waveAngle);
    drawStar(420, 200, 30, 30 * 0.382f, waveAngle);

    glPopMatrix();
}

static void renderXAMPP()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(1.0f, 0.0f, 0.0f);
    glRotatef(xamppRotation, 0.0f, 0.0f, 1.0f);
    glScalef(g_zoom, g_zoom, 1.f);

    glColor3f(0.984f, 0.478f, 0.141f);
    tessellateSingle(g_bg);

    glColor3f(1.f, 1.f, 1.f);
    Contour innerRev = g_inner;
    Contour ovalRev = g_oval;
    reverseContour(innerRev);
    reverseContour(ovalRev);
    std::vector<Contour> holes;
    holes.push_back(innerRev);
    holes.push_back(ovalRev);
    tessellateWithHoles(g_outer, holes);

    glPopMatrix();
}

static void display()
{
    glClearColor(0.8f, 0.85f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    renderFlag();

    renderXAMPP();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 500);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.2f, 0.2f, 0.3f);
    glRasterPos2f(10.0f, 485.0f);
    const char* info = "ALGERIAN FLAG (LEFT) | XAMPP LOGO (RIGHT) | +/- Zoom | F:Toggle Wave | R:Start Rotation | P:Pause Rotation | ESC:Exit";
    for(const char* c = info; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

static void reshape(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double a = (double)w / h;
    if (a >= 1.0) gluOrtho2D(-a, a, -1.0, 1.0);
    else          gluOrtho2D(-1.0, 1.0, -1.0/a, 1.0/a);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void keyboard(unsigned char k, int, int)
{
    switch (k) {
    case '+': case '=': g_zoom = std::min(2.0f, g_zoom * 1.1f);  glutPostRedisplay(); break;
    case '-': case '_': g_zoom = std::max(0.3f, g_zoom / 1.1f);  glutPostRedisplay(); break;
    case 'f': case 'F': isWaving = !isWaving; break;
    case 'r': case 'R':
        if(!isRotating) {
            isRotating = true;
            xamppRotation = 0.0f;
        }
        break;
    case 'p': case 'P': isRotating = false; break;
    case 27:  exit(0);
    }
}

static void updateAnimation(int value) {
    if(isWaving) {
        waveAngle += waveSpeed;
        if(waveAngle > 2 * M_PI) waveAngle -= 2 * M_PI;
    }
    if(isRotating) {
        xamppRotation += rotationSpeed;
        if(xamppRotation > 360.0f) xamppRotation -= 360.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateAnimation, 0);
}

int main(int argc, char** argv)
{

    g_bg    = makeBackground();
    g_outer = makeSymbolOuter();
    g_inner = makeInnerDetail();
    g_oval  = makeSmallOval();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(1200, 600);
    glutCreateWindow("Algerian Flag + XAMPP Logo");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glLineWidth(1.5f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, updateAnimation, 0);

    glutMainLoop();
    return 0;
}
