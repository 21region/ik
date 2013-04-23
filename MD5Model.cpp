#include <stdexcept>
#include "Log.h"
#include "MD5Model.h"
#include "math/Utility.h"
#include <iostream>

using namespace std;
using namespace Math;

namespace ST
{
    MD5Model::MD5Model() : hasAnimation(false)
    {
    }

    MD5Model::~MD5Model()
    {
        for (size_t i = 0; i < meshes.size(); i++)
        {
            const Mesh& mesh = meshes[i];
            glDeleteBuffers(3, mesh.vbo);
            glDeleteVertexArrays(1, &mesh.vao);
        }
    }

    MD5Model::Joint& MD5Model::GetJoint(size_t i)
    {
        return joints[i];
    }

    MD5Model::JointList& MD5Model::GetSkeleton()
    {
        return joints;
    }

    void MD5Model::Load(const string& fileName, const Shader& shader)
    {
        ifstream file(fileName);

        if (!file)
            throw runtime_error("Cannot locate file: " + fileName);

        int pos = file.tellg();
        file.seekg(0, std::ios::end );
        int fileLength = file.tellg();
        file.seekg(pos);

        if (fileLength <= 0)
            throw runtime_error("Malformed file: " + fileName);

        string junk;
        string param;

        joints.clear();
        meshes.clear();

        file >> param;

        while (file.good())
        {
            if (param == "MD5Version")
            {
                int MD5Version;
                file >> MD5Version;
                if (MD5Version != 10)
                    throw runtime_error("Incompatible version: " + MD5Version);
            }
            else if (param == "commandline")
            {
                file.ignore(fileLength, '\n');
            }
            else if (param == "numJoints")
            {
                int numJoints;
                file >> numJoints;
                joints.resize(numJoints);
            }
            else if (param == "numMeshes")
            {
                int numMeshes;
                file >> numMeshes;
                meshes.reserve(numMeshes);
            }
            else if (param == "joints")
            {
                Joint joint;
                file >> junk; // Read the '{' character.
                for (size_t i = 0; i < joints.size(); i++)
                {
                    file >> joint.name >> joint.parentID >> junk
                         >> joint.pos[0] >> joint.pos[1] >> joint.pos[2]
                         >> junk >> junk
                         >> joint.orient.x >> joint.orient.y >> joint.orient.z
                         >> junk;

                    removeQuotes(joint.name);
                    joint.orient.ComputeW();

                    joints[i] = joint;
                    file.ignore(fileLength, '\n'); // Ignore comments.
                }
                file >> junk; // Read the '}' character.
            }
            else if (param == "mesh")
            {
                Mesh mesh;

                file >> junk; // Read the '{' character.
                file >> param; // Must be 'shader'
                while (param != "}")
                {
                    if (param == "shader")
                    {
                        file >> mesh.shader;
                        removeQuotes(mesh.shader);

                        // Texture file "mesh.shader" should be loaded here.

                        file.ignore(fileLength, '\n'); // Ignore comments.
                    }
                    else if (param == "numverts")
                    {
                        int numVerts;
                        file >> numVerts;
                        mesh.verts.reserve(numVerts);
                        mesh.tex2DBuffer.reserve(numVerts);
                        mesh.positionBuffer.reserve(numVerts);
                        mesh.normalBuffer.reserve(numVerts);

                        file.ignore(fileLength, '\n'); // Ignore comments.

                        for (int i = 0; i < numVerts; i++)
                        {
                            Vertex vert;

                            file >> junk >> junk >> junk // vert vertIndex (
                                 >> vert.tex[0] >> vert.tex[1] >> junk // s t )
                                 >> vert.startWeight >> vert.weightCount;

                            file.ignore(fileLength, '\n'); // Ignore comments.

                            mesh.verts.push_back(vert);
                            mesh.tex2DBuffer.push_back(vert.tex);
                        }
                    }
                    else if (param == "numtris")
                    {
                        int numTris;
                        file >> numTris;
                        mesh.indexBuffer.reserve(3 * numTris);

                        file.ignore(fileLength, '\n'); // Ignore comments.

                        size_t ind[3];
                        for (int i = 0; i < numTris; i++)
                        {
                            file >> junk >> junk >> ind[0] >> ind[1] >> ind[2];
                            file.ignore(fileLength, '\n'); // Ignore comments.

                            mesh.indexBuffer.push_back(ind[0]);
                            mesh.indexBuffer.push_back(ind[1]);
                            mesh.indexBuffer.push_back(ind[2]);
                        }
                    }
                    else if (param == "numweights")
                    {
                        int numWeights;
                        file >> numWeights;
                        mesh.weights.reserve(numWeights);

                        file.ignore(fileLength, '\n'); // Ignore comments.

                        for (int i = 0; i < numWeights; i++)
                        {
                            Weight weight;

                            file >> junk >> junk >> weight.jointID
                                 >> weight.bias >> junk >> weight.pos[0]
                                 >> weight.pos[1] >> weight.pos[2];

                            file.ignore(fileLength, '\n'); // Ignore comments.

                            mesh.weights.push_back(weight);
                        }
                    }
                    else
                    {
                        file.ignore(fileLength, '\n'); // Ignore comments.
                    }

                    file >> param;
                }

                prepareMesh(mesh);
                prepareNormals(mesh);
                meshes.push_back(mesh);
            }

            file >> param;
        }
        printJoints();

        // Somewhere here we should know model orientation.
        // Place the model somewhere in the world.
        model = Matrix4D::MakeTranslate(0, -50, -150) *
             // Matrix4D::MakeRotY(-3.14159f / 2) *
              Matrix4D::MakeRotX(-PI / 2);

        // We can load in memory only after model was positioned.
        this->shader = &shader;
        loadModelInVideomemory();
    }

    void MD5Model::removeQuotes(string& str)
    {
        size_t n;
        while ((n = str.find('\"')) != string::npos)
        {
            str.erase(n, 1);
        }
    }

    void MD5Model::LoadAnim(const std::string& fileName)
    {
        MD5Animation tempAnim;
        tempAnim.LoadAnimation(fileName);
        if (!checkAnimation(tempAnim))
            throw runtime_error("File contains wrong animation: " + fileName);

        animation = tempAnim;
        hasAnimation = true;
    }

    bool MD5Model::checkAnimation(const MD5Animation& anim) const
    {
        if (joints.size() != (size_t)anim.GetNumJoints())
        {
            return false;
        }

        for (size_t i = 0; i < joints.size(); i++)
        {
            const Joint& meshJoint = joints[i];
            const MD5Animation::JointInfo& animJoint = anim.GetJointInfo(i);

            if (meshJoint.name != animJoint.name ||
                meshJoint.parentID != animJoint.parentID)
            {
                return false;
            }
        }

        return true;
    }

    void MD5Model::prepareMesh(Mesh& mesh)
    {
        mesh.positionBuffer.clear();

        // Compute vertex positions.
        for (size_t i = 0; i < mesh.verts.size(); i++)
        {
            Vector3D finalVertex;
            const Vertex& vert = mesh.verts[i];
            for (int j = 0; j < vert.weightCount; j++)
            {
                const Weight& weight = mesh.weights[vert.startWeight + j];
                const Joint& joint = joints[weight.jointID];

                // Convert position from Joint local space to object space.
                Vector3D rotPos = joint.orient.Rotate(weight.pos);
                finalVertex += (joint.pos + rotPos) * weight.bias;
            }

            mesh.positionBuffer.push_back(finalVertex);
        }
    }

    void MD5Model::prepareNormals(Mesh& mesh)
    {
        mesh.normalBuffer.clear();

        for (size_t i = 0; i < mesh.indexBuffer.size(); i += 3)
        {
            const Vector3D& v0 = mesh.positionBuffer[mesh.indexBuffer[i + 0]];
            const Vector3D& v1 = mesh.positionBuffer[mesh.indexBuffer[i + 1]];
            const Vector3D& v2 = mesh.positionBuffer[mesh.indexBuffer[i + 2]];

            Vector3D normal = Vector3D::Cross(v2 - v0, v1 - v0);

            // Note: normals are all 0's at the beginning.
            mesh.verts[mesh.indexBuffer[i + 0]].normal += normal;
            mesh.verts[mesh.indexBuffer[i + 1]].normal += normal;
            mesh.verts[mesh.indexBuffer[i + 2]].normal += normal;
        }

        for (size_t i = 0; i < mesh.verts.size(); i++)
        {
            Vertex& vert = mesh.verts[i];

            Vector3D normal = Vector3D::Normalize(vert.normal);
            mesh.normalBuffer.push_back(normal);

            // Reset the normal to calculate the bind-pose in joint space.
            vert.normal = Vector3D(0);

            // Put the bind-pose normal into joint-local space
            // so the animated normal can be computed faster later.
            for (int j = 0; j < vert.weightCount; j++)
            {
                const Weight& weight = mesh.weights[vert.startWeight + j];
                const Joint& joint = joints[weight.jointID];
                vert.normal += joint.orient.InverseRotate(normal) * weight.bias;
            }
        }
    }

    void MD5Model::prepareMesh(Mesh& mesh, const MD5Animation::Skeleton& skel)
    {
        for (size_t i = 0; i < mesh.verts.size(); i++)
        {
            const Vertex& vert = mesh.verts[i];
            Vector3D& pos = mesh.positionBuffer[i];
            Vector3D& normal = mesh.normalBuffer[i];

            pos = Vector3D(0);
            normal = Vector3D(0);

            for (int j = 0; j < vert.weightCount; j++)
            {
                const Weight& weight = mesh.weights[vert.startWeight + j];
                const MD5Animation::SkeletonJoint& joint = skel[weight.jointID];

                Vector3D rotPos = joint.orient.Rotate(weight.pos);
                pos += (joint.pos + rotPos) * weight.bias;

                normal += (joint.orient.Rotate(vert.normal)) * weight.bias;
            }
        }
    }

    void MD5Model::Update(float deltaTimeSec)
    {
        if (hasAnimation)
        {
            animation.Update(deltaTimeSec);
            const MD5Animation::Skeleton& skeleton = animation.GetSkeleton();
            for (size_t i = 0; i < meshes.size(); i++)
            {
                prepareMesh(meshes[i], skeleton);
            }
            reloadModel();
        }
    }

    void MD5Model::loadModelInVideomemory()
    {
        Mesh skeleton;
        for ( size_t i = joints.size() - 1, j = 0; i > 0; i-- )
        {
            const Joint& cur = joints[i];
            const Joint& parent = joints[cur.parentID];
            skeleton.positionBuffer.push_back(parent.pos - Vector3D(1, 0, 0));
            skeleton.positionBuffer.push_back(parent.pos + Vector3D(1, 0, 0));
            skeleton.positionBuffer.push_back(cur.pos);
            skeleton.indexBuffer.push_back(j);
            skeleton.indexBuffer.push_back(j + 1);
            skeleton.indexBuffer.push_back(j + 2);
            j += 3;
        }
        skeleton.normalBuffer.push_back( Vector3D(0, 0, 0) );
        meshes.push_back( skeleton );

        MeshList::iterator mesh = meshes.begin();
        while (mesh != meshes.end())
        {
            // Generate buffers.
            glGenBuffers(3, mesh->vbo);
            glGenVertexArrays(1, &mesh->vao);

            // Load mesh vertex attributes in videomemory.
            glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, mesh->positionBuffer.size() *
                sizeof(Vector3D), &mesh->positionBuffer[0], GL_STREAM_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, mesh->normalBuffer.size() *
                sizeof(Vector3D), &mesh->normalBuffer[0], GL_STREAM_DRAW);

            // Load mesh indices in videomemory.
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[2]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer.size() *
                sizeof(GLuint), &mesh->indexBuffer[0], GL_STATIC_DRAW);

            posLocation = shader->GetAttribLocation("position");
            normLocation = shader->GetAttribLocation("normal");

            // Let OpenGL know layout of the vertices in memory
            // and bind these data to a variable in shader.
            glBindVertexArray(mesh->vao);
            glEnableVertexAttribArray(posLocation);
            glEnableVertexAttribArray(normLocation);
            glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);
            glVertexAttribPointer(
                posLocation, 3, GL_FLOAT, 0, sizeof(Vector3D), 0);
            glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[1]);
            glVertexAttribPointer(
                normLocation, 3, GL_FLOAT, 0, sizeof(Vector3D), 0);

            ++mesh;
        }

        // Load correct position of the model.
        GLint modelLocation = shader->GetUniformLocation("model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0]);
    }

    void MD5Model::reloadModel()
    {
        for (size_t i = 0; i < meshes.size(); i++)
        {
            const Mesh& mesh = meshes[i];
            // Load mesh vertex attributes in videomemory.
            glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.positionBuffer.size() *
                sizeof(Vector3D), &mesh.positionBuffer[0]);
            glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[1]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.normalBuffer.size() *
                sizeof(Vector3D), &mesh.normalBuffer[0]);
        }
    }

    void MD5Model::Draw(bool draw_skeleton)
    {
        shader->SetUniformBool("has_light", true);

        MeshList::iterator mesh = meshes.begin();
        while (mesh != (meshes.end() - 1))
        {
            glBindVertexArray(mesh->vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vbo[2]);
            glDrawElements(GL_TRIANGLES, mesh->indexBuffer.size(),
                       GL_UNSIGNED_INT, 0);
            ++mesh;
        }

        if (draw_skeleton)
        {
            const Mesh& skeleton = meshes.back();
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            shader->SetUniformBool("has_light", false);

            glBindVertexArray(skeleton.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skeleton.vbo[2]);
            glDrawElements(GL_TRIANGLES, skeleton.indexBuffer.size(),
                GL_UNSIGNED_INT, 0);

            glEnable(GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);
        }
    }

    const Matrix4D& MD5Model::GetModelTrans() const
    {
        return model;
    }

    void MD5Model::AffectJoint()
    {
        // Написать конвертор q -> mat и посмотреть, что происходит.
        joints[7].orient = Quaternion( 3.14159 / 4, Vector3D(0, 0, 1) ) * joints[7].orient;
        for (size_t i = 0; i < meshes.size(); i++)
        {
            prepareMesh(meshes[i]);
            prepareNormals(meshes[i]);
        }

        reloadModel();
    }

    void MD5Model::printJoints()
    {
        Vector4D v;
        JointList::iterator it = joints.begin();
        while ( it != joints.end() )
        {
            v[0] = it->pos[0];
            v[1] = it->pos[1];
            v[2] = it->pos[2];
            v[3] = 1.0f;
            cout << it->name << ": " << model * v << "\n";
            ++it;
        }
    }
}
