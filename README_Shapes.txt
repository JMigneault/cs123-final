To implement Shapes I made a class for each type of shape. To reuse common 
code I made a CircularSide component that acted as the base of the cone and 
cylinder. I also made a RectangularSide component that acted as the 6 sides of 
the cube. If I were to do the project again I would not have gone through the 
trouble of making the rectangular component as it is only used by one shape. 
That said, it would make my design more extensible in the future if I were to 
add other kinds of shapes. For these ShapeComponent classes I always generated 
them facing the positive-z direction and then used transformation matrices to 
move them to their appropriate place. For the other parts of the shapes I 
explicitly programmed the vertices and normals. In general, I looped through 
rectangles on each object, drawing 6 vertices and 6 normals on each iteration.

There are no known bugs.