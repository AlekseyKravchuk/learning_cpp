/*
 * Bitmap.cpp
 *
 *  Created on: May 04, 2020
 *      Author: Aleksey Kravchuk
 */
#include <fstream>
#include <iostream>
#include "Bitmap.h"
#include "BitmapInfoHeader.h"
#include "BitmapFileHeader.h"

namespace bmp {

Bitmap::Bitmap(int width, int height) :
		m_width(width), m_height(height), m_pPixels(
				new uint8_t[width * height * 3] { }) {

}

bool Bitmap::write(string filename) {

	BitmapFileHeader fileHeader;
	BitmapInfoHeader infoHeader;

	fileHeader.fileSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader)
			+ m_width * m_height * 3;
	fileHeader.dataOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

	infoHeader.width = m_width;
	infoHeader.height = m_height;

	ofstream file;
	file.open(filename, ios::out | ios::binary);

	if (!file) {
    std::cout << "file: " << filename << " could not be opened for binary writing.";
		return false;
	}

	file.write((char *) &fileHeader, sizeof(fileHeader));
	file.write((char *) &infoHeader, sizeof(infoHeader));
	file.write((char *) m_pPixels.get(), m_width * m_height * 3);

	file.close();

	if (!file) {
		return false;
	}

	return true;
}

void Bitmap::setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
	uint8_t *pPixel = m_pPixels.get();

	pPixel += y * m_width * 3 + (x * 3);

	pPixel[0] = blue;
	pPixel[1] = green;
	pPixel[2] = red;
}

Bitmap::~Bitmap() {
	// TODO Auto-generated destructor stub
}


} /* namespace bmp */
