#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

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

    /* Make the window's context current */
    //initialization of openGL
    glfwMakeContextCurrent(window);
    gladLoadGL();

    std::string path = "3D/bunny.obj"; //path of model
    std::vector<tinyobj::shape_t> shapes; //definition of model
    std::vector<tinyobj::material_t> material;  //definition of model's texture
    std::string warning, error; //error checking

    tinyobj::attrib_t attributes; //3D model itself

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    //getting the EBO
    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    GLfloat vertices[]{
        //x   y     z
        0.f, 0.5f, 0.f,     //0
        -0.5f, -0.5f, 0.f,  //1
        0.5f, -0.5f, 0.f    //2
    };

    GLuint indices[]{
        0, 1, 2
    };

    //function call start
    //create and initialize VAO/VBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //currently editing VAO = null
    glBindVertexArray(VAO);
    //currently editing VAO = VAO;

    //currently editing VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //currently editing VBO = VBO
    //VAO <- VBO

    //define VBO
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * attributes.vertices.size(),  //size in bytes
        &attributes.vertices[0],   //array or //attributes.vertices.data()
        GL_STATIC_DRAW); //gl_static_draw ideally for static models(?)
    
    glVertexAttribPointer(
        0, //0 for pos, 1 for norm, 2 for texture
        3, //the number of components, x y z
        GL_FLOAT,
        GL_FALSE,   //is normalized
        3 * sizeof(float),
        (void*)0
    );

    //currently editing VBO = VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //currently editing VBO = EBO

    //define EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(), //or &mesh_indices[0]
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    //function call end
    
    //currently editing VBO = VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //currently editing VBO = null

    //currently editing VAO = VAO
    glBindVertexArray(0);
    //currently editing VAO = null

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        //clear to transparency before drawing
        glClear(GL_COLOR_BUFFER_BIT);
      
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        //keyboard and mouse events
        glfwPollEvents();
    }

    //garbage collection
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}   