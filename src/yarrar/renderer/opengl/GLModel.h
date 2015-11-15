#pragma once

namespace yarrar
{

class GLModel
{
public:
    virtual ~GLModel() {};
    virtual void render() const = 0;
};

}

