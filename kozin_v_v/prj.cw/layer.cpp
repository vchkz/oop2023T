#include "layer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "iostream"

Layer::Layer()
{
    size[0] = 0;
    size[1] = 0;
}



void Layer::setSize(int size[2], const byte pixel[])
{
    setSize(size[0], size[1], pixel);
}

void Layer::setSize(int w, int h, const byte pixel[])
{


    if (w != size[0] || h != size[1])
    {
        size[0] = w;
        size[1] = h;

        if (data != nullptr)
        {
            delete[] data;
        }

        data = new unsigned char[dataSize()];
    }

    for (int y = 0; y < size[1]; y++)
    {
        for (int x = 0; x < size[0]; x++)
        {
            setPixel(x, y, pixel);
        }
    }
}

int Layer::dataSize() const
{
    return size[0] * size[1] * bpp;
}


Layer *Layer::defaultLayer(int size[2], const byte pixel[])
{
    auto layer = new Layer();

    layer->setSize(size, pixel);

    return layer;
}

Layer *Layer::fromFile(const char *filename)
{
    auto layer = new Layer();

    layer->data = stbi_load(filename, &(layer->size[0]), &(layer->size[1]), &(layer->bpp), STBI_rgb);

    if (layer->data != NULL) {
        int imagesize = layer->size[0] * layer->size[1] * 4;
        unsigned char* new_imagedata = new unsigned char[imagesize];

        for (int i = 0; i < layer->size[0] * layer->size[1]; i++) {
            new_imagedata[i * 4] = layer->data[i * layer->bpp];
            new_imagedata[i * 4 + 1] = layer->data[i * layer->bpp + 1];
            new_imagedata[i * 4 + 2] = layer->data[i * layer->bpp + 2];
            new_imagedata[i * 4 + 3] = 255;
        }

        stbi_image_free(layer->data);

        layer->data = new_imagedata;

        layer->bpp = 4;}


    return layer;
}

void Layer::setPixel(int x, int y, const byte pixel[])
{
    int position = y * size[0] + x;
    for (int b = 0; b < 4; b++)
    {
        data[position * 4 + b] = pixel[b];
    }
}


