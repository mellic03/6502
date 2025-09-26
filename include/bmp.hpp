// #pragma once

// #include <stddef.h>
// #include <stdint.h>
// #include <algorithm>

// /*
//     http://www.daubnet.com/en/file-format-bmp
// */

// namespace memu
// {
//     enum BI_: uint32_t
//     {
//         BI_RGB       = 0,  /* No compression - straight BGR data */
//         BI_RLE8      = 1,  /* 8-bit run-length compression */
//         BI_RLE4      = 2,  /* 4-bit run-length compression */
//         BI_BITFIELDS = 3,  /* RGB bitmap with RGB masks */
//     };

//     static inline
//     const char *BI_string( uint32_t compression )
//     {
//         switch (compression)
//         {
//             default:            return "BI_INVALID";
//             case BI_RGB:        return "BI_RGB";
//             case BI_RLE8:       return "BI_RLE8";
//             case BI_RLE4:       return "BI_RLE4";
//             case BI_BITFIELDS:  return "BI_BITFIELDS";
//         }
//     }


//     struct BMP_infoheader
//     {
//         uint32_t width;
//         uint32_t height;
//         uint16_t planes;
//         uint16_t bpp;           // bits per pixel
//         uint32_t compression;   // 0, 1, 2, 3 --> none, 8bit run length, 4bit run length, mask
//         uint32_t imagesize;

//         // Entire field can be set to 0 --> zero compression
//         uint32_t ppm_x;             // pixels per meter.
//         uint32_t ppm_y;             // pixels per meter.
//         uint32_t colors_used;       // number of used colors.
//         uint32_t colors_important;  // 0 == all
//         uint8_t  color_table[4];    // present only if bpp <= 8

//     } __attribute__((packed));


//     struct BMP_header
//     {
//         uint16_t signature;
//         uint32_t filesize;
//         uint32_t reserved;
//         uint32_t offset; // offset to image data

//         uint32_t infosize; // should be sizeof(infoheader)
//         BMP_infoheader infoheader;

//         uint8_t rest[8];

//     } __attribute__((packed));


//     struct BMP_File
//     {
//         uint8_t *pixels;
//         int w, h;
//         int bpp;    // bits per pixel
//         int bypp;   // bytes per pixel
//         int pitch;  // bytes per row
//         size_t nbytes;

//         BMP_File( void *addr )
//         {
//             auto *header = (BMP_header*)addr;
//             auto &info   = header->infoheader;
        
//             this->w      = int(info.width);
//             this->h      = int(info.height);
//             this->bpp    = int(info.bpp);
//             this->bypp   = int(info.bpp) / 8;
//             this->pitch  = this->w * this->bypp;
//             this->nbytes = header->filesize;
//             this->pixels = (uint8_t*)header + header->offset;

//             // Flip vertically
//             auto *buf = (uint8_t*)pixels;

//             for (int i=0; i<h/2; i++)
//             {
//                 for (int j=0; j<w; j++)
//                 {
//                     for (int k=0; k<bypp; k++)
//                     {
//                         int idx0 = pitch*(h-1 - i) + bypp*j + k;
//                         int idx1 = pitch*i + bypp*j + k;
//                         std::swap(buf[idx0], buf[idx1]);
//                     }
//                 }
//             }
//         }

//     };

// }
