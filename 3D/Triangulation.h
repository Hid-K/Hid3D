#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "Vec.h"
#include "stdbool.h"
#include "Triangulation.h"
#include "3D.h"
#include "SLL.h"

typedef struct TriangulatablePoint
{
    Vec3 * point;
    bool wasDomainPoint; //Domain point - point, which from triangle creating started
} TriangulatablePoint;

#define  triangulate(a,b,c)   \
    _Generic(         \
    (a),               \
    const Vec3 * : triangulate_Vec3, \
    TriangulatablePoint * : triangulate_TriangulatablePoint   \
    ) ((a), (b), (c))

TriangulatablePoint * pointsIntoTriangulatable( const Vec3 * points, size_t pointsCount );

Node * triangulatablePointsToLinkedList( TriangulatablePoint * points, size_t pointsCount );

Vec3 ** getTwoNearestPoints( Node * points, TriangulatablePoint * point );

Polygon * triangulate_Vec3( const Vec3 * points, size_t pointsCount, size_t * polygonsCount );

Polygon * triangulate_TriangulatablePoint( TriangulatablePoint * points, size_t pointsCount, size_t * polygonsCount );

TriangulatablePoint * polygonsToTriangulatablePoints(Polygon * polygons, size_t polygonsCount);

#endif