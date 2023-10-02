#include "glm/glm.hpp"
#include "uniform.h"
#include "fragment.h"
#include "color.h"
#include "vertexArray.h"
#include "FastNoiseLite.h"
#pragma once

void printMatrix(const glm::mat4& myMatrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << myMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void printVec4(const glm::vec4& vector) {
    std::cout << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";
}

void printVec3(const glm::vec3& vector) {
    std::cout << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
}

Vertex vertexShader(const Vertex& vertex, const Uniform& uniform) {
    glm::vec4 transformedVertex = uniform.viewport * uniform.projection * uniform.view * uniform.model * glm::vec4(vertex.position, 1.0f);
    glm::vec3 vertexRedux;
    vertexRedux.x = transformedVertex.x / transformedVertex.w;
    vertexRedux.y = transformedVertex.y / transformedVertex.w;
    vertexRedux.z = transformedVertex.z / transformedVertex.w;
    Color fragmentColor(255, 0, 0, 255);
    glm::vec3 normal = glm::normalize(glm::mat3(uniform.model) * vertex.normal);
    Fragment fragment;
    fragment.position = glm::ivec2(transformedVertex.x, transformedVertex.y);
    fragment.color = fragmentColor;
    return Vertex {vertexRedux, normal, vertex.tex, vertex.position};
}

float nextTime = 0.5f;

Color fragmentShader(Fragment& fragment) {
    return fragment.color;
}

Color fragmentShaderSun(Fragment& fragment) {
    // Obtiene las coordenadas del fragmento en el espacio 2D
    glm::vec2 fragmentCoords(fragment.original.x, fragment.original.y);

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite:: NoiseType_Cellular);
    noise.SetSeed(123);
    noise.SetFrequency(0.1f);
    noise.SetFractalType(FastNoiseLite:: FractalType_PingPong);
    noise.SetFractalOctaves(4);
    noise.SetFractalLacunarity(2 + nextTime);
    noise.SetFractalGain(0.90f);
    noise.SetFractalWeightedStrength(0.70f);
    noise.SetFractalPingPongStrength(3 );
    noise.SetCellularDistanceFunction(FastNoiseLite:: CellularDistanceFunction_Euclidean);
    noise.SetCellularReturnType(FastNoiseLite:: CellularReturnType_Distance2Add);
    noise.SetCellularJitter(1);

    // Definir el rango de colores desde amarillo hasta rojo
    Color yellowColor(255, 255, 0, 255);
    Color redColor(255, 0, 0, 255);


    float ox = 3000.0f;
    float oy = 3000.0f;
    float zoom = 5000.0f;

    float noiseValue = abs(noise.GetNoise((fragment.original.x + ox) * zoom, (fragment.original.y + oy) * zoom, fragment.original.z * zoom));

    Color tmpColor = (noiseValue < 0.1f) ? yellowColor : redColor;

    fragment.color = tmpColor * fragment.z;
    return fragment.color;
}
