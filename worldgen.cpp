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


// void renderWorld(const SDL_Rect& cameraView)
// {
//     for (int x = 0; x < 215; ++x)
//     {
//         for (int y = 0; y < 96; ++y)
//         {
//             // World coordinates
//             int worldX = x * 16;
//             int worldY = y * 16;

//             // Optional optimization: skip tiles outside the camera view
//             if (worldX + 16 < cameraView.x || worldX > cameraView.x + cameraView.w ||
//                 worldY + 16 < cameraView.y || worldY > cameraView.y + cameraView.h)
//             {
//                 continue; // Tile is not visible
//             }

//             // Get tile based on noise
//             SDL_Point tile = getTileFromNoise(x, y);

//             // Source rect (from tileset)
//             SDL_Rect tileClip = { tile.x * 16, tile.y * 16, 16, 16 };

//             // Destination rect (on screen), offset by camera
//             SDL_Rect dest = { worldX - cameraView.x, worldY - cameraView.y, 16, 16 };

//             // Render tile
//             gTileTexture.render(dest.x, dest.y, &tileClip);
//         }
//     }
// }

// void renderWorld(const SDL_Rect& cameraView)
// {
//     const int tileSize = 16;

//     int startX = (cameraView.x / tileSize) - 1;
//     int startY = (cameraView.y / tileSize) - 1;
//     int endX = ((cameraView.x + cameraView.w) / tileSize) + 1;
//     int endY = ((cameraView.y + cameraView.h) / tileSize) + 1;

//     for (int x = startX; x <= endX; ++x)
//     {
//         for (int y = startY; y <= endY; ++y)
//         {
//             SDL_Point tile = getTileFromNoise(x, y);
//             SDL_Rect tileClip = { tile.x * tileSize, tile.y * tileSize, tileSize, tileSize };

//             // Screen position relative to camera
//             int screenX = x * tileSize - cameraView.x;
//             int screenY = y * tileSize - cameraView.y;
//             SDL_Rect dest = { screenX, screenY, tileSize, tileSize };

//             gTileTexture.render(dest.x, dest.y, &tileClip);
//         }
//     }
// }

void renderWorld(const SDL_Rect& cameraView)
{
    int startX = cameraView.x / TILE_SIZE;
    int startY = cameraView.y / TILE_SIZE;
    int tilesX = cameraView.w / TILE_SIZE + 2; // +2 to cover edge cases
    int tilesY = cameraView.h / TILE_SIZE + 2;

    for (int x = startX; x < startX + tilesX; ++x)
    {
        for (int y = startY; y < startY + tilesY; ++y)
        {
            SDL_Point tile = getTileFromNoise(x, y);

            SDL_Rect tileClip = { tile.x * TILE_SIZE, tile.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            // Subtract cameraView.x/y to place the tile relative to screen
            SDL_Rect dest = {
                x * TILE_SIZE - cameraView.x,
                y * TILE_SIZE - cameraView.y,
                TILE_SIZE, TILE_SIZE
            };

            gTileTexture.render(dest.x, dest.y, &tileClip);
        }
    }
}
