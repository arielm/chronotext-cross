#pragma once

#include "chr/gl/Batch.h"
#include "chr/FileSystem.h"

class BatchImporter
{
public:
    static std::vector<chr::gl::IndexedVertexBatch<chr::gl::XYZ.N.UV>> read(const chr::InputSource &inputSource);
};
