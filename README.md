# ProbabilisticChangeDetection
<h3>Parallel Probabilistic Change Detection</h3>

<h4>Description</h4>
The probabilistic change detection was developed for detecting changes from high-resoultion satellite images. The basic idea is described in <a href="file:///home/phantom/Downloads/sliding-window-based-probabilistic-change-detection-for-remote-sensed-images.pdf">this paper</a>. To summarize, it puts a sliding window over two time-series images that are geometrically aligned. The sliding window moves pixel-by-pixel. The algorithm models regions in the sliding window as Gaussian distributions and computes the probabilistic distance between the two Gaussians by using <a href="https://en.wikipedia.org/wiki/Kullback%E2%80%93Leibler_divergence">Kullback-Leibler Divergence (KLD)</a>. Once all distances have been computed, it performs <a href="https://people.csail.mit.edu/rameshvs/content/gmm-em.pdf">Gausian Mixture Models with Expectation Maximization (GMM-EM) clustering</a> to cluster regions according to their degrees of change. The clustering result is visualized by using OpenCV. Since the sliding window approach increases a lot the number of computations, we minimize its impact by parallelizing the algorithm with hybrid parallel programming (OpenMP + MPI).
<p align="center">
  <image align="center" src="https://github.com/fender3kr/Images/blob/master/ChangeDetection/Figure01.png">
</p>

This repository contains different versions of the parallel probabilistic change detection algorithm:
<ul>
  <li>Grid-based Approach: this approach is the base of our sliding window-based algorithm. Details can be found at <a href="https://www.sciencedirect.com/science/article/pii/S1877050912002189">here</a>.</li>
  <li>Sliding Window Approach: this is an extension of the grid-based approach.</li>
</ul>

<H4>Source Tree</H4>
<ul>
  <li><b>GridApproach</b>: grid-based algorithm
    <ul>
      <li><b>SequentialVersion</b>: sequential implementation</li>
      <li><b>ParallelVersion</b>: parallel implementation (OpenMP)</li>
    </ul>
  </li>
  <li><b>SlidingWindowApproach</b>: sliding window algorithm
    <ul>
      <li><b>OpenMPVersion</b>: parallel implementation (OpenMP)</li>
      <li><b>HybridVersion</b>: parallel implementation (OpenMP + MPI)</li>
    </ul>
  </li>
</ul>
<H4>Requirements</H4>
<ul>
  <li>MPI (Open MPI, Intel MPI, MPICH)</li>
  <li>OpenCV</li>
  <li><a href="http://www.vlfeat.org/">VLFeat 0.9.20</a> (the implementations internally use this library)</li>
</ul>
