/**
 * \file voxelgrid.h
 * \author Luiz Felipe Feitosa Leite
 * \date 2019
 * \brief Voxelgrid subsampling implementation and some author variations.
 */

#ifndef VOXELGRID_H
#define VOXELGRID_H

#include "vector3.h"
#include "cloud.h"

/**
 * \brief Fast offset calculation
 * \param i Coordinate x index
 * \param j Coordinate y index
 * \param k Coordinate z index
 * \param size_x The x size of the cube
 * \param size_y The y size of the cube
 * \return The disired offset
 */
uint idx_offset(uint i, uint j, uint k, uint size_x, uint size_y);

/**
 * \brief Executes voxegrid subsampling
 * \param src The target cloud
 * \param leafsize The size of the sliding cube
 * \return The src cloud with reduced density
 */
struct cloud* cubic_voxel_grid(struct cloud *src, real leafsize);

#endif // VOXELGRID_H

