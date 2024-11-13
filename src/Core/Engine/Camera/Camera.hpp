#include <HeadersBase.hpp>

const float RADIUS = 1.f;

struct Quaternion{
    float cosine;
    glm::vec3 axis;

};

class Camera{
public:
    
    glm::vec3 position = glm::vec3(0.0f, 0.0f, -3.0f);
    glm::vec3 startPos;
    glm::vec3 currentPos = startPos;
    glm::vec3 startPosUnitVector;
    glm::vec3 currentPosUnitVector;

    Quaternion currentQuaternion;
    Quaternion lastQuaternion = {0.0f, glm::vec3(1.0f, 0.0f, 0.0f)};
    
    float cosValue, cosValue_2;
    float theta;
    float angle = 180.0f;
    glm::vec3 rotationalAxis = glm::vec3(1.0f, 0.0f, 0.0f);                       
    glm::vec3 rotationalAxis_2;
    Camera (){};
    float z_axis(float,float);
    glm::vec3 getUnitVector(glm::vec3);
    float dotProduct();
    void rotation();
    void replace();

    
};