/** ***************************************************************************
* @file
*
* @brief contains the main function
******************************************************************************/

/** ***************************************************************************
 * @mainpage Recursive Flood Fill
 *
 * @authors Cameron Custer
 *
 * @date 24 Nov. 2020
 *
 * @section program_section Program Information
 * This program reads the content of a PPM image formatted in either binary or
 * ascii. The image data is specified as RGB. Each color specifications of
 * each pixel is read and stored in a 2 dimensional array. The red array hosts
 * the image data for the red value contained in each pixel, the green array
 * hosts the image data for the green value contained in each pixel, and the
 * blue array hosts the image data for the blue value contained in each pixel.
 * After all of the data has been read the operation of the cfill is performed
 * on the image data contained in the 2 dimensional arrays.
 *
 * The operator specifies a valid row and column of a pixel in the image
 * containing the location and color to overwrite in the image. The cfill
 * function locates the row and column of this pixel.
 *
 * The cfill function operated recursively by altering each pixel of each
 * array in every possible direction. Pixel, meaining each RGB value for each
 * pixel. The cfill functions stops when either another color is reached or
 * the edge of the image is reached.
 *
 * Once the pixels have been modified the image is output to overwrite
 * the origional image data contianed in the file. The image now contains
 * the modificaitons made by the cfill function and is stored for viewing.
 *
 * @section compile_section Compiling and Usage
 *
 * @par Compiling Instructions:
 *      The program stack commit and reverse size must be compiled with at
 *      least 4000000000
 *
 * @par Usage
   @verbatim
   C:\> floodfill image.ppm starting_row starting_column new_red_value new_green_value new_blue_value
   @endverbatim
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 *
 *****************************************************************************/
#include "netPBM.h"

 /** ***************************************************************************
  * @author Cameron Custer
  *
  * @par Description:
  * This is the main function in the program. The programmer specifies how the
  * program is run based on the command line arguments provided to the main
  * function. The input image and output image paths are varified and file are
  * opened in the main function. If unable to open either file, an error
  * message is output and program exits. The data is read as specified by a
  * function refrenced in main based on the command line arguments which are
  * verified. The data is then modified if necessary and output to the output
  * file. The files are closed and the and the program ends.
  *
  * @param[in] argc - an intiger containing the number of command line
  * arguments provided
  * @param[in] argv - a character array containing the command line arguments
  * provided
  *
  * @returns 0 exits with code 0 after the program has run, and exit value of a
  * 1 indicates a program failure.
  *****************************************************************************/
int main( int argc, char *argv[] )
{
    // declarations
    fstream imageFile;
    image specifications;
    int row, col;
    pixel red, green, blue, prevred, prevgreen, prevblue;

    // check for proper amount of arguments
    if( argc != 7 )
        usageStatement( );

    // open the image and verify
    imageFile.open( argv[1], ios::binary | ios::in | ios::out );
    if( !imageFile.is_open( ) )
    {
        cout << "Unable to open: " << argv[1] << endl;
        return 0;
    }

    // validate command line arguments
    validateArgs( argc, argv, row, col, red, green, blue );

    // read the image header
    readImageHeader( imageFile, specifications );

    // read the image data into the 2 dimensional arrays
    read( imageFile, specifications, argc, argv );

    // initialize the previous values of the pixel
    prevred = specifications.red[row][col];
    prevgreen = specifications.green[row][col];
    prevblue = specifications.blue[row][col];

    // perform the cfill starting at the current pixel on the image
    cfill( specifications, row, col, red, green, blue, prevred, prevgreen,
           prevblue );

    // write the modified image data containing the cfill
    // over the origional image data
    write( imageFile, specifications, argc, argv );

    // return 0 for success
    return 0;
}

/** ***************************************************************************
 * @author Cameron Custer
 *
 * @par Description:
 * This is the cfill functions. Otherwise known as a bucketfill. The cfill
 * function modifies the two dimensional arrays containing the red, green, and
 * blue values for each pixel according to the programmers specification from
 * the command line. This function utilizes the color value of the pixel
 * specified by it's row and column from the command line. The function
 * recursively modifies each pixel surrounding the specified pixel by
 * overwriting the pixel values with the new color as specified by the command
 * line. The function then returns when the current color value changes or
 * boundry of the image is reached.
 *
 * @param[in, out] specifications - the structure containing the image data for
 * all three of the color arrays to be modified ( red, green, blue )
 * @param[in, out] row - the current row of the image being modified in each
 * 2 dimensional array
 * @param[in, out] col- the current column of the image being modified in each
 * 2 dimensional array
 * @param[in] newred - the new red value for the pixel
 * @param[in] newgreen - the new green value for the pixel
 * @param[in] newblue - the new blue value for the pixel
 * @param[in] prevred - the origional red value for the pixel
 * @param[in] prevgreen - the origional green value for the pixel
 * @param[in] prevblue - the origional blue value for the pixel
 *
 * @returns None
 *****************************************************************************/
void cfill( image &specifications, int row, int col, pixel newred,
            pixel newgreen, pixel newblue, pixel prevred, pixel prevgreen,
            pixel prevblue )
{
    // base case for image boundry ( check first )
    if( row < 0 || col < 0 || row > specifications.rows - 1 ||
        col > specifications.cols - 1 )
        return;

    // base case for change in color ( check latter )
    if( specifications.red[row][col] != prevred ||
        specifications.green[row][col] != prevgreen ||
        specifications.blue[row][col] != prevblue )
        return;

    // initialize the pixels to the new color
    specifications.red[row][col] = newred;
    specifications.green[row][col] = newgreen;
    specifications.blue[row][col] = newblue;

    // fill the image left right up down until image boundry or new color is
    // reached ( e.a. base case )
    cfill( specifications, row + 1, col, newred, newgreen, newblue, prevred,
           prevgreen, prevblue );
    cfill( specifications, row - 1, col, newred, newgreen, newblue, prevred,
           prevgreen, prevblue );
    cfill( specifications, row, col + 1, newred, newgreen, newblue, prevred,
           prevgreen, prevblue );
    cfill( specifications, row, col - 1, newred, newgreen, newblue, prevred,
           prevgreen, prevblue );
}

/** ***************************************************************************
 * @author Cameron Custer
 *
 * @par Description:
 * This function initializes the values of the row, column, red, green, and
 * blue values as specified by the command line. This functions has no
 * returns becuase all of the intialized values are passed by refrence. The
 * command line arguments are also passed into this function and typecasted as
 * strings so that the data may be extracted.
 *
 * @param[in] argc - an intiger containing the number of command line
 * arguments provided
 * @param[in] argv - a character array containing the command line arguments
 * provided
 * @param[in, out] row - the row of the starting position
 * @param[in, out] col - the column of the starting position
 * @param[in, out] red - the new red pixel value
 * @param[in, out] green - the new green pixel value
 * @param[in, out] blue - the new blue pixel value
 *
 * @returns none
 *****************************************************************************/
void validateArgs( int argc, char *argv[], int &row, int &col, pixel &red,
                   pixel &green, pixel &blue )
{
    // store command line arguments 2-6
    row = stoi( (string) argv[2] );
    col = stoi( (string) argv[3] );
    red = stoi( (string) argv[4] );
    green = stoi( (string) argv[5] );
    blue = stoi( (string) argv[6] );
}
