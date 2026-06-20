#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Camera2D.h"

namespace FunkyEngine {
    class ParticleEngine2D {
        public:
            ParticleEngine2D();
            ~ParticleEngine2D();

            void addParticles(const glm::vec2& position, const glm::vec2& direction, int numParticles);
            void update(float deltaTime);
            void draw(const Camera2D& camera);

        private:
            struct Particle {
                glm::vec2 position;
                glm::vec2 velocity;
                float lifetime;
                float maxLifetime;
            };

            std::vector<Particle> _particles;
    };
}