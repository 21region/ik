#ifndef MD5ANIMATION_H_INCLUDED
#define MD5ANIMATION_H_INCLUDED

#include <vector>
#include "math/Vector3D.h"
#include "math/Quaternion.h"

namespace ST
{
    /** The class is responsible for parsing .md5anim file
        and animating of the skeleton.
    */
    class MD5Animation
    {
    public:
        MD5Animation();
        virtual ~MD5Animation();

        void LoadAnimation(const std::string& fileName);
        void Update(float deltaTimeSec);

        /** Stores info neccesery to build skeletons for each frame. */
        struct JointInfo
        {
            std::string name;
            int parentID;
            int flags;
            int startIndex;
        };
        typedef std::vector<JointInfo> JointInfoList;

        /** Bounding box for each frame. */
        struct Bound
        {
            Math::Vector3D min;
            Math::Vector3D max;
        };
        typedef std::vector<Bound> BoundList;

        /** Base frame joint. */
        struct BaseFrameJoint
        {
            Math::Vector3D pos;
            Math::Quaternion orient;
        };
        typedef std::vector<BaseFrameJoint> BaseFrameJointList;

        // TODO: add description.
        struct FrameData
        {
            int frameID;
            std::vector<float> data;
        };
        typedef std::vector<FrameData> FrameDataList;

        /** Skeleton's joint for one frame. */
        struct SkeletonJoint
        {
            SkeletonJoint() : parent(-1) {}
            SkeletonJoint(const BaseFrameJoint& copy)
                : pos(copy.pos)
                , orient(copy.orient)
            {}

            int parent;
            Math::Vector3D pos;
            Math::Quaternion orient;
        };
        /** Stores the joints of the skeleton for a single frame. */
        typedef std::vector<SkeletonJoint> Skeleton;
        typedef std::vector<Skeleton> FrameSkeletonList;

        const Skeleton& GetSkeleton() const
        {
            return animatedSkeleton;
        }

        int GetNumJoints() const
        {
            return numJoints;
        }

        const JointInfo& GetJointInfo(size_t index) const
        {
            return jointInfos[index];
        }

    private:
        void removeQuotes(std::string& str);
        void buildFrameSkeleton(const FrameData& frameData);
        void interpolateSkeletons(int frame0, int frame1, float quotient);

    private:
        JointInfoList      jointInfos;
        BoundList          bounds;
        BaseFrameJointList baseFrame;
        FrameDataList      frames;
        FrameSkeletonList  skeletons; // Skeletons for all the frames.
        Skeleton           animatedSkeleton; // Interpolated skeleton.

        int numFrames;
        int numJoints;
        int frameRate;
        int numAnimatedComponents;

        float animDuration;
        float frameDuration;
        float animTime;
    };
}

#endif // MD5ANIMATION_H_INCLUDED
