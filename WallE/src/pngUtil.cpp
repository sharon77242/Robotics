/*
 * pngUtil.cpp
 *
 *  Created on: Mar 30, 2015
 *      Author: colman
 */
#include "pngUtil.h"
#include "lodepng.h"
#include <iostream>


//Encode from raw pixels to disk with a single function call
//The image argument has width * height RGBA pixels or width * height * 4 bytes
void encodeOneStep(const char* filename, std::vector<unsigned char> image,
		unsigned width, unsigned height) {
	//Encode the image
	unsigned error = lodepng::encode(filename, image, width, height);

	//if there's an error, display it
	if (error)
		std::cout << "encoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;
}

void decodeOneStep(const char* filename) {
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;

	//decode
	unsigned error = lodepng::decode(image, width, height, filename);

	//if there's an error, display it
	if (error)
		std::cout << "decoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;

	//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
}
void ConvertMapBlackToWhiteAndWhiteToBlack(const char* filename) {
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;
	unsigned x, y;
	//decode
	unsigned error = lodepng::decode(image, width, height, filename);

	//if there's an error, display it
	if (error)
		std::cout << "decoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;

	std::vector<unsigned char> navImage; //the raw pixels
	navImage.resize(width * height * 4);
	unsigned char color;
	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++) {
			if (image[y * width * 4 + x * 4 + 0]
					|| image[y * width * 4 + x * 4 + 1]
					|| image[y * width * 4 + x * 4 + 2])
				color = 0;
			else
				color = 255;
			navImage[y * width * 4 + x * 4 + 0] = color;
			navImage[y * width * 4 + x * 4 + 1] = color;
			navImage[y * width * 4 + x * 4 + 2] = color;
			navImage[y * width * 4 + x * 4 + 3] = 255;
		}

	encodeOneStep("newMap.png", navImage, width, height);
}
