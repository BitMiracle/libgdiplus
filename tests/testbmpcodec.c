#ifdef WIN32
#ifndef __cplusplus
#error Please compile with a C++ compiler.
#endif
#endif

#if defined(USE_WINDOWS_GDIPLUS)
#include <Windows.h>
#include <GdiPlus.h>

#pragma comment(lib, "gdiplus.lib")
#else
#include <GdiPlusFlat.h>
#endif

#ifdef USE_WINDOWS_GDIPLUS
using namespace Gdiplus;
using namespace DllExports;
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "testhelpers.h"

static const char *file = "temp_asset.bmp";
static WCHAR wFile[] = {'t', 'e', 'm', 'p', '_', 'a', 's', 's', 'e', 't', '.', 'b', 'm', 'p', 0};

static GpImage *createFile (BYTE *buffer, int length, GpStatus expectedStatus)
{
	GpStatus status;
	GpImage *image;
	FILE *f = fopen (file, "w+");
	fwrite ((void *) buffer, sizeof (BYTE), length, f);
	assert (f);
	fclose (f);

	status = GdipLoadImageFromFile (wFile, &image);
	assertEqualInt (status, expectedStatus);
	
	return image;
}

static void createFileSuccess (BYTE *buffer, int length, PixelFormat expectedFormat)
{
	PixelFormat format;
	GpImage *image = createFile (buffer, length, Ok);

	GdipGetImagePixelFormat (image, &format);
	assertEqualInt (format, expectedFormat);
	
	GdipDisposeImage (image);
}

static void test_validImage1bppOS2Header ()
{
	BYTE image1x1[] = {
		'B', 'M', 36, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0,
		12, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0,
		255, 0, 0, 0, 255, 0,
		B8(10000000), B8(00000000), B8(00000000), 0
	};
	BYTE image24x4[] = {
		'B', 'M', 48, 0, 0, 0, 0, 0, 0, 0, 0x20, 0, 0, 0,
		12, 0, 0, 0, 24, 0, 4, 0, 1, 0, 1, 0,
		255, 0, 0, 0, 255, 0,
		B8(10000000), B8(00000000), B8(00000000), 0,
		B8(10000000), B8(00000000), B8(00000000), 0,
		B8(00100111), B8(00000000), B8(00000000), 0,
		B8(11111111), B8(11111111), B8(11111111), 0
	};

	createFileSuccess (image1x1, sizeof (image1x1), PixelFormat1bppIndexed);
	createFileSuccess (image24x4, sizeof (image24x4), PixelFormat1bppIndexed);
}

static void test_validImage1bppBitmapInfoHeader ()
{
	BYTE image1x1[] = {
		'B', 'M', 66, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0,
		40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255,
		B8(10000000), 0, 0, 0
	};
	BYTE image24x4CustomSizedPalette[] = {
		'B', 'M', 82, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 24, 0, 0, 0, 4, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 255, 255, 255, 255,
		B8(10000000), B8(00000000), B8(00000000), 0,
		B8(10000000), B8(00000000), B8(00000000), 0,
		B8(00100111), B8(00000000), B8(00000000), 0,
		B8(11111111), B8(11111111), B8(11111111), 0
	};
	BYTE image1x1NegativeHeight[] = {
		'B', 'M', 66, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0,
		40, 0, 0, 0, 1, 0, 0, 0, 255, 255, 255, 255, 1, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255,
		B8(1000000), 0, 0, 0
	};
	BYTE image24x4NegativeHeight[] = {
		'B', 'M', 82, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 24, 0, 0, 0, 252, 255, 255, 255, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 255, 255, 255, 255,
		B8(10000000), B8(00000000), B8(00000000), 0,
		B8(10000000), B8(00000000), B8(00000000), 0,
		B8(00100111), B8(00000000), B8(00000000), 0,
		B8(11111111), B8(11111111), B8(11111111), 0
	};

	createFileSuccess (image1x1, sizeof (image1x1), PixelFormat1bppIndexed);
	createFileSuccess (image24x4CustomSizedPalette, sizeof (image24x4CustomSizedPalette), PixelFormat1bppIndexed);
	createFileSuccess (image1x1NegativeHeight, sizeof (image1x1NegativeHeight), PixelFormat1bppIndexed);
	createFileSuccess (image24x4NegativeHeight, sizeof (image24x4NegativeHeight), PixelFormat1bppIndexed);
}

static void test_validImage4bppOS2Header ()
{
	BYTE image1x1[] = {
		'B', 'M', 122, 0, 0, 0, 0, 0, 0, 0, 0x59, 0, 0, 0,
		12, 0, 0, 0, 1, 0, 1, 0, 1, 0, 4, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 0, 255, 255, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 8, 8, 8, 8, 255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 0, 255, 255, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255,
		B8(00010010), 0, 0, 0
	};
	BYTE image6x4[] = {
		'B', 'M', 94, 0, 0, 0, 0, 0, 0, 0, 0x29, 0, 0, 0,
		12, 0, 0, 0, 6, 0, 4, 0, 1, 0, 4, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 0, 255, 255, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 8, 8, 8, 8, 255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 0, 255, 255, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255,
		B8(00010011), B8(11111111), B8(00110001), 0,
		B8(00010011), B8(10101010), B8(00110001), 0,
		B8(00010011), B8(00110011), B8(00110001), 0,
		B8(00010001), B8(00010001), B8(00010001), 0
	};

	createFileSuccess (image1x1, sizeof (image1x1), PixelFormat4bppIndexed);
	createFileSuccess (image6x4, sizeof (image6x4), PixelFormat4bppIndexed);
}

static void test_validImage4bppBitmapInfoHeader ()
{
	BYTE image1x1[] = {
		'B', 'M', 122, 0, 0, 0, 0, 0, 0, 0, 0x75, 0, 0, 0,
		40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 0, 255, 255, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 8, 8, 8, 8, 255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 0, 255, 255, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255,
		B8(00010010), 0, 0, 0
	};
	BYTE image6x4CustomSizedPalette[] = {
		'B', 'M', 122, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		B8(00010011), B8(11111111), B8(00110001), 0,
		B8(00010011), B8(10101010), B8(00110001), 0,
		B8(00010011), B8(00110011), B8(00110001), 0,
		B8(00010001), B8(00010001), B8(00010001), 0
	};

	BYTE image1x1NegativeHeight[] = {
		'B', 'M', 122, 0, 0, 0, 0, 0, 0, 0, 0x75, 0, 0, 0,
		40, 0, 0, 0, 1, 0, 0, 0, 255, 255, 255, 255, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 0, 255, 255, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 8, 8, 8, 8, 255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 0, 255, 255, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255,
		B8(00010010), 0, 0, 0
	};
	BYTE image6x4NegativeHeight[] = {
		'B', 'M', 122, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 252, 255, 255, 255, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		B8(00010011), B8(11111111), B8(00110001), 0,
		B8(00010011), B8(10101010), B8(00110001), 0,
		B8(00010011), B8(00110011), B8(00110001), 0,
		B8(00010001), B8(00010001), B8(00010001), 0
	};
	
	createFileSuccess (image1x1, sizeof (image1x1), PixelFormat4bppIndexed);
	createFileSuccess (image6x4CustomSizedPalette, sizeof (image6x4CustomSizedPalette), PixelFormat4bppIndexed);

	createFileSuccess (image1x1NegativeHeight, sizeof (image1x1NegativeHeight), PixelFormat4bppIndexed);
	createFileSuccess (image6x4NegativeHeight, sizeof (image6x4NegativeHeight), PixelFormat4bppIndexed);
}

static void test_validImage4bppRle4Compression ()
{
	BYTE fullNoTerminator[] = {
		'B', 'M', 82, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		6, B8(00000001), 0, 0,
		6, B8(00000001), 0, 0,
		6, B8(00000001), 0, 0,
		6, B8(00000001), 0, 0
	};
	BYTE fullTerminator[] = {
		'B', 'M', 84, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		6, B8(00000001), 0, 0,
		6, B8(00000001), 0, 0,
		6, B8(00000001), 0, 0,
		6, B8(00000001), 0, 0,
		0, 1
	};
	BYTE split[] = {
		'B', 'M', 88, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		4, B8(00010001), 0, 0,
		3, B8(00000000), 3, B8(00010001), 0, 0,
		5, B8(00110011), 0, 0,
		1, B8(00000000), 2, B8(00110011), 0, 0,
		0, 1
	};
	BYTE overflowNoTerminator[] = {
		'B', 'M', 68, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		255, 0
	};
	BYTE overflowTerminator[] = {
		'B', 'M', 70, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		255, 0,
		0, 1
	};

	BYTE jump[] = {
		'B', 'M', 70, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		255, 0,
		0, 1
	};
	BYTE jumpMissingDx[] = {
		'B', 'M', 68, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		0, 2
	};
	BYTE jumpMissingDy[] = {
		'B', 'M', 69, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		0, 2, 0
	};
	BYTE jumpInvalidDx[] = {
		'B', 'M', 74, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		0, 2, 255, 2,
		3, 0,
		0, 1
	};
#if defined(USE_WINDOWS_GDIPLUS)
	BYTE jumpInvalidDy[] = {
		'B', 'M', 74, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		0, 2, 2, 255,
		3, 0,
		0, 1
	};
#endif

	BYTE absoluteModeOddPixels[] = {
		'B', 'M', 72, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		0, 3, B8(00010011), B8(00000001),
		0, 1
	};
	BYTE absoluteModeEvenPixels[] = {
		'B', 'M', 72, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		0, 4, B8(00010011), B8(00000001),
		0, 1
	};

	BYTE absoluteModeNoData[] = {
		'B', 'M', 68, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		0, 3
	};
	BYTE absoluteModeShortData[] = {
		'B', 'M', 69, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		0, 3, 1
	};
	BYTE absoluteModeNoPadding[] = {
		'B', 'M', 71, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		0, 5, 1, 2, 1
	};

	BYTE fullNegativeHeight[] = {
		'B', 'M', 84, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 252, 255, 255, 255, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		6, 1, 0, 0,
		6, 0, 0, 0,
		6, 2, 0, 0,
		6, 0, 0, 0,
		0, 1
	};
	BYTE incompleteAfterLineBreak[] = {
		'B', 'M', 70, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		6, 1, 0, 0
	};
	BYTE noImageData[] = {
		'B', 'M', 66, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255
	};
	BYTE singleByteData[] = {
		'B', 'M', 67, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		3
	};
	BYTE singleEscapeCode[] = {
		'B', 'M', 67, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		0
	};
	BYTE earlyTerminator[] = {
		'B', 'M', 74, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		6, 1, 0, 0,
		2, 0, 0, 1
	};
	BYTE earlyTerminatorWithTrailingBytes[] = {
		'B', 'M', 88, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		6, 1, 0, 0,
		2, 0, 0, 1,
		6, 2, 255, 255
	};
	BYTE tooManyNewLines[] = {
		'B', 'M', 78, 0, 0, 0, 0, 0, 0, 0, 0x42, 0, 0, 0,
		40, 0, 0, 0, 6, 0, 0, 0, 4, 0, 0, 0, 1, 0, 4, 0, BI_RLE4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		1, 0
	};

	// FIXME: GDI+ converts BI_RLE4 bitmaps to 32bpp.
#if defined(USE_WINDOWS_GDIPLUS)
	PixelFormat expectedRle4Format = PixelFormat32bppRGB;
#else
	PixelFormat expectedRle4Format = PixelFormat4bppIndexed;
#endif
	createFileSuccess (fullNoTerminator, sizeof (fullNoTerminator), expectedRle4Format);
	createFileSuccess (fullTerminator, sizeof (fullTerminator), expectedRle4Format);
	createFileSuccess (split, sizeof (split), expectedRle4Format);
	createFileSuccess (overflowNoTerminator, sizeof (overflowNoTerminator), expectedRle4Format);
	createFileSuccess (overflowTerminator, sizeof (overflowTerminator), expectedRle4Format);

	createFileSuccess (jump, sizeof (jump), expectedRle4Format);
	createFileSuccess (jumpMissingDx, sizeof (jumpMissingDx), expectedRle4Format);
	createFileSuccess (jumpMissingDy, sizeof (jumpMissingDy), expectedRle4Format);
	createFileSuccess (jumpInvalidDx, sizeof (jumpInvalidDx), expectedRle4Format);
	// FIXME: this causes a heap buffer overflow in libgdiplus.
#if defined(USE_WINDOWS_GDIPLUS)
	createFileSuccess (jumpInvalidDy, sizeof (jumpInvalidDy), expectedRle4Format);
#endif

	createFileSuccess (absoluteModeOddPixels, sizeof (absoluteModeOddPixels), expectedRle4Format);
	createFileSuccess (absoluteModeEvenPixels, sizeof (absoluteModeEvenPixels), expectedRle4Format);
	createFileSuccess (absoluteModeNoData, sizeof (absoluteModeNoData), expectedRle4Format);
	createFileSuccess (absoluteModeShortData, sizeof (absoluteModeShortData), expectedRle4Format);
	createFileSuccess (absoluteModeNoPadding, sizeof (absoluteModeNoPadding), expectedRle4Format);
	
	createFileSuccess (fullNegativeHeight, sizeof (fullNegativeHeight), expectedRle4Format);
	createFileSuccess (incompleteAfterLineBreak, sizeof (incompleteAfterLineBreak), expectedRle4Format);
	createFileSuccess (noImageData, sizeof (noImageData), expectedRle4Format);
	createFileSuccess (singleByteData, sizeof (singleByteData), expectedRle4Format);
	createFileSuccess (singleEscapeCode, sizeof (singleEscapeCode), expectedRle4Format);
	createFileSuccess (earlyTerminator, sizeof (earlyTerminator), expectedRle4Format);
	createFileSuccess (earlyTerminatorWithTrailingBytes, sizeof (earlyTerminatorWithTrailingBytes), expectedRle4Format);
	createFileSuccess (tooManyNewLines, sizeof (tooManyNewLines), expectedRle4Format);
}

static void test_invalidFileHeader ()
{
	BYTE shortSignature[] = {'B'};
	BYTE noImageSize[]    = {'B', 'M'};
	BYTE shortImageSize[] = {'B', 'M', 5, 0, 0};
	BYTE noReserved1[]    = {'B', 'M', 6, 0, 0, 0};
	BYTE shortReserved1[] = {'B', 'M', 7, 0, 0, 0, 0};
	BYTE noReserved2[]    = {'B', 'M', 8, 0, 0, 0, 0, 0};
	BYTE shortReserved2[] = {'B', 'M', 9, 0, 0, 0, 0, 0, 0};
	BYTE noOffset[]       = {'B', 'M', 10, 0, 0, 0, 0, 0, 0, 0};
	BYTE shortOffset[]    = {'B', 'M', 13, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0};

	createFile (shortSignature, sizeof (shortSignature), OutOfMemory);
	createFile (noImageSize, sizeof (noImageSize), OutOfMemory);
	createFile (shortImageSize, sizeof (shortImageSize), OutOfMemory);
	createFile (noReserved1, sizeof (noReserved1), OutOfMemory);
	createFile (shortReserved1, sizeof (shortReserved1), OutOfMemory);
	createFile (noReserved2, sizeof (noReserved2), OutOfMemory);
	createFile (shortReserved2, sizeof (shortReserved2), OutOfMemory);
	createFile (noOffset, sizeof (noOffset), OutOfMemory);
	createFile (shortOffset, sizeof (shortOffset), OutOfMemory);
}

static void test_invalidHeader ()
{
	BYTE noHeader[]               = {'B', 'M', 14, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0};
	BYTE zeroHeaderSize[]         = {'B', 'M', 18, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 0, 0, 0, 0, 0};
	BYTE newFormatNoWidth[]       = {'B', 'M', 18, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0};
	BYTE newFormatNoHeight[]      = {'B', 'M', 22, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0};
	BYTE os2FormatNoWidthHeight[] =	{'B', 'M', 18, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 12, 0, 0, 0};
	BYTE noColorPlanes[]          = {'B', 'M', 26, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0};
	BYTE noBitsPerPixel[]         = {'B', 'M', 28, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0};
	BYTE noCompression[]          = {'B', 'M', 30, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0};
	BYTE noSize[]                 = {'B', 'M', 34, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0};
	BYTE noHorizontalResolution[] = {'B', 'M', 38, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0};
	BYTE noVerticalResolution[]   = {'B', 'M', 42, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0};
	BYTE noColors[]               = {'B', 'M', 46, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	BYTE noImportantColors[]      = {'B', 'M', 50, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	createFile (noHeader, sizeof (noHeader), OutOfMemory);
	createFile (zeroHeaderSize, sizeof (zeroHeaderSize), OutOfMemory);
	createFile (newFormatNoWidth, sizeof (newFormatNoWidth), OutOfMemory);
	createFile (newFormatNoHeight, sizeof (newFormatNoHeight), OutOfMemory);
	createFile (os2FormatNoWidthHeight, sizeof (os2FormatNoWidthHeight), OutOfMemory);
	createFile (noColorPlanes, sizeof (noColorPlanes), OutOfMemory);
	createFile (noBitsPerPixel, sizeof (noBitsPerPixel), OutOfMemory);
	createFile (noCompression, sizeof (noCompression), OutOfMemory);
	createFile (noSize, sizeof (noSize), OutOfMemory);
	createFile (noHorizontalResolution, sizeof (noHorizontalResolution), OutOfMemory);
	createFile (noVerticalResolution, sizeof (noVerticalResolution), OutOfMemory);
	createFile (noColors, sizeof (noColors), OutOfMemory);
	createFile (noImportantColors, sizeof (noImportantColors), OutOfMemory);
}

static void test_invalidImageData ()
{
	BYTE noColorEntries[]         = {'B', 'M', 54, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	BYTE notEnoughColorEntries[]  = {'B', 'M', 58, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#if defined(USE_WINDOWS_GDIPLUS)
	BYTE noImageData[]            = {'B', 'M', 62, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 0};
#endif
	BYTE noImageDataBigSize[]     = {'B', 'M', 62, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 0};
	BYTE hasImageData4bpp[]       = {'B', 'M', 62, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 0, 128, 0, 0, 0};
	BYTE hasImageData8bpp[]       = {'B', 'M', 62, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 0, 128, 0, 0, 0};
	BYTE invalidBitsPerPixel[]    = {'B', 'M', 62, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 10, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 0, 128, 0, 0, 0};
	BYTE bitfieldsNon16Bpp[]      = {'B', 'M', 62, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 32, 0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 0, 128, 0, 0, 0};
#if defined(USE_WINDOWS_GDIPLUS)
	BYTE bitfields16Bpp[]         = {'B', 'M', 62, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0, 40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 16, 0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 0, 128, 0, 0, 0};
	BYTE notPaddedTo4Bytes[]      = {
		'B', 'M', 62, 0, 0, 0, 0, 0, 0, 0, 0x3E, 0, 0, 0,
		40, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 255, 255, 255, 0,
		B8(1000000)
	};
#endif

	createFile (noColorEntries, sizeof (noColorEntries), OutOfMemory);
	createFile (notEnoughColorEntries, sizeof (notEnoughColorEntries), OutOfMemory);
	// FIXME: this should fail with OutOfMemory in libgdiplus.
#if defined(USE_WINDOWS_GDIPLUS)
	createFile (noImageData, sizeof (noImageData), OutOfMemory);
#endif
	createFile (noImageDataBigSize, sizeof (noImageDataBigSize), OutOfMemory);
	createFile (hasImageData4bpp, sizeof (hasImageData4bpp), OutOfMemory);
	createFile (hasImageData8bpp, sizeof (hasImageData8bpp), OutOfMemory);
	createFile (invalidBitsPerPixel, sizeof (invalidBitsPerPixel), OutOfMemory);
	createFile (bitfieldsNon16Bpp, sizeof (bitfieldsNon16Bpp), OutOfMemory);
	
	// FIXME: this returns Ok with libgdiplus.
#if defined(USE_WINDOWS_GDIPLUS)
	createFile (bitfields16Bpp, sizeof (bitfields16Bpp), OutOfMemory);
	createFile (notPaddedTo4Bytes, sizeof (notPaddedTo4Bytes), OutOfMemory);
#endif
}

int
main (int argc, char**argv)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup (&gdiplusToken, &gdiplusStartupInput, NULL);

	test_validImage1bppOS2Header ();
	test_validImage1bppBitmapInfoHeader ();
	test_validImage4bppOS2Header ();
	test_validImage4bppBitmapInfoHeader ();
	test_validImage4bppRle4Compression ();
	test_invalidFileHeader ();
	test_invalidHeader ();
	test_invalidImageData ();

	deleteFile (file);
	
	GdiplusShutdown (gdiplusToken);
	return 0;
}