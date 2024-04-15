#pragma once

class IRender
{
public:
    virtual void Draw(const class Shader* shader = nullptr) const = 0; // If the object is to be drawn
};
