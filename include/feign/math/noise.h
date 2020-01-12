// /**
//  * Author:    Zackary Misso
//  * Last Edited:   10.12.2019
//  *
//  * This code is taken directly from PBRT.
//  *
//  * Anyone has permission to use the following code as long as proper
//  * acknowledgement is provided to the original author(s).
//  **/
//
// // TODO: should this be apart of the image edit library???
//
// #pragma once
//
// #include <feign/common.h>
//
// static int NoiseTableSize = 256;
//
// // straight from pbrt
// static int NoiseTable[512] =
// {
//     151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,
//     36, 103, 30, 69, 142,
//     // Remainder of the noise permutation table
//     8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62,
//     94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
//     20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77,
//     146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55,
//     46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76,
//     132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100,
//     109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147,
//     118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28,
//     42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101,
//     155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232,
//     178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12,
//     191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31,
//     181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
//     138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66,
//     215, 61, 156, 180, 151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194,
//     233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6,
//     148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
//     57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
//     165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60,
//     211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
//     63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135,
//     130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
//     250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
//     227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2,
//     44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
//     98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251,
//     34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249,
//     14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115,
//     121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72,
//     243, 141, 128, 195, 78, 66, 215, 61, 156, 180
// };
//
// static float noise(Point3f p)
// {
//     return noise(p[0], p[1], p[2]);
// }
//
// static float noise(float x, float y, float z)
// {
//     int xIndex = floor(x);
//     int yIndex = floor(y);
//     int zIndex = floor(z);
//
//     float dx = x - xIndex;
//     float dy = y - yIndex;
//     float dz = z - zIndex;
//
//     xIndex = xIndex & NoiseTableSize - 1;
//     yIndex = yIndex & NoiseTableSize - 1;
//     zIndex = zIndex & NoiseTableSize - 1;
//
//     int xip1 = xIndex + 1;
//     int yip1 = yIndex + 1;
//     int zip1 = zIndex + 1;
//
//     float dxm1 = dx - 1;
//     float dym1 = dy - 1;
//     float dzm1 = dz - 1;
//
//     float g100 = gradient(xip1, yIndex, zIndex, dxm1, dy, dz);
//     float g010 = gradient(xIndex, yip1, zIndex, dx, dym1, dz);
//     float g001 = gradient(xIndex, yIndex, zip1, dx, dy, dym1);
//     float g110 = gradient(xip1, yip1, zIndex, dxm1, dym1, dz);
//     float g101 = gradient(xip1, yIndex, zip1, dxm1, dy, dzm1);
//     float g011 = gradient(xIndex, yip1, zip1, dx, dym1, dzm1);
//     float g111 = gradient(xip1, yip1, zip1, dxm1, dym1, dzm1);
//     float g000 = gradient(xIndex, yIndex, zIndex, dx, dy, dz);
//
//     float nwx = noiseW(dx);
//     float nwy = noiseW(dy);
//     float nwz = noiseW(dz);
//
//     // trilinear interpolation
//     float x00 = lerp(nwx, g000, g100);
//     float x10 = lerp(nwx, g010, g110);
//     float x01 = lerp(nwx, g001, g101);
//     float x11 = lerp(nwx, g011, g111);
//     float y0 = lerp(nwy, x00, x10);
//     float y1 = lerp(nwy, x01, x11);
//
//     return lerp(nwz, y0, y1);
// }
//
// static float gradient(int x,
//                       int y,
//                       int z,
//                       float dx,
//                       float dy,
//                       float dz)
// {
//     int val = NoiseTable[NoiseTable[NoiseTable[x] + y] + z];
//
//     val &= 15;
//
//     float u = val < 8 || val == 12 || val == 13 ? dx : dy;
//     float v = val < 4 || val == 12 || val == 13 ? dy : dz;
//     return ((val & 1) ? -u : u) + ((val & 2) ? -v : v);
// }
//
// static float lerp(float t, float min, float max)
// {
//     return (1.f - t) * min + t * max;
// }
//
// static float noiseW(float t)
// {
//     float one = t * t * t;
//     float two = t * t * t * t;
//     return 6 * two * t - 15 * two + 10 * one;
// }
//
// static float fbm(const Point3f& p, float octaves, float omega)
// {
//     float result = 0.f;
//     float lambda = 1.f;
//     float prod = 1.f;
//     int n = floor(octaves);
//
//     for (int i = 0; i < n; ++i) {
//         result += prod * Noise::noise(lambda * p);
//         lambda *= 1.99f;
//         prod *= omega;
//     }
//
//     float octavePart = octaves - n;
//
//     // hermite interpolation
//     float val = clamp((octavePart - 0.33f) / (0.67f - 0.33f), 0.f, 1.f);
//     val = val * val * (-2.f * val + 3.f);
//     result += prod * val * noise(lambda * p);
//
//     return result;
// }
