#include <iostream>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include "bmplib.h"

using namespace std;

//============================Add function prototypes here======================

void dummy(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB]);

void convolve(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
			  int N, double kernel[][11]);

void sobel(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB]);

void gaussian(double k[][11], int N, double sigma);

void gaussian_filter(unsigned char out[][SIZE][RGB], 
					 unsigned char in[][SIZE][RGB], int N, double sigma);

void unsharp(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
			 int N, double sigma, double alpha);

//============================Do not change code in main()======================

#ifndef AUTOTEST

int main(int argc, char* argv[])
{
	// First check argc
	if(argc < 3)
	{
		//we need at least ./filter <input file> <filter name> to continue
		cout << "usage: ./filter <input file> <filter name> <filter parameters>";
		cout << " <output file name>" << endl;
		return -1;
	}
	// then check to see if we can open the input file
	unsigned char input[SIZE][SIZE][RGB];
	unsigned char output[SIZE][SIZE][RGB];
	char* outfile;
	int N;
	double sigma, alpha;
	// double kernel[11][11];
	
	// read file contents into input array
	int status = readRGBBMP(argv[1], input);
	if(status != 0)
	{
		cout << "unable to open " << argv[1] << " for input." << endl;
		return -1;
	}
	//Input file is good, now look at next argument
	if( strcmp("sobel", argv[2]) == 0)
	{
		sobel(output, input);
		outfile = argv[3];
	}
	else if( strcmp("blur", argv[2]) == 0)
	{
		if(argc < 6)
		{
			cout << "not enough arguments for blur." << endl;
			return -1;
		}
		N = atoi(argv[3]);
		sigma = atof(argv[4]);
		outfile = argv[5];
		gaussian_filter(output, input, N, sigma);
	}
	else if( strcmp("unsharp", argv[2]) == 0)
	{
		if(argc < 7)
		{
			cout << "not enough arguments for unsharp." << endl;
			return -1;
		}
		N = atoi(argv[3]);
		sigma = atof(argv[4]);
		alpha = atof(argv[5]);
		outfile = argv[6];
		unsharp(output, input, N, sigma, alpha);
		
	}
	else if( strcmp("dummy", argv[2]) == 0)
	{
		//do dummy
		dummy(output, input);
		outfile = argv[3];
	}
	else
	{
		cout << "unknown filter type." << endl;
		return -1;
	}
	
	if(writeRGBBMP(outfile, output) != 0)
	{
		cout << "error writing file " << argv[3] << endl;
	}
	
}

#endif

//=========================End Do not change code in main()=====================

// Creates an identity kernel (dummy kernel) that will simply
// copy input to output image and applies it via convolve()
//
// ** This function is complete and need not be changed.
// Use this as an example of how to create a kernel array, fill it in
// appropriately and then use it in a call to convolve.
void dummy(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB])
{
	double k[11][11];
	for (int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			k[i][j] = 0;
		}
	}
	k[1][1] = 1;
	convolve(out, in, 3, k);
}

// Convolves an input image with an NxN kernel to produce the output kernel
// You will need to complete this function by following the
// instructions in the comments
void convolve(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
int N, double kernel[][11])
{
	int padded[SIZE+10][SIZE+10][RGB];  // Use for input image with appropriate
										// padding
	int temp[SIZE][SIZE][RGB];          // Use for the unclamped output pixel
										// values then copy from temp to out,
										// applying clamping
	
	// first set all of padded to 0 (black)
	for (int i = 0; i < SIZE + 10; i++)
	{
		for (int j = 0; j < SIZE + 10; j++)
		{
			for (int k = 0; k < RGB; k++)
			{
				padded[i][j][k] = 0;
			}
		}
	}

	// now copy input into padding to appropriate locations
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			for (int k = 0; k < RGB; k++)
			{
				padded[i + (N/2)][j + (N/2)][k] = in[i][j][k];
			}
		}
	}

	// initialize temp pixels to 0 (black)
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			for (int k = 0; k < RGB; k++)
			{
				temp[i][j][k] = 0;
			}
		}
	}
	
	
	// now perform convolve (using convolution equation on each pixel of the
	// actual image) placing the results in temp (i.e. unclamped result)
	// Here we give you the structure of the convolve for-loops, you need
	// to figure out the loop limits
	for(int y = 0; y < SIZE; y++)
	{
		for(int x = 0; x < SIZE; x++)
		{
			for(int k = 0; k < RGB; k++)
			{
				for(int i = -N/2; i < N/2 + 1; i++)
				{
					for(int j = -N/2; j < N/2 + 1; j++)
					{
						temp[y][x][k] += (int) (padded[y + i + (N/2)][x + j + 
                        (N/2)][k] * kernel[i + (N/2)][j + (N/2)]);
					}
				}
			}
		}
	}
	
	// now clamp and copy to output
	// You may need to cast to avoid warnings from the compiler:
	// (i.e. out[i][j][k] = (unsigned char) temp[i][j][k];)
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			for (int k = 0; k < RGB; k++)
			{
				int num = temp[i][j][k];
				if (num < 0) temp[i][j][k] = 0;
				else if (num > 255) temp[i][j][k] = 255;
				out[i][j][k] = (unsigned char) temp[i][j][k];
			}
		}
	}
}

// You will need to complete this function by following the
// instructions in the comments
void sobel(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB])
{
	double k[11][11];
	double s_h1[3][3] = { {-1, 0, 1},
						  {-2, 0, 2},
						  {-1, 0, 1} };
	double s_h2[3][3] = { {1, 0, -1},
						  {2, 0, -2},
						  {1, 0, -1} };
	
	unsigned char h1_soble[SIZE][SIZE][RGB]; //hold intermediate images
	unsigned char h2_soble[SIZE][SIZE][RGB];	
	
	// Copy in 1st 3x3 horizontal sobel kernel (i.e. copy s_h1 into k)
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			k[i][j] = s_h1[i][j];
		}
	}
	
	// Call convolve to apply horizontal sobel kernel with result in h1_soble
	convolve(h1_soble, in, 3, k);
	
	// Copy in 2nd 3x3 horizontal sobel kernel (i.e. copy s_h2 into k)
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			k[i][j] = s_h2[i][j];
		}
	}
	
	// Call convolve to apply horizontal sobel kernel with result in h2_soble
	convolve(h2_soble, in, 3, k);
	
	// Add the two results (applying clamping) to produce the final output
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			for (int l = 0; l < RGB; l++)
			{
                int num = (int) (h1_soble[i][j][l] + h2_soble[i][j][l]);
				if (num < 0) out[i][j][l] = 0;
				else if (num > 255)  out[i][j][l] = 255;
                out[i][j][l] = (unsigned char) num; 
			}
		}
	}
}

// Add the rest of your functions here (i.e. gaussian, gaussian_filter, unsharp)
void gaussian(double k[][11], int N, double sigma)
{
	double gaussX[11][11];
	double gaussY[11][11];
	double total = 0;

	// Initialize with arrays with proper (converted) indexing
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			gaussX[i][j] = i - N/2;
			gaussY[i][j] = j - N/2;
		}
	}

	// Evaluate Gaussian equation and assign raw values to kernel
	for (int i  = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			double x = pow(gaussX[i][j], 2) / (2 * pow(sigma, 2));
			double y = pow(gaussY[i][j], 2) / (2 * pow(sigma, 2));
			k[i][j] = exp(-1*(x + y));
			total += k[i][j];
		}
	}

	// Normalize values
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			k[i][j] /= total;
			cout << k[i][j] << " ";
		}
		cout << endl;
	}
}

void gaussian_filter(unsigned char out[][SIZE][RGB],
unsigned char in[][SIZE][RGB], int N, double sigma)
{
	// Declare and initialize kernel
	double k[11][11];

	// Create Gaussian kernel and use kernel with convolve
	gaussian(k, N, sigma);
	convolve(out, in, N, k);
}

void unsharp(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
int N, double sigma, double alpha)
{
	unsigned char b[SIZE][SIZE][RGB];
	int d[SIZE][SIZE][RGB];
	gaussian_filter(b, in, N, sigma);

	// Detailed Image = Original - Blurred
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			for (int k = 0; k < RGB; k++)
			{
				d[i][j][k] = (int) (in[i][j][k] - b[i][j][k]);
			}
		}
	}

	// Sharpened Image = Original + (alpha * Detailed)
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			for (int k = 0; k < RGB; k++)
			{
				int temp = (int) (in[i][j][k] + (alpha * d[i][j][k]));
				if (temp < 0) temp = 0;
				else if (temp > 255) temp = 255;
				out[i][j][k] = (unsigned char) temp;
			}
		}
	}
}