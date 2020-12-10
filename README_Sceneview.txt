NOTE (11/19): I have been in communication within Griffin about an issue with the handin.
I was informed that the implementation was segfaulting on scene loading, but this is not
occuring in my version. On my local windows machine the Sceneview appears fully functional and over
fastx on a department machine it matches the demo. That said, I am seeing a black screen over fastx 
for both my implementation and the demo if I have "use lighting" checked. This version includes my
 Intersect code, but no changes have been made to the Sceneview code. As I can't recreate the bug,
  I'm unsure what else to do :(. Thanks for your hard work helping figure this out!

I followed the stencil closely for my implementation of scenview. In parse, I saved in member variables the global data,
a vector of lighting data, a vector of primitives, and a vector of the matrix transformations for those primitives. 
To build the primitive list I did a iterative implementation of DFS through the scenegraph. During my traversal I 
maintained a stack of nodes, a stack of transformation matrices (where M_i = M-1 * ... * M_i-1), and a list tracking 
whether nodes' children had been added to the stack already. Using these, I was able to visit each child once and have 
access to its transformation matrix while examining it. 
 
To keep my rendering efficient, I used the flywheel pattern in SceneviewScene. To do this, I instantiated a single 
instance of each primitive in the constructor and whenever settings changes. Then, when I needed a new primitive I
just changed uniforms and redrew these objects. Finally, for lights and colors I simply called into the associated 
methods of m_phongShader.

No known bugs (though I did have trouble getting the demo to work for comparison so I may have missed something).