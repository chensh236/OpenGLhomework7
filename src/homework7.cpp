#include "Initial.h"
#include "Shader.h"
#include <vector>
#define orthoType 0
#define perspectiveType 1
typedef int projectionModel;
// ************************�趨�õĲ���************************************
const unsigned int windowsWidth = 1000;
const unsigned int windowsHeight = 1000;
const char* head = "Shadow";
float radius = 4.0f;
// �󶨣�����׸��
unsigned int planeVAO;
unsigned int cubeVAO, VBO;
// ���ڼ�¼����ƶ�
float deltaTime = 0.0f;
float lastFrame = 0.0f;
/**
* @brief ����Esc�˳�
* @param window ����Ĵ���ָ��
* @param deltaTime ��ӵ��²���
*/
void escapePress(GLFWwindow *window, float& deltaTime);
void RenderScene(Shader &shader);
int main()
{
    // ��ʼ������
    GLFWwindow* window = NULL;
    if (initialWindow::initial(window, windowsWidth, windowsHeight, head) == -1) return -1;
    //// ��ʼ��rgbֵ
    //Shader shader("rgb.vs", "rgb.frag");
    /**
    * IMGUI
    */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    //// ������ɫ��
    Shader shader("shadow.vs", "shadow.frag");
    Shader simpleDepthShader("shadow_mapping_depth.vs", "shadow_mapping_depth.frag");
    Shader lampShader("light.vs", "light.frag");
    shader.setInt("diffuseTexture", 0);
    shader.setInt("shadowMap", 1);

    // ���������Լ��ֹ���ӵķ���������
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // set floor
    GLfloat planeVertices[] = {
        // Positions          // Normals       // Texture Coords
        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
        -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
    };

    // ����ƽ���VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // Configure depth map FBO ����߿�1024
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    // Ϊ��Ⱦ�������ͼ����һ��֡�������
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // �����������
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // ��ֹ�������� ----------------------------------------
    // ��ֹ������ͼ��Զ���ظ���Ⱦ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


    // �����ɵ����������Ϊ֡�������Ȼ���
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ����ͶӰģ�͵�ѡ��
    projectionModel currentModel = orthoType;
    // �ж��Ƿ���й�Դ���ƶ�
    bool isMove = true;
    // ��Դλ��
    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

    while (!glfwWindowShouldClose(window))
    {

        // ��������
        escapePress(window, deltaTime);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /**
        * ʹ��IMGUI
        */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Attributes");
        ImGui::Text("Chooce projection type");
        ImGui::RadioButton("ortho type", &currentModel, orthoType);
        ImGui::RadioButton("perspective type", &currentModel, perspectiveType);
        ImGui::Separator();
        ImGui::Checkbox("Move the light", &isMove);

        ImGui::End();

        // �ƶ���Դλ��
        if (isMove) {
            float camPosX = sin(glfwGetTime() * 0.5) * (float)radius;
            float camPosZ = cos(glfwGetTime() * 0.5) * (float)radius;
            lightPos = glm::vec3(camPosX, lightPos.y, camPosZ);
        }

        // Ϊ���޸�peter���ƣ����������޳�
        glCullFace(GL_FRONT);

        glm::mat4 lightProjection(1.0f), lightView(1.0f), lightSpaceMatrix(1.0f);

        // ��Ϊ��Դʹ����������Ͷ��ͶӰ����
        GLfloat near_plane = 1.0f, far_plane = 7.5f;
        if (currentModel == orthoType) {
            lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        }
        else {
            lightProjection = glm::perspective(100.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
        }

        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // ��Ⱦ�����ͼ
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        RenderScene(simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glCullFace(GL_BACK);


        // ���ʹ�������ͼ��Ⱦ
        glViewport(0, 0, windowsWidth, windowsHeight);

        // ������ɫ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        shader.use();
        glm::mat4 projection(1.0f);
        glm::mat4 view(1.0f);
        glm::mat4 model(1.0f);
        view = glm::lookAt(Camera::getInstance()->Position, Camera::getInstance()->Position + Camera::getInstance()->Front, Camera::getInstance()->Up);
        projection = glm::perspective(glm::radians(Camera::getInstance()->Zoom), (float)windowsWidth / (float)windowsHeight, 0.1f, 100.0f);
        
        // ���ÿɱ����ɫ������
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("viewPos", Camera::getInstance()->Position);
        shader.setVec3("lightPos", lightPos);
        shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        // ��������
        RenderScene(shader);
       /* printf("front: x:%f, y:%f, z:%f", Camera::getInstance()->Front.x, Camera::getInstance()->Front.y, Camera::getInstance()->Front.z);
        printf("up: x:%f, y:%f, z:%f", Camera::getInstance()->Up.x, Camera::getInstance()->Up.y, Camera::getInstance()->Up.z);*/
        // ������Դ
        lampShader.use();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("model", model);
        lampShader.setMat4("view", view);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

/**
* @brief ���м��̰����ļ��
* @param window ����ָ��
* @param deltaTime ��������ʱ��
*/
void escapePress(GLFWwindow *window, float& deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // ���WSAD����
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera::getInstance()->ProcessKeyboard(RIGHT, deltaTime);
}

void RenderScene(Shader &shader)
{
    // ƽ��
    glm::mat4 model(0.5f);
    shader.setMat4("model", model);
    shader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // ����1
    model = glm::mat4(1.0f);
    model = glm::rotate(model, ((float)glfwGetTime() * 0.5f), glm::vec3(1.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(1.0f, 3.0f, 2.0f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", glm::vec3(0.0f, 1.0f, 1.0f));
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}