#pragma once

constexpr int         CHUNK_X_BLOCK_COUNT = 16;
constexpr int         CHUNK_Y_BLOCK_COUNT = 256;
constexpr int         CHUNK_Z_BLOCK_COUNT = 16;

constexpr int		  BLOCK_TYPES_TEXTURES_NUMBER = 12; // number of different block types in the texture

constexpr int RENDER_DISTANCE = 4; // in chunks
								    // the surface of the area renderer is maybe ((RENDER_DISTANCE*2)+1)**2