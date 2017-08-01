/*
 * pngUtil.h
 *
 *  Created on: Mar 30, 2015
 *      Author: colman
 */

#ifndef PNGUTIL_H_
#define PNGUTIL_H_
#include <iostream>
#include <vector>
void encodeOneStep(const char* filename, std::vector<unsigned char> image, unsigned width, unsigned height);
void decodeOneStep(const char* filename);
void ConvertMapBlackToWhiteAndWhiteToBlack(const char* filename);



#endif /* PNGUTIL_H_ */
