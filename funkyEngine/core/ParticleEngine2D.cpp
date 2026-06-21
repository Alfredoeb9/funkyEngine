#include "ParticleEngine2D.h"
#include "ParticleBatch2D.h"
#include "entities/SpriteBatch.h"

namespace FunkyEngine {

    ParticleEngine2D::ParticleEngine2D(){
        // Empty;
    }

    ParticleEngine2D::~ParticleEngine2D() {
        // shared_ptrs will automatically free the memory
        // once the last reference is destroyed

        for (auto& batch : m_batches) {
            delete batch;
        }
    }

    // void ParticleEngine2D::addParticleBatch(std::shared_ptr<ParticleBatch2D> particleBatch) {
    //     m_particleBatches.push_back(particleBatch);
    // }

    void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch) {
        m_batches.push_back(particleBatch);
    }

    // void ParticleEngine2D::update(float deltaTime) {
    //     for (auto& batch : m_particleBatches) {
    //         batch->update(deltaTime);
    //     }
    // }


    void ParticleEngine2D::update(float deltaTime) {
        for (auto& b : m_batches) {
            b->update(deltaTime);
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

    void ParticleEngine2D::draw(SpriteBatch* spriteBatch) {
        for (auto& b : m_batches) {
            spriteBatch->begin();
            b->draw(spriteBatch);
            spriteBatch->end();
            spriteBatch->renderBatch();
        }
    }
}