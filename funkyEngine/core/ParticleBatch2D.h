#pragma once

#include <glm/glm.hpp>
#include "entities/Vertex.h"
#include "entities/SpriteBatch.h"
#include "graphics/GLTexture.h"

namespace FunkyEngine {

    /**
     * class: Particle2D
     * @description:
     * - Represents a single particle in the particle system, with properties for position, velocity, color, life, and width.
     * - The life property determines how long the particle will be active, and the width determines its size when drawn.
     * The color includes an alpha component for transparency, allowing for fading effects as the particle's life decreases.
     * This class is used internally by the ParticleBatch2D to manage individual particles.
     * @note The default constructor initializes the particle with a position of (0, 0), zero velocity, a default color of opaque black, and a life of 0 (inactive).
     * @note The ParticleBatch2D class manages an array of these particles, updating and drawing them based on their properties.
     */
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

    /**
     * class: ParticleBatch2D
     * @description:
     * - Manages a batch of 2D particles, allowing for efficient updating and rendering of multiple particles at once.
     *  - Provides methods to initialize the batch with a maximum number of particles, decay rate, texture, and an optional custom update function.
     * - The addParticle method allows for adding new particles to the batch with specified properties,
     */
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