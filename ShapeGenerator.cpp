// ============================================================================
// ShapeGenerator.cpp - GUIDA DIDATTICA
// Costruzione del triangolo sulla CPU: associa una posizione e un colore a ogni vertice.
// ============================================================================

#include "ShapeGenerator.h"

#define PI 3.1415926535

MeshData ShapeGenerator::createHeart(
    const int precision,
    const float scaleX,
    const float scaleY
    ) {
    MeshData data;
    data.primitiveType = GL_TRIANGLE_FAN;

    int nPoints = (precision <= 0) ? 10 : precision;

    // +2 per includere il centro e la chiusura del loop
    data.positions.reserve(nPoints + 2);
    data.colors.reserve(nPoints + 2);

    // 1. Inserimento del vertice centrale (Hub del Triangle Fan)
    data.positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    data.colors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // Colore centrale sfumato

    float stepA = (2 * PI) / nPoints;

    // 2. Loop fino a <= nPoints per far coincidere l'ultimo punto con il primo
    for (int i = 0; i <= nPoints; ++i)
    {
        double t = (double)i * stepA;
        float x = scaleX * (16.0f * pow(sin(t), 3));
        float y = scaleY * (13.0f * cos(t) - 5.0f * cos(2.0f * t) - 2.0f * cos(3.0f * t) - cos(4.0f * t));

        data.positions.push_back(glm::vec3(x, y, 0.0f));
        data.colors.push_back(glm::vec4(0.0f, 0.7f, 0.3f, 1.0f));
    }

    return data;
}

MeshData ShapeGenerator::createButterfly(
    const int precision,
    const float scaleX,
    const float scaleY
) {

    MeshData data;
    data.primitiveType = GL_TRIANGLE_FAN;

    int nPoints = (precision <= 0) ? 10 : precision;

    data.positions.reserve(nPoints + 2);
    data.colors.reserve(nPoints + 2);

    data.positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    data.colors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    float stepA = (2 * PI) / nPoints;
    for (int i = 0; i <= nPoints; ++i)
    {
        double t = (double)i * stepA;
        float x = scaleX * sin(t) * (exp(cos(t)) - 2 * cos(4 * t) - pow(sin(t / 12.0f), 5));

        float y = scaleY * cos(t) * (exp(cos(t)) - 2 * cos(4 * t) - pow(sin(t / 12.0f), 5));

        data.positions.push_back(glm::vec3(x, y, 0.0f));
        data.colors.push_back(glm::vec4(0.1f, 0.55f, 0.5f, 1.0f));
    }

    return data;
}