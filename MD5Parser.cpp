#include "MD5Parser.h"
#include "Log.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <tuple>

using namespace std;
using namespace Math;

namespace ST
{
    const string MD5Parser::skip_strings[] =
    {
        "//", "MD5Version", "commandline", "numJoints",
        "numMeshes", "numverts", "numtris", "numweights",
        "numFrames", "numJoints", "frameRate", "numAnimatedComponents"
    };
    const function
    <
        void(MD5Parser&, const vector<string>&)
    > MD5Parser::state_parsers[] =
    {
        &MD5Parser::process_start,
        &MD5Parser::process_joints,
        &MD5Parser::process_mesh,
        &MD5Parser::process_unknown,
    };

    MD5Parser::MD5Parser() : state(START), processed_line(0)
    {
    }

    const vector<Mesh>& MD5Parser::GetSkin() const
    {
        return skin;
    }

    const vector<Joint>& MD5Parser::GetSkeleton() const
    {
        return skeleton;
    }

    //-- Check whether fullstring starts with 'start', omitting ' ' --//
    bool MD5Parser::starts_with(string fullstring, string start) const
    {
        if (start.empty()) return true;
        if (fullstring.empty()) return false;
        if (start.size() > fullstring.size()) return false;

        size_t i = fullstring.find_first_not_of(" \t");
        if (i == string::npos)
        {
            if (start.find_first_not_of(" \t") == string::npos) return true;
            else return false;
        }

        if (start.size() > fullstring.size() - i) return false;

        return fullstring.compare(i, start.length(), start) == 0;
    }

    vector<string> MD5Parser::tokenize(string line) const
    {
        string token;
        stringstream ss(line);
        vector<string> tokens;

        while (ss >> token)
        {
            size_t i = token.find("//");
            if (i != string::npos)
            {
                if (i != 0)
                    tokens.push_back(token.substr(0, i));
                break;
            }
            tokens.push_back(token);
        }

        return tokens;
    }

    void MD5Parser::Parse(string filename)
    {
        string line;
        ifstream input(filename);
        while (getline(input, line))
        {
            processed_line++;
            bool skip_line = false;
            for (auto skip_str : skip_strings)
            {
                if (starts_with(line, skip_str))
                {
                    skip_line = true;
                    break;
                }
            }
            if (skip_line) continue;

            vector<string> tokens = tokenize(line);
            if (tokens.size() == 0) continue;
            state_parsers[state](*this, tokens);
        }
        input.close();
        state = START;
        processed_line = 0;
    }

    void MD5Parser::process_start(const vector<string>& tokens)
    {
        if (tokens[0] == "joints") state = JOINTS;
        else if (tokens[0] == "mesh") state = MESH;
        else state = UNKNOWN;
    }

    void MD5Parser::process_joints(const vector<string>& tokens)
    {
        if (tokens.size() == 12)
        {
            Joint joint;
            joint.name = tokens[0].substr(1, tokens[0].size() - 2);
            joint.parent = stoul(tokens[1]);
            joint.p = Vector3D(stof(tokens[3]), stof(tokens[4]), stof(tokens[5]));
            joint.q = Quaternion(0, stof(tokens[8]), stof(tokens[9]), stof(tokens[10]));
            joint.q.ComputeW();
            skeleton.push_back(joint);
        }
        else if (tokens[0] == "}") state = START;
        else log_tokens("Malformed string: ", tokens);
    }

    void MD5Parser::process_mesh(const vector<string>& tokens)
    {
        if (tokens[0] == "shader") mesh.texture_path = tokens[1];
        else if (tokens[0] == "tri")
            for (size_t i = 2; i <= 4; i++)
                mesh.triangles.push_back(stoul(tokens[i]));
        else if (tokens[0] == "vert")
        {
            Vertex vertex;
            vertex.s = stof(tokens[3]); vertex.t = stof(tokens[4]);
            vertex.weight_index = stof(tokens[6]);
            vertex.weight_count = stof(tokens[7]);
            mesh.vertices.push_back(vertex);
        }
        else if (tokens[0] == "weight")
        {
            Weight weight;
            weight.joint = stoul(tokens[2]);
            weight.bias = stof(tokens[3]);
            weight.p = Vector3D(stof(tokens[5]), stof(tokens[6]), stof(tokens[7]));
            mesh.weights.push_back(weight);
        }
        else if (tokens[0] == "}")
        {
            state = START;
            skin.push_back(mesh);
            mesh.Clear();
        }
    }

    void MD5Parser::process_unknown(const vector<string>& tokens)
    {
        state = START;
        log_tokens("Malformed string: ", tokens);
    }

    void MD5Parser::log_tokens(string prefix, const vector<string>& tokens)
    {
        string line;
        for (auto token : tokens) line += token;
        log("(%d) %s%s", processed_line, prefix.c_str(), line.c_str());
    }
}
