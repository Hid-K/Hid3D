#include "VectorMath/Vec.h"
#include "RGB/RGB.h"
#include "3D/3D.h"
#include <SDL.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h> 
#include "MultithreadComputing/MultithreadComputing.h"
#include "Triangulation.h"


size_t windowWidth = 512;
size_t windowHeight = 512;



int main()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Cant init SDL\n");
        return -1;
    };


    SDL_Window * mainWindow = SDL_CreateWindow("HidRaymarcher 2.0", 0, 0, windowWidth, windowHeight,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    if(mainWindow == NULL)
    {
        printf("Cant create main window\n");
        return -1;
    };


    SDL_Renderer * mainWindowRenderer = SDL_CreateRenderer(mainWindow, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(mainWindowRenderer == NULL)
    {
        printf("Cant create main window renderer\n");
        return -1;
    };

    SDL_Event event;
    char quit = 0;

    RGB * frame_buffer = malloc(sizeof(RGB) * windowHeight * windowWidth);


    int polygonsCount = 4;
    Polygon * polygones = malloc( sizeof( Polygon ) * polygonsCount );   

    for(int i = 0; i < polygonsCount; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            polygones[i].points[j] = malloc(sizeof(Vec3));
        }
    }
    
    polygones[0].points[0]->x = 0;
    polygones[0].points[0]->y = 0;
    polygones[0].points[0]->z = 10;

    polygones[0].points[1]->x = 10;
    polygones[0].points[1]->y = 10;
    polygones[0].points[1]->z = 0;

    polygones[0].points[2]->x = -10;
    polygones[0].points[2]->y = 10;
    polygones[0].points[2]->z = 0;

    polygones[0].color = RGB_BLU;



    polygones[1].points[0]->x = 0;
    polygones[1].points[0]->y = 0;
    polygones[1].points[0]->z = 10;

    polygones[1].points[1]->x = 10;
    polygones[1].points[1]->y = 10;
    polygones[1].points[1]->z = 0;

    polygones[1].points[2]->x = -10;
    polygones[1].points[2]->y = -10;
    polygones[1].points[2]->z = 0;

    polygones[1].color = RGB_GREN;



    polygones[2].points[0]->x = 0;
    polygones[2].points[0]->y = 0;
    polygones[2].points[0]->z = 10;

    polygones[2].points[1]->x = 10;
    polygones[2].points[1]->y = 10;
    polygones[2].points[1]->z = 0;

    polygones[2].points[2]->x = 10;
    polygones[2].points[2]->y = 10;
    polygones[2].points[2]->z = 0;

    polygones[2].color = RGB_RED;


    polygones[3].points[0]->x = 10;
    polygones[3].points[0]->y = 10;
    polygones[3].points[0]->z = 0;

    polygones[3].points[1]->x = -10;
    polygones[3].points[1]->y = -10;
    polygones[3].points[1]->z = 0;

    polygones[3].points[2]->x = -10;
    polygones[3].points[2]->y = 10;
    polygones[3].points[2]->z = 0;

    polygones[3].color = RGB_WHI;

    TriangulatablePoint * points = polygonsToTriangulatablePoints(polygones, polygonsCount);

    size_t polygonsCount1 = 0;

    Polygon * polygonesTrianguletd = triangulate(points, polygonsCount*3, &polygonsCount1);

    polygones = polygonesTrianguletd;
    polygonsCount = polygonsCount1;

    printf("Polygons count to render:%i\n", polygonsCount);

    for (size_t i = 0; i < polygonsCount; ++i)
    {
        polygones[i].color = RGB_BLU;

        printf("Polygon:\n\tP0:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n\tP1:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n\tP2:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n", polygones[i].points[0]->x, polygones[i].points[0]->y, polygones[i].points[0]->z, polygones[i].points[1]->x, polygones[i].points[1]->y, polygones[i].points[1]->z, polygones[i].points[2]->x, polygones[i].points[2]->y, polygones[i].points[2]->z);
    }

    // initMultithreadComputer(frame_buffer, windowHeight, windowWidth, polygones, polygonsCount, 4);

    for(;quit == 0;)
    {

        for(;SDL_PollEvent(&event);)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = 1;
                break;
            
            default:
                break;
            }
        };

        memset(frame_buffer, 0, sizeof(RGB) * windowHeight * windowWidth);
        
        struct timeval t1, t2;
        double elapsedTime;

        // start timer
        gettimeofday(&t1, NULL);

        render_frame(frame_buffer, windowHeight, windowWidth, polygones, polygonsCount);

        // startRender();

        // stop timer
        gettimeofday(&t2, NULL);

        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

        printf("%f fps\nRendered for: %f ms\n", 1000/elapsedTime, elapsedTime);

        SDL_Delay((1000-elapsedTime)/60);

        SDL_SetRenderDrawColor(mainWindowRenderer, 0, 0, 0, 255);
        if(SDL_RenderClear(mainWindowRenderer) == 0)
        {
            for(size_t x = 0; x < windowWidth; ++x)
            {
                for(size_t y = 0; y < windowHeight; ++y)
                {
                    SDL_SetRenderDrawColor(mainWindowRenderer,
                                        255*frame_buffer[x + y * windowWidth].r,
                                        255*frame_buffer[x + y * windowWidth].g,
                                        255*frame_buffer[x + y * windowWidth].b,
                                        255);
                    SDL_RenderDrawPoint(mainWindowRenderer, y, x);
                };
            };
            SDL_RenderPresent(mainWindowRenderer);
        } else
        {
            printf("Error while cleaning SDL render!");
            exit(-1);
        }
    };

    return 0;
};