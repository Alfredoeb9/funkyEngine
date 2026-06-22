#include "ParticleBatch2D.h"
#include "core/Logger.h"

namespace FunkyEngine {

    ParticleBatch2D::ParticleBatch2D() {
        // Empty
    }


    /**
     * function: ~ParticleBatch2D
     * @description:
     * - Destructor for the ParticleBatch2D class.
     * - Cleans up dynamically allocated memory for particles.
     */
    ParticleBatch2D::~ParticleBatch2D() {
        delete[] m_particles;
    }

    /**
     * function: init
     * @description:
     * - Initializes the particle batch with the specified parameters.
     * - Allocates memory for the particles and sets up the decay rate, texture, and update function.
     * @param maxParticles: The maximum number of particles in the batch.
     * @param decayRate: The rate at which particles decay (life decreases).
     * @param texture: The texture used for rendering the particles.
     * @param updateFunc: A function pointer for custom particle updates (optional).
     * @returns void
     */
    void ParticleBatch2D::init(int maxParticles,
                               float decayRate,
                               GLTexture texture,
                               std::function<void(Particle2D&, float)> updateFunc /* = defaultParticleUpdate */) {
        // Log a WARNING if someone tries to init with 0 or negative particles
        if (maxParticles <= 0) {
            FunkyEngine::Logger::log(FunkyEngine::LogLevel::WARNING, 
                "ParticleBatch2D initialized with 0 or negative particles. This may cause crashes.");
        }
        
        m_maxParticles = maxParticles;
        m_particles = new Particle2D[maxParticles];
        m_decayRate = decayRate;
        m_texture = texture;
        m_updateFunc = updateFunc;

        FunkyEngine::Logger::log(FunkyEngine::LogLevel::INFO, 
            "ParticleBatch2D initialized with " + std::to_string(maxParticles) + " particles.");
    }

    /**
     * function: update
     * @description:
     * - Updates all active particles in the batch using the provided update function.
     * - Decreases the life of each active particle based on the decay rate and delta time
     * @param deltaTime: The time elapsed since the last frame, used for smooth animation.
     * @returns void
     */
    void ParticleBatch2D::update(float deltaTime) {
        // Loop through all particles and update the active ones
        for (int i = 0; i < m_maxParticles; i++) {
            // Check if it is active
            if (m_particles[i].life > 0.0f) {
                // Update using function pointer
                m_updateFunc(m_particles[i], deltaTime);
                m_particles[i].life -= m_decayRate * deltaTime;
            }
        }
    }

    /**
     * function: draw
     * @description:
     * - Draws all active particles in the batch using the provided sprite batch.
     * - For each active particle, it calculates the destination rectangle based on the particle's position and width, and uses a default UV rect that covers the entire texture.
     * @param spriteBatch: A pointer to the sprite batch used for rendering.
     * @returns void
     */
    void ParticleBatch2D::draw(SpriteBatch* spriteBatch) {
        // Create a default UV rect that covers the entire texture (assuming the texture is a single sprite)
        glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        // Loop through all particles and draw the active ones
        for (int i = 0; i < m_maxParticles; i++) {
            // Check if it is active
            auto& u_particle = m_particles[i];
            // Only draw if the particle is active (life > 0)
            if (u_particle.life > 0.0f) {
                glm::vec4 destRect(u_particle.position.x, u_particle.position.y, u_particle.width, u_particle.width);
                spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, u_particle.color);
            }
        }
    }

    /**
     * function: addParticle
     * @description:
     * - Adds a new particle to the batch with the specified properties.
     * - Finds a free particle slot, initializes it with the given parameters, and sets it
     * to active (life > 0). The particle will then be updated and drawn until its life reaches 0, at which point it will be considered inactive again.
     * @param position: The initial position of the particle.
     * @param velocity: The initial velocity of the particle.
     * @param color: The color of the particle (including alpha for transparency).
     * @param width: The width (and height) of the particle when drawn.
     * @returns void
     */
    void ParticleBatch2D::addParticle(const glm::vec2& position,
                                      const glm::vec2& velocity,
                                      const Vertex::ColorRGBA& color,
                                      float width) {
        // Find a free particle slot
        int particleIndex = findFreeParticle();

        // Initialize the particle at the found index with the provided parameters and set it to active (life > 0)
        auto& p = m_particles[particleIndex];

        p.life = 1.0f;
        p.position = position;
        p.velocity = velocity;
        p.color = color;
        p.width = width;
    }

    /**
     * function: findFreeParticle
     * @description:
     * - Finds the index of a free particle in the batch (one that is not active, i.e., life <= 0).
     * - Uses a simple linear search starting from the last known free particle index for efficiency.
     * If no free particle is found, it logs a debug message and returns index 0, which will overwrite the oldest particle.
     * @return The index of a free particle slot in the batch.
     */
    int ParticleBatch2D::findFreeParticle() {

        // First, try to find a free particle starting from the last known free index for O(1) access in most cases
        for (int i = m_lastFreeParticle; i < m_maxParticles; i++) {
            // Check if the particle is inactive (life <= 0)
            if (m_particles[i].life <= 0.0f) {
                // Update the last free particle index for the next search
                m_lastFreeParticle = i;
                return i;
            }
        }

        // If we reach the end of the array without finding a free particle, loop back to the beginning
        for (int i = 0; i < m_lastFreeParticle; i++) {
            // Check if the particle is inactive (life <= 0)
            if (m_particles[i].life <= 0.0f) {
                m_lastFreeParticle = i;
                return i;
            }
        }

        // No particles are free, we are at capacity.
        // This is a good place for a DEBUG or WARNING log.
        FunkyEngine::Logger::log(FunkyEngine::LogLevel::DEBUG, 
            "ParticleBatch2D capacity reached. Overwriting oldest particle.");

        // No particles are free, overwrite first particle
        return 0;
    }

}