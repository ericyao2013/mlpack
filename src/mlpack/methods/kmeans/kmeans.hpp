/**
 * @file kmeans.hpp
 * @author Parikshit Ram (pram@cc.gatech.edu)
 *
 * K-Means clustering.
 */
#ifndef __MLPACK_METHODS_KMEANS_KMEANS_HPP
#define __MLPACK_METHODS_KMEANS_KMEANS_HPP

#include <mlpack/core.hpp>

#include <mlpack/core/metrics/lmetric.hpp>
#include "random_partition.hpp"
#include "max_variance_new_cluster.hpp"

namespace mlpack {
namespace kmeans {

/**
 * This class implements K-Means clustering.  This implementation supports
 * overclustering, which means that more clusters than are requested will be
 * found; then, those clusters will be merged together to produce the desired
 * number of clusters.
 *
 * Two template parameters can (optionally) be supplied: the policy for how to
 * find the initial partition of the data, and the actions to be taken when an
 * empty cluster is encountered, as well as the distance metric to be used.
 *
 * @tparam DistanceMetric The distance metric to use for this KMeans; see
 *     metric::LMetric for an example.
 * @tparam InitialPartitionPolicy Initial partitioning policy; must implement a
 *     default constructor and 'void Cluster(const arma::mat&, const size_t,
 *     arma::Col<size_t>&)'.  @see RandomPartition for an example.
 * @tparam EmptyClusterPolicy Policy for what to do on an empty cluster; must
 *     implement a default constructor and 'void EmptyCluster(const arma::mat&,
 *     arma::Col<size_t&)'.  @see AllowEmptyClusters and MaxVarianceNewCluster.
 */
template<typename DistanceMetric = metric::SquaredEuclideanDistance,
         typename InitialPartitionPolicy = RandomPartition,
         typename EmptyClusterPolicy = MaxVarianceNewCluster>
class KMeans
{
 public:
  /**
   * Create a K-Means object and (optionally) set the parameters which K-Means
   * will be run with.  This implementation allows a few strategies to improve
   * the performance of K-Means, including "overclustering" and disallowing
   * empty clusters.
   *
   * The overclustering factor controls how many clusters are
   * actually found; for instance, with an overclustering factor of 4, if
   * K-Means is run to find 3 clusters, it will actually find 12, then merge the
   * nearest clusters until only 3 are left.
   *
   * @param maxIterations Maximum number of iterations allowed before giving up
   *     (0 is valid, but the algorithm may never terminate).
   * @param overclusteringFactor Factor controlling how many extra clusters are
   *     found and then merged to get the desired number of clusters.
   * @param metric Optional DistanceMetric object; for when the metric has state
   *     it needs to store.
   * @param partitioner Optional InitialPartitionPolicy object; for when a
   *     specially initialized partitioning policy is required.
   * @param emptyClusterAction Optional EmptyClusterPolicy object; for when a
   *     specially initialized empty cluster policy is required.
   */
  KMeans(const size_t maxIterations = 1000,
         const double overclusteringFactor = 1.0,
         const DistanceMetric metric = DistanceMetric(),
         const InitialPartitionPolicy partitioner = InitialPartitionPolicy(),
         const EmptyClusterPolicy emptyClusterAction = EmptyClusterPolicy());


  /**
   * Perform K-Means clustering on the data, returning a list of cluster
   * assignments.  Optionally, the vector of assignments can be set to an
   * initial guess of the cluster assignments; to do this, the number of
   * elements in the list of assignments must be equal to the number of points
   * (columns) in the dataset.
   *
   * @param data Dataset to cluster.
   * @param clusters Number of clusters to compute.
   * @param assignments Vector to store cluster assignments in.  Can contain an
   *     initial guess at cluster assignments.
   */
  void Cluster(const arma::mat& data,
               const size_t clusters,
               arma::Col<size_t>& assignments) const;

  /**
   * Return the overclustering factor.
   */
  double OverclusteringFactor() const { return overclusteringFactor; }

  /**
   * Set the overclustering factor.
   */
  void OverclusteringFactor(const double overclusteringFactor)
  {
    if (overclusteringFactor < 1.0)
    {
      Log::Warn << "KMeans::OverclusteringFactor(): invalid value (<= 1.0) "
          "ignored." << std::endl;
      return;
    }

    this->overclusteringFactor = overclusteringFactor;
  }

  /**
   * Get the maximum number of iterations.
   */
  size_t MaxIterations() const { return maxIterations; }

  /**
   * Set the maximum number of iterations.
   */
  void MaxIterations(const size_t maxIterations)
  {
    this->maxIterations = maxIterations;
  }

  //! Get the distance metric.
  const DistanceMetric& Metric() const { return metric; }
  //! Modify the distance metric.
  DistanceMetric& Metric() { return metric; }

  //! Get the initial partitioning policy.
  const InitialPartitionPolicy& Partitioner() const { return partitioner; }
  //! Modify the initial partitioning policy.
  InitialPartitionPolicy& Partitioner() { return partitioner; }

  //! Get the empty cluster policy.
  const EmptyClusterPolicy& EmptyClusterAction() const
  {
    return emptyClusterAction;
  }
  //! Modify the empty cluster policy.
  EmptyClusterPolicy& EmptyClusterAction() { return emptyClusterAction; }

 private:
  //! Factor controlling how many clusters are actually found.
  double overclusteringFactor;
  //! Maximum number of iterations before giving up.
  size_t maxIterations;
  //! Instantiated distance metric.
  DistanceMetric metric;
  //! Instantiated initial partitioning policy.
  InitialPartitionPolicy partitioner;
  //! Instantiated empty cluster policy.
  EmptyClusterPolicy emptyClusterAction;
};

}; // namespace kmeans
}; // namespace mlpack

// Include implementation.
#include "kmeans_impl.hpp"

#endif // __MLPACK_METHODS_MOG_KMEANS_HPP
