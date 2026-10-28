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

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>                

struct GridObject {
    int sceneIndex;
    int row;
    float x0, y0;
    float sx, sy;
};

std::vector<GridObject> activeObjects;

void buildGrid(const int rows, const int cols, Scene& scene);

void updateAnimation(float time, Scene& scene);

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
        1280,
        720,
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

    glViewport(0, 0, 1280, 720);


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

    int currentRows = -1;
    int currentCols = -1;

    // Ciclo dei frame: aggiorna i controlli, pulisce lo schermo, disegna la scena e presenta il risultato.
    while (!glfwWindowShouldClose(window))
    {
        /* Inizia il frame dell interfaccia prima di costruire i widget. */
        gui.beginFrame();
        gui.draw();

        if (gui.getRows() != currentRows || gui.getCols() != currentCols) {
            currentRows = gui.getRows();
            currentCols = gui.getCols();

            // Ricostruisce i buffer GPU e le matrici solo quando i parametri variano
            buildGrid(currentRows, currentCols, scene);
        }

        renderer.beginFrame(gui.clearColor());
        renderer.setWireframe(gui.wireframe());

        shader.use();

        glm::mat4 projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -1.0f, 1.0f);

        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        GLint projLoc = glGetUniformLocation(currentProgram, "u_Projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        updateAnimation(glfwGetTime(), scene);

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
                data = ShapeGenerator::createButterfly(14, 0.125, 0.125);
            }
            else {
				data = ShapeGenerator::createHeart(14, 0.0325, 0.0325);
            }

            std::size_t sceneIndex = scene.add(data, false);

            activeObjects.push_back({ (int)sceneIndex, i, cx, cy, scaleX, scaleY });

            //calcolo della model matrix statica
            glm::mat4 M = glm::mat4(1.0f);
            M = glm::translate(M, glm::vec3(cx, cy, 0.0f));
            M = glm::scale(M, glm::vec3(scaleX, scaleY, 1.0f));

			scene.getGeometry(sceneIndex).setModelMatrix(M);
        }
    }
}

void updateAnimation(float time, Scene& scene) {
    float A = 12.0f; //ampiezza oscillazione
    float f0 = 0.5f; // frequenza
    float deltaF = 0.15f; // incremento frequenza per riga
    float deltaPhi = 0.8f;// sfasamento righe
    float v = 25.0f; // velocità discesa costante

    for (const auto& obj : activeObjects) {

        float fr = f0 + obj.row * deltaF;
        float phi_r = obj.row * deltaPhi;

        // oscillazione sinusoidale orizzontale
        float xt = obj.x0 + A * sin(2.0f * glm::pi<float>() * fr * time + phi_r);

        // discesa lineare verticale
        float currentY = obj.y0 - v * time;

        // regola di riciclo --> FMODF avvolge la coordinata Y nel range 100, -100
        float yt = 100.0f - fmodf(100.0f - currentY, 200.0f);

        glm::mat4 M = glm::mat4(1.0f);
        M = glm::translate(M, glm::vec3(xt, yt, 0.0f));
        M = glm::scale(M, glm::vec3(obj.sx, obj.sy, 1.0f));

        scene.getGeometry(obj.sceneIndex).setModelMatrix(M);
    }
}
