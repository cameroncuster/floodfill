/** ***************************************************************************
* @file
*
* @brief contains protoypes a typedef and the image structure
******************************************************************************/
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#ifndef __NETPBM__H__

/** ***************************************************************************
 * @brief variable to stop redefinition errors
 *****************************************************************************/
#define __ NETPBM__H__
using namespace std;

/** **************************************************************************!
 * \typedef unsigned char to pixel.
 *****************************************************************************/
typedef unsigned char pixel;

/** ***************************************************************************
 * @brief image structure holds all the data for the image both the header,
 * and the content of the image.
 *****************************************************************************/
struct image
{
    string encType; /*!< the string containing the encoder type of the image */
    string comments; /*!< the string containing the comments of the image in
                     the image header */
    int rows; /*!< the number of rows of content in the image */
    int cols; /*!< the number of columns of conntent in the image */
    string maxValue; /*!< the string containing the maximum value of a pixel */
    pixel **red; /*!< a 2d array contianing the red pixel values */
    pixel **green; /*!< a 2d array containing the green pixel values */
    pixel **blue; /*!< a 2d array containing the blue pixel values */
};

/******************************************************************************
 *                         Function Prototypes
 *****************************************************************************/
 // recursive cfill
void cfill( image &specifications, int row, int col, pixel newred,
            pixel newgreen, pixel newblue, pixel prevred, pixel prevblue,
            pixel prevgreen );

// direct operations and output at usage statement
void usageStatement( );
void validateArgs( int argc, char *argv[], int &row, int &col, pixel &red,
                   pixel &green, pixel &blue );
void read( fstream &imageFile, image &specifications, int argc, char *argv[] );
void write( fstream &writeFile, image &specifications, int argc,
            char *argv[] );

// fileio
void readImageHeader( fstream &imageFile, image &specificaitons );
void readAscii( fstream &imageFile, image &specifications );
void readBinary( fstream &imageFile, image &specifications );
void writeAscii( fstream &writeFile, image specifications );
void writeBinary( fstream &writeFile, image specifications );

// memory
void allocArray( pixel **&color, int rows, int cols );
void free2d( pixel **&color, int rows );

#endif
