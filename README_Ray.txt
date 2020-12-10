For Ray I continued with the simple design of using static methods in a single file.
These helpers are generally organized by what type of primitive they are for. The overall
flow of the program is that Canvas2D renderImage method calls into the RayHelpers renderImage
method which iterates through each pixel, constructing a ray to cast and then passing that ray
to doRecursiveCastColor and using the color it returns to set its current pixel. 
doRecursiveCastColor raycasts into the scene and then if it hits an object, computes the
color of that object (w/o reflection) by calling getPixelColor. It then either returns this
color or computes the reflection contribution as well by reflecting its ray and making a recursive
call to itself. getPixelColor does lighting calculations including specular highlights, shadows,
attenuation, diffuse, and ambient contributions.

All the remaining functions are helpers for this general logical flow. There is a raycast function
that is used when determining if a camera ray, reflection ray, or lighting ray (for shadows) hits
an object. The raycast method iterates through every primitive and calls a corresponding intersection
function to check if they intersect. There are also methods for each primitives to compute their
normals and uv coordinates at a position in their object space. Finally, a few structs are used to
streamline raycasting, namely Ray, Raycast, and IntersectInfo.

As far as I know there are no bugs in my program. It is a bit slower than I would like. I think this
may be partially because of my design to do raycasting with chooseCast which ends up copying the Raycast
struct multiple times for each primitive for each raycast in the scene. I have already sped up this 
design immensely from Intersect when it was copying an entire matrix each time a Raycast was called and 
two Raycasts were being copied by value every time chooseCast was called :O. I hope it is no fast enough
to be gradeable at least. I was able to render up to 4 or 5 recursive spheres depending on how patient I
was. I rendered chess.xml once but it took a long time (~10+ minutes).

NOTE: I was informed that the TA grading my Sceneview could not load scenes without segfaulting on
the department machines. I was not able to replicate this bug (locally or on department machines)
for the whole time I did Sceneview, Intersect, and Ray, so it might have just been some wierd 
configuration thing. If this does occur again, sorry about that, I'm really not sure what's going on! 