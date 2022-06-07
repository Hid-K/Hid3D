#ifndef MULTITHREAD_COMPUTING
#define MULTITHREAD_COMPUTING

#include "RGB.h"
#include "Vec.h"
#include "3D.h"
#include <pthread.h>
#include "unistd.h"
#include <stdio.h>

typedef struct RenderThreadArgs
{
    Polygon * polygones;
    size_t polygonesCount;

    int * status;
} RenderThreadArgs;

static RGB * frame_buffer_RAYMARCHER_MULTITHREAD_COMPUTING;

static size_t windowWidth_RAYMARCHER_MULTITHREAD_COMPUTING;
static size_t windowHeight_RAYMARCHER_MULTITHREAD_COMPUTING;

static size_t threadsCount;

static pthread_t * threads = NULL;

static int * threadStatus = NULL;

static RenderThreadArgs * threadArgs = NULL;

void * renderThread(void * a);

void initMultithreadComputer(RGB * frame_buffer,
                             size_t buffer_height,
                             size_t buffer_width,
                             Polygon * polys,
                             size_t polygons_count,
                             size_t tc);

int checkThreadsStatus();

void startRender();

#endif