#include "ParticleEngine2D.h"
#include "ParticleBatch2D.h"
#include "entities/SpriteBatch.h"

namespace FunkyEngine {
    /**
     * function: ParticleEngine2D
     * @description:
     * - Constructor for the ParticleEngine2D class.
     */
    ParticleEngine2D::ParticleEngine2D(){
        // Empty;
    }

    /**
     * function: ~ParticleEngine2D
     * @description:
     * - Destructor for the ParticleEngine2D class.
     * - Cleans up dynamically allocated memory for particle batches.
     * - Iterates through the vector of particle batches and deletes each one.
     */
    ParticleEngine2D::~ParticleEngine2D() {
        // shared_ptrs will automatically free the memory
        // once the last reference is destroyed

        for (auto& batch : m_particleBatches) {
            delete batch;
        }
    }

    // void ParticleEngine2D::addParticleBatch(std::shared_ptr<ParticleBatch2D> particleBatch) {
    //     m_particleBatches.push_back(particleBatch);
    // }

    /**
     * function: addParticleBatch
     * @description:
     * - Adds a particle batch to the engine.
     * @param particleBatch: A pointer to the particle batch to add.
     */
    void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch) {
        m_particleBatches.push_back(particleBatch);
    }

    // void ParticleEngine2D::update(float deltaTime) {
    //     for (auto& batch : m_particleBatches) {
    //         batch->update(deltaTime);
    //     }
    // }

    /**
     * function: update
     * @description:
     * - Updates all particle batches in the engine.
     * @param deltaTime: The time elapsed since the last frame, used for smooth animation.
     */
    void ParticleEngine2D::update(float deltaTime) {
        for (auto& batch : m_particleBatches) {
            batch->update(deltaTime);
        }
    }
    // Draw all of our particle batches
    // void ParticleEngine2D::draw(SpriteBatch* spriteBatch) {
    //     for (auto& batch : m_particleBatches) {
    //         spriteBatch->begin();
    //         batch->draw(spriteBatch);
    //         spriteBatch->end();
    //         spriteBatch->renderBatch();
    //     }
    // }

    /**
     * function: draw
     * @description:
     * - Draws all particle batches in the engine using the provided sprite batch.
     * @param spriteBatch: A pointer to the sprite batch used for rendering.
     */
    void ParticleEngine2D::draw(SpriteBatch* spriteBatch) {
        // Loop through all particle batches and draw thems
        for (auto& batch : m_particleBatches) {
            spriteBatch->begin();
            batch->draw(spriteBatch);
            spriteBatch->end();
            spriteBatch->renderBatch();
        }
    }
}