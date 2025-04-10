#include "worldgen.h"
#include "textures.h"
#include <algorithm> // for std::min, std::max


FastNoiseLite moistureNoise;
FastNoiseLite temperatureNoise;
FastNoiseLite altitudeNoise;
PTexture gTileTexture;

void initNoise() {
    moistureNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    temperatureNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    altitudeNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    moistureNoise.SetSeed(rand());
    temperatureNoise.SetSeed(rand());
    altitudeNoise.SetSeed(rand());

    // Set frequency for different noise types
    moistureNoise.SetFrequency(0.001f);  // Lower value = more spread out
    temperatureNoise.SetFrequency(0.0005f); // Higher value = more compacted patterns
    altitudeNoise.SetFrequency(0.075f);   // Same as moisture, adjust as needed
}

SDL_Point getTileFromNoise(float x, float y) {
    float moist = moistureNoise.GetNoise(x, y) * 10.0f;
    float temp = temperatureNoise.GetNoise(x, y) * 10.0f;
    float alt = altitudeNoise.GetNoise(x, y) * 10.0f;

    int tileX = 0;
    int tileY = 0;

    if (alt < 2) {
        // Water tiles based on temp
        tileX = 2;
        tileY = static_cast<int>((temp + 10.0f) / 5.0f);
    } else {
        // Grass/snow/sand based on moist/temp
        tileX = static_cast<int>((moist + 10.0f) / 5.0f);
        tileY = static_cast<int>((temp + 10.0f) / 5.0f);
    }

    // Clamp to 0–3 to stay in tile bounds
    tileX = std::max(0, std::min(3, tileX));
    tileY = std::max(0, std::min(3, tileY));

    return { tileX, tileY };
}

void renderWorld()
{
    for (int x = 0; x < 215; ++x)
    {
        for (int y = 0; y < 96; ++y)
        {
            // Get noise-based tile coordinate from our 4x4 tileset
            SDL_Point tile = getTileFromNoise(x, y);

            // Source rect (from tileset)
            SDL_Rect tileClip = { tile.x * 16, tile.y * 16, 16, 16 };

            // Destination rect (on screen)
            SDL_Rect dest = { x * 16, y * 16, 16, 16 };

            // Render to screen
            gTileTexture.render(dest.x, dest.y, &tileClip);
        }
    }
}

