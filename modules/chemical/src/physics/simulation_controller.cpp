#include "simulation_controller.hpp"
#include "gui/physics.hpp"
#include "physics/kepler_orbit.hpp"
#include "spdlog/spdlog.h"
#include <chrono>
#include <fstream>

namespace Chemical {

  namespace Physics {

    using namespace std::chrono;
    using clock = high_resolution_clock;

    const std::vector<std::string> csv_header = {
        "distance",   "acceleration_a", "acceleration_b", "tick_index",
        "velocity_x", "velocity_y",     "velocity_z"};

    void SimulationDataLogger::LogDistance(double distance) {
      if (is_logging)
        distance_data.emplace_back(distance);
    }
    void SimulationDataLogger::LogAccelerationA(const glm::vec3 &a) {
      if (is_logging) {
        acceleration_a_data.emplace_back(
            std::sqrt(std::pow(a.x, 2) + std::pow(a.y, 2) + std::pow(a.z, 2)));
      }
    }
    void SimulationDataLogger::LogAccelerationB(const glm::vec3 &b) {
      if (is_logging) {
        acceleration_b_data.emplace_back(
            std::sqrt(std::pow(b.x, 2) + std::pow(b.y, 2) + std::pow(b.z, 2)));
      }
    }
    void SimulationDataLogger::LogTickIndex(unsigned int idx) {
      if (is_logging) {
        tick_index_data.emplace_back(idx);
      }
    }
    void SimulationDataLogger::LogVelocity(const glm::vec3 &velocity) {
      if (is_logging) {
        velocity_data.emplace_back(velocity);
      }
    }
    void SimulationDataLogger::Export() {
      if (distance_data.size() == 0 || acceleration_a_data.size() == 0 ||
          acceleration_b_data.size() == 0 || tick_index_data.size() == 0 ||
          velocity_data.size() == 0 ||
          distance_data.size() != acceleration_a_data.size() ||
          acceleration_a_data.size() != acceleration_b_data.size())
        return;

      std::string file_path =
          std::format("sim_data/{:%Y-%m-%d %H:%M:%S}.csv", system_clock::now());

      std::ofstream output(file_path); // Create and open file

      if (output.is_open()) {

        int i = 0;
        for (const std::string &s : csv_header) {
          output << s;
          if (i < csv_header.size() - 1)
            output << ",";
          i++;
        }
        output << std::endl;
        for (i = 0; i < distance_data.size(); i++) {
          output << distance_data[i] << ",";
          output << acceleration_a_data[i] << ",";
          output << acceleration_b_data[i] << ",";
          output << tick_index_data[i] << ",";
          output << velocity_data[i].x << ",";
          output << velocity_data[i].y << ",";
          output << velocity_data[i].z << std::endl;
        }

        output.close();
      } else {
        SPDLOG_ERROR("Unable to export CSV.");
      }

      acceleration_a_data.clear();
      acceleration_b_data.clear();
      distance_data.clear();
    }

    double delta_time = 0;
    clock::time_point old_time;
    double time_accumulator = 0;

    SimulationController::SimulationController() {

      initial_body_a.mass = 100;
      initial_transform_a.position.x = 2550;
      initial_body_a.velocity = glm::dvec3(0, 0.0f, -5.58f);
      initial_body_b.mass = 10000;

      initial_transform_b.scale =
          glm::vec3(std::sqrt(std::abs(initial_body_b.mass)) + 10);
      initial_transform_a.scale =
          glm::vec3(std::sqrt(std::abs(initial_body_a.mass)) + 10);

      old_time = clock::now();

      Reset();
    }
    void SimulationController::Play() {
      Reset();
      running = true;
    }
    void SimulationController::Reset() {
      logger.Export();
      rt_body_a = initial_body_a;
      rt_body_b = initial_body_b;

      rt_transform_a = initial_transform_a;
      rt_transform_b = initial_transform_b;

      running = false;
      tick_idx = 0;
    }
    void SimulationController::Pause() {
      running = false;
    }
    void SimulationController::ToggleLogging() {
      logger.is_logging = !logger.is_logging;
    }
    bool SimulationController::IsLogging() {
      return logger.is_logging;
    }

    constexpr double target_frame_time = 1 / 60.0f;

    void SimulationController::Update() {
      clock::time_point new_time = clock::now();
      delta_time = std::chrono::duration<float>(new_time - old_time).count();
      old_time = new_time;

      GUI::RenderPhysicsMenu(*this);
      if (running) {
        time_accumulator += delta_time;
        if (time_accumulator >= target_frame_time) {
          PhysicsUpdate();
          time_accumulator -= target_frame_time;
        }
      }
    }
    void SimulationController::PhysicsUpdate() {
      for (unsigned int i = 0; i < speed_multiplier; i++) {

        Physics::ApplyKeplerOrbit(logger, rt_transform_a.position,
                                  rt_transform_b.position, rt_body_a,
                                  rt_body_b);

        rt_transform_a.position += rt_body_a.velocity;

        rt_transform_b.position += rt_body_b.velocity;

        logger.LogTickIndex(tick_idx);

        tick_idx++;
      }
    }
    Transform &SimulationController::GetActiveTransformA() {
      if (running)
        return rt_transform_a;
      else
        return initial_transform_a;
    }
    Transform &SimulationController::GetActiveTransformB() {
      if (running)
        return rt_transform_b;
      else
        return initial_transform_b;
    }
    float SimulationController::GetActiveMassA() {
      if (running)
        return rt_body_a.mass;
      else
        return initial_body_a.mass;
    }
    float SimulationController::GetActiveMassB() {
      if (running)
        return rt_body_b.mass;
      else
        return initial_body_b.mass;
    }
    void SimulationController::SetActiveMassA(float mass) {
      if (running) {

        rt_body_a.mass = mass;
        rt_transform_a.scale = glm::vec3(std::sqrt(std::abs(mass)) + 10);
      } else {
        initial_body_a.mass = mass;
        initial_transform_a.scale = glm::vec3(std::sqrt(std::abs(mass)) + 10);
      }
    }
    void SimulationController::SetActiveMassB(float mass) {
      if (running) {

        rt_body_b.mass = mass;
        rt_transform_b.scale = glm::vec3(std::sqrt(std::abs(mass)) + 10);
      } else {
        initial_body_b.mass = mass;
        initial_transform_b.scale = glm::vec3(std::sqrt(std::abs(mass)) + 10);
      }
    }

    glm::vec3 SimulationController::GetActiveVelocityA() {
      if (running)
        return rt_body_a.velocity;
      else
        return initial_body_a.velocity;
    }
    glm::vec3 SimulationController::GetActiveVelocityB() {
      if (running)
        return rt_body_b.velocity;
      else
        return initial_body_b.velocity;
    }
    void SimulationController::SetActiveVelocityA(const glm::vec3 &velocity) {
      if (running)
        rt_body_a.velocity = velocity;
      else
        initial_body_a.velocity = velocity;
    }
    void SimulationController::SetActiveVelocityB(const glm::vec3 &velocity) {
      if (running)
        rt_body_b.velocity = velocity;
      else
        initial_body_b.velocity = velocity;
    }

  } // namespace Physics
} // namespace Chemical
