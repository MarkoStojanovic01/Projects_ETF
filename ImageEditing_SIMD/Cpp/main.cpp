#include <iostream>
#include <string>
#include <limits>
#include "../Header/Image.h"

int main() {
    const std::string inputFilename = "../images/RustCohle.jpg";
    const std::string outputFilename = "../images/RustCohle.jpg";

    // Load the input image
    Image* image = new Image(inputFilename);

    if (!image->isLoaded()) {
        std::cerr << "Failed to load the image: " << inputFilename << std::endl;
        return 1;
    }

    bool loop = true;

    Image* copy = new Image(image);
    Image* copyOpt = new Image(image);

    while(loop) {

        int option;

        std::cout << "Enter an option( 0-Exit, 1-Add, 2-Sub, 3-invSub, 4-Mul, 5-Div\n"
                     "6-Log, 7-Min, 8-Max, 9-Inverse, 10-Gray, 11-Filter, 12-Pow, 13-Sobel, 14-InvDiv ):";
        std::cin >> option;

        switch(option){
            case 0:{
                loop = false;
                break;
            }
            case 1:{ // add

                // Clear the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                int value = 0;
                std::string input;
                std::cout << "Enter a parametar: ";
                std::getline(std::cin, input);

                value = static_cast<uint8_t>(std::stoi(input));

                copy->addConstant(value);
                copyOpt->addConstantMod(value);


                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 2:{ // sub

                // Clear the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                int value = 0;
                std::string input;
                std::cout << "Enter a parametar: ";
                std::getline(std::cin, input);

                value = static_cast<uint8_t>(std::stoi(input));

                copy->subConstant(value);
                copyOpt->subConstantMod(value);

                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 3:{ // inv sub


                // Clear the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                int value = 0;
                std::string input;
                std::cout << "Enter a parametar: ";
                std::getline(std::cin, input);

                value = static_cast<uint8_t>(std::stoi(input));

                copy->InvSubConstant(value);
                copyOpt->InvSubConstantMod(value);

                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 4:{ // mul

                // Clear the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                float value = 0;
                std::string input;
                std::cout << "Enter a parametar: ";
                std::getline(std::cin, input);

                value = static_cast<float>(std::stoi(input));

                copy->mulConstant(value);
                copyOpt->mulConstantMod(value);


                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 5:{ // div

                // Clear the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                float value;
                std::string input;
                std::cout << "Enter a parametar: ";
                std::getline(std::cin, input);

                value = static_cast<float>(std::stoi(input));

                copy->divConstant(value);
                copyOpt->divConstantMod(value);

                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 6:{ // log

                copy->log();
                copyOpt->logMod();

                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 7:{ // min

                // Clear the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                int value = 0;
                std::string input;
                std::cout << "Enter a parametar: ";
                std::getline(std::cin, input);

                value = static_cast<uint8_t>(std::stoi(input));

                copy->minI(value);
                copyOpt->minIMod(value);

                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 8:{ // max

                // Clear the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                int value = 0;
                std::string input;
                std::cout << "Enter a parametar: ";
                std::getline(std::cin, input);

                value = static_cast<uint8_t>(std::stoi(input));

                copy->maxI(value);
                copyOpt->maxIMod(value);

                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 9:{ // inverse

                copy->inverse();
                copyOpt->inverseMod();

                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 10:{ // Convert to gray


                copy->convertToGrayscale();
                copyOpt->convertToGrayscaleMod();

                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 11:{ // filter

                // Clear the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                int size = 0;
                std::string input;
                std::cout << "Enter a size of matrix: ";
                std::getline(std::cin, input);

                size = static_cast<uint8_t>(std::stoi(input));

                float* matrix = new float[size*size];

                std::cout << "Enter " << size*size << "  values:" << std::endl;
                for (int i = 0; i < size*size; ++i) {
                    std::cin >> matrix[i];
                }

                image->filter(matrix, size, copy);
                image->filterMod(matrix, size,copyOpt);


                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 12:{ // pow
                // Clear the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                double value;
                std::string input;
                std::cout << "Enter a parametar: ";

                std::cin >> value;

                copy->pow(value);
                copyOpt->powMod(value);

                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 13:{ // Sobel


                image->sobel(copy);
                image->sobel(copyOpt);

                std::cout << "Image processing completed." << std::endl;

                break;
            }
            case 14:{ // inv div

                // Clear the input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                float value;
                std::string input;
                std::cout << "Enter a parametar: ";
                std::getline(std::cin, input);

                value = static_cast<float>(std::stoi(input));

                copy->InvDivConstant(value);
                copyOpt->InvDivConstantMod(value);

                std::cout << "Image processing completed." << std::endl;

                break;
            }
        }

        if(option==0)break;

        // Save the modified image
        copy->save(outputFilename, "normal");
        copyOpt->save(outputFilename,"optimized");

        std::string keepEditing = " ";
        std::string input;
        std::cout << "Keep editing the same image: y/n "<< std::endl;
        std::cin >> keepEditing;
        if(keepEditing != "y") {
            copy->copyImages(image);
            copyOpt->copyImages(image);
            std::cout << "Image reloaded!"<< std::endl;
        }

        std::cout << "-------------------" << std::endl;

    }

    if(image){
        delete image;
    }

    std::cout << "\nJob's done." << std::endl;

    return 0;
}


