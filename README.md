# ProbabilisticChangeDetection
<h3>Parallel Probabilistic Change Detection</h3>

<h4>Description</h4>
The probabilistic change detection was developed for detecting changes from high-resoultion satellite images. The basic idea is described in <a href="file:///home/phantom/Downloads/sliding-window-based-probabilistic-change-detection-for-remote-sensed-images.pdf">this paper</a>. To summarize, it puts a sliding window over two time-series images that are geometrically aligned. The sliding window moves pixel-by-pixel. The algorithm models regions in the sliding window as Gaussian distributions and computes the probabilistic distance between the two Gaussians by using <a href="https://en.wikipedia.org/wiki/Kullback%E2%80%93Leibler_divergence">Kullback-Leibler Divergence (KLD)</a>. Once all distances have been computed, it performs <a href="https://people.csail.mit.edu/rameshvs/content/gmm-em.pdf">Gausian Mixture Models with Expectation Maximization (GMM-EM) clustering</a> to cluster regions according to their degrees of change. The clustering result is visualized by using OpenCV. Since the sliding window approach increases a lot the number of computations, we minimize its impact by parallelizing the algorithm with hybrid parallel programming (OpenMP + MPI).

<image align="center" src="https://github.com/fender3kr/Images/blob/master/ChangeDetection/Figure01.png">
