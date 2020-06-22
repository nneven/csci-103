Nicolas Neven
nneven@usc.edu

1) If you hold N constant and vary sigma, what do you see? Conversely, if you
vary N and hold sigma the same, what do you see?

The sigma value defines the amount of blur you want applied to the image. The
higher the the sigma value, the more blurred the image becomes. The smaller the
sigma value, the less blurred the image becomes. Sigma can also be thought of as
the 'blur radius' in terms of pixel space. The kernel size N will determine how
many pixels to sample during convolution. A larger N removes more noise from the
image but also blurs out the edges more (trade-off). In general, N determines
how many pixels to sample during convolution and sigma defines how much to
change/blur them by.

2) What does the Sobel filter appear to do?

The sobel filer detects/highlights edges in an image and produces a resulting
image that emphasizes edges. It seems to remove most of the color by turning non
edges black in an image and thicken the edges in an image turning them the color
white. 

3) Do you get the original image back?

No you do not get the original image back because Gaussian blur and unsharp-mask
are not inverse operations. Without knowing the exact Gaussian paramaters that
went into blurring the image, it is not possible to perfectly remove the blur.
The unsharp-mask relies on an alpha paramater that determines how much of the
detailed image is added back to the original. Changing the value effects the
output image but even using a 'proper' value will not revert the blurred image
back to the original. The Guassian blue and unsharp-mask filters are opposites
of eachother, but they are not invereses. 

4) Express in mathematical terms how the number of calculations your program
does grows with the size, N, of the kernel.

The number of calculations my program does in relation to the size of the kernel
is N^2. This is because the only filter that uses N is Gaussian blur. Within the
Gaussian blur functions, there are double nested for loops that go up until N.
So the number of calculations based on N is N * N = N^2. 
