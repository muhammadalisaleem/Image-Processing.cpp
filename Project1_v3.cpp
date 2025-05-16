#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cmath>

using namespace std;

struct Image {
    char ImageFileName[100];
    vector<vector<int>> ImageData;
    int cols, rows, maxGray;
    vector<char> comment;

    bool imageLoaded;
    bool imageModified;

    // LOAD IMAGE
    int loadImage(char ImageName[]) {

        ifstream FCIN(ImageName);

        if (!FCIN.is_open())
            return -1;

        char MagicNumber[5];
        char Comment[100];

        FCIN.getline(MagicNumber, 4);
        FCIN.getline(Comment, 100);
        FCIN >> cols >> rows >> maxGray;

        ImageData.clear();
        ImageData.resize(rows, vector<int>(cols, 0));

        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++)
                FCIN >> ImageData[r][c];

        if (FCIN.fail())
            return -2;

        FCIN.close();
        imageLoaded = true;
        imageModified = false;
        strcpy(ImageFileName, ImageName);
        return 0;
    }

    // SAVE IMAGE
    int saveImage(char ImageName[]) {
        ofstream FCOUT(ImageName);
        if (!FCOUT.is_open())
            return -1;

        FCOUT << "P2\n# This is a comment\n"
            << cols << " " << rows << endl << maxGray << endl;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++)
                FCOUT << ImageData[r][c] << " ";
            FCOUT << endl;
        }
        FCOUT.close();
        imageModified = false;
        return 0;
    }


    // Function to Adjust Brightness
    void changeBrightness(double factor) {
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++) {
                ImageData[r][c] *= factor;
                if (ImageData[r][c] > maxGray)
                    ImageData[r][c] = maxGray;
            }
    }

    // Function to Adjust Contrast
    void contrastStretching() {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }
        int minPixelValue = INT_MAX;
        int maxPixelValue = INT_MIN;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                int pixelValue = ImageData[r][c];
                minPixelValue = min(minPixelValue, pixelValue);
                maxPixelValue = max(maxPixelValue, pixelValue);
            }
        }
        double factor = 255.0 / (maxPixelValue - minPixelValue);

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {

                ImageData[r][c] = static_cast<int>((ImageData[r][c] - minPixelValue) * factor);
            }
        }

        cout << "Contrast stretching applied." << endl;
        imageModified = true;
    }

    // Function to Adjust Sharpness
    void applySharpening() {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        vector<vector<double>> sharpeningKernel = {
            {-1, -1, -1},
            {-1, 9, -1},
            {-1, -1, -1}
        };

        vector<vector<double>> newImageData(rows, vector<double>(cols, 0.0));

        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                double sum = 0.0;

                for (int m = -1; m <= 1; ++m) {
                    for (int n = -1; n <= 1; ++n) {
                        sum += sharpeningKernel[m + 1][n + 1] * static_cast<double>(ImageData[i + m][j + n]);
                    }
                }
                newImageData[i][j] = sum;
            }
        }
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                ImageData[i][j] = static_cast<int>(std::min(255.0, std::max(0.0, newImageData[i][j])));
            }
        }

        cout << "Sharpness adjustment applied." << endl;
        imageModified = true;
    }

    // Function to Convert Image to Binary
    void convertToBinary(int threshold) {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                ImageData[i][j] = (ImageData[i][j] < threshold) ? 0 : maxGray;
            }
        }

        cout << "Image converted to binary." << endl;
        imageModified = true;
    }

    // Function to Resize Image
    void resizeImage(double ratio) {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        int newRows = static_cast<int>(rows * ratio);
        int newCols = static_cast<int>(cols * ratio);

        vector<vector<int>> resizedImageData(newRows, vector<int>(newCols, 0));

        for (int i = 0; i < newRows; ++i) {
            for (int j = 0; j < newCols; ++j) {
                int originalRow = static_cast<int>(i / ratio);
                int originalCol = static_cast<int>(j / ratio);
                resizedImageData[i][j] = ImageData[originalRow][originalCol];
            }
        }

        ImageData = resizedImageData;
        rows = newRows;
        cols = newCols;

        cout << "Image resized to " << newCols << "x" << newRows << "." << endl;
        imageModified = true;
    }

    // Function to Rotate Image Clockwise
    void rotate90Clockwise() {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        vector<vector<int>> rotatedImageData(cols, vector<int>(rows, 0));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                rotatedImageData[j][rows - 1 - i] = ImageData[i][j];
            }
        }

        ImageData = rotatedImageData;
        swap(rows, cols);

        cout << "Image rotated 90 degrees clockwise." << endl;
        imageModified = true;
    }

    // Function to Rotate CounterClockwise
    void rotate90CounterClockwise() {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        vector<vector<int>> rotatedImageData(cols, vector<int>(rows, 0));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                rotatedImageData[cols - 1 - j][i] = ImageData[i][j];
            }
        }

        ImageData = rotatedImageData;
        swap(rows, cols);

        cout << "Image rotated 90 degrees counterclockwise." << endl;
        imageModified = true;
    }

    // Function to Flip image Vertically
    void flipVertical() {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }
        for (int i = 0; i < rows / 2; ++i) {
            for (int j = 0; j < cols; ++j) {
                swap(ImageData[i][j], ImageData[rows - 1 - i][j]);
            }
        }

        cout << "Image flipped vertically." << endl;
        imageModified = true;
    }

    // Funtion to Flip image Horizontally
    void horzontalFlipImage() {
        for (int r = 0; r < rows / 2; r++)
            for (int c = 0; c < cols; c++) {
                int T = ImageData[r][c];
                ImageData[r][c] = ImageData[rows - r][c];
                ImageData[rows - r][c] = T;
            }
        return;
    }

    // Function to Translate Image
    void translateImage(int deltaX, int deltaY) {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        vector<vector<int>> translatedImageData(rows, vector<int>(cols, 0));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int newRow = i + deltaY;
                int newCol = j + deltaX;

                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                    translatedImageData[newRow][newCol] = ImageData[i][j];
                }
            }
        }

        ImageData = translatedImageData;

        cout << "Image translated by (" << deltaX << ", " << deltaY << ")." << endl;
        imageModified = true;
    }

    // Function to Scale Image
    void scaleImage(double scaleFactor) {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        if (scaleFactor <= 0) {
            cout << "Error: Invalid scaling factor." << endl;
            return;
        }

        int newRows = static_cast<int>(rows * scaleFactor);
        int newCols = static_cast<int>(cols * scaleFactor);

        vector<vector<int>> scaledImageData(newRows, vector<int>(newCols, 0));

        for (int i = 0; i < newRows; ++i) {
            for (int j = 0; j < newCols; ++j) {
                int originalRow = static_cast<int>(i / scaleFactor);
                int originalCol = static_cast<int>(j / scaleFactor);
                scaledImageData[i][j] = ImageData[originalRow][originalCol];
            }
        }

        ImageData = scaledImageData;
        rows = newRows;
        cols = newCols;

        cout << "Image scaled by a factor of " << scaleFactor << "." << endl;
        imageModified = true;
    }

    // Function to Crop Image
    void cropImage(int startX, int startY, int endX, int endY) {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        if (startX < 0 || startY < 0 || endX >= cols || endY >= rows || startX >= endX || startY >= endY) {
            cout << "Invalid crop coordinates." << endl;
            return;
        }

        int newRows = endY - startY + 1;
        int newCols = endX - startX + 1;

        vector<vector<int>> croppedImageData(newRows, vector<int>(newCols, 0));

        for (int i = 0; i < newRows; ++i) {
            for (int j = 0; j < newCols; ++j) {
                croppedImageData[i][j] = ImageData[startY + i][startX + j];
            }
        }

        ImageData = croppedImageData;
        rows = newRows;
        cols = newCols;

        cout << "Image cropped." << endl;
        imageModified = true;
    }

    // Function to Combine Image Side-by-side
    void combineHorizontally(const Image& image2) {
        if (!imageLoaded || !image2.imageLoaded) {
            cout << "Error: One or more images not loaded." << endl;
            return;
        }

        if (rows != image2.rows) {
            cout << "Error: Images must have the same height for horizontal combination." << endl;
            return;
        }

        vector<vector<int>> combinedImageData(rows, vector<int>(cols + image2.cols, 0));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                combinedImageData[i][j] = ImageData[i][j];
            }
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < image2.cols; ++j) {
                combinedImageData[i][cols + j] = image2.ImageData[i][j];
            }
        }

        ImageData = combinedImageData;
        cols += image2.cols;

        cout << "Images combined horizontally." << endl;
        imageModified = true;
    }

    // Function to Combine Image Top-to-bottom
    void combineVertically(const Image& image2) {
        if (!imageLoaded || !image2.imageLoaded) {
            cout << "Error: One or more images not loaded." << endl;
            return;
        }

        if (cols != image2.cols) {
            cout << "Error: Images must have the same width for vertical combination." << endl;
            return;
        }

        vector<vector<int>> combinedImageData(rows + image2.rows, vector<int>(cols, 0));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                combinedImageData[i][j] = ImageData[i][j];
            }
        }

        for (int i = 0; i < image2.rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                combinedImageData[rows + i][j] = image2.ImageData[i][j];
            }
        }

        ImageData = combinedImageData;
        rows += image2.rows;

        cout << "Images combined vertically." << endl;
        imageModified = true;
    }

    // Function to Apply Mean Filter
    void applyMeanFilter() {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        vector<vector<int>> filteredImageData(rows, vector<int>(cols, 0));

        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                int sum = 0;

                for (int ni = -1; ni <= 1; ++ni) {
                    for (int nj = -1; nj <= 1; ++nj) {
                        sum += ImageData[i + ni][j + nj];
                    }
                }

                filteredImageData[i][j] = static_cast<int>(round(sum / 9.0));
            }
        }

        ImageData = filteredImageData;

        cout << "Mean filter applied." << endl;
        imageModified = true;
    }

    // Function to Apply Median Filter
    void applyMedianFilter() {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        vector<vector<int>> filteredImageData(rows, vector<int>(cols, 0));

        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                vector<int> neighborhood;

                for (int ni = -1; ni <= 1; ++ni) {
                    for (int nj = -1; nj <= 1; ++nj) {
                        neighborhood.push_back(ImageData[i + ni][j + nj]);
                    }
                }

                sort(neighborhood.begin(), neighborhood.end());
                filteredImageData[i][j] = neighborhood[4];
            }
        }

        ImageData = filteredImageData;

        cout << "Median filter applied." << endl;
        imageModified = true;
    }

    // Function to Enhance Image by Applying Linear Filter
    void applyLinearFilter(const char* filterFileName) {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        ifstream filterFile(filterFileName);
        if (!filterFile.is_open()) {
            cout << "Error: Unable to open filter file." << endl;
            return;
        }

        int filterSize;
        filterFile >> filterSize;

        vector<vector<int>> filter(filterSize, vector<int>(filterSize, 0));

        for (int i = 0; i < filterSize; ++i) {
            for (int j = 0; j < filterSize; ++j) {
                filterFile >> filter[i][j];
            }
        }

        filterFile.close();

        vector<vector<int>> filteredImageData(rows, vector<int>(cols, 0));

        int filterOffset = filterSize / 2;
        for (int i = filterOffset; i < rows - filterOffset; ++i) {
            for (int j = filterOffset; j < cols - filterOffset; ++j) {
                int sum = 0;

                for (int ni = -filterOffset; ni <= filterOffset; ++ni) {
                    for (int nj = -filterOffset; nj <= filterOffset; ++nj) {
                        sum += ImageData[i + ni][j + nj] * filter[ni + filterOffset][nj + filterOffset];
                    }
                }

                filteredImageData[i][j] = sum;
            }
        }

        ImageData = filteredImageData;

        cout << "Linear filter applied." << endl;
        imageModified = true;
    }

    // Function to Compute Derivative
    void applySobelX() {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        vector<vector<int>> sobelXMask = { {-1, 0, 1},
                                          {-2, 0, 2},
                                          {-1, 0, 1} };

        vector<vector<int>> derivativeImageData(rows, vector<int>(cols, 0));

        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                int sum = 0;

                for (int ni = -1; ni <= 1; ++ni) {
                    for (int nj = -1; nj <= 1; ++nj) {
                        sum += ImageData[i + ni][j + nj] * sobelXMask[ni + 1][nj + 1];
                    }
                }

                derivativeImageData[i][j] = sum;
            }
        }

        ImageData = derivativeImageData;

        cout << "Sobel X filter applied (Derivative in the X direction)." << endl;
        imageModified = true;
    }

    // Function to Find Edges
    void findEdges() {
        if (!imageLoaded) {
            cout << "Error: Image not loaded." << endl;
            return;
        }

        // Sobel X mask
        vector<vector<int>> sobelXMask = { {-1, 0, 1},
                                          {-2, 0, 2},
                                          {-1, 0, 1} };

        // Sobel Y mask
        vector<vector<int>> sobelYMask = { {-1, -2, -1},
                                          {0, 0, 0},
                                          {1, 2, 1} };

        vector<vector<int>> gradientX(rows, vector<int>(cols, 0));
        vector<vector<int>> gradientY(rows, vector<int>(cols, 0));

        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                int sumX = 0;
                int sumY = 0;

                // operation for Sobel X
                for (int ni = -1; ni <= 1; ++ni) {
                    for (int nj = -1; nj <= 1; ++nj) {
                        sumX += ImageData[i + ni][j + nj] * sobelXMask[ni + 1][nj + 1];
                    }
                }

                // operation for Sobel Y
                for (int ni = -1; ni <= 1; ++ni) {
                    for (int nj = -1; nj <= 1; ++nj) {
                        sumY += ImageData[i + ni][j + nj] * sobelYMask[ni + 1][nj + 1];
                    }
                }

                gradientX[i][j] = sumX;
                gradientY[i][j] = sumY;
            }
        }

        vector<vector<int>> edgeMagnitude(rows, vector<int>(cols, 0));

        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
                edgeMagnitude[i][j] = static_cast<int>(round(sqrt(gradientX[i][j] * gradientX[i][j] +
                    gradientY[i][j] * gradientY[i][j])));
            }
        }

        ImageData = edgeMagnitude;

        cout << "Edges detected using gradients." << endl;
        imageModified = true;
    }
};

// SHOW MENU
struct Menu {
    vector<string> menuItems;

    Menu(char menuFile[]) {
        loadMenu(menuFile);
    }

    int loadMenu(char menuFile[]) {
        ifstream IN;
        IN.open(menuFile);
        if (!IN.is_open())
            return -1;
        char menuItem[100], TotalItems[10];

        int Choices;

        IN.getline(TotalItems, 8);
        Choices = atoi(TotalItems);
        for (int i = 1; i <= Choices; i++) {
            IN.getline(menuItem, 99);
            menuItems.push_back(menuItem);
        }
        IN.close();
        return Choices;
    }

    int presentMenu() {
        int userChoice;
        int totalChoices = menuItems.size();


        do {
            int k = 1;
            for (int i = 0; i < totalChoices; i++) {
                if (menuItems[i][0] != '*') {
                    cout << k << "\t" << menuItems[i] << endl;
                    k++;
                }
            }
            cout << " Enter Your Choice (1 - " << k - 1 << " ) ";
            cin >> userChoice;
        } while (userChoice < 1 || userChoice > totalChoices);
        return userChoice;
    }

};

int main() {
    char MenuFile[] = "MainMenu.txt";
    Image images[2];
    int activeImage = 0;
    int errorCode = 0;
    int userChoice;
    //    int TotalChoices = loadMenu("MainMenu.txt");
    int totalChoices;

    Menu menu(MenuFile);
    totalChoices = menu.menuItems.size();
    do {
        userChoice = menu.presentMenu();
        if (1 == userChoice) {
            char ImageFileName[100];
            cout << "Specify File Name ";
            cin >> ImageFileName;
            errorCode = images[activeImage].loadImage(ImageFileName);
            if (errorCode == 0) {
                cout << "File Loaded Successfully " << endl;
                cout << endl;
            }
            else {
                cout << "Load Error: Code " << errorCode << endl;
                cout << endl;
            }
        }
        else if (2 == userChoice) {
            char ImageFileName[100];
            cout << "Specify File Name ";
            cin >> ImageFileName;
            errorCode = images[activeImage].saveImage(ImageFileName);
            if (errorCode == 0) {
                cout << "File Saved as " << ImageFileName << endl;
                cout << endl;
            }
            else {
                cout << "Save Error: Code " << errorCode << endl;
                cout << endl;
            }
        }
        else if (3 == userChoice) {
            double factor;
            cout << "Specify Change Brightness Factor: ";
            cin >> factor;
            images[activeImage].changeBrightness(factor);
            cout << "You need to save the changes "<< endl;
            cout << endl;
        }
        else if (4 == userChoice) {
            images[activeImage].contrastStretching();
            cout << "You need to save the changes " << endl;
            cout << endl;
        }
        else if (5 == userChoice) {
            images[activeImage].applySharpening();
            cout << "You need to save the changes " << endl;
            cout << endl;
        }
        else if (6 == userChoice) {
            int threshold;
            cout << "Enter the threshold for binary conversion: ";
            cin >> threshold;
            images[activeImage].convertToBinary(threshold);
            cout << "Image converted to binary." << endl;
            cout << "You need to save the changes." << endl;
            cout << endl;
        }
        else if (7 == userChoice) {
            double ratio;
            cout << "Specify Resizing Ratio: ";
            cin >> ratio;
            images[activeImage].resizeImage(ratio);
            cout << "You need to save the changes " << endl;
            cout << endl;
        }
        else if (8 == userChoice) {
            images[activeImage].rotate90Clockwise();
            cout << "You need to save the changes " << endl;
            cout << endl;
        }
        else if (9 == userChoice) {
            images[activeImage].rotate90CounterClockwise();
            cout << "You need to save the changes " << endl;
            cout << endl;
        }
        else if (10 == userChoice) {
            images[activeImage].flipVertical();
            cout << "You need to save the changes " << endl;
            cout << endl;
        }
        else if (11 == userChoice) {
            images[activeImage].horzontalFlipImage();
            cout << "You need to save the changes " << endl;
            cout << endl;
        }
        else if (12 == userChoice) {
            int x, y;
            cout << "Specify Translating directions(x,y): ";
            cin >> x >> y;
            images[activeImage].translateImage(x, y);
            cout << "You need to save the changes " << endl;
            cout << endl;
        }
        else if (13 == userChoice) {
            double S_Factor;
            cout << "Specify Scaling Factor: ";
            cin >> S_Factor;
            images[activeImage].scaleImage(S_Factor);
            cout << "You need to save the changes " << endl;
            cout << endl;
        }
        else if (14 == userChoice) {
            int start_x, start_y, end_x, end_y;
            cout << "Enter the starting point X: ";
            cin >> start_x;
            cout << "Enter the starting point Y: ";
            cin >> start_y;
            cout << "Enter the ending point X: ";
            cin >> end_x;
            cout << "Enter the ending point Y: ";
            cin >> end_y;
            images[activeImage].cropImage(start_x, start_y, end_x, end_y);
            cout << "You need to save the changes " << endl;
            cout << endl;
        }

        else if (15 == userChoice) {
            char ImageFileName[100];
            cout << "Enter the file name of the second image for horizontal combination: ";
            cin >> ImageFileName;

            Image image2;
            errorCode = image2.loadImage(ImageFileName);

            if (errorCode == 0) {
                images[activeImage].combineHorizontally(image2);
                cout << "Images combined horizontally." << endl;
                cout << "You need to save the changes." << endl;
                cout << endl;
            }
            else {
                cout << "Load Error: Code " << errorCode << endl;
                cout << endl;
            }
        }

        else if (16 == userChoice) {
            char ImageFileName[100];
            cout << "Enter the file name of the second image for vertical combination: ";
            cin >> ImageFileName;

            Image image2;
            errorCode = image2.loadImage(ImageFileName);

            if (errorCode == 0) {
                images[activeImage].combineVertically(image2);
                cout << "Images combined vertically." << endl;
                cout << "You need to save the changes." << endl;
                cout << endl;
            }
            else {
                cout << "Load Error: Code " << errorCode << endl;
                cout << endl;
            }
        }

        else if (17 == userChoice) {
            images[activeImage].applyMeanFilter();
            cout << "You need to save the changes " << endl;
            cout << endl;
        }
        else if (18 == userChoice) {
            images[activeImage].applyMedianFilter();
            cout << "You need to save the changes " << endl;
            cout << endl;
        }
        else if (19 == userChoice) {
            char ImageFileName[100];
            cout << "Enter the file name of the image for linear filter application: ";
            cin >> ImageFileName;

            Image image;
            errorCode = image.loadImage(ImageFileName);

            if (errorCode == 0) {
                char FilterFileName[100];
                cout << "Enter the file name of the filter for linear filtering: ";
                cin >> FilterFileName;

                image.applyLinearFilter(FilterFileName);

                cout << "Linear filter applied." << endl;
                cout << "You need to save the changes." << endl;
                cout << endl;
                images[activeImage] = image;
            }
        }

        else if (20 == userChoice) {
            images[activeImage].applySobelX();
            cout << "You need to save the changes " << endl;
            cout << endl;
        }

         else if (21 == userChoice) {
            images[activeImage].findEdges();
            cout << "You need to save the changes " << endl;
            cout << endl;
        }


    } while (userChoice != totalChoices);
    return 0;
}

// Created with <3 by Ali Saleem & Haroon Osman.
