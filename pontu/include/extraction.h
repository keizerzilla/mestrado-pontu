/**
 * \file extraction.c
 * \author Artur Rodrigues Rocha Neto
 * \date 2019
 * \brief Cloud segmentation functions for enhanced better feature extraction.
 */

#ifndef EXTRACTION_H
#define EXTRACTION_H

#include "./cloud.h"
#include "./dataframe.h"

/**
 * \brief Extracts moments using cuts from a plane
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \param norm Normal vector of the plane
 * \return A dataframe com os momentos extraídos
 */
struct dataframe *extraction_plane(struct cloud *cloud,
				                   struct dataframe *(*mfunc) (struct cloud *),
				                   struct vector3 *norm);
/**
 * \brief Extracts moments using cuts from recursive planes
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \param norm Normal vector of the plane
 * \return Matrix with extracted moments
 */
struct dataframe *extraction_recursive(struct cloud *cloud,
				                    struct dataframe *(*mfunc) (struct cloud *),
				                    struct vector3 *norm);

/**
 * \brief Extracts moments using sgital cuts
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return Matrix with extracted moments
 */
struct dataframe *extraction_sagittal(struct cloud *cloud,
				                   struct dataframe *(*mfunc) (struct cloud *));

/**
 * \brief Extracts moments using transversal cuts
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return Matrix with extracted moments
 */
struct dataframe *extraction_transversal(struct cloud *cloud,
				                   struct dataframe *(*mfunc) (struct cloud *));

/**
 * \brief Extracts moments using front cuts
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return Matrix with extracted moments
 */
struct dataframe *extraction_frontal(struct cloud *cloud,
				                  struct dataframe *(*mfunc) (struct cloud *));

/**
 * \brief Extracts moments using only radial cuts
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return Matrix with extracted moments
 */
struct dataframe *extraction_radial(struct cloud *cloud,
				                 struct dataframe *(*mfunc) (struct cloud *));
/**
 * \brief Extracts moments using only the superior area of the cloud
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return Matrix with the extracted moments
 */
struct dataframe *extraction_upper(struct cloud *cloud,
				                struct dataframe *(*mfunc) (struct cloud *));
/**
 * \brief Extracts moments using only the inferior area of the cloud
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return Matrix with the extracted moments
 */
struct dataframe *extraction_lower(struct cloud *cloud,
				                struct dataframe *(*mfunc) (struct cloud *));

/**
 * \brief Extracts moments using only the nose
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return Matrix with the extracted moments
 */
struct dataframe *extraction_manhattan(struct cloud *cloud,
				                   struct dataframe *(*mfunc) (struct cloud *));

/**
 * \brief The 4 tutu sections
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return 4 sections
 */
struct dataframe *extraction_4(struct cloud *cloud,
			                struct dataframe *(*mfunc) (struct cloud *));

/**
 * \brief The 6 tutu sections
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return 6 sections
 */
struct dataframe *extraction_6(struct cloud *cloud,
			                struct dataframe *(*mfunc) (struct cloud *));
/**
 * \brief The 7 iranians sections
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return 7 sections
 */
struct dataframe *extraction_7(struct cloud *cloud,
			                struct dataframe *(*mfunc) (struct cloud *));

/**
 * \brief Cuts a face (T shape) (nose, eyes, forehead)
 * \param Target cloud
 * \return Cloud cutted in T
 */
struct cloud *extraction_vshape_base(struct cloud *cloud);

/**
 * \brief Cuts a face (T shape) (nose, eyes, forehead)
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return Moments of the cut T
 */
struct dataframe *extraction_vshape(struct cloud *cloud,
				                 struct dataframe *(*mfunc) (struct cloud *));

/**
 * \brief Cuts a face (T shape) frontally
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return Moments of the cut T
 */
struct dataframe *extraction_vshape_f(struct cloud *cloud,
				                   struct dataframe *(*mfunc) (struct cloud *));

/**
 * \brief Cuts a face (T shape) sagittally
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return Moments of the cut T
 */
struct dataframe *extraction_vshape_s(struct cloud *cloud,
				                   struct dataframe *(*mfunc) (struct cloud *));

/**
 * \brief Cuts a face (T shape) transversaly
 * \param cloud Target cloud
 * \param mfunc Function to extract moments
 * \return Moments of the cut T
 */
struct dataframe *extraction_vshape_t(struct cloud *cloud,
				                   struct dataframe *(*mfunc) (struct cloud *));

#endif // EXTRACTION_H

