#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Function to adjust brightness and contrast
void adjustBrightnessContrast(const Mat& input, Mat& output, double alpha, int beta) {
    // alpha: contrast multiplier, beta: brightness offset
    input.convertTo(output, -1, alpha, beta);
}

// Function to equalize histogram (grayscale images only)
void histogramEqualization(const Mat& input, Mat& output) {
    if (input.channels() == 1) {
        equalizeHist(input, output);
    } else {
        cout << "Histogram equalization works only on grayscale images." << endl;
        output = input.clone();
    }
}

// Function to apply Gaussian blur
void applyGaussianBlur(const Mat& input, Mat& output, int kernelSize) {
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
    string imagePath = "image.jpg"; // Replace with your image path
    Mat image = imread(imagePath);

    if (image.empty()) {
        cout << "Error: Could not load image." << endl;
        return -1;
    }

    // Display the original image
    imshow("Original Image", image);

    // Brightness and contrast adjustment
    Mat brightContrastImage;
    double alpha = 1.2; // Contrast multiplier
    int beta = 30;      // Brightness offset
    adjustBrightnessContrast(image, brightContrastImage, alpha, beta);
    imshow("Brightness & Contrast Adjusted", brightContrastImage);

    // Histogram equalization
    Mat grayImage, histEqualizedImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);
    histogramEqualization(grayImage, histEqualizedImage);
    imshow("Histogram Equalized", histEqualizedImage);

    // Gaussian blur
    Mat blurredImage;
    int kernelSize = 7; // Must be odd
    applyGaussianBlur(image, blurredImage, kernelSize);
    imshow("Gaussian Blurred", blurredImage);

    // Sharpening
    Mat sharpenedImage;
    sharpenImage(image, sharpenedImage);
    imshow("Sharpened Image", sharpenedImage);

    // Adding Gaussian noise
    Mat noisyImage;
    double mean = 0, stddev = 30; // Noise parameters
    addGaussianNoise(image, noisyImage, mean, stddev);
    imshow("Gaussian Noise Added", noisyImage);

    // Wait for user input and cleanup
    waitKey(0);
    destroyAllWindows();

    return 0;
}
