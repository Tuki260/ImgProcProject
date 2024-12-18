#include <opencv2/opencv.hpp>
#include <iostream>
#include <cctype>
#include <vector>

using namespace cv;
using namespace std;

// Function to adjust brightness and contrast
void adjustBrightnessContrast(const Mat& input, Mat& output, double alpha, int beta) 
{
    // Ensure the output matrix is the same size and type as input
    output = input.clone();

    // Loop through each pixel in the input image
    for (int i = 0; i < input.rows; i++)
    {
        for (int j = 0; j < input.cols; j++) 
        {
            for (int c = 0; c < input.channels(); c++) 
            {
                // Adjust the pixel value based on the formula output(x,y)=α⋅input(x,y)+β
                int pixelValue = static_cast<int>(alpha * input.at<Vec3b>(i, j)[c] + beta);
                // Ensure pixel values are within a valid range of [0, 255]
                output.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(pixelValue);
            }
        }
    }
}
void histogramEqualization(const Mat& input, Mat& output) 
{
    if (input.channels() == 1) {
        // Step 1: Calculate the histogram
        int hist[256] = {0};
        for (int i = 0; i < input.rows; i++) {
            for (int j = 0; j < input.cols; j++) {
                hist[input.at<uchar>(i, j)]++;
            }
        }

        // Step 2: Compute the cumulative distribution function (CDF)
        int cdf[256] = {0};
        cdf[0] = hist[0];
        for (int i = 1; i < 256; i++) {
            cdf[i] = cdf[i - 1] + hist[i];
        }

        // Step 3: Normalize the CDF
        int totalPixels = input.rows * input.cols;
        for (int i = 0; i < 256; i++) {
            cdf[i] = (cdf[i] - cdf[0]) * 255 / (totalPixels - cdf[0]);
        }

        // Step 4: Map the pixel values using the CDF
        output = input.clone();
        for (int i = 0; i < input.rows; i++) {
            for (int j = 0; j < input.cols; j++) {
                output.at<uchar>(i, j) = cdf[input.at<uchar>(i, j)];
            }
        }
    } else {
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

void boxBlur(const Mat& input, Mat& output)
{
    Mat kernel3 = (Mat_<float>(3, 3) << 
                   1/9.0, 1/9.0, 1/9.0, 
                   1/9.0, 1/9.0, 1/9.0, 
                   1/9.0, 1/9.0, 1/9.0);
    Mat kernel5 = (Mat_<float>(5, 5) << 
               1/25.0, 1/25.0, 1/25.0, 1/25.0, 1/25.0, 
               1/25.0, 1/25.0, 1/25.0, 1/25.0, 1/25.0, 
               1/25.0, 1/25.0, 1/25.0, 1/25.0, 1/25.0, 
               1/25.0, 1/25.0, 1/25.0, 1/25.0, 1/25.0, 
               1/25.0, 1/25.0, 1/25.0, 1/25.0, 1/25.0);
    int choice;
    cout<<"Please choose between a 3x3 box blur(3), or a 5x5 box blur(5)" << endl << "A 5x5 box blur results in more blurring: ";
    cin>> choice;
    if(cin.fail())
    {
        cout<< "Error: Not valid choice, please select 3 or 5";
    }
    else if(choice == 3)
    {
        filter2D(input, output, -1, kernel3);
    }
    else if(choice == 5)
    {
        filter2D(input, output, -1, kernel5);
    }
    else
    {
        cout<< "Error: Not valid choice, please select 3 or 5";
    }
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
    cout<< "Please select the number of the transformation you would like to be done to the image: Brightness & Contrast(1), Histogram equalization(2), Gaussian Blur(3), Sharpening(4), Gaussian noise(5), Box Blur(6)" <<endl;
    cin>> transformation;
    try
    {
        if(transformation>6 || transformation<1 || cin.fail())
        {
            throw "Error: Value entered is not a number between 1-6";
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
            kernelSize = abs(kernelSize);
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
            imshow("Original Image", image);
            sharpenImage(image, sharpenedImage);
            imshow("Sharpened Image", sharpenedImage);
            break;
        }
        case 5:
        {
            // Adding Gaussian noise
            Mat noisyImage;
            cout<< "Input your Mean value: ";
            double mean, stddev; // Noise parameters
            cin>> mean;
            if(cin.fail())
            {
                cout<< "Error: Non-integer value";
                return -1;
            }
            cout<< "Input your standard deviation value: ";
            cin>> stddev;
            if(cin.fail())
            {
                cout<< "Error: Non-integer value";
                return -1;
            }
            imshow("Original Image", image);
            addGaussianNoise(image, noisyImage, mean, stddev);
            imshow("Gaussian Noise Added", noisyImage);
            break;
        }
        case 6:
        {
            Mat boxBlurred;
            boxBlur(image, boxBlurred);
            imshow("Original Image", image);
            imshow("Box Blurred Image", boxBlurred);
            break;
        }
    }
    // Wait for user input and cleanup
    waitKey(0);
    destroyAllWindows();

    return 0;
}
