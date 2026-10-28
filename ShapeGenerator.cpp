// ============================================================================
// ShapeGenerator.cpp - GUIDA DIDATTICA
// Costruzione del triangolo sulla CPU: associa una posizione e un colore a ogni vertice.
// ============================================================================

#include "ShapeGenerator.h"

#define PI 3.1415926535

MeshData ShapeGenerator::createHeart(
    const int precision,
    const float scaleX,
    const float scaleY,
    const float shiftX,
    const float shiftY
) {

    MeshData data;
    data.primitiveType = GL_TRIANGLE_FAN;

    float x, y;
    int nPoints;

    if (precision <= 0) {
        nPoints = 10;
    }
    else {
        nPoints = precision;
    }

    data.positions.reserve(nPoints);
    data.colors.reserve(nPoints);

    float stepA = (2 * PI) / nPoints;
    for (int i = 0; i < nPoints; ++i)
    {
        double t = (double)i * stepA;
        x = scaleX * (16.0 * pow(sin(t), 3));
        y = scaleY * (13.0 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t));

        //posizionamento nella scena
        x += shiftX;
        y += shiftY;

        glm::vec3 pos = glm::vec3(x, y, 0.0f);
        data.positions.push_back(pos);
        data.colors.push_back(glm::vec4(0.0f, 0.7f, 0.3f, 1.0f));
    }

    return data;
}

MeshData ShapeGenerator::createButterfly(
    const int precision,
    const float scaleX,
    const float scaleY,
    const float shiftX,
    const float shiftY
) {

    MeshData data;
    data.primitiveType = GL_TRIANGLE_FAN;

    float x, y;
    int nPoints;

    if (precision <= 0) {
        nPoints = 10;
    }
    else {
        nPoints = precision;
    }

    data.positions.reserve(nPoints);
    data.colors.reserve(nPoints);

    float stepA = (2 * PI) / nPoints;
    for (int i = 0; i < nPoints; ++i)
    {
        double t = (double)i * stepA;
        x = scaleX * sin(t) * (exp(cos(t)) - 2 * cos(4 * t) - pow(sin(t / 12.0f), 5));

        y = scaleY * cos(t) * (exp(cos(t)) - 2 * cos(4 * t) - pow(sin(t / 12.0f), 5));

        //posizionamento nella scena
        x += shiftX;
        y += shiftY;

        glm::vec3 pos = glm::vec3(x, y, 0.0f);
        data.positions.push_back(pos);
        data.colors.push_back(glm::vec4(0.1f, 0.55f, 0.5f, 1.0f));
    }

    return data;
}