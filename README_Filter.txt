My architecture for Filter was very straightforward. I have classes for each type of filter and a 
class of helper functions. Each class just has static methods and so are just there for code organization.
Among the helper functions is a 2d convolution function. This function is used by both my gaussian 
blur filter and my sobelfilter. In both cases, I decomposed the 2d convolution operations into two 
1d convolutions but by specifying the kernel dimensions in 2d (1 by n or n by 1) I used this conv2d
function for 1d convolution. 

For the sobel filter I applied two 1d convolutions each for the horizontal and vertical dimensions and 
then added the resulting images in quadrature. 

For the gaussian filter I applied a 1d gaussian kernel in each of the vertical and horizontal directions.

For the scaling filter I followed the derivations done by Spike here: 
https://docs.google.com/document/d/1JN7_c8NYSyryJz2-UrV1OAylnWL59c0StyaxRswB_aY/edit.
I used a triangle function instead of sinc. I wrote a separate function for each dimension and scaling
direction (up/down) which is not the cleanest software design but was useful for debugging as I had to 
make many adjustments before scaling worked correctly.

I was hoping to add many more speed optimizations but was unfortunately slowed down by a nasty bug in 
the scaling filter and so ran out of time. The filters don't run too slowly so I'm not worried about this.
If I'd had time I would have precomputed more values and used a single index to iterate through images 
instead of tracking a row and column (to avoid multiplications). I would have also tried to replace some
column wise iterations (during vertical scales for example) with horizontal iterations for cache efficiency. 

Known bugs: Not sure if this is a bug exactly, but the sensitivty parameter affects my sobel filter differently
than the demo (the results are the same when sensitivity == 1 though).