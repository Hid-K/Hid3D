#include "MultithreadComputing.h"

extern RGB * frame_buffer_RAYMARCHER_MULTITHREAD_COMPUTING;

extern size_t windowWidth_RAYMARCHER_MULTITHREAD_COMPUTING;
extern size_t windowHeight_RAYMARCHER_MULTITHREAD_COMPUTING;

extern size_t threadsCount;

extern pthread_t * threads;

extern int * threadStatus;

extern RenderThreadArgs * threadArgs;

void * renderThread(void * a)
{

    render_frame(frame_buffer_RAYMARCHER_MULTITHREAD_COMPUTING,
                                       windowHeight_RAYMARCHER_MULTITHREAD_COMPUTING,
                                       windowWidth_RAYMARCHER_MULTITHREAD_COMPUTING,
                                       ((RenderThreadArgs *)a)->polygones,
                                       ((RenderThreadArgs *)a)->polygonesCount);

    *( ( RenderThreadArgs * ) a )->status = 10;
    pthread_exit(NULL);
};

void initMultithreadComputer(RGB * frame_buffer,
                             size_t buffer_height,
                             size_t buffer_width,
                             Polygon * polys,
                             size_t polygons_count,
                             size_t tc)
{
    frame_buffer_RAYMARCHER_MULTITHREAD_COMPUTING = frame_buffer;

    windowWidth_RAYMARCHER_MULTITHREAD_COMPUTING = buffer_width;
    windowHeight_RAYMARCHER_MULTITHREAD_COMPUTING = buffer_height;

    threadsCount = tc;

    if( ( threads = (pthread_t *)malloc( sizeof( pthread_t ) * threadsCount ) ) == NULL )
    {
        printf("Error allocating memory for threads!\n");
        exit(-1);
    };

    if( ( threadStatus = (int *)malloc( sizeof( int ) * threadsCount ) ) == NULL )
    {
        printf("Error allocating memory for threads statuses!\n");
        exit(-1);
    };

    if( ( threadArgs = (RenderThreadArgs *)malloc( sizeof( RenderThreadArgs ) * threadsCount ) ) == NULL )
    {
        printf("Error allocating memory for threads arguments! ( %lu bytes ) \n", sizeof( RenderThreadArgs ) * threadsCount);
        exit(-1);
    };

    double ppt = polygons_count/tc; 

    for (size_t i = 0; i < threadsCount; ++i)
    {
        (threadArgs[i]).polygones = &(polys[(int)(ppt*i)]);
        (threadArgs[i]).polygonesCount = ppt;

        (threadArgs[i]).status = &threadStatus[i];
    }
};

int checkThreadsStatus()
{
    for (size_t i = 0; i < threadsCount; ++i)
    {
        if(threadStatus[i] == 0) return 0;
    }

    return 10;
};

void startRender()
{
    for (size_t i = 0; i < threadsCount; ++i)
    {
        threadStatus[i] = 0;
        pthread_create(&threads[i], NULL, renderThread, &threadArgs[i]);
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i < threadsCount; ++i)
    if(!threads[i])
    {
        printf("ERROR: unable to create thread, %i\n", i);
    }
    
    for( ; checkThreadsStatus() <= 0 ; usleep( 1 ) );
};