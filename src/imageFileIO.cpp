/** ***************************************************************************
* @file
*
* @brief contains functions that read and write files both binary and ascii
******************************************************************************/
#include "netPBM.h"

/** ***************************************************************************
 * @author Cameron Custer
 *
 * @par Description:
 * This function outputs an error message and exits the program if the
 * incorrect command line arguments are provided. The functions also specifies
 * the correct arguments for the programmer.
 *
 * @returns none
 *****************************************************************************/
void usageStatement( )
{
    cout <<
        "floodfill image.ppm starting_row starting_column new_red_value new_green_value new_blue_value"
        << endl;
    // exit without fail
    exit( 0 );
}

/** ***************************************************************************
 * @author Cameron Custer
 *
 * @par Description:
 * This function dynamically allocates the 2 dimensional arrays for red,
 * green, and blue pixels. The function then calls another function to read
 * the data in Ascii or Binary based on the encoder type provided in the
 * image header.
 *
 * @param[in] imageFile - the input image file to provide the data
 * @param[in, out] specifications - the content of the image file in a
 * structure hosting the pixel (unsigned character) arrays to be read into
 * @param[in] argc - an intiger containing the number of command line
 * arguments provided
 * @param[in] argv - a character array containing the command line arguments
 * provided
 *
 * @returns None
 *****************************************************************************/
void read( fstream &imageFile, image &specifications,
           int argc, char *argv[] )
{
    // allocate 2d arrays for each color
    allocArray( specifications.red, specifications.rows,
                specifications.cols );
    allocArray( specifications.green, specifications.rows,
                specifications.cols );
    allocArray( specifications.blue, specifications.rows,
                specifications.cols );

    // check the encoder type of the image and read the data respectively
    if( specifications.encType == "P3" )
        readAscii( imageFile, specifications );
    else if( specifications.encType == "P6" )
        readBinary( imageFile, specifications );
}

/** ***************************************************************************
 * @author Cameron Custer
 *
 * @par Description:
 * This function will write the image data in ascii or binary based on the
 * command line. The encoder type is also specified in this function. This
 * function will also free the memory from the two dimensional arrays,
 * and if incorrect command line arguments are provided then a usage statement
 * is output and the program exits with a 0. The function seeks to the
 * begining of the file and clears before proceeding.
 *
 * @param[in] writeFile - the output image file to output the data too
 * @param[in, out] specifications - the content of the image file in a
 * structure containing the pixel (unsigned character) arrays which are
 * written too
 * @param[in] argc - an intiger containing the number of command line
 * arguments provided
 * @param[in] argv - a character array containing the command line arguments
 * provided
 *
 * @returns None
 *****************************************************************************/
void write( fstream &writeFile, image &specifications, int argc,
            char *argv[] )
{
    writeFile.seekp( 0, ios::beg );
    writeFile.clear( );
    if( specifications.encType == "P3" )
        writeAscii( writeFile, specifications );
    if( specifications.encType == "P6" )
        writeBinary( writeFile, specifications );
}

/** ***************************************************************************
 * @author Cameron Custer
 *
 * @par Description:
 * This function reads the header of the image to the image structure. The
 * function reads the encoder type, the comments, the columns and rows, and
 * the maximum value of a pixel contained in the image. This function uses the
 * encoder type of the image to identify the image file type.
 *
 * @param[in] imageFile - this is the input image file containing the
 * origional content of the image in Binary or Ascii
 * @param[in, out] specifications - the content of the image file in a
 * structure which is read into
 *
 * @returns None
 *****************************************************************************/
void readImageHeader( fstream &imageFile, image &specifications )
{
    string fileType;
    string line;

    imageFile.clear( );
    imageFile.seekg( 0, ios::beg );

    imageFile >> specifications.encType;
    imageFile.ignore( );

    // read as many lines of comments as necessary
    while( imageFile.peek( ) == '#' )
    {
        getline( imageFile, line );
        if( specifications.comments.size( ) != 0 )
            specifications.comments += '\n';
        specifications.comments += line;
    }
    // output the image data to the image structure
    imageFile >> specifications.cols >>
        specifications.rows >>
        specifications.maxValue;
    imageFile.ignore( );
}

/** ***************************************************************************
 * @author Cameron Custer
 *
 * @par Description:
 * This function reads the data from an Ascii (P3) type image into the
 * specifications structure. The data from the image is stored in the 2
 * dimensional arrays perviously allocated for red, green, and blue.
 *
 * @param[in] imageFile - this is the input image file containing the
 * origional content of the image in Binary or Ascii
 * @param[in, out] specifications - the content of the image file in a
 * structure which contains the 2 dimensional arrays that are read into.
 *
 * @returns None
 *****************************************************************************/
void readAscii( fstream &imageFile, image &specifications )
{
    int i, j;
    int color;
    // read the data for as many cols and rows exist in the image
    for( i = 0; i < specifications.rows; i++ )
    {
        for( j = 0; j < specifications.cols; j++ )
        {
            imageFile >> color;
            specifications.red[i][j] = color;

            imageFile >> color;
            specifications.green[i][j] = color;

            imageFile >> color;
            specifications.blue[i][j] = color;
        }
    }
}

/** ***************************************************************************
 * @author Cameron Custer
 *
 * @par Description:
 * This function reads the data from an Binary (P6) type image into the
 * specifications structure. The data from the image is stored in the 2
 * dimensional arrays perviously allocated for red, green, and blue.
 *
 * @param[in] imageFile - this is the input image file containing the
 * origional content of the image in Binary or Ascii
 * @param[in, out] specifications - the content of the image file in a
 * structure which contains the pixel (unsigned character) 2 dimensional
 * arrays which must be read into.
 *
 * @returns None
 *****************************************************************************/
void readBinary( fstream &imageFile, image &specifications )
{
    int i, j;
    int color;
    // read the data for as many cols and rows exist in the image
    for( i = 0; i < specifications.rows; i++ )
    {
        for( j = 0; j < specifications.cols; j++ )
        {
            imageFile.read( (char *) &color, 1 );
            specifications.red[i][j] = color;

            imageFile.read( (char *) &color, 1 );
            specifications.green[i][j] = color;

            imageFile.read( (char *) &color, 1 );
            specifications.blue[i][j] = color;
        }
    }
}

/** ***************************************************************************
 * @author Cameron Custer
 *
 * @par Description:
 * This function writes the data to an Ascii (P3) type image. This includes
 * writing the image header and all of that data, as well as the image
 * content, after being modified as specified.
 *
 * @param[in] writeFile - the output file that the data from the image
 * contained in the image structure is written too
 * @param[in, out] specifications - the content of the image file in a
 * structure which contains the pixel (unsigned character) 2 dimensional
 * arrays which are written too
 *
 * @returns None
 *****************************************************************************/
void writeAscii( fstream &writeFile, image specifications )
{
    int i, j;
    // check for comments in the header and write the data
    if( specifications.comments.size( ) == 0 )
    {
        writeFile << specifications.encType << endl
            << specifications.cols << ' ' << specifications.rows << endl
            << specifications.maxValue << '\n';
    }
    else
    {
        writeFile << specifications.encType << endl
            << specifications.comments << endl
            << specifications.cols << ' ' << specifications.rows << endl
            << specifications.maxValue << '\n';
    }

    // write the data of the image for each row in each column
    for( i = 0; i < specifications.rows; i++ )
    {
        for( j = 0; j < specifications.cols; j++ )
        {
            writeFile
                << (int) specifications.red[i][j] << ' '
                << (int) specifications.green[i][j] << ' '
                << (int) specifications.blue[i][j] << ' ';
        }
    }
}

/** ***************************************************************************
 * @author Cameron Custer
 *
 * @par Description:
 * This function writes the data to an Binary (P6) type image. This includes
 * writing the image header and all of that data, as well as the image
 * content, after being modified as specified.
 *
 * @param[in] writeFile - the output file that the data from the image
 * contained in the image structure is written too
 * @param[in, out] specifications - the content of the image file in a
 * structure which contains the pixel (unsigned character) arrays which the
 * function writes too
 *
 * @returns None
 *****************************************************************************/
void writeBinary( fstream &writeFile, image specifications )
{
    int i, j;
    // check for comments in the image header and output the data
    if( specifications.comments.size( ) == 0 )
    {
        writeFile << specifications.encType << endl
            << specifications.cols << ' ' << specifications.rows << endl
            << specifications.maxValue << '\n';
    }
    else
    {
        writeFile << specifications.encType << endl
            << specifications.comments << endl
            << specifications.cols << ' ' << specifications.rows << endl
            << specifications.maxValue << '\n';
    }

    // write the data for image to the output file
    for( i = 0; i < specifications.rows; i++ )
    {
        for( j = 0; j < specifications.cols; j++ )
        {
            writeFile.write( (char *) &specifications.red[i][j], 1 );
            writeFile.write( (char *) &specifications.green[i][j], 1 );
            writeFile.write( (char *) &specifications.blue[i][j], 1 );
        }
    }
}
