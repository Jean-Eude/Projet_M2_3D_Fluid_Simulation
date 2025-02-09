#include <Camera.hpp>

float Camera::z_axis(float x, float y){
    float z = 0; 
    if(sqrt((x * x) + (y * y)) <= radius) z = (float)sqrt((radius * radius) - (x * x) - (y * y)); 
    return z;
}

glm::vec3 Camera::getUnitVector(glm::vec3 vectr){
    float magnitude1;
    glm::vec3 unitVector; 
    magnitude1 = (vectr.x * vectr.x) + (vectr.y * vectr.y) + (vectr.z * vectr.z); 
    magnitude1 = sqrt(magnitude1);
    if(magnitude1 == 0){
        unitVector.x = 0;
        unitVector.y = 0;
        unitVector.z = 0;
            }
    else {
    unitVector.x = vectr.x / magnitude1; //if magnitude 0, then this func give 'nan' error.
    unitVector.y = vectr.y / magnitude1;
    unitVector.z = vectr.z / magnitude1;
}
    return unitVector;    
}

float Camera::dotProduct(){
    float result = (startPosUnitVector.x * currentPosUnitVector.x) + (startPosUnitVector.y * currentPosUnitVector.y) + (startPosUnitVector.z * currentPosUnitVector.z);
    return result;
}

void Camera::rotation(){
    startPosUnitVector = getUnitVector(startPos);
    currentPosUnitVector = getUnitVector(currentPos);
    currentQuaternion.axis = glm::cross(startPos, currentPos);
    currentQuaternion.axis = getUnitVector(currentQuaternion.axis);
    
    cosValue = dotProduct(); //q0 is cosine of the angle here.
    if(cosValue > 1) cosValue = 1; // when dot product gives '1' as result, it doesn't equal to 1 actually. It equals to just like 1.00000000001 . 
    theta = (acos(cosValue) * 180 / 3.1416); //theta is the angle now.
    currentQuaternion.cosine = cos((theta / 2) * 3.1416 / 180); //currentQuaternion.cosine is cos of half the angle now.

    currentQuaternion.axis.x = currentQuaternion.axis.x * sin((theta / 2) * 3.1416 / 180);
    currentQuaternion.axis.y = currentQuaternion.axis.y * sin((theta / 2) * 3.1416 / 180);
    currentQuaternion.axis.z = currentQuaternion.axis.z * sin((theta / 2) * 3.1416 / 180);
    
    cosValue_2 = (currentQuaternion.cosine * lastQuaternion.cosine) - glm::dot(currentQuaternion.axis, lastQuaternion.axis);
    
    
    glm::vec3 temporaryVector;

    temporaryVector = glm::cross(currentQuaternion.axis, lastQuaternion.axis);    

    rotationalAxis_2.x = (currentQuaternion.cosine * lastQuaternion.axis.x) + 
                            (lastQuaternion.cosine * currentQuaternion.axis.x ) +
                            temporaryVector.x;

    rotationalAxis_2.y = (currentQuaternion.cosine * lastQuaternion.axis.y) + 
                            (lastQuaternion.cosine * currentQuaternion.axis.y ) +
                            temporaryVector.y;

    rotationalAxis_2.z = (currentQuaternion.cosine * lastQuaternion.axis.z) + 
                            (lastQuaternion.cosine * currentQuaternion.axis.z ) +
                            temporaryVector.z;
    
    angle = (acos(cosValue_2) * 180 / 3.1416) * 2;

    rotationalAxis.x = rotationalAxis_2.x / sin((angle / 2) * 3.1416 / 180);
    rotationalAxis.y = rotationalAxis_2.y / sin((angle / 2) * 3.1416 / 180);
    rotationalAxis.z = rotationalAxis_2.z / sin((angle / 2) * 3.1416 / 180);
}

void Camera::replace(){
    lastQuaternion.cosine = cosValue_2;
    lastQuaternion.axis = rotationalAxis_2;
}

glm::mat4 Camera::getViewMatrix() {
    glm::vec3 forward = getUnitVector(currentPos - position);
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f))); 
    glm::vec3 adjustedUp = glm::normalize(glm::cross(right, forward)); 
    return glm::lookAt(position, position + forward, adjustedUp);
}

void Camera::setFlag(bool newFlag) {
    this->flag = newFlag;
}

bool Camera::getFlag() {
    return this->flag;
}