#include "Shader.h"
/**
* @brief ������ɫ��
* @param vertexPath ������ɫ��·��
* @param fragmentPath Ƭ����ɫ��·��
*/
Shader::Shader(string vertexPath, string fragmentPath)
{
    // ��ȡ�ļ�
    string vCode = loadFile(vertexPath);
    string fCode = loadFile(fragmentPath);
    const char* vShaderCode = vCode.c_str();
    const char* fShaderCode = fCode.c_str();

    unsigned int vertex, fragment;
   /* cout << "test:" << endl << vShaderCode << endl;*/

    // ������ɫ��
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    errDetect(vertex, GL_COMPILE_STATUS);

    // Ƭ����ɫ��
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    errDetect(fragment, GL_COMPILE_STATUS);

    // ������ɫ��
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    errDetect(program, GL_LINK_STATUS);
    // ����Ҫʱɾ����ɫ��
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
/**
* @brief ʹ����ɫ��
*/
void Shader::use()
{
    glUseProgram(program);
}

// ����Ϊuniform����

/**
* @brief ����Bool�ͱ���
* @param name ������
* @param val ����ֵ
*/
void Shader::setBool(const string &name, bool val) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int)val);
}

/**
* @brief ����Int�ͱ���
* @param name ������
* @param val ����ֵ
*/
void Shader::setInt(const string &name, int val) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), val);
}

/**
* @brief ����Float�ͱ���
* @param name ������
* @param val ����ֵ
*/
void Shader::setFloat(const string &name, float val) const
{
    glUniform1f(glGetUniformLocation(program, name.c_str()), val);
}


/**
* @brief ������
* @param shader ��ɫ��
* @param type ��ɫ������
*/
void Shader::errDetect(unsigned int shader, GLenum type) {
    int success;
    glGetShaderiv(shader, type, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        cout << "Err: " << infoLog << endl;
    }

}

/**
* @brief �ļ���ȡ
* @param filePath �ļ���·��
* @return �ļ��е��ַ���
*/
string Shader::loadFile(string filePath) {

    string code;
    ifstream shaderFile;
    shaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    try
    {
        // ���ļ�
        shaderFile.open(filePath.c_str());
        stringstream shaderStream;
        // ���ı���buffer��ȡ������
        shaderStream << shaderFile.rdbuf();
        // �ر�
        shaderFile.close();
        // ��ת��Ϊstring
        code = shaderStream.str();
    }
    catch (ifstream::failure e)
    {
        cout << "Err: Fail to read file: " << filePath << endl;
    }
    return code;
}

void Shader::setMat4(const std::string &name, glm::mat4 mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string &name, float value1, float value2, float value3) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), value1, value2, value3);
}


void Shader::setVec3(const std::string &name, glm::vec3 vec) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), vec.x, vec.y, vec.z);
}