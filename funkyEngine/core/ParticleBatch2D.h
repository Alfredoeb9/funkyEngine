#pragma once

#include <glm/glm.hpp>
#include "entities/Vertex.h"
#include "entities/SpriteBatch.h"
#include "graphics/GLTexture.h"

namespace FunkyEngine {

    class Particle2D {
        public:
            glm::vec2 position = glm::vec2(0.0f);
            glm::vec2 velocity = glm::vec2(0.0f);
            Vertex::ColorRGBA color;
            float life = 0.0f;
            float width = 0.0f;                
    };

    // Default function pointer
    inline void defaultParticleUpdate(Particle2D& particle, float deltaTime) {
        particle.position += particle.velocity * deltaTime;
    }

    class ParticleBatch2D {
        public:
            ParticleBatch2D();
            ~ParticleBatch2D();

            void init(int maxParticles,
                  float decayRate,
                  GLTexture texture,
                  std::function<void(Particle2D&, float)> updateFunc = defaultParticleUpdate);

            void update(float deltaTime);
            void draw(SpriteBatch* spriteBatch);

            // Will find a particle in the myParticles array that is not active = false
            // then initialize it and set it to active = true
            // then updating drawing the particle until its life reaches 0 (decayRate)
            // then set it back to isActive = false
            void addParticle(const glm::vec2& position, 
                                const glm::vec2& velocity, 
                                const Vertex::ColorRGBA& color, 
                                float width
                            );
            
            
        private:
            int findFreeParticle();

            std::function<void(Particle2D&, float)> m_updateFunc; ///< Function pointer for custom updates
            float m_decayRate = 0.1f;
            Particle2D* m_particles = nullptr;
            int m_maxParticles = 0;
            int m_lastFreeParticle = 0;
            GLTexture m_texture;             // Keep track of particle in array that is not in use for O(1) lookup and insert

        };
}