#include "lensfilereader.h"
#include <vector>
#include "scenedata.h"
#include <ctype.h>
#include <stdlib.h>

LensFileReader::LensFileReader(const std::string &filename) {
    m_filename = filename;
    m_lensInterfaces = std::vector<LensInterface>{};
}

std::vector<LensInterface> LensFileReader::getLensInterfaces() {
    return m_lensInterfaces;
}

bool LensFileReader::readLensFile() {

    if (m_filename == "") {
        return false;
    }
    std::vector<float> lensData;


    // Lens file reading code borrowed from pbrt-v3 on GitHub with some modifications
    FILE *file = fopen(m_filename.c_str(), "r");
    if (!file) {
        return false;
    }

    int c;
    bool inNumber = false;
    char curNumber[32];
    int curNumberPos = 0;
    int lineNumber = 1;
    while ((c = getc(file)) != EOF) {
        if (c == '\n') ++lineNumber;
        if (inNumber) {
            if (curNumberPos > (int)sizeof(curNumber)) {
                return false;
            }
            if (isdigit(c) || c == '.' || c == 'e' || c == '-' || c == '+')
                curNumber[curNumberPos++] = c;
            else {
                curNumber[curNumberPos++] = '\0';
                lensData.push_back(atof(curNumber));
                inNumber = false;
                curNumberPos = 0;
            }
        } else {
            if (isdigit(c) || c == '.' || c == '-' || c == '+') {
                inNumber = true;
                curNumber[curNumberPos++] = c;
            } else if (c == '#') {
                while ((c = getc(file)) != '\n' && c != EOF)
                    ;
                ++lineNumber;
            } else if (!isspace(c)) {
                return false;
            }
        }
    }
    fclose(file);

    if (lensData.size() % 4 != 0) {
        return false;

    }

    for (int i = 0; i < (int)lensData.size(); i += 4) {
        m_lensInterfaces.push_back(LensInterface
            {lensData[i] * 0.001f, lensData[i + 1] * 0.001f,
             lensData[i + 2], lensData[i + 3] * 0.001f / 2.0f});
    }

    return true;
}
