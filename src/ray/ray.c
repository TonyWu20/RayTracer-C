#include <ray/ray.h>

SphereIntersect *hit_Sphere(IntersectCollection *collection)
{
    sort_xs(collection);
    SphereIntersect *result;
    for (int i = 0; i < collection->intersects_counts; ++i)
    {
        if (collection->intersects[i]->t > 0)
        {
            result = collection->intersects[i];
            return result;
        }
    }
    return NULL;
}
