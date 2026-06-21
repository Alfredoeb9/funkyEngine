#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Camera2D.h"

namespace FunkyEngine {
    class ParticleBatch2D;
    class SpriteBatch;

    class ParticleEngine2D {
        public:
            ParticleEngine2D();
            ~ParticleEngine2D();

            
            // void addParticleBatch(std::shared_ptr<ParticleBatch2D> particleBatch);                 // Pass in each particle batch that we watn the particle engine to manage
            void addParticleBatch(ParticleBatch2D* particleBatch);
            void update(float deltaTime);
            void draw(SpriteBatch* spriteBatch);

        private:
            // std::vector<std::shared_ptr<ParticleBatch2D>> m_particleBatches;
            std::vector<ParticleBatch2D*> m_batches;
    };
}