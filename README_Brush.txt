I followed the suggestions in the PDF when implementing brush. Canvas2d tracks 
some state in the form of settings and an instance of the current brush. When 
the mouse down, up or dragged events are called Canvas2d simply calls the 
corresponding method in the current brush. For the constants linear and quadratic
brushes the only nonempty method is brushDragged which puts color onto the canvas
using the equation from the Brush algo assignment. The smudge brush utilizes all
three methods. brushDown picks up paint, brushUp clears the currently held paint,
and brushDragged writes in the same way as the other brushes (with the one 
difference that each pixel has a different "brush color" based on the picked up
paint) and then picks up paint again. 

There are no known bugs but the functionality of the smudge brush near the edges
of the canvas is unintuitive. When you smudge away from the edge of the canvas 
black paint appears from the paint buffer pixels that could not be filled due to
the edge. This is unexpected behavior but my implementation seems to match the 
demo so I do not believe this is a bug.
