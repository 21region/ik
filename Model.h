#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include "MD5Parser.h"
#include "math/Vector3D.h"

namespace ST
{
    class Model
    {
    public:
        void LoadModel(std::string filename);
        const std::vector<size_t>& GetIndices() const;
        const std::vector<Math::Vector3D>& GetNormals() const;
        const std::vector<Math::Vector3D>& GetPositions() const;

    private:
        void construct_normals();
        void construct_positions();

    private:
        MD5Parser parser;

        std::vector<size_t> indices;
        std::vector<Weight> weights;
        std::vector<Vertex> vertices;
        std::vector<Math::Vector3D> normals;
        std::vector<Math::Vector3D> positions;

        std::vector<Joint> skeleton;
    };
}

#endif // MODEL_H
