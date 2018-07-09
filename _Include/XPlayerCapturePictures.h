#pragma once

#ifndef WRITE_JPEG_FILE_32489jkasdh8723
#define WRITE_JPEG_FILE_32489jkasdh8723




extern "C" {
#include "jpeglib.h" 
}

/* The following declarations and 5 functions are jpeg related
* functions used by put_jpeg_grey_memory and put_jpeg_yuv420p_memory
*/
typedef struct {
	struct jpeg_destination_mgr pub;
	JOCTET *buf;
	size_t bufsize;
	size_t jpegsize;
} mem_destination_mgr;

typedef mem_destination_mgr *mem_dest_ptr;


static void init_destination(j_compress_ptr cinfo)
{
	mem_dest_ptr dest = (mem_dest_ptr)cinfo->dest;
	dest->pub.next_output_byte = dest->buf;
	dest->pub.free_in_buffer = dest->bufsize;
	dest->jpegsize = 0;
}

static boolean empty_output_buffer(j_compress_ptr cinfo)
{
	mem_dest_ptr dest = (mem_dest_ptr)cinfo->dest;
	dest->pub.next_output_byte = dest->buf;
	dest->pub.free_in_buffer = dest->bufsize;

	return FALSE;
}

static void term_destination(j_compress_ptr cinfo)
{
	mem_dest_ptr dest = (mem_dest_ptr)cinfo->dest;
	dest->jpegsize = dest->bufsize - dest->pub.free_in_buffer;
}

static void jpeg_mem_dest_ex(j_compress_ptr cinfo, unsigned char* buf, size_t bufsize)
{
	mem_dest_ptr dest;

	if (cinfo->dest == NULL) {
		cinfo->dest = (struct jpeg_destination_mgr *)
			(*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT,
			sizeof(mem_destination_mgr));
	}

	dest = (mem_dest_ptr)cinfo->dest;

	dest->pub.init_destination = init_destination;
	dest->pub.empty_output_buffer = empty_output_buffer;
	dest->pub.term_destination = term_destination;

	dest->buf = buf;
	dest->bufsize = bufsize;
	dest->jpegsize = 0;
}

static unsigned int jpeg_mem_size(j_compress_ptr cinfo)
{
	mem_dest_ptr dest = (mem_dest_ptr)cinfo->dest;
	return dest->jpegsize;
}

inline int yuv420_to_jpeg(unsigned char *input_image, int width, int height, unsigned char *dest_image, unsigned int dest_len, int quality)  
{  
	int i, j, jpeg_image_size;

	JSAMPROW y[16], cb[16], cr[16]; // y[2][5] = color sample of row 2 and pixel column 5; (one plane)  
	JSAMPARRAY data[3]; // t[0][2][5] = color sample 0 of row 2 and column 5  

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	unsigned int offset;
	unsigned int offset_h = 0;

	data[0] = y;
	data[1] = cb;
	data[2] = cr;

	cinfo.err = jpeg_std_error(&jerr);  // errors get written to stderr   

	jpeg_create_compress(&cinfo);

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;

	jpeg_set_defaults(&cinfo);

	jpeg_set_colorspace(&cinfo, JCS_YCbCr);

	cinfo.raw_data_in = TRUE;                  // supply downsampled data  
//	cinfo.do_fancy_downsampling = FALSE;       // fix segfaulst with v7  
	cinfo.comp_info[0].h_samp_factor = 2;
	cinfo.comp_info[0].v_samp_factor = 2;
	cinfo.comp_info[1].h_samp_factor = 1;
	cinfo.comp_info[1].v_samp_factor = 1;
	cinfo.comp_info[2].h_samp_factor = 1;
	cinfo.comp_info[2].v_samp_factor = 1;

	jpeg_set_quality(&cinfo, quality, TRUE);
	cinfo.dct_method = JDCT_FASTEST;

	jpeg_mem_dest_ex(&cinfo, dest_image, dest_len);    // data written to mem  

	jpeg_start_compress(&cinfo, TRUE);

	offset = width * height;

	for (j = 0; j < height; j += 16)
	{
		for (i = 0; i < 16; i++)
		{
			//y[i] = input_image + width * (i + j);  
			y[i] = input_image + offset_h;
			offset_h += width;
			if (i % 2 == 0)
			{
				cb[(i >> 1)] = input_image + offset + (width >> 1) * ((i + j) >> 1);
				cr[(i >> 1)] = input_image + offset + (offset >> 2) + (width >> 1) * ((i + j) >> 1);
			}
		}
		jpeg_write_raw_data(&cinfo, data, 16);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_image_size = jpeg_mem_size(&cinfo);
	jpeg_destroy_compress(&cinfo);

	return jpeg_image_size;
}



//quality是个0～100之间的整数，表示压缩比率
inline int WRITE_JPEG_FILE(char * filepath,char * filename, unsigned char* yuvData, int image_width, int image_height, int quality = 90)
{
#ifdef _PLATFORM_WINDOW
	DWORD dw = GetTickCount();
#endif

	FILE * outfile;    // target file 
	char fpath[256] = { 0 };
	memcpy(fpath, filepath, strlen(filepath));

	if (CreatDir(fpath) != 0)
	{
		return -1;
	}

	char szFilePathName[256] = { 0 };

#ifdef _PLATFORM_WINDOW
	sprintf_s(szFilePathName, "%s\\%s", filepath, filename);
	fopen_s(&outfile, szFilePathName, "wb");
#endif

#ifdef _PLATFORM_ANDROID
	sprintf(szFilePathName, "%s/%s", filepath, filename);
	outfile = fopen(szFilePathName, "wb");//创建文件变量用于指定压缩数据的输出目标
#endif

#ifdef _PLATFORM_IOS
	sprintf(szFilePathName, "%s/%s", filepath, filename);
	outfile = fopen(szFilePathName, "wb");//创建文件变量用于指定压缩数据的输出目标
#endif

	if (outfile == NULL)
	{
		//fprintf(stderr, "can't open %s\n", szFilePathName);
		return -1;
	}



	unsigned char* dst = (unsigned char*)malloc(1920*1080 * 3 * sizeof(char));

	int len = yuv420_to_jpeg(yuvData, image_width, image_height, dst, 1920 * 1080 * 4, quality);

	fwrite(dst, len, 1, outfile);

	free(dst);



	fclose(outfile);



#ifdef _PLATFORM_WINDOW
	CROSS_TRACE("cap = %d\n", GetTickCount() - dw);
#endif


	return 0;

}

#endif