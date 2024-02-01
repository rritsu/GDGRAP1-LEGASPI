/*
date modified: 02-01-2024 - 10:37 am
linear transformation eme
*/


#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float x_mod = 0;
float y_mod = 0;

float x = 0, y = 0, z = 0;
float scale_x = 3, scale_y = 3, scale_z = 3;
float axis_x = 0, axis_y = 1, axis_z = 0;
float theta = 90;

glm::mat4 identity_matrix = glm::mat4(1.0f);

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        theta += 5.0f;
    }

    if (key == GLFW_KEY_J && action == GLFW_PRESS)
    {
        theta -= 5.0f;
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        y_mod += 0.1f;
    }

    if (key == GLFW_KEY_A)
    {
        x_mod -= 0.1f;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        y_mod -= 0.1f;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        x_mod += 0.1f;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        y_mod -= 0.1f;
    }
}




int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Andrea Maxene Legaspi", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;

    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();


    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;

    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();


    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // glViewport(0, 0, 300, 600);

    glfwSetKeyCallback(window, Key_Callback);


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);

    std::string path = "3D/bunny.obj";
    std::vector <tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }


    GLfloat vertices[]
    {
        0.f, 0.5f, 0.f, //0
        -0.5f, -0.5f, 0.f, //1
        0.5f, -0.5f, 0.f //2
    };

    GLuint indices[]{
        0,1,2
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Currently editing VAO = null
    glBindVertexArray(VAO);
    //Currently editing VAO = VAO

    //Currently editing VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Currently editing VBO = VBO
    //VAO <- VBO

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * attributes.vertices.size(), //size in bytes
        &attributes.vertices[0], //array
        GL_STATIC_DRAW);

    glVertexAttribPointer
    (0,//0 pos, 1 = norm, 2 = texture
        3, //X Y Z
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    //Currently editing VBO = VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Currently editing VBO = EBO

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), &mesh_indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* glBegin(GL_POLYGON);
             glVertex2f (0.0f, 0.5f);
             glVertex2f(-0.4741f, 0.15552f);
             glVertex2f(-0.294f, -0.4045f);
             glVertex2f(0.294f, -0.4045f);
             glVertex2f(0.4741f, 0.15552f);
             glEnd();
         */


        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x, y, z));

        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_x, scale_y, scale_z));

        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::normalize(glm::vec3(axis_x, axis_y, axis_z)));

        unsigned int transformloc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformloc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));


        glUseProgram(shaderProg);
        unsigned int xLoc = glGetUniformLocation(shaderProg, "x");
        glUniform1f(xLoc, x_mod);
        unsigned int yLoc = glGetUniformLocation(shaderProg, "y");
        glUniform1f(yLoc, y_mod);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}