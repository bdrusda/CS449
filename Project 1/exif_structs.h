struct exif_header
{
    unsigned short JPEG_marker;//0-1
    unsigned short APP1;//2-3
    unsigned short APP1_length;   //4-5
    unsigned char string[4];//6-9
    unsigned short nul;//10-11
    unsigned char endianness[2];//12-13
    unsigned short version;//14-15
    unsigned int offset;//16-19
};

struct tiff_tag
{
    unsigned short iden;//0-1          if the identifier is 0x010F, we want an ASCII string,
    unsigned short data_type;//2-3
    unsigned int num_items;//4-7
    unsigned int offset;//8-11
};
