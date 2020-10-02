//
// Created by HADMARINE on 2020/09/20.
//

#include "VulkanEngine.hpp"

int main() {
  SharkEngine::VulkanEngine engine;
  try{
    engine.run();
  } catch (const std::exception& e) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}