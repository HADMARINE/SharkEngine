//
// Created by HADMARINE on 2020/10/28.
//

#ifndef SHARKENGINE_CAMERA_HPP
#define SHARKENGINE_CAMERA_HPP

#include <glm/glm.hpp>

namespace SharkEngine {
    class Camera {
    public:
        struct CameraProperties {

            CameraProperties(glm::vec3 eye,
                             glm::vec3 center,
                             glm::vec3 upVector,
                             glm::mat4 model,
                             float fieldOfView) : center(center),
                                                  eye(eye),
                                                  upVector(upVector),
                                                  model(model) {
                this->eye = eye;
                this->center = center;
                this->upVector = upVector;
                this->model = model;
                this->fieldOfView = fieldOfView;
            }

            glm::vec3 &eye;
            glm::vec3 &center;
            glm::vec3 &upVector;
            glm::mat4 &model;
            float fieldOfView;
        };

        glm::vec3 getEye() { return eye; };
        glm::vec3 getCenter() { return center; };
        glm::vec3 getUpVector() { return upVector; };
        glm::mat4 getModel() { return model; };
        float getFieldOfView() { return fieldOfView; };

        Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 upVector, glm::mat4 model, float fieldOfView) : camProp(eye, center, upVector, model, fieldOfView){
            this->eye = eye;
            this->center = center;
            this->upVector = upVector;
            this->model = model;
            this->fieldOfView = fieldOfView;
            CameraProperties camProp{
                    eye, center,
                    upVector, model,
                    fieldOfView};
        }

        Camera() : camProp(glm::vec3(10, 3, 10),
                           glm::vec3(0, 0, 0),
                           glm::vec3(0, -1, 0),
                           glm::mat4(1.0f), 45.0f) {
            this->eye = glm::vec3(10, 3, 10);
            this->center = glm::vec3(0, 0, 0);
            this->upVector = glm::vec3(0, -1, 0);
            this->model = glm::mat4(1.0f);
            this->fieldOfView = 45.0f;
        }

        Camera(Camera *cam, CameraProperties camProp) : camProp(camProp) {
            this->eye = cam->getEye();
            this->center = cam->getCenter();
            this->upVector = cam->getUpVector();
            this->model = cam->getModel();
            this->fieldOfView = cam->getFieldOfView();
        }

        CameraProperties *getCameraProperties() {
            return &camProp;
        }

        static void mountCamera(Camera *cam) {
            Camera::currentCam = cam;
        }

        static Camera* getCurrentCamera() { return Camera::currentCam; };

    private:
        CameraProperties camProp;
        static Camera *currentCam;
        float fieldOfView = 45.0f;
        glm::vec3 eye{10, 3, 10};
        glm::vec3 center{0, 0, 0};
        glm::vec3 upVector{0, -1, 0};
        glm::mat4 model{1.0f};
    };

    Camera* Camera::currentCam{};

}// namespace SharkEngine

#endif//SHARKENGINE_CAMERA_HPP
