#include "Triangulation.h"

#include "Vec.h"
#include "stdbool.h"
#include "Triangulation.h"
#include "3D.h"
#include "SLL.h"

TriangulatablePoint * pointsIntoTriangulatable( const Vec3 * points, size_t pointsCount )
{
    TriangulatablePoint * triangulatablePoints = malloc( sizeof(TriangulatablePoint) * pointsCount );

    for (size_t i = 0; i < pointsCount; ++i)
    {
        triangulatablePoints[i].point = &points[i];
        triangulatablePoints[i].wasDomainPoint = false;
    }
    
    return triangulatablePoints;
}

Node * triangulatablePointsToLinkedList( TriangulatablePoint * points, size_t pointsCount )
{
    Node * firstNode = createSll();

    for (size_t i = 0; i < pointsCount; ++i)
    {
        sllPush( firstNode, &points[i] );
    }
    
    return firstNode;
}

Vec3 ** getTwoNearestPoints( Node * points, TriangulatablePoint * point )
{
    size_t pointsCount = sllGetListSize( points );

    point->wasDomainPoint = true;

    Vec3 ** twoNearestPoints = malloc( sizeof( Vec3 * ) * 2 );
    double lastNearestPointDist = MAXFLOAT;

    for( size_t i = 0; i < pointsCount ; ++i )
    {
        TriangulatablePoint * currentPoint = sllGetNodeByIndex( points, i )->data;

        if (currentPoint->point == point->point ||
            currentPoint->wasDomainPoint) continue;

        if( length( substract( *currentPoint->point,
        *point->point ) ) < lastNearestPointDist )
        {
            twoNearestPoints[0] = currentPoint->point;
            lastNearestPointDist = length( substract( *currentPoint->point, *point->point ) );
        }
    };


    lastNearestPointDist = MAXFLOAT;

    for( size_t i = 0; i < pointsCount ; ++i )
    {
        TriangulatablePoint * currentPoint = sllGetNodeByIndex( points, i )->data;

        if (currentPoint->point == point->point ||
            currentPoint->wasDomainPoint ||
            currentPoint->point == twoNearestPoints[0]) continue;

        if( currentPoint->point != twoNearestPoints[0] && length( substract( *currentPoint->point, *point->point ) ) < lastNearestPointDist )
        {
            twoNearestPoints[1] = currentPoint->point;
            lastNearestPointDist = length( substract( *currentPoint->point, *point->point ) );
        }
    };

    return twoNearestPoints;

}

#define  triangulate(a,b,c)   \
    _Generic(         \
    (a),               \
    const Vec3 * : triangulate_Vec3, \
    TriangulatablePoint * : triangulate_TriangulatablePoint   \
    ) ((a), (b), (c))

Polygon * triangulate_Vec3( const Vec3 * points, size_t pointsCount, size_t * polygonsCount )
{
    return triangulate_TriangulatablePoint(
                                        pointsIntoTriangulatable( points, pointsCount ),
                                        pointsCount,
                                        polygonsCount
                                    );
}

Polygon * triangulate_TriangulatablePoint( TriangulatablePoint * points, size_t pointsCount, size_t * polygonsCount )
{
    Node * triangulatablePoints = triangulatablePointsToLinkedList(
                                        points,
                                        pointsCount
                                    );

    Node * polygons = createSll();

    
    for (size_t i = 0; i < pointsCount; i++)
    {
        TriangulatablePoint * domainPoint = ((TriangulatablePoint *)sllGetNodeByIndex(triangulatablePoints, i)->data);

        domainPoint->wasDomainPoint = true;

        Vec3 ** twoNearestPoints = getTwoNearestPoints(triangulatablePoints, domainPoint);
        if(twoNearestPoints[0] != NULL && twoNearestPoints[1] != NULL){
            Polygon * polygon = malloc( sizeof(Polygon) );

            polygon->points[0] = domainPoint->point;
            polygon->points[1] = twoNearestPoints[0];
            polygon->points[2] = twoNearestPoints[1];

            sllPush(polygons, polygon);

            ++*polygonsCount;
        } else break;
    }

    (*polygonsCount)--;
    
    return sllToArray( polygons, sizeof( Polygon ) );
}

TriangulatablePoint * polygonsToTriangulatablePoints(Polygon * polygons, size_t polygonsCount)
{
    Node * points = createSll();

    for (size_t i = 0; i < polygonsCount; i++)
    {
        TriangulatablePoint * p = malloc( sizeof( TriangulatablePoint ) * 3 );

        for( size_t j = 0; j < 3; ++j )
        {
            p[j].point = polygons[i].points[j];
            p[j].wasDomainPoint = false;

            sllPush(points, &p[j]);
        }
    }
    size_t points_count = sllGetListSize(points);
    
    return sllToArray( points, sizeof( TriangulatablePoint ) );
};