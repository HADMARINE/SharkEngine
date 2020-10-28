//
// Created by HADMARINE on 2020/09/20.
//

#include "core/engine/VulkanEngine.h"

int main() {
    using namespace SharkEngine;
    SharkEngine::VulkanEngine engine;
    try {

        //std::vector<TextureImageStruct> textures;
        engine.run();

        //std::vector<GraphicsObject> graphicsObjects;
        //        VulkanEngine::setGraphicsObject(graphicsObjects);
        //textures.push_back(*engine.createTextureImage("texture.jpg"));
        //textures.push_back(*engine.createTextureImage("texture2.jpg"));

//        GraphicsObject obj1(std::vector<Vertex>{
//                                    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
//                                    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//                                    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
//                                    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
//                            },
//                            &textures.at(0)),
//                obj2(std::vector<Vertex>{
//                             {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
//                             {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
//                             {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
//                             {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}},
//                     &textures.at(1));
//
//
//        graphicsObjects.push_back(obj1);
//        graphicsObjects.push_back(obj2);
//
//        engine.setGraphicsObjects(&graphicsObjects);
    } catch (const std::exception &e) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}