#include "Model.h"
#include <iostream>

using namespace std;
using namespace Math;

namespace ST
{
    void Model::LoadModel(string filename)
    {
        parser.Parse(filename);
        skeleton = parser.GetSkeleton();

        size_t weight_shift = 0;
        size_t vertex_shift = 0;
        for (const Mesh& mesh : parser.GetSkin())
        {
            for (size_t index : mesh.triangles)
            {
                index += vertex_shift;
                indices.push_back(index);
            }
            vertex_shift += mesh.vertices.size();
            for (Vertex vertex : mesh.vertices)
            {
                vertex.weight_index += weight_shift;
                vertices.push_back(vertex);
            }
            weights.insert(weights.end(),
                           mesh.weights.begin(), mesh.weights.end());
            weight_shift += mesh.weights.size();
        }

        construct_positions();
        construct_normals();
    }

    void Model::construct_positions()
    {
        for (const Vertex& vertex : vertices)
        {
            Vector3D final_vertex;
            for (size_t i = 0; i < vertex.weight_count; i++)
            {
                const Weight& weight = weights[vertex.weight_index + i];
                const Joint& joint = skeleton[weight.joint];

                // Convert position from Joint local space to object space.
                Vector3D rot_position = joint.q.Rotate(weight.p);
                final_vertex += (joint.p + rot_position) * weight.bias;
            }
            positions.push_back(final_vertex);
        }
    }

    void Model::construct_normals()
    {
        for (size_t i = 0; i < indices.size(); i += 3)
        {
            const Vector3D& v0 = positions[indices[i + 0]];
            const Vector3D& v1 = positions[indices[i + 1]];
            const Vector3D& v2 = positions[indices[i + 2]];

            Vector3D normal = Vector3D::Cross(v2 - v0, v1 - v0);

            vertices[indices[i + 0]].normal += normal;
            vertices[indices[i + 1]].normal += normal;
            vertices[indices[i + 2]].normal += normal;
        }
        for (Vertex& vertex : vertices)
        {
            Vector3D normal = Vector3D::Normalize(vertex.normal);
            normals.push_back(normal);
            vertex.normal = Vector3D(0);

            for (size_t i = 0; i < vertex.weight_count; i++)
            {
                const Weight& weight = weights[vertex.weight_index + i];
                const Joint& joint = skeleton[weight.joint];
                vertex.normal += joint.q.InverseRotate(normal) * weight.bias;
            }
        }
    }

    const vector<size_t>& Model::GetIndices() const
    {
        return indices;
    }

    const vector<Vector3D>& Model::GetNormals() const
    {
        return normals;
    }

    const vector<Vector3D>& Model::GetPositions() const
    {
        return positions;
    }
}
