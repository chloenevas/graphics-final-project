#pragma once

#include <vector>
#include "scenedata.h"

class LensFileReader {
public:
    LensFileReader(const std::string &filename);

    std::vector<LensInterface> getLensInterfaces();

    bool readLensFile();
private:
    std::string m_filename;
    std::vector<LensInterface> m_lensInterfaces;
};
