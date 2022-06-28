#include "3D.h"

inline void render_frame(RGB * frame_buffer,
                  size_t buffer_height,
                  size_t buffer_width,
                  Polygon * polys,
                  size_t polygons_count)
{
    static double angle = 0;
    angle += 0.01;

    double c = 1;
    Vec2 xTransformVector = {0, sin(M_PI/2)*c};
    Vec2 yTransformVector = {cos(M_PI/2+(2*M_PI)/3)*c, -5*c};
    Vec2 zTransformVector = {cos(M_PI/2+(4*M_PI)/3)*c, -5*c};

    Vec2 center = {buffer_width/2, buffer_height/2};

    for (size_t i = 0; i < polygons_count; ++i)
    {
        #ifndef NDEBUG
            printf("Info: Processing polygon:\n\tP0:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n\tP1:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n\tP2:\n\t\tX:%f\n\t\tY:%f\n\t\tZ:%f\n", polys[i].points[0]->x, polys[i].points[0]->y, polys[i].points[0]->z, polys[i].points[1]->x, polys[i].points[1]->y, polys[i].points[1]->z, polys[i].points[2]->x, polys[i].points[2]->y, polys[i].points[2]->z);
        #endif
        Vec2 transformedCoordinates[3];
        
        for (size_t j = 0; j < 3; ++j)
        {
            transformedCoordinates[j] = transform3DVector(
                                                          rotate3DVector(*(polys[i].points[j]),
                                                          angle, angle, angle),
                                                          xTransformVector,
                                                          yTransformVector,
                                                          zTransformVector
                                                          );
        }

        drawLine(frame_buffer, buffer_height, buffer_width, transformedCoordinates[0], transformedCoordinates[1], polys[i].color);
        drawLine(frame_buffer, buffer_height, buffer_width, transformedCoordinates[1], transformedCoordinates[2], polys[i].color);
        drawLine(frame_buffer, buffer_height, buffer_width, transformedCoordinates[2], transformedCoordinates[0], polys[i].color);

        // drawLine(frame_buffer, buffer_height, buffer_width, xTransformVector, yTransformVector, RGB_BLU);
        // drawLine(frame_buffer, buffer_height, buffer_width, yTransformVector, zTransformVector, RGB_BLU);
        // drawLine(frame_buffer, buffer_height, buffer_width, zTransformVector, xTransformVector, RGB_BLU);
    }
    
};

inline Vec3 rotate3DVector( Vec3 vector, double xa, double ya, double za )
{
    return rotateAboutZ(rotateAboutY(rotateAboutX(vector, xa), ya), za);
}

inline Vec2 transform3DVector(Vec3 vec, Vec2 xTransformVector, Vec2 yTransformVector, Vec2 zTransformVector)
{
    Vec2 xTransformed = multiply(xTransformVector, vec.x);
    Vec2 yTransformed = multiply(yTransformVector, vec.y);
    Vec2 zTransformed = multiply(zTransformVector, vec.z);

    Vec2 result = add(xTransformed, add(yTransformed, zTransformed));

    return result;
};

inline void drawLine(RGB * frame_buffer,
              size_t buffer_height,
              size_t buffer_width,
              Vec2 p1,
              Vec2 p2,
              RGB color)
{
    Vec2 delta = substract(p1,p2);
    double lengh = length(delta);

    double dx = delta.x/lengh;
    double dy = delta.y/lengh;

    Vec2 center = {buffer_width/2, buffer_height/2};

    for (double i = 0; i < lengh; ++i)
    {
        int x = dx * i + center.x + p2.x;
        int y = dy * i + center.y + p2.y;
        frame_buffer[(int)(x + y * buffer_width)] = color;
    }
};