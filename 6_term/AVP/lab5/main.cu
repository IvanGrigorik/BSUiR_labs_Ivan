#include "Image.cuh"
#include <algorithm>
#include <cmath>
#include <iostream>

#define degToRad(val) (val * M_PI / 180)

std::vector<std::vector<int>> houghAccum;
Pixel marker{0, 255, 0};

/// Returns line angle relative to the horizon (see explanation ahead the function)
/*
 * |                     ----
 * |                 ----
 * |             ---- A (equivalent)
 * | ****************************** horizon line
 * |       A ----
 * |     ----
 * | ----
 * |||||||||||||||||||||||||||||||| image x-axis
 * Return angle A
 */
double houghTransform(const Image &image) {
    const int maxDist = static_cast<int>(round(sqrt(pow(image.getHeight(), 2) + pow(image.getWidth(), 2))));
    // Little theta and step optimization
    const int thetasCount = 2 * (image.getHeight() + image.getWidth()) - 4;

    // Memory allocation
    houghAccum.resize(2 * maxDist);
    for (int i = 0; i < 2 * maxDist; i++) { houghAccum[i].resize(thetasCount); }

    const float step = 180.f / static_cast<float>(thetasCount);

    for (int y = 0; y < image.getHeight(); y++) {
        for (int x = 0; x < image.getWidth(); x++) {
            // Find pixel fits the marker
            if (image.getPixel(x, y) != marker) { continue; }

            double ang = -90;
            for (int h = 0; h < thetasCount; ang += step, h++) {
                int idx = static_cast<int>(maxDist + x * cos(degToRad(ang)) + y * sin(degToRad(ang)));
                houghAccum[idx][h]++;
            }
        }
    }

    int idx{}, max{};
    for (auto &i: houghAccum) {
        for (int j = 0; j < i.size(); j++) {
            if (i[j] > max) {
                max = i[j];
                idx = j;
            }
        }
    }

    return -90 + step * static_cast<float>(idx);
}

Image rotateImage(const Image &image, const double angle) {
    Image newImage{"../outImage.png"};

    const auto height = image.getHeight(), width = image.getWidth();

    const int centerX = width / 2, centerY = height / 2;
    newImage.setProperties(height, width, image.getChannels());

    const auto radAngle = degToRad(angle);
    const auto k1 = angle > 0 ? sin(radAngle) : cos(radAngle);
    const auto k2 = angle > 0 ? cos(radAngle) : sin(radAngle);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto newX = static_cast<int>(k1 * (x - centerX) + k2 * (y - centerY) + centerX);
            auto newY = static_cast<int>(k1 * (y - centerY) - k2 * (x - centerX) + centerY);

            if (newX < 0 or newX >= width or newY < 0 or newY >= height) { continue; }

            newImage.setPixel(newX, newY, image.getPixel(x, y));
        }
    }

    return newImage;
}

int main() {
    using namespace std;
    //    Image image{"../images/line65.png"};
    Image image{"../images/man2.png"};
    image.readImage();
    cout << "Image height: " << image.getHeight() << endl << "Image width: " << image.getWidth() << endl;

    //     Get angle to rotate image
    const auto houghtResult = houghTransform(image);
    cout << endl << "Hough result: " << houghtResult << endl;
    const auto rotationAngle = houghtResult > 0 ? 180 - houghtResult :  - (90 + houghtResult);
    cout  << "Rotation angle: " << rotationAngle << endl;

    //    rotation angle is 38.6744 in man.png

    Image rotatedImage = rotateImage(image, rotationAngle);
    rotatedImage.writeImage();

    return 0;
}
