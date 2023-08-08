
#ifndef AOR2PROJEKAT_IMAGE_H
#define AOR2PROJEKAT_IMAGE_H

#include <iostream>
#include <string>

class Image {
public:

    Image(const std::string& filename);
    Image(Image* copy);
    ~Image();
    void copyImages(Image* copy);
    void printChannels();

    void printInfo() const;
    bool isLoaded() const;
    void save(const std::string& filename, const std::string& mark) const;

    unsigned char *getData() const;
    void setData(unsigned char *data);

    int getWidth() const;
    void setWidth(int width);

    int getHeight() const;
    void setHeight(int height);

    int getChannels() const;
    void setChannels(int channels);

    void addConstant(uint8_t value);
    void addConstantMod(uint8_t value);

    void subConstant(uint8_t value);
    void subConstantMod(uint8_t value);

    void InvSubConstant(uint8_t value);
    void InvSubConstantMod(uint8_t value);

    void mulConstant(float value);
    void mulConstantMod(float value);

    void divConstant(float value);
    void divConstantMod(float value);

    void InvDivConstant(float value);
    void InvDivConstantMod(float value);

    void log();
    void logMod();

    void minI(uint8_t value);
    void minIMod(uint8_t value);

    void maxI(uint8_t value);
    void maxIMod(uint8_t value);

    void convertToGrayscale();
    void convertToGrayscaleMod();

    void inverse();
    void inverseMod();

    void filter(float* mat, int size,Image* img);
    void filterMod(float* mat, int size,Image* img);

    void pow(double value);
    void powMod(double value);

    void sobel(Image* edges);
    void sobelMod(Image* edges);

private:
    void loadImage() ;

private:
    uint8_t* data;
    int width;
    int height;
    int channels;
    bool original;

public:
    uint8_t* redChannel;
    uint8_t* greenChannel;
    uint8_t* blueChannel;
    uint8_t* alphaChannel;
};

#endif //AOR2PROJEKAT_IMAGE_H
