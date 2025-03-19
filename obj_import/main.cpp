#include <glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "model.h"


#include <iostream>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// ���������
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// ��������
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
   // glfw: ������������� � ����������������
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   // glfw: �������� ����
   GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com!", NULL, NULL);
   if (window == NULL)
   {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);

   // �������� GLFW, ����� �� �������� ��� ������
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   // glad: �������� ���� ���������� �� OpenGL-�������
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
   }

   // �������� stb_image.h, ����� �� ���������� ����������� �������� ������������ y-��� (�� �������� ������)
   //stbi_set_flip_vertically_on_load(true);

   // ���������������� ����������� ��������� OpenGL
   glEnable(GL_DEPTH_TEST);

   // �������������� ����� ��������� ���������
   Shader ourShader("C:/Users/flowe/source/repos/obj_import/obj_import/1.model_loading.vs", "C:/Users/flowe/source/repos/obj_import/obj_import/1.model_loading.fs");

   // �������� �������
   //Model ourModel("C:/Users/flowe/source/repos/obj_import/obj_import/resources/objects/backpack/backpack.obj");
   Model ourModel("C:/Users/flowe/source/repos/obj_import/obj_import/resources/objects/cat/12221_Cat_v1_l3.obj");
   //Model ourModel("C:/Users/flowe/source/repos/obj_import/obj_import/resources/objects/skull/12140_Skull_v3_L2.obj");

   // ��������� � ������ �������
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   // ���� ����������
   while (!glfwWindowShouldClose(window))
   {
      // ���������� ����� ������ �� �������� ��� ������� �����
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      // ��������� �����
      processInput(window);

      // ���������
      glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // ����������, ��� ������������ ������ ������, ��� ����������� uniform-����������/�������_���������
      ourShader.use();

      // �������������� ����/��������
      glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      glm::mat4 view = camera.GetViewMatrix();
      ourShader.setMat4("projection", projection);
      ourShader.setMat4("view", view);

      // ��������� ����������� ������
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // ������� ���� ����� ���� � ������ �����
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// ������ ������� ������� ��� ����� �����, ������� ������� �������� ���
      ourShader.setMat4("model", model);
      ourModel.Draw(ourShader);


      // glfw: ����� ���������� front- � back- �������. ������������ ������� �����/������ (���� �� ������/�������� ������, ��������� ������ ���� � �.�.)
      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   // glfw: ����������, ������������ ���� ���������� ����� GLFW-��c�����
   glfwTerminate();
   return 0;
}

// ��������� ���� ������� �����: ������ GLFW � �������/���������� ������ ���� � ������ ����� � ��������������� ��������� ������ �������
void processInput(GLFWwindow* window)
{
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(FORWARD, deltaTime);
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(BACKWARD, deltaTime);
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(LEFT, deltaTime);
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ������������ ��������), ���������� ������ callback-�������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   // ����������, ��� ���� ��������� ������������� ����� �������� ����.
   // �������� ��������, ������ � ������ ����� ����������� ������, ��� �������, �� Retina-��������
   glViewport(0, 0, width, height);
}

// glfw: ������ ���, ����� ������������ ����, ���������� ������ callback-�������
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
   if (firstMouse)
   {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
   }

   float xoffset = xpos - lastX;
   float yoffset = lastY - ypos; // �����������, ��� ��� y-���������� ���� ����� �����

   lastX = xpos;
   lastY = ypos;

   camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: ������ ���, ����� �������������� �������� ����, ���������� ������ callback-�������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
   camera.ProcessMouseScroll(yoffset);
}