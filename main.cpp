#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>
int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerPosX = 5.0f;
float fPlayerPosY = 5.0f;
float fPlayerAngle = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159f / 4.0f;
float fDepth = 16.0f;
float fRotateSpeed = 1.5f;
float fMoveSpeed = 2.5f;


char lookingLeft = '>';
char lookingRight = '<';
char lookingUp = '^';
char lookingDown = 'v';
;


float lookThreshold = 0.75f;


int main() {
    // creating screen buffer here
    auto screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER,
                                                nullptr);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    std::wstring map;
    map += L"################";
    map += L"#.......#......#";
    map += L"#.......#......#";
    map += L"#....#######...#";
    map += L"#..............#";
    map += L"#.......#......#";
    map += L"#.......#......#";
    map += L"#.......#..v...#";
    map += L"#.......#......#";
    map += L"###...#####..###";
    map += L"#.......#......#";
    map += L"#..............#";
    map += L"#.....#####....#";
    map += L"#.....#...#....#";
    map += L"#..............#";
    map += L"################";

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();

    // we are going to use the > < ^ v characters to represent the direction the player is looking and the initial position on spawn on the map
    //scan the map for any of these characters and set the player position and angle accordingly

    for (int y = 0; y < nMapHeight; y++) {
        for (int x = 0; x < nMapWidth; x++) {
            if (map[y * nMapWidth + x] == lookingLeft) {
                fPlayerPosX = (float) x;
                fPlayerPosY = (float) y;
                fPlayerAngle = 3.14159f;
                map[y * nMapWidth + x] = '.';
            } else if (map[y * nMapWidth + x] == lookingRight) {
                fPlayerPosX = (float) x;
                fPlayerPosY = (float) y;
                fPlayerAngle = 0.0f;
                map[y * nMapWidth + x] = '.';
            } else if (map[y * nMapWidth + x] == lookingUp) {
                fPlayerPosX = (float) x;
                fPlayerPosY = (float) y;
                fPlayerAngle = 0.5f * 3.14159f;
                map[y * nMapWidth + x] = '.';
            } else if (map[y * nMapWidth + x] == lookingDown) {
                fPlayerPosX = (float) x;
                fPlayerPosY = (float) y;
                fPlayerAngle = 3.14159f;
                map[y * nMapWidth + x] = '.';
            }
        }
    }


    while (true) {
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fDeltaTime = elapsedTime.count();


        //displaying map

        if (GetAsyncKeyState((unsigned short) 'A') & 0x8000) {
            fPlayerAngle -= fRotateSpeed * fDeltaTime;
        }
        if (GetAsyncKeyState((unsigned short) 'D') & 0x8000) {
            fPlayerAngle += fRotateSpeed * fDeltaTime;
        }
        if (GetAsyncKeyState((unsigned short) 'W') & 0x8000) {
            float newX = fPlayerPosX + sinf(fPlayerAngle) * fMoveSpeed * fDeltaTime;
            float newY = fPlayerPosY + cosf(fPlayerAngle) * fMoveSpeed * fDeltaTime;
            if (map[(int) newY * nMapWidth + (int) newX] != '#') {
                fPlayerPosX = newX;
                fPlayerPosY = newY;
            }
        }
        if (GetAsyncKeyState((unsigned short) 'S') & 0x8000) {
            float newX = fPlayerPosX - sinf(fPlayerAngle) * fMoveSpeed * fDeltaTime;
            float newY = fPlayerPosY - cosf(fPlayerAngle) * fMoveSpeed * fDeltaTime;
            if (map[(int) newY * nMapWidth + (int) newX] != '#') {
                fPlayerPosX = newX;
                fPlayerPosY = newY;
            }
        }

        for (int x = 0; x < nScreenWidth; x++) {

            //----debug stuff start--------------
            //map
            for (int x = 0; x < nMapWidth; x++) {
                for (int y = 0; y < nMapHeight; y++) {

                    if (int(fPlayerPosX) == x && int(fPlayerPosY) == y) {
                        wchar_t lookChar;
                        float angle = fPlayerAngle;
                        // Normalize angle to [0, 2π]
                        while (angle < 0) angle += 2 * 3.14159f;
                        while (angle > 2 * 3.14159f) angle -= 2 * 3.14159f;

                        if (abs(angle - 0.0f) < lookThreshold || abs(angle - 2 * 3.14159f) < lookThreshold)
                            lookChar = lookingDown;
                        else if (abs(angle - 3.14159f) < lookThreshold)
                            lookChar = lookingUp;
                        else if (abs(angle - 0.5f * 3.14159f) < lookThreshold)
                            lookChar = lookingLeft;
                        else if (abs(angle - 1.5f * 3.14159f) < lookThreshold)
                            lookChar = lookingRight;
                        else
                            lookChar = '+';

                        screen[(y + 1) * nScreenWidth + x] = lookChar;
                    } else {
                        screen[(y + 1) * nScreenWidth + x] = map[y * nMapWidth + x];
                    }
                }
            }

            //fps
            swprintf_s(screen, nScreenWidth * nScreenHeight, L"FPS: %f", 1.0f / fDeltaTime);
            //----debug stuff end--------------

            float fRayAngle = (fPlayerAngle - fFOV / 2.0f) + ((float) x / (float) nScreenWidth) * fFOV;

            float fDistanceToWall = 0;

            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);


            bool bHitWall = false;
            bool bHitBoundary = false;
            while (!bHitWall && fDistanceToWall < fDepth) {
                fDistanceToWall += 0.1f;

                int nTestX = (int) (fPlayerPosX + fEyeX * fDistanceToWall);
                int nTestY = (int) (fPlayerPosY + fEyeY * fDistanceToWall);

                //out of bounds check
                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                } else {
                    // ray is in bounds so do a map check
                    if (map[nTestY * nMapWidth + nTestX] == '#') {
                        bHitWall = true;

                        //the plan is to return a ray from the corner of a wall if it's
                        //close enough to the original ray from the player,
                        //then we are looking at a boundary

                        std::vector<std::pair<float,float>> p; //distance and dot product (helpful to avoid counting the back 'culled' corners of a wall)
                        for (int cornerX = 0; cornerX < 2; cornerX ++) {
                            for (int cornerY = 0; cornerY <2; cornerY++) {
                                float vectorY = (float) nTestY + cornerY - fPlayerPosY;
                                float vectorX = (float) nTestX + cornerX - fPlayerPosX;
                                float d = sqrtf(vectorX * vectorX + vectorY * vectorY);
                                float dot = (fEyeX * vectorX / d) + (fEyeY * vectorY / d);
                                p.emplace_back(d, dot);

                                sort( p.begin(), p.end(), [](const std::pair<float,float>& left, const std::pair<float,float>& right) {
                                    return left.first < right.first;
                                });

                                // trying to alter the thickness of the boundary depending on distance
                                // if the wall is further away, then the boundary should be thinner (smaller number)
                                // otherwise it should be thicker (larger number)
                                float fBound;
                                if (fDistanceToWall < fDepth / 4.0f) {
                                    fBound = 0.01f;
                                } else if (fDistanceToWall < fDepth / 2.0f) {
                                    fBound = 0.005f;
                                } else if (fDistanceToWall < fDepth) {
                                    fBound = 0.002f;
                                } else {
                                    fBound = 0.001f;
                                }

                                if (p.capacity() > 2) {

                                    if (acos(p.at(0).second)<fBound) bHitBoundary = true;
                                    if (acos(p.at(1).second)<fBound) bHitBoundary = true;
                                    // if (acos(p.at(2).second)<fBound) bHitBoundary = true;
                                }
                            }
                        }

                    }
                }
            }

            // ceiling and floor
            int nCeiling = (float) (nScreenHeight / 2.0) - nScreenHeight / (float) fDistanceToWall;
            int nFloor = nScreenHeight - nCeiling;

            short nWallVisual = ' ';
            wchar_t nFloorVisual = L' ';

            if (fDistanceToWall <= fDepth /4.0f) {
                nWallVisual = 0x2588; // 0x2588 is a solid block
            } else if (fDistanceToWall < fDepth / 3.0f) {
                nWallVisual = 0x2593; // dark shade
            } else if (fDistanceToWall < fDepth / 2.0f) {
                nWallVisual = 0x2592; // medium shade
            } else if (fDistanceToWall < fDepth) {
                nWallVisual = 0x2591; // light shade
            } else {
                nWallVisual = ' ';
            }

            if (bHitBoundary == true) {
                nWallVisual = ' ';
            }

            for (int y = 0; y < nScreenHeight; y++) {
                if (y < nCeiling) {
                    screen[y * nScreenWidth + x] = ' ';
                } else if (y > nCeiling && y <= nFloor) {
                    screen[y * nScreenWidth + x] = nWallVisual;
                } else {
                    //shading floor same way we shaded the walls
                    float b = 1.0f - ((float) (y - nScreenHeight / 2.0) / (float) (nScreenHeight / 2.0f));
                    if (b < 0.25) {
                        nFloorVisual = '#'; // light shade
                    } else if (b < 0.5) {
                        nFloorVisual = 'x'; // medium shade
                    } else if (b < 0.75) {
                        nFloorVisual = '-'; // dark shade
                    } else if (b < 0.9) {
                        nFloorVisual = '.'; // dark shade
                    } else {
                        nFloorVisual = ' ';
                    }
                    screen[y * nScreenWidth + x] = nFloorVisual;
                }
            }
        }

        screen[nScreenWidth * nScreenHeight - 1] = L'A';
        WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
    }


    return 0;
}
