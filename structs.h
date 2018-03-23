/*
*   Lucas Gomes Pereira
*   13/0013242
*/ 

//Pixel with RGB channels
typedef struct _pixel {
    unsigned short int r; //Red
    unsigned short int g; //Green
    unsigned short int b; //Blue
} Pixel;

//Image struct
typedef struct _image {
    Pixel pixel[512][512];
    unsigned int w; //Width
    unsigned int h; //Height
} Image;
