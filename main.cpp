// ============================================================================
// main.cpp - GUIDA DIDATTICA
// Punto di ingresso: inizializza GLFW e GLAD, crea shader e scena, avvia il ciclo di rendering e rilascia le risorse.
// ============================================================================

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"
#include "ShapeGenerator.h"
#include "Scene.h"
#include "Renderer.h"
#include "Gui.h"
#include "Input.h"

struct GridObject {
    int sceneIndex;
    int row;
    float x0, y0;
    float sx, sy;
};

std::vector<GridObject> activeObjects;

void buildGrid(const int rows, const int cols, const Scene& scene);

// Avvio del programma. Le risorse OpenGL vanno create solo dopo che esiste un contesto corrente.
int main()
{
    // ========================================================================
    // 1. GLFW
    // ========================================================================

    if (!/* Inizializza GLFW prima di creare la finestra. */
    glfwInit())
    {
        std::cout << "Errore inizializzazione GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // ========================================================================
    // 2. FINESTRA
    // ========================================================================

    GLFWwindow* window = glfwCreateWindow(
        800,
        800,
        "OpenGL - Template classi",
        nullptr,
        nullptr
    );

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    // ========================================================================
    // 3. GLAD
    // ========================================================================

    if (!/* Carica le funzioni OpenGL dopo avere reso corrente il contesto. */
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Errore caricamento GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 800);


    // ========================================================================
    // 4. CALLBACK
    // ========================================================================

    glfwSetKeyCallback(window, Input::keyCallback);
    glfwSetCursorPosCallback(window, Input::cursorPositionCallback);
    glfwSetFramebufferSizeCallback(window, Input::framebufferSizeCallback);


    // ========================================================================
    // 5. SHADER
    // ========================================================================

    Shader shader;

    if (!shader.load(
        "shader/vertexshaderC.glsl",
        "shader/fragmentshaderC.glsl"))
    {
        std::cout << "Errore caricamento shader" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    shader.use();


    // ========================================================================
    // 6. SCENA, RENDERER E GUI
    // ========================================================================

    Scene scene;
    Renderer renderer;
    Gui gui;

    gui.initialize(window);

    glPointSize(5.0);
    glLineWidth(10.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    // ========================================================================
    // 8. RENDER LOOP
    // ========================================================================

    // Ciclo dei frame: aggiorna i controlli, pulisce lo schermo, disegna la scena e presenta il risultato.
    while (!glfwWindowShouldClose(window))
    {
        /* Inizia il frame dell interfaccia prima di costruire i widget. */
    gui.beginFrame();
        gui.draw();

        renderer.beginFrame(gui.clearColor());
        renderer.setWireframe(gui.wireframe());

        shader.use();
        /* Il Renderer visita tutte le Geometry e ne richiama draw(). */
    renderer.drawScene(scene);

        // ImGui va disegnata in modalita' fill.
        renderer.setWireframe(false);
        /* Disegna la GUI sopra la scena OpenGL. */
    gui.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // ========================================================================
    // 9. CLEANUP
    // ========================================================================

    gui.shutdown();
    scene.clear();
    shader.destroy();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void buildGrid(const int rows, const int cols, Scene& scene) {

    scene.clear();
	activeObjects.clear();

    // metà superiore dello schermo
	float L = -100.0f;
	float R = 100.0f;
	float B = 0.0f;
    float T = 100.0f;

    float deltaX = (R - L) / cols;
    float deltaY = (T - B) / rows;

	float scaleX = (deltaX / 2.0f) * 0.8f;
	float scaleY = (deltaY / 2.0f) * 0.8f;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

			float cx = L + (j + 0.5f) * deltaX;
			float cy = B + (i + 0.5f) * deltaY;

            MeshData data;

            if (i % 2 == 0) {
                data = ShapeGenerator::createButterfly(14,
                    0.0625,
                    0.0625,
                    0.1,
                    -0.3
                );
            }
            else {
				data = ShapeGenerator::createHeart(14,
                    0.0125,
                    0.0125,
                    -0.4,
                    -0.4
                );
            }

            std::size_t sceneIndex = scene.add(data, false);

        }
    }
}

