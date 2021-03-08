#pragma once

constexpr int         CHUNK_X_BLOCK_COUNT = 16;
constexpr int         CHUNK_Y_BLOCK_COUNT = 256;
constexpr int         CHUNK_Z_BLOCK_COUNT = 16;
constexpr float       BLOCK_LENGTH = 1.f;

constexpr int         TEXTURE_SIDE_LENGTH = 16; // in pixels
constexpr int		  BLOCK_TYPES_TEXTURES_NUMBER = 1;

constexpr int RENDER_DISTANCE = 10; // in chunks

// Meta constants
constexpr float CHUNK_X_LENGTH = CHUNK_X_BLOCK_COUNT * BLOCK_LENGTH;
constexpr float CHUNK_Y_LENGTH = CHUNK_Y_BLOCK_COUNT * BLOCK_LENGTH;
constexpr float CHUNK_Z_LENGTH = CHUNK_Z_BLOCK_COUNT * BLOCK_LENGTH;