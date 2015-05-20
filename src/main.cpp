#include <stdio.h>
#include <iostream>
#include <vector>
#include <complex>
#include <string>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <time.h>

#include "define.h"
#include "QVector.h"
#include "QObject.h"
#include "QPlane.h"
#include "QBall.h"
#include "QTriangle.h"

#include "QImplicitSurface.h"
#include "ViewingParams.h"
#include "ScreenParams.h"


#include "SDL/SDL.h"




#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

using namespace std;
//int _count = 0;
bool debug = false;

QVector eye;
QVector lookAtPt;
QVector direction;
QVector headUp;
QVector lightSrc;
ViewingParams g_view;
ScreenParams screen;
std::vector<QObject*> listObjs;

char *data = "data.txt";
//char *data = "data_near_view_good.txt";

QVector backgroundColor = QVector::parseColor(COLOR_SKY);
int nx2;
int ny2;

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

int64_t getMSTime(){
    struct timespec tms;
    /* The C11 way */
    /*if ( !timespec_get(&tms, TIME_UTC)) {
        return 0;
    }*/

    /* POSIX.1-2008 way */
    if (clock_gettime(CLOCK_REALTIME,&tms)) {
        return -1;
    }
    /* seconds, multiplied with 1 million */
    int64_t micros = tms.tv_sec * 1000000;
    /* Add full microseconds */
    micros += tms.tv_nsec/1000;
    /* round up if necessary */
    if (tms.tv_nsec % 1000 >= 500) {
        ++micros;
    }
    return micros;    
}

void readInputFile(bool readObjOnly = false) {
    listObjs.clear();
    FILE *f;
    QObject obj;

    f = fopen(data, "r");
    if (f <= 0) {
        cout << "File Opening error" << endl;
        exit(0);
    }
    double ex, ey, ez;
    double atx, aty, atz;
    double upx, upy, upz;
    fscanf(f, "%lf %lf %lf ", &ex, &ey, &ez);
    fscanf(f, "%lf %lf %lf ", &atx, &aty, &atz);
    fscanf(f, "%lf %lf %lf ", &upx, &upy, &upz);

    int nx, ny;
    double fov;
    fscanf(f, "%d %d ", &nx, &ny);
    fscanf(f, "%lf ", &fov);
    double lx, ly, lz;
    fscanf(f, "%lf %lf %lf", &lx, &ly, &lz);

    if (!readObjOnly) {
        eye = QVector(ex, ey, ez);
        lookAtPt = QVector(atx, aty, atz);
        direction = lookAtPt - eye;
        headUp = QVector(upx, upy, upz);
        screen = ScreenParams(nx, ny, fov * M_PI / 360);
        nx2 = (int) screen.get_nx() / 2;
        ny2 = (int) screen.get_ny() / 2;
        lightSrc = QVector(lx, ly, lz);
        g_view = ViewingParams(eye, direction, headUp);
    }

    int nObj;
    fscanf(f, "%d ", &nObj);
    char line [1000];
    int k = 0;
    while (k < nObj && fgets(line, sizeof line, f) != NULL) /* read a line from a file */ {
        //fprintf(stdout,"%s",line); //print the file contents on stdout.
        std::string str(line);
        vector<string> tokens;
        vector<double> values;
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter<vector<string> >(tokens));
        for (int i = 0; i < tokens.size(); i++) {
            values.push_back(stof(tokens[i]));
            //values.push_back(tokens[i]);
        }
        QPlane *pl;
        QBall *bl;
        QImplicitSurface *imsur;
        QTriangle *tr;
        bool is_sqecular;
        std::vector<QVector> centers;
        switch (int(values[0])) {
            case OBJ_TYPE_PLANE:
                //plane
                if (fabs(values[7]) > EPSILON)
                    is_sqecular = true;
                else
                    is_sqecular = false;
                pl = new QPlane(QVector(values[1], values[2], values[3]), QVector(values[4], values[5], values[6]), is_sqecular, values[8], values[9], values[10]);
                listObjs.push_back(pl);
                break;
            case OBJ_TYPE_BALL:
                //ball
                if (fabs(values[8]) > EPSILON)
                    is_sqecular = true;
                else
                    is_sqecular = false;
                bl = new QBall(QVector(values[1], values[2], values[3]), values[4], QVector(values[5], values[6], values[7]), is_sqecular, values[9], values[10], values[11]);
                listObjs.push_back(bl);
                break;
            case OBJ_TYPE_IMPLICIT:
            {
                // implicit surface object
                if (fabs(values[5]) > EPSILON)
                    is_sqecular = true;
                else
                    is_sqecular = false;
                double n_center = values[9];
                for (int i = 0; i < n_center; i++) {
                    int pos = 9 + 3 * i;
                    centers.push_back(QVector(values[pos + 1], values[pos + 2], values[pos + 3]));
                }
                imsur = new QImplicitSurface(centers, values[1], QVector(values[2], values[3], values[4]), is_sqecular, values[6], values[7], values[8]);
                listObjs.push_back(imsur);
                break;
            }//case 3 block
            case OBJ_TYPE_TRIANGLE:
                //Triangle
                if (fabs(values[13]) > EPSILON)
                    is_sqecular = true;
                else
                    is_sqecular = false;
                tr = new QTriangle(QVector(values[1], values[2], values[3]), QVector(values[4], values[5], values[6]), QVector(values[7], values[8], values[9]), QVector(values[10], values[11], values[12]), is_sqecular, values[14], values[15], values[16]);
                listObjs.push_back(tr);
                break;
        }
        k++;
    }
    fclose(f);
}

void printData() {
    //Eye
    cout << "Eye:" << eye.toString() << endl;
    cout << "LookAtPoint:" << lookAtPt.toString() << endl;
    cout << "HeadUp:" << headUp.toString() << endl;
    cout << "Screen Size:[" << screen.get_nx() << "," << screen.get_ny() << "]" << endl;
    cout << "Field Of View:" << screen.get_theta()*360 / M_PI << endl;
    cout << "Number Of Object:" << listObjs.size() << endl;
    for (int i = 0; i < listObjs.size(); i++) {
        cout << "Object " << i << ":" << endl;
        listObjs[i]->printInfo();
    }
}

bool find1stHit(const QVector& rayOrig, const QVector& rayDir, int *hit_id, double *tmin) {
    *tmin = INFINITY;
    *hit_id = -1;
    for (int i = 0; i < listObjs.size(); i++) {
        double t = -1;
        if (listObjs[i]->intersect(rayOrig, rayDir, &t)) {
            if (t < *tmin) {
                *tmin = t;
                *hit_id = i;
            }
        }
    }
    if (*hit_id == -1 || fabs(*tmin - INFINITY) < EPSILON)
        return false;
    return true;
}

bool isInShadowArea(const QVector& rayOrig, const QVector& rayDir, int obj_id) {
    for (int i = 0; i < listObjs.size(); i++) {
        if (i == obj_id)
            continue;
        double t = -1;
        if (listObjs[i]->intersect(rayOrig, rayDir, &t)) {
            if (t > 0 && t < 1 && fabs(listObjs[i]->getRefraction() - 1) > EPSILON)//ignore total transparent obj
                //if(t > 0 && t < 1 )
                return true;
        }
    }
    return false;
}

/*return color*/
QVector trace(const QVector& rayOrig, const QVector& rayDir, double eta_I, double eta_T, int depth_level) {
    double tmin = -1;
    int hit_id = -1;
    QVector pColor;
    if (find1stHit(rayOrig, rayDir, &hit_id, &tmin)) {
        QObject *hitObj = listObjs[hit_id];
        QVector phit = rayOrig + rayDir*tmin;
        //cout<<phit.toString()<<endl;
        QVector N = hitObj->getNormalAt(phit);
        //N = -N;
        double cosTheta = QVector::cos(lightSrc - phit, N);
        double alpha = ((1 - MIN_DARK) * cosTheta + MIN_DARK + 1) / 2;
        QVector hitColor = hitObj->getColor() * alpha; //color of hitting point

        QVector reflecColor;
        QVector refracColor;
        QVector shadowColor;

        if (depth_level < DEPTH_MAX && (hitObj->getReflection() > 0 || hitObj->getRefraction() > 0)) {
            // reflection
            QVector R = QVector::reflect(rayDir, N);
            //recurse trace
            if (hitObj->getReflection() > 0) {
                reflecColor = trace(phit, R, eta_I, eta_T, depth_level + 1);
            }
            QVector T;
            if (hitObj->getRefraction() > 0) {
                // refraction
                T = QVector::refract(rayDir, N, eta_I, hitObj->getEta());
                if (hitObj->getObjType() != OBJ_TYPE_TRIANGLE) {
                    refracColor = trace(phit + N*EPSILON, T, hitObj->getEta(), eta_I, depth_level + 1);
                } else {
                    refracColor = trace(phit + N*EPSILON, T, eta_I, eta_I, depth_level + 1);
                }
            }
        }
        //combine colors
        pColor = hitColor * (1 - hitObj->getReflection() - hitObj->getRefraction()) + reflecColor * hitObj->getReflection() + refracColor * hitObj->getRefraction();
        //shadow effect
        QVector shadowRay = lightSrc - phit;
        bool isInShadow = false;
        if (isInShadowArea(phit, shadowRay, hit_id)) {
            pColor = pColor*SHADOW;
            isInShadow = true;
        }
        if (!isInShadow && hitObj->isSqecular()) {// Don't have highlight effect in shadow area
            QVector R = QVector::reflect(phit - lightSrc, N);
            double costheta2 = pow(QVector::cos(R, rayOrig - phit), N_HL);
            //if(depth_level == 1 && costheta2 > HIGHLIGHT_MIN){
            if (costheta2 > HIGHLIGHT_MIN) {
                QVector w = QVector::parseColor(COLOR_WHITE);
                QVector HC = (pColor * (1 - costheta2) + w * (costheta2 - HIGHLIGHT_MIN)) / (1 - HIGHLIGHT_MIN);
                pColor = HC;
            }
        }

    } else {
        pColor = backgroundColor;
    }
    return pColor;
}

void display(SDL_Surface *sdl_screen) {
    //glClear(GL_COLOR_BUFFER_BIT);
    //readInputFile(true);

    //glBegin(GL_POINTS);
    for (int i = -nx2; i < nx2; i++)
    //for (int i = 0; i < 2*nx2; i++)
        for (int j = -ny2; j < ny2; j++) {
        //for (int j = 0; j < 2*ny2; j++) {
            //glVertex2i(i, j);
            QVector pColor;
            if (!ANTI_ALIASING) {
                QVector p = QVector(i, j, -(screen.get_distance2eye()), 1);
                p = g_view.calPosInWorldCoord(p);
                QVector ray = p - eye;
                pColor = trace(eye, ray, AIR_ETA, INFINITY, 1);
            } else {
                //Jittered anti-aliasing
                for (int xx = 0; xx < NUM_SUB_ANTI_ALIASING; xx++) {
                    for (int yy = 0; yy < NUM_SUB_ANTI_ALIASING; yy++) {
                        // random a number from 0 to 1
                        double randx = (rand() % 100) / 100.0;
                        double sub_x = ((i - 1 - xx * 1.0 / NUM_SUB_ANTI_ALIASING) + 1.0 / NUM_SUB_ANTI_ALIASING * randx);
                        double randy = (rand() % 100) / 100.0;
                        double sub_y = ((j - 1 - yy * 1.0 / NUM_SUB_ANTI_ALIASING) + 1.0 / NUM_SUB_ANTI_ALIASING * randy);
                        QVector p = QVector(sub_x, sub_y, -(screen.get_distance2eye()), 1);
                        p = g_view.calPosInWorldCoord(p);
                        QVector ray = p - eye;
                        pColor = pColor + trace(eye, ray, AIR_ETA, INFINITY, 1);
                    }
                }
                pColor = pColor / (NUM_SUB_ANTI_ALIASING * NUM_SUB_ANTI_ALIASING);
            }
            //glColor3d(pColor.getX(), pColor.getY(), pColor.getZ());
            Uint32 color = SDL_MapRGB(sdl_screen->format, (int)(pColor.getX()*255), (int)(pColor.getY()*255), (int)(pColor.getZ()*255));
            //Uint32 color = SDL_MapRGB(sdl_screen->format, 0xff, 0xff, 0x00);
            putpixel(sdl_screen, i+nx2, -j+ny2-1, color);
            //SDL_UpdateRect(sdl_screen, i, j, 1, 1);
            //SDL_Flip(sdl_screen);
        }
    //glEnd();
    //glFlush();
}

int main(int argc, char **argv) {
    
    if (argc >= 2) {
        data = argv[1];
    }
    cout << "Input file:" << data << endl;
    readInputFile();

    SDL_Surface *sdl_screen;
    int quit = 0;
    SDL_Event event;
    int x, y;
    Uint32 yellow;

    // Initialize defaults, Video and Audio
    if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)) {
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        return -1;
    }

    //sdl_screen = SDL_SetVideoMode(nx2*2, ny2*2, 24, SDL_SWSURFACE | SDL_FULLSCREEN);
    sdl_screen = SDL_SetVideoMode(nx2*2, ny2*2, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (sdl_screen == NULL) {
        fprintf(stderr, "Couldn't set 800x600x24 video mode: %s\n", SDL_GetError());
        return -2;
    }


    // Map the color yellow to this display (R=0xff, G=0xFF, B=0x00)
    yellow = SDL_MapRGB(sdl_screen->format, 0xff, 0xff, 0x00);

    // Make the dot at the center of the screen
    x = sdl_screen->w / 2;
    y = sdl_screen->h / 2;
    int need_update = 1;
    int first = 1;
    while (!quit) {
        // Poll for events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        quit = 1;
                    break;
                    if (event.key.keysym.sym == SDLK_F1)
                        SDL_WM_ToggleFullScreen(sdl_screen); // Only on X11
                    break;
                case SDL_ACTIVEEVENT:
                    if (event.active.gain){
                        need_update = 1;
                    } else {
                        need_update = 0;
                        //printf("inactive\n");
                        cout << "inactive \n";
                    }
                    break;
                case SDL_QUIT:
                    quit = 1;
                    break;
                default:
                    break;
            }
        }

        // Lock the screen for direct access to the pixels
        if (SDL_MUSTLOCK(sdl_screen)) {
            if (SDL_LockSurface(sdl_screen) < 0) {
                fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
                return -3;
            }
        }

        // Plot the Pixel
        //putpixel(sdl_screen, x, y, yellow);
        //display(layer);
        if (need_update || first){
            int64_t t1, t2;
            t1 = getMSTime();
            display(sdl_screen);
            t2 = getMSTime();
            // Unlock Surface if necessary
            if (SDL_MUSTLOCK(sdl_screen)) {
                SDL_UnlockSurface(sdl_screen);
            }

            // Update just the part of the display that we've changed
            //SDL_UpdateRect(sdl_screen, x, y, 1, 1);
            //SDL_UpdateRect(sdl_screen,0,0,0,0);
            //SDL_GL_SwapBuffers();
            //SDL_BlitSurface(layer, NULL, sdl_screen, NULL);
            SDL_Flip(sdl_screen);
            need_update = 0;
            first = 0;
            cout << "screen updated : "<<(t2-t1)/1000.0<<"(ms)\n";
            #ifdef EMSCRIPTEN
                //cout << "Intentionally adding SDL_Delay() to force throw error, stop program, avoid freeze browser \n";
                //SDL_Delay(5); // For Emscripten only: intentionally adding to force throw error, stop program, avoid freeze
                // browser
                SDL_FreeSurface(sdl_screen);
                SDL_Quit();
                return 0;
            #endif                
        }
    }
    SDL_FreeSurface(sdl_screen);
    SDL_Quit();

    return 0;
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to set
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *) p = pixel;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *(Uint32 *) p = pixel;
            break;
    }
}
