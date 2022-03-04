#include <main.h>

void test_intersect()
{
    Cube *c = create_Cube();
    Point origins[] = {(Point){5, 0.5, 0, 1}, (Point){-5, 0.5, 0, 1},
                       (Point){0.5, 5, 0, 1}, (Point){0.5, -5, 0, 1},
                       (Point){0.5, 0, 5, 1}, (Point){0.5, 0, -5, 1},
                       (Point){0, 0.5, 0, 1}};
    Vector directions[] = {make_Vector(-1, 0, 0), make_Vector(1, 0, 0),
                           make_Vector(0, -1, 0), make_Vector(0, 1, 0),
                           make_Vector(0, 0, -1), make_Vector(0, 0, 1),
                           make_Vector(0, 0, 1)};
    for (int i = 0; i < 7; ++i)
    {
        Ray r = init_Ray(origins[i], directions[i]);
        Shape_intersect_with_ray((Shape *)c, &r);
        printf("%d, %f, %f\n", i, r.xs->intersects[0]->t,
               r.xs->intersects[1]->t);
        r.destroy_XS(&r);
    }
}

int main(int argc, char *argv[])
{
    test_intersect();
    return 0;
}
