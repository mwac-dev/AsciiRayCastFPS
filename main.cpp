#include <cmath>
#include <iostream>
#include <Windows.h>
int nScreenWidth = 120;
int nScreenHeight = 40;

float fPosX = 0.0f;
float fPosY = 0.0f;
float fAngle = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159f / 4.0f;
float fDepth = 16.0f;
int main() {
    // creating screen buffer here
    auto screen = new wchar_t[nScreenWidth*nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    std::wstring map;
    map += L"################";
    map += L"#........#.....#";
    map += L"#........#.....#";
    map += L"####..####.....#";
    map += L"#........#.....#";
    map += L"#..............#";
    map += L"#........###..##";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#.....##########";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";


    while (true) {
        for (int x=0; x < nScreenWidth; x++) {
            float fRayAngle = (fAngle - fFOV /2.0f) + ((float)x/(float)nScreenWidth) * fFOV;

            float fDistanceToWall = 0;
            bool bHitWall = false;

            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            while (!bHitWall && fDistanceToWall < fDepth) {
                fDistanceToWall += 0.1f;

                int nTestX = (int)(fPosX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPosY + fEyeY * fDistanceToWall);

                //out of bounds check
                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                }
                else {
                    // ray is in bounds so do a map check
                    if (map[nTestY*nMapWidth + nTestX] == '#') {
                        bHitWall = true;
                    }
                }
            }

            // ceiling and floor
            int nCeiling = (float)(nScreenHeight /2.0) - nScreenHeight / (float)fDistanceToWall;
            int nFloor = nScreenHeight - nCeiling;

            for (int y=0; y<nScreenHeight; y++) {
                if (y < nCeiling) {
                    screen[y * nScreenWidth + x] = ' ';
                }
                else if(y > nCeiling && y <= nFloor)
                {
                    screen[y * nScreenWidth + x] = '#';
                }
                else {
                    screen[y * nScreenWidth + x] = ' ';
                }
            }
        }

            screen[nScreenWidth * nScreenHeight - 1] = L'A';
        WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
    }


    return 0;
}