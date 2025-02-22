
#include "../../include/module_1/cluster.h"

cluster::cluster() { m.pt(" input received."); }
cluster::cluster(const int nVertices)
    : nVertices{nVertices}, currentCluster{nVertices} {

  clusters.resize(nVertices);

  for (int i = 0; i < nVertices; ++i)
    clusters[i] = i;

  m.pt(" input received.");
}
cluster::cluster(const int nVertices, const int nCluster)
    : nVertices{nVertices}, nCluster{nCluster}, currentCluster{nVertices} {

  clusters.resize(nVertices);

  for (int i = 0; i < nVertices; ++i)
    clusters[i] = i;

  m.pt(" input received.");
}

// =============================================================================
// calculates the number of clusters in the graph
void cluster::numberCluster(const int counter) {

  int nclus = 0;
  // int var1, var2;
  std::vector<int>::iterator vit;
  std::vector<int> ndiff;
  ndiff.resize(nVertices);

  for (int i = 0; i < nVertices; ++i) {
    for (vit = clusters.begin(); vit != clusters.end(); ++vit) {
      if (i == *vit) {
        ++nclus;
        break;
      }
    }
    // var1 = vit - clusters.begin();
    // var2 = clusters.end() - clusters.begin();
    // if (var1 == var2)
    //  printf(" %d ", i);
  }
  // printf("\n no clusters: %d %d \n", nclus, counter);
  // if ((nclus + counter) != nVertices)
  //  printf("zzzzzzzzzzzzzzzz\n");
}

// =============================================================================
int cluster::indexfinder(const int min, const int max) {
  int index = 0;
  for (int i = 1; i <= min; ++i) {
    index += nCluster - i;
  }
  index += -1 + max - min;

  return index;
}

// =============================================================================
void cluster::findDist() {
  std::vector<int> distances;
  distances.resize(nCluster * (nCluster - 1) / 2);
  for (std::vector<int>::iterator it = distances.begin(); it < distances.end();
       ++it)
    *it = INT_MAX;

  int clus_min, clus_max;
  int index = 0;
  for (std::multimap<int, std::pair<int, int>>::iterator it = inpGraph.begin();
       it != inpGraph.end(); ++it) {
    index++;
    // printf(" max dis: %d \n", index);
    if (clusters[it->second.first] != clusters[it->second.second]) {
      clus_min = (clusters[it->second.first] > clusters[it->second.second])
                     ? clusters[it->second.second]
                     : clusters[it->second.first];
      clus_max = (clusters[it->second.first] < clusters[it->second.second])
                     ? clusters[it->second.second]
                     : clusters[it->second.first];
      if (DEBUG)
        printf(" %d %d %d %d\n", clus_min, clus_max,
               distances[indexfinder(clus_min, clus_max)], it->first);
      if (distances[indexfinder(clus_min, clus_max)] > it->first) {
        distances[indexfinder(clus_min, clus_max)] = it->first;
      }
    }
  }
  for (std::vector<int>::iterator it = distances.begin(); it < distances.end();
       ++it)

    if (DEBUG)
      printf(" output: %d \n", *it);
}

// =============================================================================
void cluster::clusterRenumber() {
  int clust = -1;
  int add = nCluster;
  int index;
  int check;
  std::vector<int>::iterator vit;
  for (int i = 0; i < nCluster; ++i) {
    check = 0;
    for (vit = clusters.begin(); vit != clusters.end(); ++vit) {
      if (*vit >= 0 && check == 0) {
        index = *vit;
        check = 1;
      }
      if (*vit == index)
        *vit = clust;
    }
    --clust;
  }
  std::for_each(clusters.begin(), clusters.end(), [add](int &d) { d += add; });
}

// =============================================================================
void cluster::pushEdge(const int u, const int v, const int w) {
  if (DEBUG)
    printf("edge %d %d: %d \n", u, v, w);

  inpGraph.insert(
      std::pair<int, std::pair<int, int>>(w, std::pair<int, int>(u, v)));
}

// =============================================================================
void cluster::printInput() {
  m.pt("graph: ");
  int index;
  for (std::vector<std::vector<int>>::iterator it = graph.begin();
       it != graph.end(); ++it) {
    index = it - graph.begin();
    printf(" %3d", index);
    for (std::vector<int>::iterator vit = (*it).begin(); vit != (*it).end();
         ++vit) {
      printf(" %d", *vit);
    }
    printf("\n");
  }
}

// =============================================================================
// prints the graph, converted from input
void cluster::printGraph() {
  m.pt("graph: ");
  int index = 0;
  for (auto e : inpGraph) {
    printf("%3d w: %3d - %3d--%3d \n", index, e.first, e.second.first,
           e.second.second);
    index++;
  }
  m.pt("clusters: ");
  for (int i = 0; i < nVertices; ++i)
    std::cout << i << ": " << clusters[i] << "\n";
}

// =============================================================================
void cluster::printCluster() {
  m.pti("clusters: ", currentCluster);
  for (int i = 0; i < nVertices; ++i)
    std::cout << i << ": " << clusters[i] << "\n";
  m.pti("distance: ", distance);
  m.pti("cluster: ", currentCluster);
}

//==============================================================================
// find clusters

void cluster::findClusters() {
  m.pt("\n finding clusters:");
  int counter = 0;
  int group, index;
  int var;
  std::multimap<int, std::pair<int, int>>::iterator nit;
  for (std::multimap<int, std::pair<int, int>>::iterator it = inpGraph.begin();
       it != inpGraph.end(); ++it) {

    std::cout << it->first << " " << it->second.first << " "
              << it->second.second << "\n";

    if (currentCluster > nCluster) {

      if (clusters[it->second.first] < clusters[it->second.second]) {
        group = clusters[it->second.first];
        index = it->second.second;
      } else if (clusters[it->second.first] > clusters[it->second.second]) {
        group = clusters[it->second.second];
        index = it->second.first;
      } else
        continue;

      if (DEBUG)
        std::cout << " \n merging " << counter << ": "
                  << "  " << it->first << "  " << it->second.first << "  "
                  << it->second.second << "  --" << group << "-" << index
                  << " || " << clusters[it->second.first] << "  "
                  << clusters[it->second.second] << " " << clusters[index]
                  << "\n";
      --currentCluster;
      ++counter;
      int old = clusters[index];
      for (std::vector<int>::iterator vit = clusters.begin();
           vit != clusters.end(); ++vit) {
        if (*vit == old) {
          *vit = group;
          var = vit - clusters.begin();
          if (DEBUG)
            std::cout << std::setw(10) << var << "--" << group << "  ";
        }
      }
      if (DEBUG)
        std::cout << std::endl;

      numberCluster(counter);
      if (DEBUG)
        printCluster();
    }

    nit = it;
    ++nit;
    if (it->first == nit->first)
      if (DEBUG)
        m.pt("equal  ====");

    if (it->first != nit->first && currentCluster <= nCluster) {
      break;
    }
  }

  m.pt(" cluster ends.");
}
