#include <fstream>
#include <stdexcept>
#include "MD5Animation.h"

using namespace std;
using namespace Math;

namespace ST
{
    MD5Animation::MD5Animation()
    {
    }

    MD5Animation::~MD5Animation()
    {
    }

    void MD5Animation::LoadAnimation(const string& fileName)
    {
        ifstream file(fileName);

        if (!file)
            throw runtime_error("Cannot locate file: " + fileName);

        int pos = file.tellg();
        file.seekg(0, std::ios::end);
        int fileLength = file.tellg();
        file.seekg(pos);

        if (fileLength <= 0)
            throw runtime_error("Malformed file: " + fileName);

        string param, junk;
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
            else if (param == "numFrames")
            {
                file >> numFrames;
                file.ignore(fileLength, '\n');
                frames.reserve(numFrames);
            }
            else if (param == "numJoints")
            {
                file >> numJoints;
                file.ignore(fileLength, '\n');
            }
            else if (param == "frameRate")
            {
                file >> frameRate;
                file.ignore(fileLength, '\n');
            }
            else if (param == "numAnimatedComponents")
            {
                file >> numAnimatedComponents;
                file.ignore(fileLength, '\n');
            }
            else if (param == "hierarchy")
            {
                jointInfos.reserve(numJoints);
                file >> junk; // Read the '{' character.
                for (int i = 0; i < numJoints; i++)
                {
                    JointInfo joint;
                    file >> joint.name >> joint.parentID >> joint.flags
                         >> joint.startIndex;
                    file.ignore(fileLength, '\n');

                    removeQuotes(joint.name);

                    jointInfos.push_back(joint);
                }
                file >> junk; // Read the '}' character.
                file.ignore(fileLength, '\n');
            }
            else if (param == "bounds")
            {
                file >> junk; // Read the '{' character.
                file.ignore(fileLength, '\n');
                bounds.reserve(numFrames);
                for (int i = 0; i < numFrames; i++)
                {
                    Bound bound;
                    file >> junk; // Read the '(' character.
                    file >> bound.min[0] >> bound.min[1] >> bound.min[2];
                    file >> junk >> junk; // Read the ')' and '(' characters.
                    file >> bound.max[0] >> bound.max[1] >> bound.max[2];
                    file.ignore(fileLength, '\n');

                    bounds.push_back(bound);
                }
                file >> junk; // Read the '}' character.
                file.ignore(fileLength, '\n');
            }
            else if (param == "baseframe")
            {
                file >> junk; // Read the '{' character.
                file.ignore(fileLength, '\n');
                baseFrame.reserve(numJoints);
                for (int i = 0; i < numJoints; i++)
                {
                    BaseFrameJoint joint;
                    file >> junk;
                    file >> joint.pos[0] >> joint.pos[1] >> joint.pos[2];
                    file >> junk >> junk;
                    file >> joint.orient.x >> joint.orient.y >> joint.orient.z;
                    file.ignore(fileLength, '\n');

                    baseFrame.push_back(joint);
                }
                file >> junk; // Read the '}' character.
                file.ignore(fileLength, '\n');
            }
            else if (param == "frame")
            {
                FrameData frame;
                file >> frame.frameID >> junk;
                file.ignore(fileLength, '\n');
                frame.data.reserve(numAnimatedComponents);
                for (int i = 0; i < numAnimatedComponents; i++)
                {
                    float frameDatum;
                    file >> frameDatum;
                    frame.data.push_back(frameDatum);
                }
                frames.push_back(frame);

                // Build a skeleton for this frame.
                buildFrameSkeleton(frame);

                file >> junk;
                file.ignore(fileLength, '\n');
            }

            file >> param;
        }

        // There will be no push_back() for animatedSkeleton,
        // so we need to assign something inside a vector.
        animatedSkeleton.assign(numJoints, SkeletonJoint());

        animTime = 0.0f;
        frameDuration = 1.0f / frameRate;
        animDuration  = frameDuration * numFrames;
    }

    void MD5Animation::removeQuotes(string& str)
    {
        size_t n;
        while ((n = str.find('\"')) != string::npos)
        {
            str.erase(n, 1);
        }
    }

    void MD5Animation::buildFrameSkeleton(const FrameData& frameData)
    {
        Skeleton skeleton;
        skeleton.reserve(numJoints);
        for (int i = 0; i < numJoints; i++) // Construct it joint by joint.
        {
            int j = 0;
            const JointInfo& jointInfo = jointInfos[i];
            SkeletonJoint animatedJoint = baseFrame[i];
            animatedJoint.parent = jointInfos[i].parentID;

            // Extract coordinates where joint "i" is placed in given frame.
            if (jointInfo.flags & 1)
            {
                animatedJoint.pos[0] = frameData.data[jointInfo.startIndex + j];
                j++;
            }
            if (jointInfo.flags & 2)
            {
                animatedJoint.pos[1] = frameData.data[jointInfo.startIndex + j];
                j++;
            }
            if (jointInfo.flags & 4)
            {
                animatedJoint.pos[2] = frameData.data[jointInfo.startIndex + j];
                j++;
            }
            if (jointInfo.flags & 8)
            {
                animatedJoint.orient.x = frameData.data[jointInfo.startIndex +
                                                        j];
                j++;
            }
            if (jointInfo.flags & 16)
            {
                animatedJoint.orient.y = frameData.data[jointInfo.startIndex +
                                                        j];
                j++;
            }
            if (jointInfo.flags & 32)
            {
                animatedJoint.orient.z = frameData.data[jointInfo.startIndex +
                                                        j];
                j++;
            }

            animatedJoint.orient.ComputeW();

            // Joints are given in such order that parent of joint "i"
            // has been already processed and is now in object coordinate space.
            if (animatedJoint.parent >= 0)
            {
                SkeletonJoint& parentJoint = skeleton[animatedJoint.parent];
                Vector3D rotPos = parentJoint.orient.Rotate(animatedJoint.pos);

                animatedJoint.pos = parentJoint.pos + rotPos;
                animatedJoint.orient = parentJoint.orient * animatedJoint.orient;
                animatedJoint.orient.Normalize();
            }

            skeleton.push_back(animatedJoint);
        }

        skeletons.push_back(skeleton);
    }

    void MD5Animation::Update(float deltaTimeSec)
    {
        if (numFrames < 1) return;

        animTime += deltaTimeSec;

        while (animTime > animDuration)
            animTime -= animDuration;
        while (animTime < 0.0f)
            animTime += animDuration;

        float frameNumber = animTime * frameRate;
        int frame0 = int(floor(frameNumber)) % numFrames;
        int frame1 = int(ceil(frameNumber)) % numFrames;

        float quotient = fmod(animTime, frameDuration) / frameDuration;
        interpolateSkeletons(frame0, frame1, quotient);
    }

    void MD5Animation::interpolateSkeletons(int frame0, int frame1,
                                            float quotient)
    {
        for (int i = 0; i < numJoints; i++)
        {
            SkeletonJoint& animatedJoint = animatedSkeleton[i];
            const SkeletonJoint& joint0 = skeletons[frame0][i];
            const SkeletonJoint& joint1 = skeletons[frame1][i];

            animatedJoint.pos = Vector3D::Lerp(joint0.pos, joint1.pos, quotient);
            animatedJoint.orient = Quaternion::Slerp(joint0.orient,
                                                     joint1.orient, quotient);
        }
    }
}

