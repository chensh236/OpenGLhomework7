#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Initial.h"
using namespace std;
class Shader
{
public:
    unsigned int program;

    /**
    * @brief ������ɫ��
    * @param vertexPath ������ɫ��·��
    * @param fragmentPath Ƭ����ɫ��·��
    */
    Shader(string vertexPath, string fragmentPath);
    /**
    * @brief ʹ����ɫ��
    */
    void use();

    // ����Ϊuniform����

    /**
    * @brief ����Bool�ͱ���
    * @param name ������
    * @param val ����ֵ
    */
    void setBool(const string &name, bool val) const;

    /**
    * @brief ����Int�ͱ���
    * @param name ������
    * @param val ����ֵ
    */
    void setInt(const string &name, int val) const;

    /**
    * @brief ����Float�ͱ���
    * @param name ������
    * @param val ����ֵ
    */
    void setFloat(const string &name, float val) const;

    /**
    * @brief ����3ά�ռ�
    * @param name ������
    * @param mat ����ֵ
    */
    void setMat4(const std::string &name, glm::mat4 mat) const;

    void setVec3(const std::string &name, float value1, float value2, float value3) const;
    void setVec3(const std::string &name, glm::vec3 vec) const;
private:

    /**
    * @brief ������
    * @param shader ��ɫ��
    * @param type ��ɫ������
    */
    void errDetect(unsigned int shader, GLenum type);

    /**
    * @brief �ļ���ȡ
    * @param filePath �ļ���·��
    * @return �ļ��е��ַ���
    */
    string loadFile(string filePath);

};