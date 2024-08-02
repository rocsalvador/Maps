#include "Data.hh"

void Data::load(string fileName) {
    pugi::xml_document file;
    pugi::xml_parse_result result = file.load_file(fileName.c_str());
    string s;
    ulong iNode = 0;
    unordered_map<ulong, ulong> idMap;
    pugi::xml_node root = file.first_child();
    pugi::xml_node bounds = root.child("bounds");
    minY = stof(bounds.attribute("minlat").value()) * 1000;
    minX = stof(bounds.attribute("minlon").value()) * 1000;
    maxY = stof(bounds.attribute("maxlat").value()) * 1000;
    maxX = stof(bounds.attribute("maxlon").value()) * 1000;
    for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling())
    {
        string name = node.name();
        if (name == "node") {
            float x = stof(node.attribute("lon").value()) * 1000;
            float y = stof(node.attribute("lat").value()) * 1000;
            ulong id = stol(node.attribute("id").value());
            nodes.push_back(glm::vec2(x, y));
            idMap.insert({id, iNode});
            ++iNode;
        } else if (name == "way") {
            vector<ulong> way;
            for (pugi::xml_node snode = node.first_child(); snode; snode = snode.next_sibling()) {
                string sname = snode.name();
                if (sname == "nd") {
                    ulong ref = stol(snode.attribute("ref").value());
                    way.push_back(idMap.find(ref)->second);
                }
            }
            ways.push_back(way);
        }
    }
}

uint Data::getWaySize(uint i) const
{
    assert(i < ways.size());
    return ways[i].size();
}

uint Data::getNWays() const {
    return ways.size();
}

vector<glm::vec3> Data::getWay(uint i) const {
    assert(i < ways.size());
    vector<glm::vec3> way(ways[i].size());
    for (uint j = 0; j < ways[i].size(); ++j)
        way[j] = glm::vec3(nodes[ways[i][j]], 0);
    return way;
}

glm::vec4 Data::getBoundaries() const {
    return glm::vec4(minX, minY, maxX, maxY);
}