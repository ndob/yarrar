#pragma once

#include "Types.h"

#include <map>
#include <string>
#include <vector>

namespace yarrar
{

struct Model
{
    std::string name;
    std::vector<float> vertices;
    Vector3 translation;
};

class Scene
{
public:
    void addModel(int coordinateSystemId, const Model& model);
    const std::vector<Model>& getModels(int coordinateSystemId) const;

private:
    std::map<int, std::vector<Model>> m_models;
};
}
