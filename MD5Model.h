#ifndef MD5MODEL_H_INCLUDED
#define MD5MODEL_H_INCLUDED

#include "OpenGL.h"
#include "Shader.h"
#include "math/Matrix4D.h"
#include "math/Vector2D.h"
#include "math/Vector3D.h"
#include "math/Quaternion.h"
#include "MD5Animation.h"

namespace ST
{
    struct Joint
    {
        std::string name;
        int parentID;
        Math::Vector3D pos;
        Math::Quaternion orient;
    };
    typedef std::vector<Joint> JointList;

    class MD5Model
    {
    public:
        MD5Model();
        virtual ~MD5Model();

        void Load(const std::string& fileName, const Shader& shader);
        void LoadAnim(const std::string& fileName);
        void Draw( bool draw_skeleton );
        void Update(float deltaTimeSec);

        void AffectJoint();

        Joint& GetJoint(size_t i);
        JointList& GetSkeleton();
        const Math::Matrix4D& GetModelTrans() const;

    protected:
        typedef std::vector<Math::Vector3D> PositionBuffer;
        typedef std::vector<Math::Vector3D> NormalBuffer;
        typedef std::vector<Math::Vector2D> Tex2DBuffer;
        typedef std::vector<GLuint> IndexBuffer;

        struct Vertex
        {
            Math::Vector3D pos;
            Math::Vector3D normal;
            Math::Vector2D tex;
            int startWeight;
            int weightCount;
        };
        typedef std::vector<Vertex> VertexList;

        struct Weight
        {
            int jointID;
            float bias;
            Math::Vector3D pos;
        };
        typedef std::vector<Weight> WeightList;

        struct Mesh
        {
            // Name of the TGA file that is in the same
            // folder as the mesh being loaded.
            std::string shader;

            // Vertices in bind-pose.
            VertexList verts;
            WeightList weights;

            // TexID for material.
            GLuint texID;
            GLuint vbo[3];       // Buffer for vertex attributes and indices.
            GLuint vao;          // Vertex Array Object.

            // Buffers used for rendering.
            PositionBuffer positionBuffer;
            NormalBuffer   normalBuffer;
            Tex2DBuffer    tex2DBuffer;
            IndexBuffer    indexBuffer;
        };
        typedef std::vector<Mesh> MeshList;

    private:
        void removeQuotes(std::string& str);
        void prepareMesh(Mesh& mesh);
        void prepareMesh(Mesh& mesh, const MD5Animation::Skeleton& skeleton);
        void prepareNormals(Mesh& mesh);
        void loadModelInVideomemory();
        void reloadModel();
        bool checkAnimation(const MD5Animation& anim) const;
        void printJoints();

    private:
        const Shader*  shader;
        MeshList       meshes;       // Meshes that make up the whole.
        JointList      joints;       // Joints are the same for all meshes.
        Math::Matrix4D model;        // Model transformation.
        MD5Animation   animation;    // Single animation for the model.
        bool           hasAnimation;
        GLint          posLocation;  // Position location in shader.
        GLint          normLocation; // Position location in shader.
    };
}

#endif // MD5MODEL_H_INCLUDED
