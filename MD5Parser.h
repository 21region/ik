#ifndef MD5PARSER_H
#define MD5PARSER_H

#include <vector>
#include <string>
#include <functional>
#include "math/Vector3D.h"
#include "math/Quaternion.h"

namespace ST
{
    /** Joint is a part of the whole skeleton.
        Joint can be thought of as the coordinate system,
        where we define positions of the vertices.
    */
    struct Joint
    {
        int parent;         //!< Index of the parent joint.
        std::string name;   //!< Usually animators give them names.
        Math::Vector3D p;   //!< Relative or absolute position of the joint.
        Math::Quaternion q; //!< Relative or absolute orientation of the joint.
    };

    /** Vertex is a constituent part of the mesh.
        Vertex position relies on 'weight'.
    */
    struct Vertex
    {
        float s, t; //!< Texture coordinates of the vertex (UV-coordinates).
        size_t weight_index,
               weight_count; //!< Weights that define coords for this vertex.
        Math::Vector3D normal; //!< Normal in joint local space.
    };

    /** Weight makes contribution to vertices position and orientation.
        Bias of the weight is in the range (0; 1].
    */
    struct Weight
    {
        size_t joint;     //!< Index of the joint this weight depends on.
        float bias;       //!< Contribution of the weight to vertices position.
        Math::Vector3D p; //!< Wight's position in space.
    };

    /** Mesh is what actually seen by the user.
        Mesh can be thought of as the skin (or meat).
    */
    struct Mesh
    {
        void Clear() { vertices.clear(); triangles.clear(); weights.clear(); }
        std::string texture_path;      //!< Path to the .tga texture file.
        std::vector<Vertex> vertices;  //!< Skin of the mesh.
        std::vector<size_t> triangles; //!< Define order of the vertices.
        std::vector<Weight> weights;   //!< Weights for vertices.
    };

    /** Parser that reads *.md5mesh, *.md5anim files and fills its own structure.
        First type of files define a skeleton (joints) and a flesh (mesh).
    */
    class MD5Parser
    {
    public:
        MD5Parser();

        void Parse(std::string filename);
        const std::vector<Mesh>& GetSkin() const;
        const std::vector<Joint>& GetSkeleton() const;

    private:
        void parseModel();
        bool starts_with(std::string fullstring, std::string start) const;
        bool ends_with(std::string fullstring, std::string start) const;
        std::vector<std::string> tokenize(std::string line) const;
        void process_start(const std::vector<std::string>& tokens);
        void process_joints(const std::vector<std::string>& tokens);
        void process_mesh(const std::vector<std::string>& tokens);
        void process_unknown(const std::vector<std::string>& tokens);
        void log_tokens(std::string, const std::vector<std::string>&);

    private:
        static const size_t START = 0;
        static const size_t JOINTS = 1;
        static const size_t MESH = 2;
        static const size_t UNKNOWN = 4;

        static const std::string skip_strings[];
        static const std::function
        <
            void(MD5Parser&, const std::vector<std::string>&)
        > state_parsers[];

        Mesh mesh; //!< Mesh, that is currently filled by parser.
        size_t state; //!< State of the parser.
        size_t processed_line; //!< Line number that is being processed.
        std::vector<Mesh> skin; //!< Skin of the model is composed of meshes.
        std::vector<Joint> skeleton; //!< Skeleton of the model.
    };
}

#endif // MD5PARSER_H
