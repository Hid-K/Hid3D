#ifndef HID3D_h
#define HID3D_h

#include "../VectorMath/Vec.h"
#include "../RGB/RGB.h"
#include <stdio.h>

typedef struct Polygon
{
    Vec3 * points[3];
    RGB color;
} Polygon;


void render_frame(RGB * frame_buffer,
                  size_t buffer_height,
                  size_t buffer_width,
                  Polygon * polys,
                  size_t polygons_count);

Vec2 transform3DVector(Vec3 vec, Vec2 xTransformVector, Vec2 yTransformVector, Vec2 zTransformVector);

void drawLine(RGB * frame_buffer,
              size_t buffer_height,
              size_t buffer_width,
              Vec2 p1,
              Vec2 p2,
              RGB color);

Vec3 rotate3DVector( Vec3 vector, double xa, double ya, double za );


#endif