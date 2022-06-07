#include "VectorMath/Vec.h"
#include "RGB/RGB.h"
#include "3D/3D.h"
#include <SDL.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h> 


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


    Polygon polygones[4];

    
    polygones[0].points[0].x = 0;
    polygones[0].points[0].y = 0;
    polygones[0].points[0].z = 1;

    polygones[0].points[1].x = 1;
    polygones[0].points[1].y = 1;
    polygones[0].points[1].z = 0;

    polygones[0].points[2].x = -1;
    polygones[0].points[2].y = 1;
    polygones[0].points[2].z = 0;



    polygones[1].points[0].x = 0;
    polygones[1].points[0].y = 0;
    polygones[1].points[0].z = 1;

    polygones[1].points[1].x = 1;
    polygones[1].points[1].y = 1;
    polygones[1].points[1].z = 0;

    polygones[1].points[2].x = -1;
    polygones[1].points[2].y = -1;
    polygones[1].points[2].z = 0;



    polygones[2].points[0].x = 0;
    polygones[2].points[0].y = 0;
    polygones[2].points[0].z = 1;

    polygones[2].points[1].x = 1;
    polygones[2].points[1].y = 1;
    polygones[2].points[1].z = 0;

    polygones[2].points[2].x = 1;
    polygones[2].points[2].y = 1;
    polygones[2].points[2].z = 0;



    polygones[3].points[0].x = 1;
    polygones[3].points[0].y = 1;
    polygones[3].points[0].z = 0;

    polygones[3].points[1].x = -1;
    polygones[3].points[1].y = -1;
    polygones[3].points[1].z = 0;

    polygones[3].points[2].x = -1;
    polygones[3].points[2].y = 1;
    polygones[3].points[2].z = 0;


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
        
        struct timeval t1, t2;
        double elapsedTime;

        // start timer
        gettimeofday(&t1, NULL);

        render_frame(frame_buffer, windowHeight, windowWidth, &polygones, sizeof(polygones)/sizeof(Polygon));

        SDL_RenderClear(mainWindowRenderer);
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

        SDL_Delay(1000/6);
        // stop timer
        gettimeofday(&t2, NULL);

        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

        printf("%f fps\n", 1000/elapsedTime);

        printf("Rendered!\n");
    };

    return 0;
};