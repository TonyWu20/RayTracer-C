#include <main.h>
void test_ray_transform()
{
    Sphere *s = create_Sphere(make_Point(0, 0, 0), 1);
    Ray r = init_Ray(make_Point(0, 0, -5), make_Vector(0, 0, 1));
    printf("Value of Sphere *s: %p\n", s);
    printf("Address of s->shape: %p\n", &s->shape);
    printf("size of *s: %lu\n", sizeof(*s));
    printf("Size of Sphere *: %lu, size of Shape *: %lu\n", sizeof(Sphere *), sizeof(Shape *));
    Matrix_4x4 scaling = scaling_matrix(2, 2, 2);
    Shape *shape = (Shape *)s;
    s->shape.funcTab->set_transform((Shape *)s, &scaling);
    /* shape->funcTab->set_transform(shape, &scaling); */
    print_float4x4(*s->shape.transform);
    Shape_intersect_with_ray(shape, &r);
    /* s->shape->funcTab->set_transform(s, &scaling); */
    /* s->shape->funcTab->intersect_with_ray(s, &r); */
    printf("t %f\n", r.xs->intersects[1]->t);
    printf("%f\n",((Sphere *)r.xs->intersects[1]->object)->r);
    print_float4(((Sphere *)r.xs->intersects[1]->object)->origin);
    Intersect *hit = hit_Object(r.xs);
    PreComp *comp = prepare_computations(hit, &r);
    printf("%f\n",((Sphere *)comp->object)->r);
    print_float4(((Sphere *)comp->object)->origin);
    r.destroy_XS(&r);
    print_float4(shape->local_ray.origin);
    print_float4(shape->local_ray.directionVec);
    shape->funcTab->destroy(shape);
    comp->destroy(comp);
    Plane *p = create_Plane();
    World *w = init_default_world();
    w->funcTab->add_object(w, (Shape *)p);
    simd_float4x4 translateMiddle =
        translation_matrix((simd_float3){-0.5, 1, 0.5});
    w->shapeArray[0]->funcTab->set_transform(w->shapeArray[0], &translateMiddle);
    if (w->shapeArray[2])
    {
        printf("Plane added");
    }
    else {
        printf("Failed");
    }
    Camera *cam = init_Camera(100, 48, PI/3);
    simd_float4 from, to, upVector;
    from = (simd_float4){0, 1.5, -8, 1};
    to = (simd_float4){0, 1, 0, 1};
    upVector = (simd_float4){0, 1, 0, 0};
    simd_float4x4 transMat = view_transform(&from, &to, &upVector);
    *cam->transform = simd_mul(transMat, *cam->transform);
    /* Canvas *termFig = cam->funcTab->ascii_render(cam, w); */
    PPMCanvas *fig = cam->funcTab->render(cam, w);
    fig->printCanvas(fig, "r2.ppm");
    fig->destroy(fig);
    printf("\e[?25l");
    /* termFig->show(termFig); */
    /* termFig->destroy(termFig); */
    printf("\033[0m");
    printf("\e[?25h");
    w->funcTab->destroy(w);
    cam->funcTab->destroy(cam);
    /* s->funcTab->destroy(s); */
    /* Sphere *s2 = create_Sphere(make_Point(0, 0, 0), 1); */
    /* Matrix_4x4 transform = translation_matrix(5, 0, 0); */
    /* s2->funcTab->set_transform(s2, &transform); */
    /* s2->funcTab->intersect_with_ray(s2, &r); */
    /* r.destroy_XS(&r); */
    /* print_float4(s2->shape->local_ray.origin); */
    /* print_float4(s2->shape->local_ray.directionVec); */
    /* s2->shape->funcTab->destroy(s2); */
    /* Sphere *s3 = create_Sphere(make_Point(0, 0, 0), 1); */
    /* transform = translation_matrix(0, 1, 0); */
    /* s3->funcTab->set_transform(s3, &transform); */
    /* Point test = make_Point(0, 1.70711, -0.70711); */
    /* print_float4(s3->funcTab->surface_normal_at(s3, &test)); */
    /* s3->funcTab->destroy(s3); */
    /* transform = simd_mul(scaling_matrix(1, 0.5, 1), */
    /*                      rotation_matrix(PI / 5, simd_make_float3(0, 0, 1)));
     */
    /* s3 = create_Sphere(); */
    /* s3->funcTab->set_transform(s3, &transform); */
    /* test = make_Point(0, sqrtf(2) / 2.0, -sqrtf(2) / 2.0); */
    /* print_float4(s3->funcTab->surface_normal_at(s3, &test)); */
    /* s3->funcTab->destroy(s3); */
}

void plane_test()
{
    Plane *p = create_Plane();
    Point n1 = {0,0,0};
    print_float4(Plane_surface_normal_at((Shape *)p, &n1));
    Ray r = init_Ray(make_Point(0,10,0), make_Vector(0,0,1));
    p->shape.funcTab->intersect_with_ray((Shape *)p, &r);
    if (!r.xs)
        printf("No intersects\n");
    r.destroy_XS(&r);
    r = init_Ray(make_Point(0,-0.0009,0), make_Vector(0,1,0));
    printf("abs %f\n", fabsf(r.directionVec.y));
    /* p->shape.funcTab->intersect_with_ray((Shape *)p, &r); */
    Shape_intersect_with_ray((Shape *)p, &r);
    if (!r.xs)
        printf("No intersects\n");
    else
    {
        Intersect *hit = hit_Object(r.xs);
        printf("%f\n", r.xs->intersects[0]->t);
        if (!hit)
        {
            printf("No hit\n");
        }
        PreComp *precomp = prepare_computations(hit, &r);
        printf("%f\n", precomp->t);
        precomp->destroy(precomp);
    }
    r.destroy_XS(&r);
    
}

int main(int argc, char *argv[])
{
    test_ray_transform();
    /* plane_test(); */
    return 0;
}
