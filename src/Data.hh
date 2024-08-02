#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "pugixml.hpp"
#include "Point.hh"
#include "glm/glm.hpp"
using namespace std;

class Data {
private:
    vector<glm::vec2> nodes;
    vector<vector<ulong>> ways;

    float minX, minY;
    float maxX, maxY;
public:
    void load(string fileName);

    uint getNWays() const;

    uint getWaySize(uint i) const;

    vector<glm::vec3> getWay(uint i) const;

    glm::vec4 getBoundaries() const;
};