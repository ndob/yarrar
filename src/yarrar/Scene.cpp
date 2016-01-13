#include "Scene.h"

namespace
{
std::vector<yarrar::Model> s_empty;
}

namespace yarrar
{

void Scene::addModel(int coordinateSystemId, const Model& model)
{
    m_models[coordinateSystemId].push_back(model);
}

const std::vector<Model>& Scene::getModels(int coordinateSystemId) const
{
    auto it = m_models.find(coordinateSystemId);
    if(it == m_models.end())
    {
        return s_empty;
    }
    return it->second;
}
}
