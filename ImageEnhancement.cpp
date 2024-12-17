#include <opencv2/opencv.hpp>
#include <iostream>
#include <cctype>

using namespace cv;
using namespace std;

// Function to adjust brightness and contrast
void adjustBrightnessContrast(const Mat& input, Mat& output, double alpha, int beta) {
    // alpha: contrast multiplier, beta: brightness offset
    input.convertTo(output, -1, alpha, beta);
}

// Function to equalize histogram (grayscale images only)
void histogramEqualization(const Mat& input, Mat& output) 
{
    if (input.channels() == 1) 
    {
        equalizeHist(input, output);
    } 
    else 
    {
        cout << "Histogram equalization works only on grayscale images." << endl;
        output = input.clone();
    }
}

// Function to apply Gaussian blur
void applyGaussianBlur(const Mat& input, Mat& output, int kernelSize) 
{
    GaussianBlur(input, output, Size(kernelSize, kernelSize), 0);
}

// Function to apply sharpening using a kernel
void sharpenImage(const Mat& input, Mat& output) {
    Mat kernel = (Mat_<float>(3, 3) << 
                   0, -1,  0, 
                  -1,  5, -1, 
                   0, -1,  0);
    filter2D(input, output, -1, kernel);
}

// Function to add Gaussian noise
void addGaussianNoise(const Mat& input, Mat& output, double mean, double stddev) {
    Mat noise(input.size(), input.type());
    randn(noise, mean, stddev); // Generate Gaussian noise
    output = input + noise;
}

int main() {
    // Load an image
    string imagePath;
    cout<< "Please enter your Image Path: ";
    cin>> imagePath;
    Mat image = imread(imagePath);
    int transformation;
    if (image.empty()) {
        cout << "Error: Could not load image." << endl;
        return -1;
    }
    cout<< "Please select the number of the transformation you would like to be done to the image: Brightness & Contrast(1), Histogram equalization(2), Gaussian Blur(3), Sharpening(4), Gaussian noise(5)" <<endl;
    cin>> transformation;
    try
    {
        if(transformation>5 || transformation<1 || cin.fail())
        {
            throw "Error: Value entered is not a number between 1-5";
        }
    }
    catch(const char* err)
    {
        cout<< err<< endl;
        return -1;
    }
    switch(transformation)
    {
        case 1:
        {
            // Brightness and contrast adjustment
            Mat brightContrastImage;
            double alpha = 1;
            int beta = 0;
            cout<< "Enter Contrast Mulitplier Value(Alpha)"<<endl;
            cin>> alpha;
            if(cin.fail())
            {
                cout<< "Error: Non-Integer Entered"<<endl;
                return -1;
            }
            cout<< "Enter Brightness Offset Value(Beta)"<<endl;
            cin>> beta;      
            if(cin.fail())
            {
                cout<< "Error: Non-Integer Entered"<<endl;
                return -1;
            }
            imshow("Original Image", image);
            adjustBrightnessContrast(image, brightContrastImage, alpha, beta);
            imshow("Brightness & Contrast Adjusted", brightContrastImage);
            break;
        }
        case 2:
        {
            // Histogram equalization
            imshow("Original Image", image);
            Mat grayImage, histEqualizedImage;
            cvtColor(image, grayImage, COLOR_BGR2GRAY);
            histogramEqualization(grayImage, histEqualizedImage);
            imshow("Histogram Equalized", histEqualizedImage);
            break;
        }
        case 3:
        {
            // Gaussian blur
            int kernelSize = 1;
            cout<<"Enter Kernel Size for the blurred image(Must be odd): ";
            cin>> kernelSize;
            if(kernelSize%2 == 0 || cin.fail())
            {
                cout<< "Error: Non-Odd Number entered"<<endl;
                return -1;
            }
            imshow("Original Image", image);
            Mat blurredImage;
            applyGaussianBlur(image, blurredImage, kernelSize);
            imshow("Gaussian Blurred", blurredImage);
            break;
        }
        case 4:
        {
            // Sharpening
            Mat sharpenedImage;
            sharpenImage(image, sharpenedImage);
            imshow("Sharpened Image", sharpenedImage);
            break;
        }
        case 5:
        {
            // Adding Gaussian noise
            Mat noisyImage;
            double mean = 0, stddev = 30; // Noise parameters
            addGaussianNoise(image, noisyImage, mean, stddev);
            imshow("Gaussian Noise Added", noisyImage);
            break;
        }
    }
    // Wait for user input and cleanup
    waitKey(0);
    destroyAllWindows();

    return 0;
}
