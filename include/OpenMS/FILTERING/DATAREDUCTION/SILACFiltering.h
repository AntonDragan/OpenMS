// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2011 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Lars Nilse $
// $Authors: Steffen Sass, Holger Plattfaut $
// --------------------------------------------------------------------------

#ifndef OPENMS_FILTERING_DATAREDUCTION_SILACFILTERING_H
#define OPENMS_FILTERING_DATAREDUCTION_SILACFILTERING_H

#include <OpenMS/KERNEL/StandardTypes.h>
#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/FILTERING/DATAREDUCTION/SILACFilter.h>
#include <OpenMS/DATASTRUCTURES/DataPoint.h>
#include <OpenMS/DATASTRUCTURES/BlacklistEntry2.h>
#include <OpenMS/CONCEPT/ProgressLogger.h>
#include <OpenMS/DATASTRUCTURES/DRange.h>
#include <OpenMS/DATASTRUCTURES/HashGrid.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include <list>
#include <map>
#include <vector>

namespace OpenMS
{
  class SILACFilter;
  class BlacklistEntry2;

  /**
   * @brief Filtering for SILAC data.
   * This filtering can be used to extract SILAC features from an MS experiment. Several SILACFilters can be added to the filtering to search for specific SILAC patterns.
   * @see SILACFilter
   */
   class OPENMS_DLLAPI SILACFiltering
     : public ProgressLogger
   {
     friend class SILACFilter;

  private:

  /**
   * @brief holds all filters used in the filtering
   */
   std::list<SILACFilter*> filters;

  /**
   * @brief characteristic RT size for clusters
   */
   static DoubleReal rt_threshold;

  /**
   * @brief characteristic m/z size for clusters
   */
   static DoubleReal mz_threshold;

  /**
   * @brief average m/z distance between scanned data points
   */
   DoubleReal mz_stepwidth;

  /**
   * @brief minimal intensity of SILAC features
   */
   static DoubleReal intensity_cutoff;

  /**
   * @brief minimal intensity correlation between regions of different peaks
   */
   static DoubleReal intensity_correlation;

  /**
   * @brief flag for missing peaks
   */
   static bool allow_missing_peaks;

   static gsl_interp_accel* current_aki;
   static gsl_interp_accel* current_spl;
   static gsl_spline* spline_aki;
   static gsl_spline* spline_spl;

  /**
   * @brief current feature id
   */
   static Int feature_id;

  /**
   * @brief lowest m/z value of the experiment
   */
   static DoubleReal mz_min;

  /**
   * @brief raw data
   */
   MSExperiment<Peak1D>& exp;

  public:

  /**
   * @brief detailed constructor
   * @param exp raw data
   * @param rt_threshold_ average characteristic rt size of clusters
   * @param mz_threshold_ average characteristic m/z size of clusters
   * @param mz_stepwidth_ average m/z distance between scanned data points
   * @param intensity_cutoff_ minimal intensity of SILAC features
   * @param intensity_correlation_ minimal intensity correlation between regions of different peaks
   * @param allow_missing_peaks flag for missing peaks
   */
   SILACFiltering(MSExperiment<Peak1D>& exp_, const DoubleReal rt_threshold_, const DoubleReal mz_threshold_, const DoubleReal mz_stepwidth_, const DoubleReal intensity_cutoff_, const DoubleReal intensity_correlation_, const bool allow_missing_peaks_);

  /**
   * destructor
   */
   virtual ~SILACFiltering();

  /**
   * @brief adds a new filter to the filtering
   * @param filter filter to add
   */
   void addFilter(SILACFilter& filter);

  /**
   * @brief starts the filtering based on the added filters
   */
   void filterDataPoints();

  /**
   * @brief structure for blacklist
   * @param range m/z and RT interval to be blacklisted
   * @param charge charge of the generating filter
   * @param mass_separations mass separations of the generating filter
   * @param relative_peak_position m/z position of the blacklisted area relative to the mono-isotopic peak of the unlabelled peptide
   */
   struct BlacklistEntry
   {
     DRange<2> range;
     Int charge;
     std::vector<DoubleReal> mass_separations;
     DoubleReal relative_peak_position;
   };

  /**
   * @brief holds the range that is blacklisted for other filters and the filter that generated the blacklist entry
   */
   std::multimap<DoubleReal, BlacklistEntry> blacklist;

  /**
   * @brief holds the range that is blacklisted for other filters and the filter that generated the blacklist entry
   */
   HashGrid blacklist2;

  };
}

#endif /* SILACFILTERING_H_ */
