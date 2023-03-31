#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
using namespace std;

// g++ --std=c++11 src/main.cpp -o project2
//// First 10 TASK
//  ./project2 cmake-build-debug/output/part1.tga cmake-build-debug/layer1.tga multiply cmake-build-debug/pattern1.tga
// ./project2 cmake-build-debug/output/part2.tga cmake-build-debug/layer2.tga subtract cmake-build-debug/car.tga
// ./project2 cmake-build-debug/output/part3.tga cmake-build-debug/text.tga screen cmake-build-debug/output/part3.tga
// ./project2 cmake-build-debug/output/part4.tga cmake-build-debug/pattern2.tga subtract cmake-build-debug/output/part4.tga
// ./project2 cmake-build-debug/output/part5.tga cmake-build-debug/layer1.tga overlay cmake-build-debug/pattern1.tga
// ./project2 cmake-build-debug/output/part6.tga cmake-build-debug/car.tga addgreen 200
// ./project2 cmake-build-debug/output/part7.tga cmake-build-debug/car.tga scalered 4 scaleblue 0
// ./project2 cmake-build-debug/output/part8_r.tga cmake-build-debug/car.tga onlyred
// ./project2 cmake-build-debug/output/part8_g.tga cmake-build-debug/car.tga onlygreen
// ./project2 cmake-build-debug/output/part8_b.tga cmake-build-debug/car.tga onlyblue
// ./project2 cmake-build-debug/output/part9.tga cmake-build-debug/layer_blue.tga combine cmake-build-debug/layer_green.tga cmake-build-debug/layer_red.tga
// ./project2 cmake-build-debug/output/part10.tga cmake-build-debug/text2.tga flip

//// EXTRA TASKS
// ./project2 --help
// .project2
// ./project2 cmake-build-debug/output/part13.tga cmake-build-debug/car.tga multiply cmake-build-debug/output/part1.tga
// ./project2 cmake-build-debug/output/part14.tga cmake-build-debug/car.tga multiply cmake-build-debug/output/part1.tga screen cmake-build-debug/output/part2.tga
// ./project2 cmake-build-debug/output/part15.tga cmake-build-debug/car.tga flip flip flip flip
// ./project2 cmake-build-debug/output/part16.tga cmake-build-debug/car.tga scalegreen 5 addred 100 scaleblue 0
// ./project2 cmake-build-debug/output/out.tga
// ./project2 cmake-build-debug/output/out.tga cmake-build-debug/output/part1.tga weirdmethod


struct HeaderInfo {
    char idLength = 0;
    char colorMapType = 0;
    char dataTypeCode = 0;
    short colorMapOrigin = 0;
    short colorMapLength = 0;
    char colorMapDepth = 0;
    short xOrigin = 0;
    short yOrigin = 0;
    short width = 0;
    short height = 0;
    char bitsPerPixel = 0;
    char imageDescription = 0;
    int totalPixels;
};

struct Pixels {
    unsigned char blue = 0;
    unsigned char green = 0;
    unsigned char red = 0;
};

class Image {
public:
    HeaderInfo header;
    vector <Pixels> pixels;
};

// Read data from file
Image ReadData(const string& name){
    ifstream inFile(name, ios_base::binary);
    if(!inFile.is_open()){
        runtime_error("Failed to open file.");
    }
    Image data;

    // read header information
    inFile.read(reinterpret_cast<char*>(&data.header.idLength), sizeof(data.header.idLength));
    inFile.read(reinterpret_cast<char*>(&data.header.colorMapType), sizeof(data.header.colorMapType));
    inFile.read(reinterpret_cast<char*>(&data.header.dataTypeCode), sizeof(data.header.dataTypeCode));
    inFile.read(reinterpret_cast<char*>(&data.header.colorMapOrigin), sizeof(data.header.colorMapOrigin));
    inFile.read(reinterpret_cast<char*>(&data.header.colorMapLength), sizeof(data.header.colorMapLength));
    inFile.read(reinterpret_cast<char*>(&data.header.colorMapDepth), sizeof(data.header.colorMapDepth));
    inFile.read(reinterpret_cast<char*>(&data.header.xOrigin), sizeof(data.header.xOrigin));
    inFile.read(reinterpret_cast<char*>(&data.header.yOrigin), sizeof(data.header.yOrigin));
    inFile.read(reinterpret_cast<char*>(&data.header.width), sizeof(data.header.width));
    inFile.read(reinterpret_cast<char*>(&data.header.height), sizeof(data.header.height));
    inFile.read(reinterpret_cast<char*>(&data.header.bitsPerPixel), sizeof(data.header.bitsPerPixel));
    inFile.read(reinterpret_cast<char*>(&data.header.imageDescription), sizeof(data.header.imageDescription));

    // Read image data
    data.header.totalPixels = data.header.width * data.header.height;

    //Pixels pixels{};
    for(unsigned int i = 0; i < data.header.totalPixels; i++){
        Pixels pixel;
        inFile.read((char*)&pixel.blue, sizeof(pixel.blue));
        inFile.read((char*)&pixel.green, sizeof(pixel.green));
        inFile.read((char*)&pixel.red, sizeof(pixel.red));
        data.pixels.push_back(pixel);
    }
    //cout << (int)data.pixels[0].blue << endl;
    return data;
}
void WriteData(const string& name, Image& data){
    ofstream outFile(name, ios_base::out | ios_base::binary);
    if(outFile.is_open()){
        outFile.write(&data.header.idLength, sizeof(data.header.idLength));
        outFile.write(&data.header.colorMapType, sizeof(data.header.colorMapType));
        outFile.write(&data.header.dataTypeCode, sizeof(data.header.dataTypeCode));
        outFile.write((char*)&data.header.colorMapOrigin, sizeof(data.header.colorMapOrigin));
        outFile.write((char*)&data.header.colorMapLength, sizeof(data.header.colorMapLength));
        outFile.write(&data.header.colorMapDepth, sizeof(data.header.colorMapDepth));
        outFile.write((char*)&data.header.xOrigin, sizeof(data.header.xOrigin));
        outFile.write((char*)&data.header.yOrigin, sizeof(data.header.yOrigin));
        outFile.write((char*)&data.header.width, sizeof(data.header.width));
        outFile.write((char*)&data.header.height, sizeof(data.header.height));
        outFile.write(&data.header.bitsPerPixel, sizeof(data.header.bitsPerPixel));
        outFile.write(&data.header.imageDescription, sizeof(data.header.imageDescription));

        for(Pixels& pixel : data.pixels){
            outFile.write((char*)&pixel.blue, sizeof(pixel.blue));
            outFile.write((char*)&pixel.green, sizeof(pixel.green));
            outFile.write((char*)&pixel.red, sizeof(pixel.red));
        }
    }
}

Image multiply(Image &image1, Image &image2){
    Image result;
    result.header = image1.header;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;
        newPixel.blue = (image1.pixels[i].blue * image2.pixels[i].blue) / 255.0f + 0.5f;
        newPixel.green = (image1.pixels[i].green * image2.pixels[i].green) / 255.0f + 0.5f;
        newPixel.red = (image1.pixels[i].red * image2.pixels[i].red) / 255.0f + 0.5f;
        result.pixels.push_back(newPixel);
    }
    return result;
}

Image Screen(Image &image1, Image &image2){
    Image result;
    result.header = image1.header;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;
        newPixel.blue = 255 - ((255 - image1.pixels[i].blue) * (255 - image2.pixels[i].blue)) / 255.0f + 0.5f;
        newPixel.green = 255 - ((255 - image1.pixels[i].green) * (255 - image2.pixels[i].green)) / 255.0f + 0.5f;
        newPixel.red = 255 - ((255 - image1.pixels[i].red) * (255 - image2.pixels[i].red)) / 255.0f + 0.5f;
        result.pixels.push_back(newPixel);
    }
    return result;
}
Image Subtract(Image &image1, Image &image2){
    Image result;
    result.header = image1.header;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;

        if(image1.pixels[i].blue - image2.pixels[i].blue < 0) {
            newPixel.blue = 0;
        }
        else{
            newPixel.blue = image1.pixels[i].blue - image2.pixels[i].blue;
        }
        if(image1.pixels[i].green - image2.pixels[i].green < 0) {
            newPixel.green = 0;
        }
        else{
            newPixel.green = image1.pixels[i].green - image2.pixels[i].green;
        }
        if(image1.pixels[i].red - image2.pixels[i].red < 0) {
            newPixel.red = 0;
        }
        else{
            newPixel.red = image1.pixels[i].red - image2.pixels[i].red;
        }


        result.pixels.push_back(newPixel);
    }
    return result;
}
Image Addition(Image &image1, Image &image2){
    Image result;
    result.header = image1.header;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;

        if(image2.pixels[i].blue + image1.pixels[i].blue > 255) {
            newPixel.blue = 255;
        }
        else{
            newPixel.blue = image2.pixels[i].blue + image1.pixels[i].blue;
        }
        if(image2.pixels[i].green + image1.pixels[i].green > 255) {
            newPixel.green = 255;
        }
        else{
            newPixel.green = image2.pixels[i].green + image1.pixels[i].green;
        }
        if(image2.pixels[i].red + image1.pixels[i].red > 255) {
            newPixel.red = 255;
        }
        else{
            newPixel.red = image2.pixels[i].red + image1.pixels[i].red;
        }


        result.pixels.push_back(newPixel);
    }
    return result;
}
Image Overlay(Image &image1, Image &image2){
    Image result;
    result.header = image1.header;
    for(unsigned int i = 0; i < image1.pixels.size(); i++) {
        Pixels newPixel;

        if ((image2.pixels[i].blue / 255.0f) > 0.5){
            newPixel.blue = 255 - (2 * (255 - image1.pixels[i].blue) * (255 - image2.pixels[i].blue)) / 255.0f + 0.5f;
        }
        else{
            newPixel.blue = (2 * image1.pixels[i].blue * image2.pixels[i].blue) / 255.0f + 0.5f;
        }
        if ((image2.pixels[i].green / 255.0f) > 0.5){
            newPixel.green = 255 - (2* (255 - image1.pixels[i].green) * (255 - image2.pixels[i].green)) / 255.0f + 0.5f;
        }
        else{
            newPixel.green = (2 * image1.pixels[i].green * image2.pixels[i].green) / 255.0f + 0.5f;
        }
        if ((image2.pixels[i].red / 255.0f) > 0.5){
            newPixel.red = 255 - (2* (255 - image1.pixels[i].red) * (255 - image2.pixels[i].red)) / 255.0f + 0.5f;
        }
        else{
            newPixel.red = (2 * image1.pixels[i].red * image2.pixels[i].red) / 255.0f + 0.5f;
        }
        result.pixels.push_back(newPixel);
    }
    return result;
}

Image addGreen(Image &image1, int const &num){
    Image result;
    result.header = image1.header;
    int green;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;
        green = image1.pixels[i].green + num;
        if (green > 255){
            newPixel.green = 255;
        }
        else if(green < 0){
            newPixel.green = 0;
        }
        else{
            newPixel.green = green;
        }
        newPixel.blue = image1.pixels[i].blue;
        newPixel.red = image1.pixels[i].red;
        result.pixels.push_back(newPixel);
    }
    return result;

}
Image addRed(Image &image1, int const &num){
    Image result;
    result.header = image1.header;
    int red;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;
        red = image1.pixels[i].red + num;
        if (red > 255){
            newPixel.red = 255;
        }
        else if(red < 0){
            newPixel.red = 0;
        }
        else{
            newPixel.red = red;
        }
        newPixel.blue = image1.pixels[i].blue;
        newPixel.green = image1.pixels[i].green;
        result.pixels.push_back(newPixel);
    }
    return result;

}
Image addBlue(Image &image1, int const &num){
    Image result;
    result.header = image1.header;
    int blue;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;
        blue = image1.pixels[i].blue + num;
        if (blue > 255){
            newPixel.blue = 255;
        }
        else if(blue < 0){
            newPixel.blue = 0;
        }
        else{
            newPixel.blue = blue;
        }
        newPixel.green = image1.pixels[i].green;
        newPixel.red = image1.pixels[i].red;
        result.pixels.push_back(newPixel);
    }
    return result;
}
Image Scale(Image &image1, int b, int g, int r){
    Image result;
    result.header = image1.header;
    int blue, green, red;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;
        blue = image1.pixels[i].blue * b;
        if (blue > 255){
            newPixel.blue = 255;
        }
        else if(blue < 0){
            newPixel.blue = 0;
        }

        else{
            newPixel.blue = blue;
        }
        green = image1.pixels[i].green * g;
        if (green > 255){
            newPixel.green = 255;
        }
        else if(green < 0){
            newPixel.green = 0;
        }
        else{
            newPixel.green = green;
        }
        red = image1.pixels[i].red * r;
        if (red > 255){
            newPixel.red = 255;
        }
        else if(red < 0){
            newPixel.red = 0;
        }
        else{
            newPixel.red = red;
        }
        result.pixels.push_back(newPixel);
    }
    return result;
}
Image onlyRed(Image &image1){
    Image result;
    result.header = image1.header;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;
        newPixel.red = image1.pixels[i].red;
        newPixel.green = image1.pixels[i].red;
        newPixel.blue = image1.pixels[i].red;
        result.pixels.push_back(newPixel);
    }
    return result;
}
Image onlyGreen(Image &image1){
    Image result;
    result.header = image1.header;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;
        newPixel.red = image1.pixels[i].green;
        newPixel.green = image1.pixels[i].green;
        newPixel.blue = image1.pixels[i].green;
        result.pixels.push_back(newPixel);
    }
    return result;
}
Image onlyBlue(Image &image1){
    Image result;
    result.header = image1.header;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;
        newPixel.red = image1.pixels[i].blue;
        newPixel.green = image1.pixels[i].blue;
        newPixel.blue = image1.pixels[i].blue;
        result.pixels.push_back(newPixel);
    }
    return result;
}

Image combineFile(Image &image1, Image &image2, Image &image3){
    Image result;
    result.header = image1.header;
    for(unsigned int i = 0; i < image1.pixels.size(); i++){
        Pixels newPixel;

        newPixel.red = image1.pixels[i].red;
        newPixel.green = image2.pixels[i].green;
        newPixel.blue = image3.pixels[i].blue;

        result.pixels.push_back(newPixel);
    }
    return result;
}

Image flipImg(Image &image1){
    Image result;
    result.header = image1.header;
    vector<Pixels> pixel;
    int area = int(image1.header.width) * int(image1.header.height);
    for(int i = area - 1; i >= 0; i--){
        Pixels newPixel;

        newPixel.blue = image1.pixels[i].blue;
        newPixel.green = image1.pixels[i].green;
        newPixel.red = image1.pixels[i].red;

        result.pixels.push_back(newPixel);
    }
    return result;
}



void taskOne(){
    // task 1
    Image layer1 = ReadData("layer1.tga");
    Image pattern1 = ReadData("pattern1.tga");
    Image result = multiply(layer1, pattern1);
    WriteData("task1.tga",result);
}
void taskTwo(){
    // Task 2
    Image layer2 = ReadData("layer2.tga");
    Image car = ReadData("car.tga");
    Image result = Subtract(layer2, car);
    WriteData("task2.tga",result);
}
void taskThree(){
    // task 3
    Image layer1 = ReadData("layer1.tga");
    Image pattern2 = ReadData("pattern2.tga");
    Image text = ReadData("text.tga");
    Image result = multiply(layer1, pattern2);
    result = Screen(text, result);
    WriteData("task3.tga", result);
}
void taskFour(){
    Image layer2 = ReadData("layer2.tga");
    Image circle = ReadData("circles.tga");
    Image result = multiply(layer2, circle);
    Image pattern2 = ReadData("pattern2.tga");
    result = Subtract(pattern2, result);
    WriteData("task4.tga", result);
}
void taskFive(){
    Image layer1 = ReadData("layer1.tga");
    Image pattern1 = ReadData("pattern1.tga");
    Image result = Overlay(layer1, pattern1);
    WriteData("task5.tga", result);
}
void taskSix(){
    Image car = ReadData("car.tga");
    Image result = addGreen(car, 200);
    WriteData("task6.tga",result);
}
void taskSeven(){
    Image car = ReadData("car.tga");
    Image result = Scale(car, 0, 1, 4);
    WriteData("task7.tga", result);
}
void taskEight(){
    Image car = ReadData("car.tga");
    Image result_r = onlyRed(car);
    Image result_g = onlyGreen(car);
    Image result_b = onlyBlue(car);
    WriteData("part8_r.tga", result_r);
    WriteData("part8_g.tga", result_g);
    WriteData("part8_b.tga", result_b);
}
void taskNine(){
    Image layerR = ReadData("layer_red.tga");
    Image layerG = ReadData("layer_green.tga");
    Image layerB = ReadData("layer_blue.tga");
    Image result = combineFile(layerR, layerG, layerB);
    WriteData("task9.tga", result);

}
void taskTen(){
    Image task2 = ReadData("car.tga");
    Image result = flipImg(task2);
    WriteData("task10.tga", result);
}
int main(int argc, char* argv[]) {



    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        cout << "Project 2: Image Processing, Spring 2023\n" << endl;
        cout << "Usage:" << endl;
        cout << "\t./project2.out [output] [firstImage] [method] [...]" << endl;
        return 0;
    }
        // check if first file exists
//    if (argc <= 2) {
//        cout << "Invalid file name." << endl;
//        return 0;
//    }
    string filename = argv[1]; // out.tga
//
    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".tga") {
        cout << "Invalid file name." << endl;
        return 0;
    }

    string targetFile = argv[2];
    if (targetFile.size() < 4 || targetFile.substr(targetFile.size() - 4) != ".tga") {
        cout << "Invalid file name." << endl;
        return 0;
    }
    // check if target file exists
    ifstream filename1(targetFile, ios_base::binary);
    if (!filename1.is_open()) {
        cout << "File does not exist." << endl;
        return 0;
    }
    Image currentImage = ReadData(targetFile);
    string outName = argv[1];
    int index = 3;
    while(index < argc){
        string methodName = argv[index];
        //index; // we read the method name, now to go onto next thing in input
        index++;
        if(methodName == "multiply"){
            if(index == argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            string filename = argv[index];
            if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".tga") {
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            ifstream file(filename, ios_base::binary);
            if (!file.is_open()) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }
            Image outRead = ReadData(outName);
            Image secondImage = ReadData(filename);
            currentImage = multiply(currentImage, secondImage);
            WriteData(outName, currentImage);
            index++;
        }
        else if(methodName == "subtract"){
            if(index == argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            string filename = argv[index];
            if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".tga") {
                cout << "Invalid argument, file does not exists." << endl;
                return 0;
            }
            ifstream file(filename, ios_base::binary);
            if (!file.is_open()) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }
            Image outRead = ReadData(outName);
            Image secondImage = ReadData(filename);
            currentImage = Subtract(currentImage, secondImage);
            WriteData(outName, currentImage);
            index++;
        }
        else if(methodName == "overlay"){
            if(index == argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            string filename = argv[index];
            if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".tga") {
                cout << "Invalid argument, file does not exists." << endl;
                return 0;
            }
            ifstream file(filename, ios_base::binary);
            if (!file.is_open()) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }
            Image outRead = ReadData(outName);
            Image secondImage = ReadData(filename);
            currentImage = Overlay(currentImage, secondImage);
            WriteData(outName, currentImage);
            index++;
        }
        else if(methodName == "addblue"){
            if(index == argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            int num = stoi(argv[index]);
            Image outRead = ReadData(outName);
            currentImage = addBlue(currentImage, num);
            WriteData(outName, currentImage);
            index++;
        }
        else if(methodName == "addgreen"){
            if(index == argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            int num = stoi(argv[index]);
            Image outRead = ReadData(outName);
            currentImage = addGreen(currentImage, num);
            WriteData(outName, currentImage);
            index++;
        }
        else if(methodName == "addred"){
            if(index == argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            try{
                int num = stoi(argv[index]);
                Image outRead = ReadData(outName);
                currentImage = addRed(currentImage, num);
                WriteData(outName, currentImage);
                index++;
            }
            catch(invalid_argument& a){
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }
        }
        else if(methodName == "scalered"){
            if(index == argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            int num = stoi(argv[index]);
            Image outRead = ReadData(outName);
            currentImage = Scale(currentImage, 1, 1, num);
            WriteData(outName, currentImage);
            index++;
        }
        else if(methodName == "scalegreen"){
            if(index == argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            int num = stoi(argv[index]);
            Image outRead = ReadData(outName);
            currentImage = Scale(currentImage, 1, num, 1);
            WriteData(outName, currentImage);
            index++;
        }
        else if(methodName == "scaleblue"){
            if(index == argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            try{
                int num = stoi(argv[index]);
                Image outRead = ReadData(outName);
                currentImage = Scale(currentImage, num, 1, 1);
                WriteData(outName, currentImage);
                index++;

            }
            catch(invalid_argument& a){
                cout << "Invalid argument, expected number." << endl;
                return 0;
            }


        }
        else if(methodName == "onlyred"){
            Image outRead = ReadData(outName);
            currentImage = onlyRed(currentImage);
            WriteData(outName, currentImage);
            index++;
        }
        else if(methodName == "onlygreen"){
            Image outRead = ReadData(outName);
            currentImage = onlyGreen(currentImage);
            WriteData(outName, currentImage);
            index++;
        }
        else if(methodName == "onlyblue"){
            Image outRead = ReadData(outName);
            currentImage = onlyBlue(currentImage);
            WriteData(outName, currentImage);
            index++;
        }
        else if(methodName == "combine"){
            if(index == argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            string filename1 = argv[index - 2];
            cout << filename1 << endl;
            string filename2 = argv[index];
            cout << filename2 << endl;
            string filename3 = argv[index + 1];
            cout << filename3 << endl;


            if (filename1.size() < 4 || filename1.substr(filename1.size() - 4) != ".tga") {
                cout << "Invalid argument, file does not exists." << endl;
                return 0;
            }
            ifstream file1(filename1, ios_base::binary);
            if (!file1.is_open()) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }
            if (filename2.size() < 4 || filename2.substr(filename2.size() - 4) != ".tga") {
                cout << "Invalid argument, file does not exists." << endl;
                return 0;
            }
            ifstream file2(filename2, ios_base::binary);
            if (!file2.is_open()) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }
            if (filename3.size() < 4 || filename3.substr(filename3.size() - 4) != ".tga") {
                cout << "Invalid argument, file does not exists." << endl;
                return 0;
            }
            ifstream file3(filename3, ios_base::binary);
            if (!file3.is_open()) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            Image outRead = ReadData(outName);
            Image firstImage = ReadData(filename1);
            Image secondImage = ReadData(filename2);
            Image thirdImage = ReadData(filename3);
            currentImage = combineFile(firstImage, secondImage, thirdImage);
            WriteData(outName, currentImage);
            index++;
            index++;
    
        }
        // flip function
        else if(methodName == "flip"){
            if(index == argc){
                cout << "missing argument." << endl;
            }
            Image outRead = ReadData(outName);
            currentImage = flipImg(currentImage);
            WriteData(outName, currentImage);
            //index++;
        }
        else if(methodName == "screen"){
            if(index == argc){
                cout << "Missing argument." << endl;
                return 0;
            }
            string filename = argv[index];
            if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".tga") {
                cout << "Invalid argument, file does not exists." << endl;
                return 0;
            }
            ifstream file(filename, ios_base::binary);
            if (!file.is_open()) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }
            Image outRead = ReadData(outName);
            Image secondImage = ReadData(filename);
            currentImage = Screen(currentImage, secondImage);
            WriteData(outName, currentImage);
            index++;
        }
        else{
            cout << "Invalid method name." << endl;
            return 0;
        }
    }
    return 0;
}






