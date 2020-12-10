NOTE (11/19): I have been in communication within Griffin about an issue with the handin.
It seems likely I did not push the .pro file along with my source code so with Griffin's 
permission I am resubmitting this project on gradescope with all files in the repository pushed.

My design for intersect was to use a large amount of static helper functions to do 
raycasting and rendering. I put these functions in the ray/rayhelpers.cpp file. 

In short, there is a helper that does the rendering by taking individual scene 
information and then going through a big loop that generates rays, finds intersections,
and computes the lighting at the intersection points. I made a helper to generate the 
rays and each implicity shape had its own helper: rayIntersctSHAPE(). This function
took in a Ray struct, which had the eye point and direction unit vector, and returned
a Raycast object, which contains information about the intersection point and the
material and normal vector at that point. After determining the closest object, these
Raycast objects are used to compute the color of that pixel using the equation in the
PDF. Only point lights are implemented and there are no shadows or other additional features.

Raycasting is currently pretty slow and I didn't do a lot of fine tuned optimization, nor 
did I implement a more complex extra credit feature for speading it up. I did try to minimize
the number of operations I was doing in inner loops and pre-computed all my inverse matrices
for each object in the scene to avoid doing those computations each loop. I didn't see any
requirements about speed, so I hope the current status of things is ok!